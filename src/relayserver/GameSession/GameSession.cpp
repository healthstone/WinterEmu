#include "GameSession.hpp"
#include "Logger.hpp"
#include "src/relayserver/handlers/Handlers.hpp"
#include "srp6/CryptoRandom.hpp"
#include <iostream>

using boost::asio::ip::tcp;

GameSession::GameSession(tcp::socket socket, std::shared_ptr<RelayServer> server)
        : socket_(std::move(socket)), server_(std::move(server)), read_buffer_(4096) {}

void GameSession::start() {
    auto ep = socket_.remote_endpoint();
    Logger::get()->debug("[relay_session][start] New connection from {}:{}",
                         ep.address().to_string(), ep.port());
    do_read();

    // Задержка отправки auth_challenge
    auto self = shared_from_this();
    auto timer = std::make_shared<boost::asio::steady_timer>(socket_.get_executor());
    timer->expires_after(std::chrono::milliseconds(100));
    timer->async_wait([this, self, timer](const boost::system::error_code& ec) {
        if (!ec)
            send_auth_challenge();
    });
}

void GameSession::close() {
    if (closed_.exchange(true)) return;

    auto log = Logger::get();

    boost::system::error_code ec;
    socket_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[relay_session][close] Failed to cancel socket: {}", ec.message());
    }

    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (ec && ec != boost::asio::error::operation_aborted &&
        ec != boost::asio::error::eof &&
        ec != boost::asio::error::not_connected) {
        log->error("[relay_session][close] Failed to shutdown socket: {}", ec.message());
    }
    socket_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[relay_session][close] Failed to close socket: {}", ec.message());
    }

    read_buffer_.clear();
    write_queue_.clear();

    if (server_) {
        server_->remove_session(shared_from_this());
    }
}

void GameSession::do_read() {
    auto self = shared_from_this();

    read_buffer_.ensure_free_space(512);

    socket_.async_read_some(
            boost::asio::buffer(read_buffer_.write_ptr(), read_buffer_.get_remaining_space()),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                auto log = Logger::get();
                if (ec) {
                    if (ec == boost::asio::error::operation_aborted ||
                        ec == boost::asio::error::eof ||
                        ec == boost::asio::error::connection_reset) {
                        log->debug("[client_session][do_read] Client disconnected: {}", ec.message());
                    } else {
                        log->error("[relay_session][do_read] Read error: {}", ec.message());
                    }
                    close();
                    return;
                }

                if (!isOpened()) return;
                read_buffer_.write_completed(bytes_transferred);

                log->debug("[do_read] Received {} bytes", bytes_transferred);
                process_read_buffer();
                if (isOpened()) do_read();
            }
    );
}

void GameSession::process_read_buffer() {
    auto log = Logger::get();
    MessageBuffer &buffer = read_buffer();

    while (true) {
        if (buffer.get_active_size() < 4)
            return;

        const uint8_t *data = buffer.read_ptr();

        // Чтение длины и opcode в LITTLE ENDIAN
        uint16_t size   = static_cast<uint16_t>(data[1]) << 8 | static_cast<uint16_t>(data[0]);
        uint16_t opcode = static_cast<uint16_t>(data[3]) << 8 | static_cast<uint16_t>(data[2]);

        log->debug("[process_read_buffer] Buffer size: {}, Packet length: {}, Opcode: 0x{:04X}", buffer.get_active_size(), size, opcode);

        if (buffer.get_active_size() < 2 + size) {
            log->debug("[process_read_buffer] Not enough data yet to read full packet");
            return;
        }

        if (size > 2048) {
            log->error("WoWPacket payload too big: {}", size);
            close();
            return;
        }

        std::vector<uint8_t> full_packet(data, data + 2 + size);
        buffer.read_completed(2 + size);

        auto packet = std::make_shared<WoWPacket>();
        try {
            packet->deserialize(full_packet);
            Handlers::dispatch(shared_from_this(), packet);
        } catch (const std::exception &ex) {
            log->error("[GameSession][process_read_buffer] WoWPacket processing failed: {}", ex.what());
            close();
            return;
        }
    }
}


/**
 * Обертка для безопасной отправки пакета из любого потока и корутины
 */
void GameSession::send_packet(const std::shared_ptr<const WoWPacket>& packet) {
    if (closed_) {
        Logger::get()->debug("[relay_session][send_packet] called. closed_={}", closed_.load());
        return;
    }

    auto self = shared_from_this();
    boost::asio::post(
            socket_.get_executor(),
            [self, packet]() {
                self->do_send_packet(*packet);
            });
}

/**
 * Отправка пакета клиенту
 */
void GameSession::do_send_packet(const WoWPacket &packet) {
    std::vector<uint8_t> full_packet = packet.build_packet();
    write_queue_.push_back(std::move(full_packet));

    if (!writing_) {
        do_write();
    }
}

void GameSession::do_write() {
    if (write_queue_.empty()) {
        writing_ = false;
        return;
    }

    writing_ = true;
    auto self = shared_from_this();
    Packet::log_raw_payload("do_write::SMSG_AUTH_CHALLENGE", write_queue_.front());
    boost::asio::async_write(
            socket_,
            boost::asio::buffer(write_queue_.front()),
            [this, self](boost::system::error_code ec, std::size_t) {
                auto log = Logger::get();

                if (ec) {
                    if (ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
                        log->error("[relay_session] Write failed: {}", ec.message());
                    }
                    close();
                    return;
                }

                write_queue_.pop_front();
                do_write();
            }
    );
}

void GameSession::send_auth_challenge() {
    //Crypto::GetRandomBytes(_authSeed);
    //auto random_bytes = Crypto::GetRandomBytes<32>();

    _authSeed = {0x01, 0x00, 0x00, 0x00};
    std::array<uint8_t, 32> fixed_random = {
            0x84, 0xDA, 0x14, 0xC7, 0x68, 0x95, 0x57, 0xC0,
            0xEE, 0xDB, 0xED, 0x2F, 0x21, 0xD4, 0xA9, 0xBD,
            0xB9, 0x66, 0xBF, 0x68, 0x66, 0x15, 0x39, 0x57,
            0xB5, 0xB5, 0x0A, 0xD0, 0x6D, 0x04, 0x2C, 0xF5
    };

    WoWPacket challenge_pkt(WoWOpcodes::SMSG_AUTH_CHALLENGE);
    challenge_pkt.write_uint32_le(12340);
    challenge_pkt.write_bytes(_authSeed.data(), 4);
    challenge_pkt.write_bytes(fixed_random.data(), fixed_random.size());

    Packet::log_raw_payload("send_auth_challenge::SMSG_AUTH_CHALLENGE", challenge_pkt.serialize());

    send_packet(std::make_shared<WoWPacket>(challenge_pkt));
}