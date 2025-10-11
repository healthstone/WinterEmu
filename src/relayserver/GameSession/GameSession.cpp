#include "GameSession.hpp"
#include "Cryptography/CryptoRandom.hpp"
#include "Logger.hpp"
#include "src/relayserver/handlers/Handlers.hpp"

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
    // Очищаем очередь пакетов при закрытии
    recv_queue_.clear();

    cleanBeforeDelete();

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

    if (auto srv = server_) {
        if (!getCurrentPlayerObjectGuid().IsEmpty())
            srv->removeSessionFromPlayerMap(getCurrentPlayerObjectGuid(), shared_from_this());
        srv->remove_session(shared_from_this());
    }
}

void GameSession::cleanBeforeDelete() {
    for (auto &i: m_accountData)
        i = AccountData();
    memset(m_Tutorials, 0, sizeof(uint32_t) * MAX_ACCOUNT_TUTORIAL_VALUES);
    legitCharacters_.clear();
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
                process_read_buffer(); // Дешифровка выполняется здесь
                if (isOpened()) do_read();
            }
    );
}

void GameSession::process_read_buffer() {
    auto log = Logger::get();
    MessageBuffer &buffer = read_buffer();

    while (buffer.get_active_size() >= 6) { // Минимум 6 байт: size(2) + cmd(4)
        const uint8_t *data = buffer.read_ptr();
        uint8_t header_buffer[6];
        std::memcpy(header_buffer, data, 6); // Копируем заголовок для дешифровки

        // Дешифруем только заголовок (6 байт)
        if (isAuthed()) {
            authCrypt_.DecryptRecv(header_buffer, 6);
        }

        // Чтение длины пакета (BE) из расшифрованного заголовка
        uint16_t total_length = (header_buffer[0] << 8) | header_buffer[1];

        // Чтение opcode (uint32 LE) из расшифрованного заголовка
        uint32_t full_opcode = header_buffer[2] |
                               (header_buffer[3] << 8) |
                               (header_buffer[4] << 16) |
                               (header_buffer[5] << 24);
        uint16_t opcode = full_opcode & 0xFFFF;

        log->debug("[process_read_buffer] Total length: {}, Full opcode: 0x{:08X}, Opcode: 0x{:04X}",
                   total_length, full_opcode, opcode);

        // Проверка размера пакета
        if (total_length > 10240) {
            log->error("Packet size too big: {}", total_length);
            close();
            return;
        }

        // Полный размер пакета = 2 (длина) + total_length
        size_t full_packet_size = 2 + total_length;

        if (buffer.get_active_size() < full_packet_size) {
            log->debug("Waiting for more data (need {} bytes, have {})",
                       full_packet_size, buffer.get_active_size());
            return;
        }

        try {
            // Копируем весь пакет
            std::vector<uint8_t> packet_data(data, data + full_packet_size);

            // Заменяем заголовок на расшифрованную версию
            std::memcpy(packet_data.data(), header_buffer, 6);

            buffer.read_completed(full_packet_size);

            auto packet = std::make_shared<WoWPacket>();
            packet->deserialize(packet_data);

            // Добавляем пакет в очередь вместо немедленной обработки
            recv_queue_.push_back(packet);
            schedule_processing();
        }
        catch (const std::exception &ex) {
            log->error("Packet processing failed: {}", ex.what());
            close();
            return;
        }
    }
}

/**
 * Обертка для безопасной отправки пакета из любого потока и корутины
 */
void GameSession::send_packet(const std::shared_ptr<const WoWPacket> &packet) {
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
    auto base_packet = packet.build_packet();
    std::vector<uint8_t> final_packet;

    if (base_packet.size() > 0x7FFF) {
        ByteBuffer temp;
        uint16_t total_size = static_cast<uint16_t>(base_packet.size());

        temp.write_uint8(0x80 | (0xFF & (total_size >> 16)));
        temp.write_uint8(0xFF & (total_size >> 8));
        temp.write_uint8(0xFF & total_size);
        temp.write_bytes(base_packet.data() + 2, base_packet.size() - 2);
        final_packet = temp.data();
    } else {
        final_packet = std::move(base_packet);
    }

    // Шифруем только первые 4 байта для исходящих пакетов
    if (isAuthed() && final_packet.size() >= 4) {
        uint8_t header_buffer[4];
        std::memcpy(header_buffer, final_packet.data(), 4);
        authCrypt_.EncryptSend(header_buffer, 4);
        std::memcpy(final_packet.data(), header_buffer, 4);
    }

    write_queue_.push_back(std::move(final_packet));
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

void GameSession::send_auth_challenge() {
    // Генерация seed ДОЛЖНА происходить здесь
    std::array<uint8_t, 4> seed{};
    Crypto::GetRandomBytes(seed);
    setAuthSeed(seed);

    auto random_bytes = Crypto::GetRandomBytes<32>();

    WoWPacket challenge_pkt(WoWOpcodes::SMSG_AUTH_CHALLENGE);
    challenge_pkt.write_uint32_le(1);
    challenge_pkt.write_bytes(seed.data(), 4);
    challenge_pkt.write_bytes(random_bytes.data(), random_bytes.size());

    send_packet(std::make_shared<WoWPacket>(challenge_pkt));
}

void GameSession::initCrypt(const std::array<uint8_t, 40> &key) {
    // Преобразуем 40-байтный ключ в формат SessionKey
    SessionKey sessionKey;
    std::copy(key.begin(), key.end(), sessionKey.begin());

    // Инициализируем шифрование
    authCrypt_.Init(sessionKey);
}

void GameSession::schedule_processing() {
    if (processing_queue_ || closed_) return;

    processing_queue_ = true;
    auto self = shared_from_this();
    boost::asio::post(
            socket_.get_executor(),
            [self]() {
                self->process_packets();
            }
    );
}

void GameSession::process_packets() {
    if (closed_) return;

    std::deque<std::shared_ptr<WoWPacket>> queue;
    std::swap(recv_queue_, queue);

    for (auto &packet: queue) {
        if (closed_) break;
        Handlers::dispatch(shared_from_this(), packet);
    }

    processing_queue_ = false;
    if (!recv_queue_.empty() && !closed_) {
        schedule_processing();
    }
}