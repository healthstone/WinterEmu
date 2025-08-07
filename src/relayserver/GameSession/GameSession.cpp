#include "GameSession.hpp"
#include "Logger.hpp"
#include "src/relayserver/handlers/Handlers.hpp"
#include "Cryptography/CryptoRandom.hpp"
#include <iostream>

using boost::asio::ip::tcp;

GameSession::GameSession(tcp::socket socket, std::shared_ptr<RelayServer> server)
        : socket_(std::move(socket)), server_(std::move(server)), read_buffer_(4096) {}

void GameSession::start() {
    auto ep = socket_.remote_endpoint();
    Logger::get()->debug("[relay_session][start] New connection from {}:{}",
                         ep.address().to_string(), ep.port());
    do_read();
    send_auth_challenge();
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
    MessageBuffer& buffer = read_buffer();

    while (buffer.get_active_size() >= 4) {
        const uint8_t* data = buffer.read_ptr();

        // Чтение длины пакета (BE) — включает OPCODE + PAYLOAD
        uint16_t total_length = (data[0] << 8) | data[1];

        // Для дебага — считываем OPCODE тоже (LE)
        uint16_t opcode = (static_cast<uint16_t>(data[3]) << 8) | data[2];

        log->debug("[process_read_buffer] Buffer size: {}, Total length: {}, Opcode: 0x{:04X}",
                   buffer.get_active_size(), total_length, opcode);

        // Безопасность: ограничение по длине
        if (total_length > 2048) {
            log->error("Packet size too big: {}", total_length);
            close();
            return;
        }

        // Пакет ещё не полностью получен
        if (buffer.get_active_size() < 2 + total_length) {
            log->debug("Waiting for more data (need {} bytes, have {})",
                       2 + total_length, buffer.get_active_size());
            return;
        }

        try {
            // Берём весь пакет (без 2 байт длины)
            std::vector<uint8_t> packet_data(data + 2, data + 2 + total_length);
            //Packet::log_raw_payload("process_read_buffer", packet_data);
            buffer.read_completed(2 + total_length);

            auto packet = std::make_shared<WoWPacket>();
            packet->deserialize(packet_data);

            Handlers::dispatch(shared_from_this(), packet);
        }
        catch (const std::exception& ex) {
            log->error("Packet processing failed: {}", ex.what());
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
    Crypto::GetRandomBytes(authSeed_);
    auto random_bytes = Crypto::GetRandomBytes<32>();

    WoWPacket challenge_pkt(WoWOpcodes::SMSG_AUTH_CHALLENGE);
    challenge_pkt.write_uint32_le(1);
    challenge_pkt.write_bytes(authSeed_.data(), 4);
    challenge_pkt.write_bytes(random_bytes.data(), random_bytes.size());

    Packet::log_raw_payload("send_auth_challenge::SMSG_AUTH_CHALLENGE", challenge_pkt.serialize());

    send_packet(std::make_shared<WoWPacket>(challenge_pkt));
}