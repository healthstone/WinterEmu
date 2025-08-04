#include "NodeConnector.hpp"
#include "src/relayserver/Entity/NodeConnector/nodehandlers/NodeHandlers.hpp"

NodeConnector::NodeConnector(boost::asio::io_context &io, const std::string &host, int port, uint8_t node_id)
        : io_(io),
          socket_(io),
          host_(host),
          port_(port),
          connected_(false),
          writing_(false),
          awaiting_pong_(false),
          reconnect_timer_(io),
          heartbeat_timer_(io),
          pong_timeout_timer_(io),
          node_id_(node_id) {
}

void NodeConnector::start() {
    connect();
}

void NodeConnector::stop() {
    boost::asio::post(io_, [self = shared_from_this()]() {
        boost::system::error_code ignored_ec;
        self->reconnect_timer_.cancel(ignored_ec);
        self->heartbeat_timer_.cancel(ignored_ec);
        self->pong_timeout_timer_.cancel(ignored_ec);

        if (self->socket_.is_open()) {
            self->socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
            self->socket_.close(ignored_ec);
        }
        self->connected_ = false;
        self->writing_ = false;
        self->write_queue_.clear();

        Logger::get()->info("[NodeConnector] Disconnected");
    });
}

void NodeConnector::connect() {
    tcp::resolver resolver(io_);
    auto endpoints = resolver.resolve(host_, std::to_string(port_));

    boost::asio::async_connect(socket_, endpoints,
                               [self = shared_from_this()](boost::system::error_code ec, const tcp::endpoint &) {
                                   auto log = Logger::get();
                                   if (!ec) {
                                       self->connected_ = true;
                                       log->info("[NodeConnector] Connected to Node: {}", self->node_id_);
                                       self->flush_queue();
                                       self->start_receive_loop();
                                       self->start_heartbeat();
                                   } else {
                                       log->error("[NodeConnector] Connect failed: {}", ec.message());
                                       self->schedule_reconnect();
                                   }
                               });
}

void NodeConnector::schedule_reconnect() {
    connected_ = false;
    boost::system::error_code ec;
    socket_.close(ec);
    reconnect_timer_.expires_after(std::chrono::seconds(5));
    reconnect_timer_.async_wait([self = shared_from_this()](const boost::system::error_code &ec) {
        if (!ec) {
            Logger::get()->info("[NodeConnector] Reconnecting...");
            self->connect();
        }
    });
}

void NodeConnector::send_packet(const NodePacket &packet) {
    auto self = shared_from_this();
    boost::asio::post(io_, [self, packet]() {
        self->do_send_packet(packet);
    });
}

void NodeConnector::do_send_packet(const NodePacket &packet) {
    if (!connected_) {
        Logger::get()->info("[NodeConnector] Not connected. Packet queued.");
    }
    auto data = packet.build_packet();
    write_queue_.push_back(std::move(data));

    if (!writing_) {
        do_write();
    }
}

void NodeConnector::do_write() {
    if (write_queue_.empty()) {
        writing_ = false;
        return;
    }

    writing_ = true;
    auto self = shared_from_this();

    boost::asio::async_write(socket_, boost::asio::buffer(write_queue_.front()),
                             [this, self](boost::system::error_code ec, std::size_t) {
                                 if (ec) {
                                     Logger::get()->error("[NodeConnector] Write failed: {}", ec.message());
                                     connected_ = false;
                                     writing_ = false;
                                     write_queue_.clear();
                                     self->schedule_reconnect();
                                     return;
                                 }
                                 write_queue_.pop_front();
                                 do_write();
                             });
}

void NodeConnector::flush_queue() {
    if (!connected_ || write_queue_.empty()) return;
    if (!writing_) do_write();
}

void NodeConnector::start_receive_loop() {
    read_header();
}

