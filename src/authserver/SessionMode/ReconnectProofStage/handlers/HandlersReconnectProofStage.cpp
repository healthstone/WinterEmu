#include "HandlersReconnectProofStage.hpp"
#include "src/authserver/Entity/ClientBuildInfo/ClientBuildInfo.hpp"
#include "packet/RawPacket.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "utils/PacketUtils.hpp"

using namespace HandlersReconnectProofStage;

/** AUTH_RECONNECT_PROOF **/
void HandlersReconnectProofStage::HandleReconnectProof(std::shared_ptr<AuthSession> session,
                                                       const std::shared_ptr<std::vector<uint8_t>>& payload) {
    auto log = Logger::get();
    ByteBuffer buffer(*payload);
    session->set_session_mode(SessionMode::STATUS_CLOSED);

    try {
        // 1. Разбор payload
        uint8_t cmd = buffer.read_uint8();
        std::array<uint8_t, 16> R1_array = buffer.read_bytes_as_array<16>();  // uint8_t R1[16]
        Crypto::SHA1::Digest R2 = buffer.read_bytes_as_array<20>();
        Crypto::SHA1::Digest R3 = buffer.read_bytes_as_array<20>();
        uint8_t number_of_keys = buffer.read_uint8();

        if (session->getAccountInfo()->Login.empty())
            return;

        uint8_t R1[16];
        std::memcpy(R1, R1_array.data(), 16);

        Crypto::SHA1 sha;
        sha.UpdateData(session->getAccountInfo()->Login);
        sha.UpdateData(R1, 16);
        sha.UpdateData(session->_reconnectProof);
        sha.UpdateData(session->_sessionKey);
        sha.Finalize();

        if (sha.GetDigest() == R2) {
            if (!VerifyVersion(session, R1, sizeof(R1), R3, true)) {
                RawPacket packet;
                packet.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_RECONNECT_PROOF));
                packet.write_uint8(static_cast<uint8_t>(AuthResult::WOW_FAIL_VERSION_INVALID));
                PacketUtils::send_packet_as<RawPacket>(std::move(session), packet);
                return;
            }

            // Sending response
            RawPacket packet;
            packet.write_uint8(static_cast<uint8_t>(AuthCmd::AUTH_RECONNECT_PROOF));
            packet.write_uint8(static_cast<uint8_t>(AuthResult::WOW_SUCCESS));
            packet.write_uint16_le(0);  // LoginFlags, 1 has account message

            session->set_session_mode(SessionMode::STATUS_WAITING_FOR_REALM_LIST);
            //Packet::log_raw_payload("REQUEST  AUTH_RECONNECT_PROOF", *payload);
            //Packet::log_raw_payload("RESPONSE AUTH_RECONNECT_PROOF", packet.serialize());

            PacketUtils::send_packet_as<RawPacket>(std::move(session), packet);
            return;
        } else {
            log->error("[HandleReconnectProof] '{}:{}' [ERROR] user {} tried to login, but session is invalid",
                       session->GetRemoteIpAddress().to_string(), session->GetRemotePort(), session->getAccountInfo()->Login);
            return;
        }

    } catch (const std::exception &ex) {
        log->error("[HandleReconnectProof] Exception in read packet: {}", ex.what());
    }
}

bool HandlersReconnectProofStage::VerifyVersion(std::shared_ptr<AuthSession> session,
                                                uint8_t const *a, int32_t aLength,
                                                Crypto::SHA1::Digest const &versionProof, bool isReconnect) {
//    if (!sConfigMgr->GetBoolDefault("StrictVersionCheck", false))
//        return true;

    Crypto::SHA1::Digest zeros = {};
    Crypto::SHA1::Digest const *versionHash = nullptr;
    if (!isReconnect) {
        ClientBuild::Info const *buildInfo = ClientBuild::GetBuildInfo(session->_build);
        if (!buildInfo)
            return false;

        auto platformItr = std::ranges::find(buildInfo->ExecutableHashes, ClientBuild::ToFourCC(session->_os),
                                             &ClientBuild::ExecutableHash::Platform);
        if (platformItr == buildInfo->ExecutableHashes.end())
            return true;                                                            // not filled serverside

        versionHash = &platformItr->Hash;
    } else
        versionHash = &zeros;

    Crypto::SHA1 version;
    version.UpdateData(a, aLength);
    version.UpdateData(*versionHash);
    version.Finalize();

    return (versionProof == version.GetDigest());
}