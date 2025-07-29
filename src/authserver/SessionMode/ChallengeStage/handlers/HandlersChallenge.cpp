#include "HandlersChallenge.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/LoginResult.hpp"
#include "utils/NetUtils.hpp"

using namespace HandlersChallenge;

void HandlersChallenge::HandleLogonChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {
    auto log = Logger::get();
    ByteBuffer buffer(payload);

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

        log->debug(
                "AUTH_LOGON_CHALLENGE: cmd=0x{:02X} size={} Game: {} Version: {}.{}.{} Build: {} Platform: {} OS: {} Country: {} TZ: {} IP: {} Username: {}",
                cmd, size, gamename, v1, v2, v3, build, platform, os, country, timezone_bias, NetUtils::uint32_to_ip_le(ip), username);

        Packet::log_raw_payload("AUTH_LOGON_CHALLENGE", payload);

        // Дальше — твоя логика SRP6, проверки и т.д.
        // session->set_auth_status(...); и отправка challenge

    } catch (const std::exception& ex) {
        log->error("[HandleLogonChallenge] Exception while parsing: {}", ex.what());
        session->close();
    }
}

void HandlersChallenge::HandleReconnectChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {
    auto log = Logger::get();
    ByteBuffer buffer(payload);

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

        log->debug(
                "AUTH_RECONNECT_CHALLENGE: cmd=0x{:02X} size={} Game: {} Version: {}.{}.{} Build: {} Platform: {} OS: {} Country: {} TZ: {} IP: {} Username: {}",
                cmd, size, gamename, v1, v2, v3, build, platform, os, country, timezone_bias, NetUtils::uint32_to_ip_le(ip), username);

        Packet::log_raw_payload("AUTH_RECONNECT_CHALLENGE", payload);

        // Дальше — твоя логика SRP6, проверки и т.д.
        // session->set_auth_status(...); и отправка challenge

    } catch (const std::exception& ex) {
        log->error("[HandleLogonChallenge] Exception while parsing: {}", ex.what());
        session->close();
    }
}
