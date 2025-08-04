#include "AuthHandlers.hpp"

using namespace AuthHandlers;

void AuthHandlers::handleAuthPacket(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p) {

    //example
//    NodeData nodeData;
//    nodeData.write_uint8(1);
//
//    NodePacket packet(NodeOpcodes::REL_TO_NODE_PING, nodeData, *p);
//
//    // 4. Взять случайный коннектор из пула NodeID = 1
//    auto connector = session->server()->get_node_manager()->get_first_connector(1);
//    if (!connector) {
//        Logger::get()->error("[AuthHandlers] No available NodeConnector for NodeID 1");
//        return;
//    }
//
//    // 5. Отправить
//    connector->send_packet(packet);
}