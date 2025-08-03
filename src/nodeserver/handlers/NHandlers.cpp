#include "NHandlers.hpp"
#include "utils/PacketUtils.hpp"

using namespace NHandlers;

void NHandlers::dispatch(const std::shared_ptr<NodeSession>& session, const std::shared_ptr<NodePacket> &p) {
    NodeOpcodes opcode = p->opcode();
    switch (opcode) {
        case NodeOpcodes::REL_TO_NODE_PING: handle_ping(session, p); break;
        default:
            Logger::get()->warn("[NHandlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}

void NHandlers::handle_ping(const std::shared_ptr<NodeSession>& session, const std::shared_ptr<NodePacket>& p) {
    Logger::get()->debug("[NHandlers] Handling REL_TO_NODE_PING");

    // Читаем NodeData
    if (p->node_data()) {
        NodeData responseNodeData = p->node_data().value();
        uint8_t received_nodeID = responseNodeData.read_uint8();
        Logger::get()->debug("[NHandlers] received_nodeID={}, expected={}", received_nodeID, session->_nodeID);
    }

    NodeData replyData;
    replyData.write_uint8(session->_nodeID);
    NodePacket pong_packet(NodeOpcodes::NODE_TO_REL_PONG, replyData);

    PacketUtils::send_packet_as<NodePacket>(session, pong_packet);
    Logger::get()->debug("[NHandlers] Sent NODE_TO_REL_PONG");
}