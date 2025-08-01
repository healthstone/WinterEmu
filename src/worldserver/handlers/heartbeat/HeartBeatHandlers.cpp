#include "HeartBeatHandlers.hpp"
#include "utils/PacketUtils.hpp"

using namespace HeartBeatHandlers;

void HeartBeatHandlers::handlePing(const std::shared_ptr<WorldSession>& session, const std::shared_ptr<WoWPacket> &p) {
    uint32_t ping = p->read_uint32_le();
    p->skip(2); //uint32_t latency = p->read_uint32_le();

    WoWPacket reply(WoWOpcodes::SMSG_PONG);
    reply.write_uint32_le(ping);

    PacketUtils::send_packet_as<WoWPacket>(session, reply);
}