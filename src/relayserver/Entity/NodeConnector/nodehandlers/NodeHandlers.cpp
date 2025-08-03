#include "NodeHandlers.hpp"

using namespace NodeHandlers;

void NodeHandlers::dispatch(const std::shared_ptr<NodeConnector>& connector, const std::shared_ptr<NodePacket> &p) {
    NodeOpcodes opcode = p->opcode();
    switch (opcode) {
        case NodeOpcodes::NODE_TO_REL_PONG: handle_ping(connector, p); break;
        default:
            Logger::get()->warn("[NodeHandlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }

}

void NodeHandlers::handle_ping(const std::shared_ptr<NodeConnector>& connector, const std::shared_ptr<NodePacket>& p) {
    Logger::get()->debug("[NodeHandlers] Handling NODE_TO_REL_PONG");
    // Читаем NodeData
    if (p->node_data()) {
        NodeData responseNodeData = p->node_data().value();
        uint8_t nodeID = responseNodeData.read_uint8();

        Logger::get()->debug("[NodeHandlers] Handling NODE_TO_REL_PONG from node {}", nodeID);
        connector->cancel_pong_timeout();
    }
}