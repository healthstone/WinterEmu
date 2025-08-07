#include "AuthHandlers.hpp"
#include "utils/PacketUtils.hpp"
#include "utils/HexUtils.hpp"

using namespace AuthHandlers;

void AuthHandlers::handleAuthPacket(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket>& p) {
    auto log = Logger::get();
    log->debug("handleAuthPacket called");
    auto AuthSessionData = ReadPacketFields(p);
    if (!AuthSessionData) {
        sendAuthResponse(session, ResponseCodes::AUTH_UNKNOWN_ACCOUNT);
        return;
    }

    session->setClientSeed(AuthSessionData->client_seed);
    session->setNeedCrypt(true);
}

std::optional<AuthSessionData> AuthHandlers::ReadPacketFields(const std::shared_ptr<WoWPacket>& p) {
    Packet::log_raw_payload("CMSG_AUTH_SESSION", p->serialize());
    auto log = Logger::get();
    try {
        AuthSessionData asd;
        p->skip(2);
        asd.client_build = p->read_uint32_le();
        asd.login_server_id = p->read_uint32_le();
        asd.accountName = p->read_string_nt_le();
        asd.login_server_type = p->read_uint32_le();
        asd.client_seed = p->read_uint32_le();
        asd.region_id = p->read_uint32_le();
        asd.battleground_id = p->read_uint32_le();
        asd.realm_id = p->read_uint32_le();
        asd.dos_response = p->read_uint32_le();
        asd.digest = p->read_bytes_as_array<20>();
        if (p->remaining() > 0) {
            asd.addonData = p->read_bytes(p->remaining());
        }

        std::string digest_str = HexUtils::byte_array_to_hex(asd.digest);
        log->debug("[AuthHandlers][ReadPacketFields] Parsed AuthSessionData:\n"
                   "  client_build: {}\n"
                   "  login_server_id: {}\n"
                   "  accountName: '{}'\n"
                   "  login_server_type: {}\n"
                   "  client_seed: {}\n"
                   "  region_id: {}\n"
                   "  battleground_id: {}\n"
                   "  realm_id: {}\n"
                   "  dos_response: {}\n"
                   "  digest: {}\n"
                   "  addonData size: {} bytes",
                   asd.client_build,
                   asd.login_server_id,
                   asd.accountName,
                   asd.login_server_type,
                   asd.client_seed,
                   asd.region_id,
                   asd.battleground_id,
                   asd.realm_id,
                   asd.dos_response,
                   digest_str,
                   asd.addonData.size());

        return asd;
    } catch (const std::exception &ex) {
        log->error("[AuthHandlers][ReadPacketFields] Exception while parsing: {}", ex.what());
        return std::nullopt;
    }
}

void AuthHandlers::sendAuthResponse(const std::shared_ptr<GameSession>& session, ResponseCodes code) {
    WoWPacket pkt(WoWOpcodes::SMSG_AUTH_RESPONSE);
    pkt.write_uint8(static_cast<uint8_t>(code));
    PacketUtils::send_packet_as<WoWPacket>(session, pkt);
}