void NodeConnector::read_header() {
    auto header = std::make_shared<std::vector<uint8_t>>(4); // opcode(2) + length(2)
    boost::asio::async_read(socket_, boost::asio::buffer(*header),
                            [self = shared_from_this(), header](boost::system::error_code ec,
                                                                std::size_t bytes_transferred) {
                                auto log = Logger::get();
                                if (ec) {
                                    if (ec == boost::asio::error::operation_aborted ||
                                        ec == boost::asio::error::eof) {
                                        //log->info("[NodeConnector] Disconnected while reading auth header");
                                    } else {
                                        log->error("[NodeConnector] Header read failed: {}", ec.message());
                                    }
                                    self->connected_ = false;
                                    self->schedule_reconnect();
                                    return;
                                }
                                if (bytes_transferred != 4) {
                                    log->error("[NodeConnector] Incomplete header read: {} bytes", bytes_transferred);
                                    self->connected_ = false;
                                    self->schedule_reconnect();
                                    return;
                                }

                                uint16_t opcode = (static_cast<uint16_t>((*header)[0]) << 8) | (*header)[1];
                                uint16_t length = (static_cast<uint16_t>((*header)[2]) << 8) | (*header)[3];

                                log->debug("[NodeConnector] Header read: opcode={} length={}", opcode, length);

                                self->read_payload(length, *header);
                            });
}

void NodeConnector::read_payload(std::size_t length, const std::vector<uint8_t> &header) {
    if (length > 2048) {
        Logger::get()->error("[NodeConnector] Payload size too large: {}", length);
        connected_ = false;
        schedule_reconnect();
        return;
    }

    auto payload = std::make_shared<std::vector<uint8_t>>(length);
    boost::asio::async_read(socket_, boost::asio::buffer(*payload),
                            [self = shared_from_this(), payload, header](boost::system::error_code ec,
                                                                         std::size_t bytes_transferred) {
                                auto log = Logger::get();
                                if (ec) {
                                    if (ec == boost::asio::error::operation_aborted ||
                                        ec == boost::asio::error::eof) {
                                        //log->info("[NodeConnector] Disconnected while reading auth header");
                                    } else {
                                        log->error("[NodeConnector] Payload read failed: {}", ec.message());
                                    }
                                    self->connected_ = false;
                                    self->schedule_reconnect();
                                    return;
                                }
                                if (bytes_transferred != payload->size()) {
                                    log->error("[NodeConnector] Incomplete payload read: {} bytes, expected {}",
                                               bytes_transferred, payload->size());
                                    self->connected_ = false;
                                    self->schedule_reconnect();
                                    return;
                                }

                                std::vector<uint8_t> raw_packet;
                                raw_packet.reserve(4 + payload->size());

                                // opcode(2) + length(2)
                                raw_packet.insert(raw_packet.end(), header.begin(), header.end());
                                // payload
                                raw_packet.insert(raw_packet.end(), payload->begin(), payload->end());

                                auto packet = std::make_shared<NodePacket>();
                                try {
                                    packet->deserialize(raw_packet);
                                    NodeHandlers::dispatch(self, packet);
                                } catch (const std::exception &ex) {
                                    log->error("[NodeConnector] Packet deserialize failed: {}", ex.what());
                                }

                                self->read_header();
                            });
}

void NodeConnector::start_heartbeat() {
    heartbeat_timer_.expires_after(std::chrono::seconds(30));
    heartbeat_timer_.async_wait([self = shared_from_this()](const boost::system::error_code &ec) {
        if (!ec && self->connected_) {
            self->send_ping();
            self->start_heartbeat();
        }
    });
}

void NodeConnector::send_ping() {
    NodeData nodeData;
    nodeData.write_uint8(node_id_);

    NodePacket ping_packet(NodeOpcodes::REL_TO_NODE_PING, nodeData);

    send_packet(ping_packet);
    Logger::get()->trace("[NodeConnector] Sent REL_TO_NODE_PING");

    awaiting_pong_ = true;
    schedule_pong_timeout();
}

void NodeConnector::schedule_pong_timeout() {
    pong_timeout_timer_.expires_after(std::chrono::seconds(10));
    pong_timeout_timer_.async_wait([self = shared_from_this()](const boost::system::error_code &ec) {
        if (!ec) {
            if (self->awaiting_pong_) {
                Logger::get()->warn("[NodeConnector] PONG timeout - disconnecting and reconnecting");
                self->awaiting_pong_ = false;
                self->stop();
                self->connect();
            }
        }
    });
}

void NodeConnector::cancel_pong_timeout() {
    awaiting_pong_ = false;
    boost::system::error_code ignored_ec;
    pong_timeout_timer_.cancel(ignored_ec);
}
