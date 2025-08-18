#include "GameSession.hpp"
#include "Logger.hpp"
#include "src/relayserver/handlers/Handlers.hpp"
#include "Cryptography/CryptoRandom.hpp"
#include "Time/GameTime.hpp"
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
                        //log->debug("[client_session][do_read] Client disconnected: {}", ec.message());
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
    MessageBuffer& buffer = read_buffer();

    while (buffer.get_active_size() >= 6) { // Минимум 6 байт: size(2) + cmd(4)
        const uint8_t* data = buffer.read_ptr();
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
    std::array<uint8_t, 4> seed;
    Crypto::GetRandomBytes(seed);
    setAuthSeed(seed);

    auto random_bytes = Crypto::GetRandomBytes<32>();

    WoWPacket challenge_pkt(WoWOpcodes::SMSG_AUTH_CHALLENGE);
    challenge_pkt.write_uint32_le(1);
    challenge_pkt.write_bytes(seed.data(), 4);
    challenge_pkt.write_bytes(random_bytes.data(), random_bytes.size());

    send_packet(std::make_shared<WoWPacket>(challenge_pkt));
}

void GameSession::initCrypt(const std::array<uint8_t, 40>& key) {
    // Преобразуем 40-байтный ключ в формат SessionKey
    SessionKey sessionKey;
    std::copy(key.begin(), key.end(), sessionKey.begin());

    // Инициализируем шифрование
    authCrypt_.Init(sessionKey);
}

bool GameSession::handlePing(const std::shared_ptr<WoWPacket>& p) {
    uint32_t ping = p->read_uint32_le();
    uint32_t latency = p->read_uint32_le();
    using namespace std::chrono;

    if (lastPingTime_ == steady_clock::time_point())
    {
        lastPingTime_ = steady_clock::now();
    }
    else
    {
        steady_clock::time_point now = steady_clock::now();

        steady_clock::duration diff = now - lastPingTime_;

        lastPingTime_ = now;

        if (diff < seconds(27))
        {
            ++overSpeedPings_;

            //std::getenv("CONFIG_MAX_OVERSPEED_PINGS") = 2
            uint32_t maxAllowed = 2;

            if (maxAllowed && overSpeedPings_ > maxAllowed)
                return false;
        }
        else
            overSpeedPings_ = 0;
    }
    setLatency(latency);

    WoWPacket reply(WoWOpcodes::SMSG_PONG);
    reply.write_uint32_le(ping);
    send_packet(std::make_shared<WoWPacket>(reply));
    return true;
}

