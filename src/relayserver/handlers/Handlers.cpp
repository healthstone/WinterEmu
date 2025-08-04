#include "Handlers.hpp"
#include "enums/WoWOpcodes.hpp"
#include "src/relayserver/handlers/heartbeat/HeartBeatHandlers.hpp"
#include "src/relayserver/handlers/authsession/AuthHandlers.hpp"

using namespace Handlers;

void Handlers::dispatch(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p) {
    WoWOpcodes opcode = p->get_opcode();
    switch (opcode) {
        case WoWOpcodes::CMSG_PING: HeartBeatHandlers::handlePing(session, p); break;
        case WoWOpcodes::CMSG_AUTH_SESSION: AuthHandlers::handleAuthPacket(session, p); break;
        default:
            Logger::get()->warn("[Handlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}