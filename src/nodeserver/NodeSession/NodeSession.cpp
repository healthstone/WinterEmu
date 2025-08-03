#include "NodeSession.hpp"
#include "Logger.hpp"
#include "src/nodeserver/NodeServer.hpp"
#include "src/nodeserver/handlers/NHandlers.hpp"

using boost::asio::ip::tcp;

NodeSession::NodeSession(tcp::socket socket, std::shared_ptr<NodeServer> server)
        : socket_(std::move(socket)), server_(std::move(server)), read_buffer_(4096) {}

void NodeSession::start() {
    try {
        auto ep = socket_.remote_endpoint();
        Logger::get()->debug("[node_session][start] New connection from {}:{}",
                             ep.address().to_string(), ep.port());
    } catch (...) {
        // Игнорируем ошибки remote_endpoint
    }
    do_read();
}

void NodeSession::close() {
    if (closed_.exchange(true)) return;

    auto log = Logger::get();

    boost::system::error_code ec;
    socket_.cancel(ec);

    socket_.shutdown(tcp::socket::shutdown_both, ec);

    socket_.close(ec);

    read_buffer_.clear();
    write_queue_.clear();

    if (server_) {
        server_->remove_session(shared_from_this());
    }
}

void NodeSession::do_read() {
    if (closed_) return;

    auto self = shared_from_this();

    read_buffer_.ensure_free_space(512);

    socket_.async_read_some(
            boost::asio::buffer(read_buffer_.write_ptr(), read_buffer_.get_remaining_space()),
            [this, self](boost::system::error_code ec, std::size_t bytes_transferred) {
                auto log = Logger::get();
                if (ec) {
                    if (ec != boost::asio::error::operation_aborted &&
                        ec != boost::asio::error::eof &&
                        ec != boost::asio::error::connection_reset) {
                        log->error("[node_session][do_read] Read error: {}", ec.message());
                    }
                    close();
                    return;
                }

                if (!isOpened()) return;

                read_buffer_.write_completed(bytes_transferred);
                process_read_buffer();

                if (isOpened()) do_read();
            });
}

void NodeSession::process_read_buffer() {
    auto log = Logger::get();
    MessageBuffer& buffer = read_buffer_;

    // Минимум 4 байта заголовка (opcode + node_data_length)
    if (buffer.get_active_size() < 4)
        return;

    const uint8_t* data = buffer.read_ptr();

    uint16_t opcode = (static_cast<uint16_t>(data[0]) << 8) | data[1];
    uint16_t node_data_length = (static_cast<uint16_t>(data[2]) << 8) | data[3];

    // Полная длина ожидаемого пакета: 2 (opcode) + 2 (length) + node_data_length + оставшиеся байты wow_packet
    // Но wow_packet длина не задана в заголовке, надо проверить хотя бы наличие node_data_length
    // Проверяем, что в буфере достаточно данных для node_data
    if (buffer.get_active_size() < 4 + node_data_length)
        return;  // Ждем ещё данных

    // Теперь достаём полные данные для NodePacket
    std::vector<uint8_t> full_packet(data, data + 4 + node_data_length);

    // Учитываем, что wow_packet может быть пустым — если данные остались, их тоже надо включить
    size_t wow_packet_length = buffer.get_active_size() - (4 + node_data_length);
    if (wow_packet_length > 0) {
        const uint8_t* wow_start = data + 4 + node_data_length;
        full_packet.insert(full_packet.end(), wow_start, wow_start + wow_packet_length);
    }

    // Сдвигаем буфер на длину обработанных данных
    buffer.read_completed(buffer.get_active_size());

    auto packet = std::make_shared<NodePacket>();
    try {
        packet->deserialize(full_packet);

        if (!packet->is_valid()) {
            log->error("[NodeSession] NodePacket invalid: opcode={}", static_cast<uint16_t>(packet->opcode()));
            close();
            return;
        }

        NHandlers::dispatch(shared_from_this(), packet);

    } catch (const std::exception& ex) {
        log->error("[NodeSession] Packet deserialize failed: {}", ex.what());
        close();
    }
}


void NodeSession::send_packet(const std::shared_ptr<const Packet>& packet) {
    if (closed_) {
        Logger::get()->debug("[node_session][send_packet] called on closed session");
        return;
    }

    auto self = shared_from_this();
    boost::asio::post(socket_.get_executor(),
                      [self, packet]() {
                          self->do_send_packet(*packet);
                      });
}

void NodeSession::do_send_packet(const Packet& packet) {
    std::vector<uint8_t> full_packet = packet.build_packet();
    write_queue_.push_back(std::move(full_packet));

    if (!writing_) {
        do_write();
    }
}

void NodeSession::do_write() {
    if (write_queue_.empty()) {
        writing_ = false;
        return;
    }

    writing_ = true;
    auto self = shared_from_this();

    boost::asio::async_write(socket_,
                             boost::asio::buffer(write_queue_.front()),
                             [this, self](boost::system::error_code ec, std::size_t) {
                                 if (ec) {
                                     if (ec != boost::asio::error::operation_aborted &&
                                         ec != boost::asio::error::eof) {
                                         Logger::get()->error("[node_session] Write failed: {}", ec.message());
                                     }
                                     close();
                                     return;
                                 }

                                 write_queue_.pop_front();
                                 do_write();
                             });
}
