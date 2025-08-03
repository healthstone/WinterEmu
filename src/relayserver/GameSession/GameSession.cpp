#include "GameSession.hpp"
#include "Logger.hpp"
#include "src/relayserver/handlers/Handlers.hpp"
#include <iostream>

using boost::asio::ip::tcp;

GameSession::GameSession(tcp::socket socket, std::shared_ptr<RelayServer> server)
        : socket_(std::move(socket)), server_(std::move(server)), read_buffer_(4096) {}

void GameSession::start() {
    auto ep = socket_.remote_endpoint();
    Logger::get()->debug("[relay_session][start] New connection from {}:{}",
                         ep.address().to_string(), ep.port());
    do_read();
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
                        //log->debug("[client_session][do_read] Client disconnected: {}", ec.message());
                    } else {
                        log->error("[relay_session][do_read] Read error: {}", ec.message());
                    }
                    close();
                    return;
                }

                if (!isOpened()) return;
                read_buffer_.write_completed(bytes_transferred);
                process_read_buffer();
                if (isOpened()) do_read();
            }
    );
}

void GameSession::process_read_buffer() {
    auto log = Logger::get();
    MessageBuffer &buffer = read_buffer();

    // Нужно минимум 4 байта: [Length(2)] + [Opcode(2)]
    if (buffer.get_active_size() < 4)
        return;

    const uint8_t *data = buffer.read_ptr();

    // Читаем Length (Big Endian)
    uint16_t size = static_cast<uint16_t>(data[0]) << 8 | static_cast<uint16_t>(data[1]);
    // Читаем Opcode (Big Endian)
    uint16_t opcode = static_cast<uint16_t>(data[2]) << 8 | static_cast<uint16_t>(data[3]);

    // Проверяем, что весь пакет в буфере
    if (buffer.get_active_size() < 4 + size - 2 /* минус 2 байта за opcode */)
        return;

    if (size > 2048) {
        log->error("WoWPacket payload too big: {}", size);
        close();
        return;
    }

    // Копируем полный пакет [Length][Opcode][Payload]
    std::vector<uint8_t> full_packet(data, data + 2 + size); // 2 байта Length + size

    // Сдвигаем read_ptr
    buffer.read_completed(2 + size);
    auto packet = std::make_shared<WoWPacket>();

    try {
        packet->deserialize(full_packet);
        Handlers::dispatch(shared_from_this(), packet);

    } catch (const std::exception &ex) {
        log->error("[GameSession][process_read_buffer] WoWPacket processing failed: {}", ex.what());
        close();
    }
}

/**
 * Обертка для безопасной отправки пакета из любого потока и корутины
 */
void GameSession::send_packet(const std::shared_ptr<const Packet>& packet) {
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
void GameSession::do_send_packet(const Packet &packet) {
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