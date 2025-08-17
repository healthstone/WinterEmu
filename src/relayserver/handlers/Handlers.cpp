#include "Handlers.hpp"
#include "enums/WoWOpcodes.hpp"
#include "src/relayserver/handlers/authsession/AuthHandlers.hpp"
#include "utils/PacketUtils.hpp"
#include "Time/GameTime.hpp"
#include <zlib.h> // Для распаковки данных

#define GLOBAL_CACHE_MASK           0x15

using namespace Handlers;

void Handlers::dispatch(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    WoWOpcodes opcode = p->get_opcode();
    switch (opcode) {
        case WoWOpcodes::CMSG_PING: {
            handleClientPingCommand(session, p);
            break;
        }
        case WoWOpcodes::CMSG_AUTH_SESSION: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await AuthHandlers::handleAuthPacket(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_READY_FOR_ACCOUNT_DATA_TIMES: {
            handleReadyForAccountDataTimes(session);
            break;
        }
        case WoWOpcodes::CMSG_UPDATE_ACCOUNT_DATA: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await handleUpdateAccountData(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_CHAR_ENUM: {
            handleCharacterEnum(session);
            break;
        }
        case WoWOpcodes::CMSG_REALM_SPLIT: {
            handleRealmSplitOpcode(session, p);
            break;
        }
        default:
            Logger::get()->warn("[Handlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}

void Handlers::handleClientPingCommand(const std::shared_ptr<GameSession>& session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    log->debug("CMSG_PING called");
    if (!session->handlePing(p)) {
        log->error("Handlers::handleClientPingCommand kicked for over-speed pings (address: {})", session->GetRemoteIpAddress().to_string());
        session->close();
    }
}

void Handlers::handleReadyForAccountDataTimes(const std::shared_ptr<GameSession>& session) {
    Logger::get()->debug("CMSG_READY_FOR_ACCOUNT_DATA_TIMES called");
    session->sendAccountDataTimes(GLOBAL_CACHE_MASK);
}

boost::asio::awaitable<void> Handlers::handleUpdateAccountData(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    log->debug("CMSG_UPDATE_ACCOUNT_DATA");

    try {
        // Читаем заголовочные данные
        uint32_t type = p->read_uint32_le();
        uint32_t timestamp = p->read_uint32_le();
        uint32_t decompressedSize = p->read_uint32_le();

        log->debug("UAD: type {}, time {}, decompressedSize {}", type, timestamp, decompressedSize);

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
        std::string data(reinterpret_cast<const char*>(dest.data()), realSize);

        // Сохраняем данные (нужно реализовать метод в GameSession)
        co_await session->setAccountData(static_cast<AccountDataType>(type), timestamp, data);

        // Отправляем подтверждение
        WoWPacket response(WoWOpcodes::SMSG_UPDATE_ACCOUNT_DATA_COMPLETE);
        response.write_uint32_le(type);
        response.write_uint32_le(0); // result, 0=success
        session->send_packet(std::make_shared<WoWPacket>(response));
        co_return;
    } catch (const std::exception& ex) {
        log->error("Error handling CMSG_UPDATE_ACCOUNT_DATA: {}", ex.what());
        co_return;
    }
}

void Handlers::handleCharacterEnum(const std::shared_ptr<GameSession>& session) {
    Logger::get()->info("CMSG_CHAR_ENUM called");

    WoWPacket pkt(WoWOpcodes::SMSG_CHAR_ENUM);
    pkt.write_uint8(0); // Количество персонажей (0)
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

void Handlers::handleRealmSplitOpcode(const std::shared_ptr<GameSession>& session, std::shared_ptr<WoWPacket> p) {
    Logger::get()->debug("CMSG_REALM_SPLIT called");

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