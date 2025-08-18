#include "MiscHandlers.hpp"
#include <zlib.h> // Для распаковки данных

/** CMSG_PING **/
void MiscHandlers::handleClientPingCommand(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    log->debug("MiscHandlers::handleClientPingCommand - CMSG_PING");

    uint32_t ping = p->read_uint32_le();
    uint32_t latency = p->read_uint32_le();

    using namespace std::chrono;

    TimePoint lastPingTime = session->getLastPingTime();
    uint32_t overSpeedPings = session->getOverSpeedPings();

    if (lastPingTime == steady_clock::time_point())
        lastPingTime = steady_clock::now();
    else {
        steady_clock::time_point now = steady_clock::now();
        steady_clock::duration diff = now - lastPingTime;
        lastPingTime = now;

        if (diff < seconds(27)) {
            ++overSpeedPings;
            //std::getenv("CONFIG_MAX_OVERSPEED_PINGS") = 2
            uint32_t maxAllowed = 2;
            if (maxAllowed && overSpeedPings > maxAllowed) {
                log->error("Handlers::handleClientPingCommand kicked for over-speed pings (address: {})",
                           session->GetRemoteIpAddress().to_string());
                session->close();
            }
        } else
            overSpeedPings = 0;
    }

    session->setLastPingTime(lastPingTime);
    session->setOverSpeedPings(overSpeedPings);
    session->setLatency(latency);

    WoWPacket reply(WoWOpcodes::SMSG_PONG);
    reply.write_uint32_le(ping);
    session->send_packet(std::make_shared<WoWPacket>(reply));
}

/** CMSG_READY_FOR_ACCOUNT_DATA_TIMES **/
void MiscHandlers::handleReadyForAccountDataTimes(const std::shared_ptr<GameSession> &session) {
    Logger::get()->debug("MiscHandlers::handleReadyForAccountDataTimes - CMSG_READY_FOR_ACCOUNT_DATA_TIMES");
    session->sendAccountDataTimes(GLOBAL_CACHE_MASK);
}

/** CMSG_UPDATE_ACCOUNT_DATA **/
boost::asio::awaitable<void>
MiscHandlers::handleUpdateAccountData(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    log->debug("MiscHandlers::handleUpdateAccountData - CMSG_UPDATE_ACCOUNT_DATA");

    try {
        // Читаем заголовочные данные
        uint32_t type = p->read_uint32_le();
        uint32_t timestamp = p->read_uint32_le();
        uint32_t decompressedSize = p->read_uint32_le();

        log->trace("UAD: type {}, time {}, decompressedSize {}", type, timestamp, decompressedSize);

        // Проверяем валидность типа данных
        if (type >= NUM_ACCOUNT_DATA_TYPES) {
            log->error("Invalid account data type: {}", type);
            co_return;
        }

        // Обработка случая очистки данных
        if (decompressedSize == 0) {
            // Очищаем данные (нужно реализовать метод в GameSession)
            co_await session->setAccountData(static_cast<AccountDataType>(type), timestamp, "");

            // Отправляем подтверждение
            WoWPacket response(WoWOpcodes::SMSG_UPDATE_ACCOUNT_DATA_COMPLETE);
            response.write_uint32_le(type);
            response.write_uint32_le(0); // result, 0=success
            session->send_packet(std::make_shared<WoWPacket>(response));
            co_return;
        }

        // Проверка размера
        if (decompressedSize > 0xFFFF) {
            log->error("UAD: Account data packet too big, size {}", decompressedSize);
            p->skip(p->remaining()); // Пропускаем оставшиеся данные
            co_return;
        }

        // Читаем сжатые данные
        auto compressedData = p->read_bytes(p->remaining());

        // Распаковываем данные
        std::vector<uint8_t> dest(decompressedSize);
        uLongf realSize = decompressedSize;

        int z_res = uncompress(
                dest.data(),
                &realSize,
                compressedData.data(),
                compressedData.size()
        );

        if (z_res != Z_OK) {
            log->error("UAD: Failed to decompress account data. Error: {}", z_res);
            co_return;
        }

        if (realSize != decompressedSize) {
            log->warn("UAD: Decompressed size {} does not match expected {}", realSize, decompressedSize);
        }

        // Преобразуем в строку
        std::string data(reinterpret_cast<const char *>(dest.data()), realSize);

        // Сохраняем данные (нужно реализовать метод в GameSession)
        co_await session->setAccountData(static_cast<AccountDataType>(type), timestamp, data);

        // Отправляем подтверждение
        WoWPacket response(WoWOpcodes::SMSG_UPDATE_ACCOUNT_DATA_COMPLETE);
        response.write_uint32_le(type);
        response.write_uint32_le(0); // result, 0=success
        session->send_packet(std::make_shared<WoWPacket>(response));
        co_return;
    } catch (const std::exception &ex) {
        log->error("Error handling CMSG_UPDATE_ACCOUNT_DATA: {}", ex.what());
        co_return;
    }
}

/** CMSG_CHAR_ENUM **/
void MiscHandlers::handleCharacterEnum(const std::shared_ptr<GameSession> &session) {
    Logger::get()->debug("MiscHandlers::handleCharacterEnum - CMSG_CHAR_ENUM");

    WoWPacket pkt(WoWOpcodes::SMSG_CHAR_ENUM);
    pkt.write_uint8(0); // Количество персонажей (0)
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

/** CMSG_REALM_SPLIT **/
void MiscHandlers::handleRealmSplitOpcode(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p) {
    Logger::get()->debug("MiscHandlers::handleRealmSplitOpcode - CMSG_REALM_SPLIT");

    uint32_t unk = p->read_uint32_le();
    std::string split_date = "01/01/01";

    WoWPacket pkt(WoWOpcodes::SMSG_REALM_SPLIT);
    pkt.write_uint32_le(unk);
    pkt.write_uint32_le(0x00000000);    // realm split state
    // split states:
    // 0x0 realm normal
    // 0x1 realm split
    // 0x2 realm split pending
    pkt.write_string_nt_be(split_date);
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}