#include "ClientSession.hpp"
#include "Logger.hpp"
#include "src/authserver/SessionMode/ChallengeStage/reader/ReaderChallengeStage.hpp"
#include "src/authserver/SessionMode/LogonProofStage/reader/ReaderLogonProofStage.hpp"
#include "src/authserver/SessionMode/ReconnectProofStage/reader/ReaderReconnectProofStage.hpp"
#include "src/authserver/SessionMode/RealmStage//reader/ReaderRealmStage.hpp"
#include <iostream>

using boost::asio::ip::tcp;

ClientSession::ClientSession(tcp::socket socket, std::shared_ptr<AuthServer> server)
        : socket_(std::move(socket)), server_(std::move(server)), read_buffer_(4096) {}

void ClientSession::start() {
    auto ep = socket_.remote_endpoint();
    Logger::get()->debug("[client_session][start] New connection from {}:{}",
                         ep.address().to_string(), ep.port());

    set_session_mode(SessionMode::STATUS_CHALLENGE);  // Начинаем с STATUS_CHALLENGE
    do_read();
}

void ClientSession::close() {
    if (closed_.exchange(true)) return;

    if (getAccountInfo())
        getAccountInfo()->handle_close_state();

    auto log = Logger::get();

    boost::system::error_code ec;
    socket_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[client_session][close] Failed to cancel socket: {}", ec.message());
    }

    socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    if (ec && ec != boost::asio::error::operation_aborted &&
        ec != boost::asio::error::eof &&
        ec != boost::asio::error::not_connected) {
        log->error("[client_session][close] Failed to shutdown socket: {}", ec.message());
    }
    socket_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[client_session][close] Failed to close socket: {}", ec.message());
    }

    read_buffer_.clear();
    write_queue_.clear();

    log->debug("[client_session][close] Socket closed. closed_={}", closed_.load());

    if (server_) {
        server_->remove_session(shared_from_this());
    }
}

void ClientSession::do_read() {
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
                        log->error("[client_session][do_read] Read error: {}", ec.message());
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

void ClientSession::process_read_buffer() {
    switch (session_mode_) {
        case SessionMode::STATUS_CHALLENGE:
            ReaderChallengeStage::process_read_buffer(shared_from_this());
            break;
        case SessionMode::STATUS_LOGON_PROOF:
            ReaderLogonProofStage::process_read_buffer(shared_from_this());
            break;
        case SessionMode::STATUS_RECONNECT_PROOF:
            ReaderReconnectProofStage::process_read_buffer(shared_from_this());
            break;
        case SessionMode::STATUS_WAITING_FOR_REALM_LIST:
            ReaderRealmStage::process_read_buffer(shared_from_this());
            break;
        default:
            const uint8_t* data = read_buffer_.read_ptr();
            std::vector<uint8_t> payload(data, data + read_buffer_.get_active_size());
            Packet::log_raw_payload("UNEXPECTED", payload);
            Logger::get()->error("[client_session][process_read_buffer] Unknown session mode!");
            break;
    }
}

/**
 * Обертка для безопасной отправки пакета из любого потока и корутины
 */
void ClientSession::send_packet(std::shared_ptr<const Packet> packet) {
    if (closed_) {
        Logger::get()->debug("[client_session][send_packet] called. closed_={}", closed_.load());
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
void ClientSession::do_send_packet(const Packet &packet) {
    std::vector<uint8_t> full_packet = packet.build_packet();
    write_queue_.push_back(std::move(full_packet));

    if (!writing_) {
        do_write();
    }
}

void ClientSession::do_write() {
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
                        log->error("[client_session] Write failed: {}", ec.message());
                    }
                    close();
                    return;
                }

                write_queue_.pop_front();
                do_write();
            }
    );
}