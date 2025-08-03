#include "Handlers.hpp"
#include "src/relayserver/handlers/heartbeat/HeartBeatHandlers.hpp"
#include "enums/WoWOpcodes.hpp"

using namespace Handlers;

void Handlers::dispatch(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p) {
    WoWOpcodes opcode = p->get_opcode();
    switch (opcode) {
        case WoWOpcodes::CMSG_PING: HeartBeatHandlers::handlePing(session, p); break;
        default:
            Logger::get()->warn("[Handlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}