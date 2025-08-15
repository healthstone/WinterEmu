#include "HeartBeatHandlers.hpp"

#include <utility>
#include "utils/PacketUtils.hpp"
#include "enums/WoWOpcodes.hpp"

using namespace HeartBeatHandlers;

void HeartBeatHandlers::handlePing(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket>& p) {
    uint32_t ping = p->read_uint32_le();
    p->skip(2); //uint32_t latency = p->read_uint32_le();

    WoWPacket reply(WoWOpcodes::SMSG_PONG);
    reply.write_uint32_le(ping);

    PacketUtils::send_packet_as<WoWPacket>(std::move(session), reply);
}