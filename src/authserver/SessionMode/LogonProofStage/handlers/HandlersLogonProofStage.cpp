#include "HandlersLogonProofStage.hpp"
#include "packet/RawPacket.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "utils/PacketUtils.hpp"
#include "utils/NetUtils.hpp"
#include "src/authserver/Entity/AuthCodes/AuthCodes.hpp"

using namespace HandlersLogonProofStage;

boost::asio::awaitable<void> HandlersLogonProofStage::HandleLogonProof(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload)
{
    auto log = Logger::get();
    ByteBuffer buffer(payload);

    try {
        // 1. Проверка сессии и получение SRP объекта
        if (!session->_srp6) {
            log->error("[HandleLogonProof]: No SRP6 object in session");
            RawPacket failReply;
            failReply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF));
            failReply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_DISCONNECTED));
            failReply.write_uint16_le(0);
            PacketUtils::send_packet_as<RawPacket>(session, failReply);
            co_return;
        }
        Crypto::SRP6& srp = *session->_srp6;

        // 2. Разбор payload
        uint8_t cmd                   = buffer.read_uint8();
        Crypto::SRP6::EphemeralKey A  = buffer.read_bytes_as_array<32>();
        Crypto::SHA1::Digest clientM  = buffer.read_bytes_as_array<20>();
        Crypto::SHA1::Digest crc_hash = buffer.read_bytes_as_array<20>();
        uint8_t number_of_keys        = buffer.read_uint8();
        uint8_t securityFlags         = buffer.read_uint8();

        // 3. TODO Проверка версии клиента (заменить на свой метод)
//        if (session->getAccountInfo()->expversion == NO_VALID_EXP_FLAG) {
//            log->debug("Client with invalid version, patching not implemented");
//            co_return;
//        }

        // 4. Проверка SRP результата
        auto K_opt = srp.VerifyChallengeResponse(A, clientM);
        if (!K_opt) {
            log->warn("[HandleLogonProof] Invalid SRP proof for user {}", session->getAccountInfo()->Login);
            // Неверный пароль или SRP проверка не прошла
            RawPacket failReply;
            failReply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF));
            failReply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_UNKNOWN_ACCOUNT));
            failReply.write_uint16_le(0); // LoginFlags
            PacketUtils::send_packet_as<RawPacket>(session, failReply);
            co_return;
        }

        // 5. Сохраняем session key
        session->_sessionKey = *K_opt;

        // 6. TODO Проверка 2FA токена, если используется (пример)

        // 7. TODO Проверка версии клиента (логика ValidateVersion)
        log->trace("[HandleLogonProof] User '{}' successfully authenticated", session->getAccountInfo()->Login);

        // 8. Обновление записи в базе (асинхронно, пример)
        try {
            std::string ip_str = NetUtils::uint32_to_ip_be(session->socket().remote_endpoint().address().to_v4().to_uint());
            std::string safe_ip = ip_str.empty() ? "127.0.0.1" : ip_str;

            PreparedStatement stmt("UPDATE_LOGIN_LOGONPROOF");
            stmt.set_param(0, *K_opt);
            stmt.set_param(1, safe_ip);
            stmt.set_param(2, session->getAccountInfo()->Login);

            session->server()->db()->execute_sync_one<NothingRow>(stmt);
        } catch (const std::exception& e) {
            log->error("[HandleLogonProof] Database update error: {}", e.what());
            RawPacket failReply;
            failReply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF));
            failReply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_DB_BUSY));
            failReply.write_uint16_le(0); // LoginFlags
            PacketUtils::send_packet_as<RawPacket>(session, failReply);
            co_return;
        }

        // 9. Отправка подтверждения клиенту (M2)
        auto M2 = Crypto::SRP6::GetSessionVerifier(A, clientM, *K_opt);

        RawPacket packet;
        packet.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF));
        packet.write_uint8(0x00); // reserved or error = 0
        packet.write_bytes(M2.data(), M2.size());

        if (session->_expversion & POST_BC_EXP_FLAG) {  // 2.x and 3.x clients
            packet.write_uint32_le(0x00800000);         // AccountFlags
            packet.write_uint32_le(0);                  // SurveyId
            packet.write_uint16_le(0);                  // LoginFlags
        } else {
            packet.write_uint32_le(0);                  // unk2
        }

        session->set_session_mode(SessionMode::STATUS_WAITING_FOR_REALM_LIST);
        Packet::log_raw_payload("REQUEST AUTH_LOGON_PROOF", payload);
        Packet::log_raw_payload("RESPONSE AUTH_LOGON_PROOF", packet.serialize());
        PacketUtils::send_packet_as<RawPacket>(session, packet);
        co_return;
    }
    catch (const std::exception& ex) {
        log->error("[HandleLogonProof] Exception in read packet: {}", ex.what());
        RawPacket failReply;
        failReply.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_LOGON_PROOF));
        failReply.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_DISCONNECTED));
        failReply.write_uint16_le(0);
        PacketUtils::send_packet_as<RawPacket>(session, failReply);
        co_return;
    }
}