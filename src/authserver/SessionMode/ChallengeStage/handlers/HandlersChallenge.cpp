#include "HandlersChallenge.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/LoginResult.hpp"
#include "utils/NetUtils.hpp"
#include "utils/utf8utils/UTF8Utils.hpp"
#include "packet/RawPacket.hpp"
#include "utils/PacketUtils.hpp"

using namespace HandlersChallenge;

std::array<uint8_t, 16> VersionChallenge = { { 0xBA, 0xA3, 0x1E, 0x99, 0xA0, 0x0B, 0x21, 0x57, 0xFC, 0x37, 0x3F, 0xB3, 0x69, 0xCD, 0xD2, 0xF1 } };

boost::asio::awaitable<void> HandlersChallenge::HandleLogonChallenge(const std::string &opcode_name, std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {
    auto log = Logger::get();
    ByteBuffer buffer(payload);

    // 1 - читаем поля
    try {
        uint8_t cmd = buffer.read_uint8();
        uint8_t error = buffer.read_uint8();
        uint16_t size = buffer.read_uint16_le();  // или be, смотри по протоколу!

        std::string gamename = buffer.read_string_raw_le(4);
        uint8_t v1 = buffer.read_uint8();
        uint8_t v2 = buffer.read_uint8();
        uint8_t v3 = buffer.read_uint8();
        uint16_t build = buffer.read_uint16_le();
        std::string platform = buffer.read_string_raw_le(4);
        std::string os = buffer.read_string_raw_le(4);
        std::string country = buffer.read_string_raw_le(4);
        uint32_t timezone_bias = buffer.read_uint32_le();
        uint32_t ip = buffer.read_uint32_le();
        uint8_t I_len = buffer.read_uint8();
        std::string username = buffer.read_string_raw_le(I_len);

        log->trace(
                "{}: cmd=0x{:02X} size={} Game: {} Version: {}.{}.{} Build: {} Platform: {} OS: {} Country: {} TZ: {} IP: {} Username: {}",
                opcode_name, cmd, size, gamename, v1, v2, v3, build, platform, os, country, timezone_bias, NetUtils::uint32_to_ip_le(ip), username);

        Packet::log_raw_payload(opcode_name, payload);

        // 1 - проверка на UTF8
        if (!UTF8Utils::is_valid_utf8(username)) {
            log->error("[HandleLogonChallenge] {} - Invalid UTF-8 username received: {}", opcode_name, username);
            // Обработка ошибки, например отказ
            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT));
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        // 2 - делаем верхний регистр
        username = UTF8Utils::to_uppercase(username);

        // 2.5 - кикаем все сессии, где уже авторизованы под данным логином + откидываем этого с оповещением, что уже авторизован
        if (session->server()->disconnectSessionIfExists(username)) {
            log->warn("[HandleLogonChallenge] Duplicate session for '{}'", username);

            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_ALREADY_ONLINE));
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        session->getAccountInfo()->setUserName(username);

        auto cache = session->server()->account_cache();
        auto cached_user_opt = cache->get(username);

        auto auth = session->getAccountInfo();
        auto srp = auth->srp();

        uint8_t securityFlags = 0;

        // 3 - пробуем взять из кэша
        if (cached_user_opt) {
            auto &cached_user = *cached_user_opt;

            // Загружаем salt и verifier из кэша
            srp->LoadVerifier(cached_user.salt, cached_user.verifier);
            // Генерируем B
            srp->GenerateServerEphemeral();

            log->debug("[HandleLogonChallenge] {}: B.size={}, g={}, N.size={}, salt.size={}",
                       opcode_name,
                       srp->GetBBytes().size(),
                       srp->GetGenerator(),
                       srp->GetNBytes().size(),
                       cached_user.salt.size());

            RawPacket reply;
            reply.write_uint8(0x00); // AUTH_LOGON_CHALLENGE opcode ID
            reply.write_uint8(0x00); // reserved
            reply.write_uint8(0x00); // WOW_SUCCESS

            auto B_bytes = srp->GetBBytes();
            reply.write_bytes(B_bytes.data(), B_bytes.size());

            reply.write_uint8(1); // length of g
            reply.write_uint8(srp->GetGenerator());

            auto N_bytes = srp->GetNBytes();
            reply.write_uint8(static_cast<uint8_t>(N_bytes.size()));
            reply.write_bytes(N_bytes.data(), N_bytes.size());

            auto salt = srp->GetSalt();
            reply.write_bytes(salt.data(), salt.size());

            // Твоя версия клиента (16 байт), например:
            uint8_t version[16] = {
                    0xBA, 0xA3, 0x1E, 0x99, 0xA0, 0x0B, 0x21, 0x57,
                    0xFC, 0x37, 0x3F, 0xB3, 0x69, 0xCD, 0xD2, 0xF1
            };
            reply.write_bytes(version, 16);
            reply.write_uint8(0x00); // securityFlags
            Packet::log_raw_payload("cache reply " + opcode_name, reply.serialize());

            session->set_session_mode(SessionMode::STATUS_LOGON_PROOF);

            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        // 4 - лезем в бд
        try {
            PreparedStatement stmt("SELECT_ACCOUNT_BY_USERNAME");
            stmt.set_param(0, username);

            auto user = session->server()->db()->execute_sync<AccountsRow>(stmt);

            // 5 - если нет аккаунта
            if (!user) {
                log->error("[HandleLogonChallenge] User '{}' not found", username);

                RawPacket reply;
                reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
                reply.write_uint8(0);
                reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_NO_GAME_ACCOUNT));
                PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
                co_return;
            }

            // 6 --- Проверка salt и verifier ---
            if (!user->salt.has_value() || !user->verifier.has_value() ||
                user->salt->size() != 32 || user->verifier->size() != 32) {
                log->error("[HandleLogonChallenge] User '{}' has invalid salt/verifier length", username);

                RawPacket reply;
                reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
                reply.write_uint8(0);
                reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_INCORRECT_PASSWORD));
                PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
                co_return;
            }

            AccountCache::AccountCacheEntry cacheEntry;
            cacheEntry.salt = *user->salt;
            cacheEntry.verifier = *user->verifier;
            cache->put(username, cacheEntry);

            // 7 --- Инициализация SRP ---
            // Загружаем salt и verifier из БД (из user)
            srp->LoadVerifier(*user->salt, *user->verifier);
            // Генерируем ephemeral B
            srp->GenerateServerEphemeral();

            log->debug("[HandleLogonChallenge] {}: B.size={}, g={}, N.size={}, salt.size={}",
                       opcode_name,
                       srp->GetBBytes().size(),
                       srp->GetGenerator(),
                       srp->GetNBytes().size(),
                       user->salt->size());

            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE)); // AUTH_LOGON_CHALLENGE opcode ID
            reply.write_uint8(0x00); // reserved
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_SUCCESS)); // WOW_SUCCESS

            auto B_bytes = srp->GetBBytes();
            reply.write_bytes(B_bytes.data(), B_bytes.size());

            reply.write_uint8(1); // length of g
            reply.write_uint8(srp->GetGenerator());

            auto N_bytes = srp->GetNBytes();
            reply.write_uint8(static_cast<uint8_t>(N_bytes.size()));
            reply.write_bytes(N_bytes.data(), N_bytes.size());

            auto salt = srp->GetSalt();
            reply.write_bytes(salt.data(), salt.size());

            // Твоя версия клиента (16 байт), например:
            uint8_t version[16] = {
                    0xBA, 0xA3, 0x1E, 0x99, 0xA0, 0x0B, 0x21, 0x57,
                    0xFC, 0x37, 0x3F, 0xB3, 0x69, 0xCD, 0xD2, 0xF1
            };
            reply.write_bytes(version, 16);
            reply.write_uint8(0x00); // securityFlags

            // 👉 Лог и отправка
            Packet::log_raw_payload("AUTH_LOGON_CHALLENGE", reply.serialize());
            session->set_session_mode(SessionMode::STATUS_LOGON_PROOF);

            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);

            co_return;
        }
        catch (const std::exception &ex) {
            log->error("[HandlersAuth] {}: {}", opcode_name, ex.what());
            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_DB_BUSY));
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

    } catch (const std::exception& ex) {
        log->error("[HandleLogonChallenge] Exception while parsing: {}", ex.what());
        // Обработка ошибки, например отказ
        RawPacket reply;
        reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
        reply.write_uint8(0);
        reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_DISCONNECTED));
        PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
        co_return;
    }
}