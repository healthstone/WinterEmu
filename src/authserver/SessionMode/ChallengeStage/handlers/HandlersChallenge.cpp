#include "HandlersChallenge.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/LoginResult.hpp"
#include "utils/NetUtils.hpp"
#include "utils/utf8utils/UTF8Utils.hpp"
#include "packet/RawPacket.hpp"
#include "utils/PacketUtils.hpp"
#include "src/authserver/Entity/AuthCodes/AuthCodes.hpp"

using namespace HandlersChallenge;

uint8_t VersionChallenge[16] = {
        0xBA, 0xA3, 0x1E, 0x99, 0xA0, 0x0B, 0x21, 0x57,
        0xFC, 0x37, 0x3F, 0xB3, 0x69, 0xCD, 0xD2, 0xF1
};

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
        std::string accountName = buffer.read_string_raw_le(I_len);

        log->trace(
                "{}: cmd=0x{:02X} size={} Game: {} Version: {}.{}.{} Build: {} Platform: {} OS: {} Country: {} TZ: {} IP: {} Username: {}",
                opcode_name, cmd, size, gamename, v1, v2, v3, build, platform, os, country, timezone_bias, NetUtils::uint32_to_ip_le(ip), accountName);

        //Packet::log_raw_payload(opcode_name, payload);

        // 1 - проверка на UTF8
        if (!UTF8Utils::is_valid_utf8(accountName)) {
            log->error("[HandleLogonChallenge] {} - Invalid UTF-8 accountName received: {}", opcode_name, accountName);
            // Обработка ошибки, например отказ
            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT));
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        // 2 - делаем верхний регистр
        accountName = UTF8Utils::to_uppercase(accountName);

        // 2.5 - кикаем все сессии, где уже авторизованы под данным логином + откидываем этого с оповещением, что уже авторизован
        if (session->server()->disconnectSessionIfExists(accountName)) {
            log->warn("[HandleLogonChallenge] Duplicate session for AccID: {}", accountName);

            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);
            reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_ALREADY_ONLINE));
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        session->getAccountInfo()->Login = accountName;
        session->_build = build;
        session->_expversion = uint8_t(AuthHelper::IsPostBCAcceptedClientBuild(build) ? POST_BC_EXP_FLAG : (AuthHelper::IsPreBCAcceptedClientBuild(build) ? PRE_BC_EXP_FLAG : NO_VALID_EXP_FLAG));

        session->_timezoneOffset = timezone_bias;
        session->_localizationName = country;

        auto cache = session->server()->account_cache();
        auto cached_user_opt = cache->get(accountName);

        uint8_t securityFlags = 0;

        // 3 - пробуем взять из кэша
        if (cached_user_opt) {
            auto &cached_user = *cached_user_opt;
            session->getAccountInfo()->AccountID = cached_user.accountID;

            // Инициализация SRP6 с salt и verifier
            session->_srp6.emplace(accountName, cached_user.salt, cached_user.verifier);
            auto& srp = *session->_srp6;

            log->trace("[HandleLogonChallenge] AccID: {} opcode: {} B.size={}, g={}, N.size={}, salt.size={}",
                       cached_user.accountID,
                       opcode_name,
                       srp.B.size(),
                       Crypto::SRP6::g[0],
                       Crypto::SRP6::N.size(),
                       cached_user.salt.size());

            RawPacket reply;
            reply.write_uint8(0x00); // AUTH_LOGON_CHALLENGE opcode ID
            reply.write_uint8(0x00); // reserved
            reply.write_uint8(0x00); // WOW_SUCCESS

            reply.write_bytes(srp.B.data(), srp.B.size());

            reply.write_uint8(static_cast<uint8_t>(Crypto::SRP6::g.size())); // length of g
            reply.write_uint8(Crypto::SRP6::g[0]); // g

            reply.write_uint8(static_cast<uint8_t>(Crypto::SRP6::N.size()));
            reply.write_bytes(Crypto::SRP6::N.data(), Crypto::SRP6::N.size());

            reply.write_bytes(srp.s.data(), srp.s.size());

            reply.write_bytes(VersionChallenge, 16);
            reply.write_uint8(0x00); // securityFlags

            Packet::log_raw_payload("REQUEST " + opcode_name, payload);
            Packet::log_raw_payload("RESPONSE " + opcode_name, reply.serialize());

            session->set_session_mode(SessionMode::STATUS_LOGON_PROOF);
            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }

        // 4 - лезем в бд
        try {
            PreparedStatement stmt("SELECT_ACCOUNT_BY_USERNAME");
            stmt.set_param(0, accountName);

            auto user = session->server()->db()->execute_sync_one<AccountsRow>(stmt);

            // 5 - если нет аккаунта
            if (!user) {
                log->error("[HandleLogonChallenge] Account {} not found", accountName);

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
                log->error("[HandleLogonChallenge] Account {} has invalid salt/verifier length", accountName);

                RawPacket reply;
                reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
                reply.write_uint8(0);
                reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_INCORRECT_PASSWORD));
                PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
                co_return;
            }

            session->getAccountInfo()->AccountID = user->id;

            AccountCache::AccountCacheEntry cacheEntry;
            cacheEntry.accountID = user->id;
            cacheEntry.salt = *user->salt;
            cacheEntry.verifier = *user->verifier;
            cache->put(accountName, cacheEntry);

            session->_srp6.emplace(accountName, *user->salt, *user->verifier);
            auto& srp = *session->_srp6;
            log->trace("[HandleLogonChallenge] AccID: {}, opcode: {} B.size={}, g={}, N.size={}, salt.size={}",
                       user->id,
                       opcode_name,
                       srp.B.size(),
                       Crypto::SRP6::g[0],
                       Crypto::SRP6::N.size(),
                       user->salt->size());

            RawPacket reply;
            reply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_CHALLENGE));
            reply.write_uint8(0);

            if (AuthHelper::IsAcceptedClientBuild(session->_build)) {
                reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_SUCCESS));

                reply.write_bytes(srp.B.data(), srp.B.size());

                reply.write_uint8(static_cast<uint8_t>(Crypto::SRP6::g.size()));
                reply.write_uint8(Crypto::SRP6::g[0]);

                reply.write_uint8(static_cast<uint8_t>(Crypto::SRP6::N.size()));
                reply.write_bytes(Crypto::SRP6::N.data(), Crypto::SRP6::N.size());

                reply.write_bytes(srp.s.data(), srp.s.size());

                reply.write_bytes(VersionChallenge, 16);
                reply.write_uint8(0x00);

                session->set_session_mode(SessionMode::STATUS_LOGON_PROOF);
            }
            else {
                reply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_VERSION_INVALID));
            }

            // 👉 Лог и отправка
            Packet::log_raw_payload("REQUEST " + opcode_name, payload);
            Packet::log_raw_payload("RESPONSE " + opcode_name, reply.serialize());

            PacketUtils::send_packet_as<RawPacket>(std::move(session), reply);
            co_return;
        }
        catch (const std::exception &ex) {
            log->error("[HandleLogonChallenge] DB exception {}: {}", opcode_name, ex.what());
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