void GameSession::sendAccountDataTimes(uint32_t mask) {
    WoWPacket pkt(WoWOpcodes::SMSG_ACCOUNT_DATA_TIMES);
    pkt.write_uint32_le(GameTime::GetGameTime());    // Server time
    pkt.write_uint8(1);
    pkt.write_uint32_le(mask);                       // type mask
    for (uint32_t i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        if (mask & (1 << i))
            pkt.write_uint32_le(getAccountData(AccountDataType(i))->Time); // also unix time

    Packet::log_raw_payload("SMSG_ACCOUNT_DATA_TIMES", pkt.serialize());
    send_packet(std::make_shared<WoWPacket>(pkt));
}

boost::asio::awaitable<void> GameSession::setAccountData(AccountDataType type, time_t tm, std::string const& data)
{
    try {
        if ((1 << type) & GLOBAL_CACHE_MASK) {
            PreparedStatement stmt("REPLACE_ACCOUNT_DATA");
            stmt.set_param(0, getAccountId());
            stmt.set_param(1, type);
            stmt.set_param(2, uint32_t(tm));
            stmt.set_param(3, data);
            co_await server()->db()->execute_async_one<NothingRow>(stmt);
        } else {
            if (!getCharacterId().is_nil()) {
                PreparedStatement stmt("REPLACE_CHARACTER_ACCOUNT_DATA");
                stmt.set_param(0, getCharacterId());
                stmt.set_param(1, type);
                stmt.set_param(2, uint32_t(tm));
                stmt.set_param(3, data);
                co_await server()->db()->execute_async_one<NothingRow>(stmt);
            }
        }

        m_accountData[type].Time = tm;
        m_accountData[type].Data = data;
        co_return;
    } catch (const std::exception &ex) {
        Logger::get()->error("[GameSession]::setAccountData DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void> GameSession::loadAccountData(uint32_t mask) {
    auto log = Logger::get();
    log->debug("GameSession::loadAccountData called");
    try {
        for (uint32_t i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
            if (mask & (1 << i))
                m_accountData[i] = AccountData();

        PreparedStatement stmt("SELECT_ACCOUNT_DATA");
        stmt.set_param(0, getAccountId());
        auto rows = co_await server()->db()->execute_async_many<AccountDataRow>(stmt);
        if (!rows.empty()) {
            for (const auto &row: rows) {
                if (row.type >= NUM_ACCOUNT_DATA_TYPES)
                {
                    log->error("Table `{}` have invalid account data type ({}), ignore.",
                               mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", row.type);
                    continue;
                }

                if ((mask & (1 << row.type)) == 0)
                {
                    log->error("Table `{}` have non appropriate for table account data type ({}), ignore.",
                                 mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", row.type);
                    continue;
                }

                m_accountData[row.type].Time = time_t(row.time);
                std::string data(reinterpret_cast<const char*>(row.data.data()), row.data.size());
                m_accountData[row.type].Data = data;
            }
        }
        co_return;

    } catch (const std::exception &ex) {
        Logger::get()->error("[GameSession]::loadAccountData DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void> GameSession::loadTutorialsData() {
    auto log = Logger::get();
    log->debug("GameSession::loadTutorialsData called");

    try {
        memset(m_Tutorials, 0, sizeof(uint32_t) * MAX_ACCOUNT_TUTORIAL_VALUES);

        PreparedStatement stmt("SELECT_ACCOUNT_TUTORIALS");
        stmt.set_param(0, getAccountId());
        auto row = co_await server()->db()->execute_async_one<AccountTutorialRow>(stmt);
        if (row) {
            m_Tutorials[0] = row->tut0;
            m_Tutorials[1] = row->tut1;
            m_Tutorials[2] = row->tut2;
            m_Tutorials[3] = row->tut3;
            m_Tutorials[4] = row->tut4;
            m_Tutorials[5] = row->tut5;
            m_Tutorials[6] = row->tut6;
            m_Tutorials[7] = row->tut7;
            m_TutorialsChanged |= TUTORIALS_FLAG_LOADED_FROM_DB;
        }
        m_TutorialsChanged &= ~TUTORIALS_FLAG_CHANGED;
        co_return;

    } catch (const std::exception &ex) {
        Logger::get()->error("[GameSession]::loadTutorialsData DB exception: {}", ex.what());
        co_return;
    }
}

void GameSession::sendTutorialsData() {
    WoWPacket pkt(WoWOpcodes::SMSG_TUTORIAL_FLAGS);
    for (uint8_t i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        pkt.write_uint32_le(m_Tutorials[i]);

    Packet::log_raw_payload("SMSG_TUTORIAL_FLAGS", pkt.serialize());
    send_packet(std::make_shared<WoWPacket>(pkt));
}

void GameSession::sendAddonsInfo(const std::vector<uint8_t> &addonData) {
    // Формат пакета: [uint8: count][repeated: uint32: crc, uint8: flags]
    WoWPacket pkt(WoWOpcodes::SMSG_ADDON_INFO);

    // Простейшая реализация - без поддержки аддонов
    pkt.write_uint8(0); // Количество аддонов = 0

    Packet::log_raw_payload("SMSG_ADDON_INFO", pkt.serialize());
    send_packet(std::make_shared<WoWPacket>(pkt));
}

void GameSession::sendClientCacheVersion() {
    WoWPacket pkt(WoWOpcodes::SMSG_CLIENTCACHE_VERSION);
    pkt.write_uint32_le(server()->clientCacheVersion());

    Packet::log_raw_payload("SMSG_CLIENTCACHE_VERSION", pkt.serialize());
    send_packet(std::make_shared<WoWPacket>(pkt));
}