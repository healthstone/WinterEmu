#include "MiscHandlers.hpp"

void MiscHandlers::handle_ping(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p) {
    // Читаем NodeData
    if (p->node_data()) {
        NodeData responseNodeData = p->node_data().value();
        uint8_t received_nodeID = responseNodeData.read_uint8();
        if (received_nodeID != session->_nodeID)
            Logger::get()->warn("[NHandlers] received_nodeID={}, expected={}", received_nodeID, session->_nodeID);
    }

    NodeData replyData;
    replyData.write_uint8(session->_nodeID);
    NodePacket pong_packet(NodeOpcodes::NODE_TO_REL_PONG, replyData);

    session->send_packet(std::make_shared<NodePacket>(pong_packet));
    Logger::get()->trace("[NHandlers] Sent NODE_TO_REL_PONG");
}