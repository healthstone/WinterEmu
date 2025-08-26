#include "NHandlers.hpp"
#include "utils/PacketUtils.hpp"
#include "src/nodeserver/handlers/MiscHandlers/MiscHandlers.hpp"
#include "src/nodeserver/handlers/PlayerHandlers/PlayerHandlers.hpp"

using namespace NHandlers;

void NHandlers::dispatch(std::shared_ptr<NodeSession> session, std::shared_ptr<NodePacket> p) {
    NodeOpcodes opcode = p->opcode();
    switch (opcode) {
        case NodeOpcodes::REL_TO_NODE_PING:
            MiscHandlers::handle_ping(session, p);
            break;
        case NodeOpcodes::REL_TO_NODE_WOWPACKET:
            PlayerHandlers::handleDispatchWoWPacket(session, p);
            break;
        default:
            Logger::get()->warn("[NHandlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}

