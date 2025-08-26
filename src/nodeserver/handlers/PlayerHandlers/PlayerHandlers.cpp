#include "PlayerHandlers.hpp"

void PlayerHandlers::handleDispatchWoWPacket(std::shared_ptr<NodeSession> session, std::shared_ptr<NodePacket> p) {
    auto log = Logger::get();

    NodeData nodeData = p->node_data().value();
    WoWPacket woWPacket = p->wow_packet().value();
    log->info("PlayerHandlers::handleDispatchWoWPacket handle nodeData.size={} and wowpacket.size={}", nodeData.size(),
              woWPacket.size());
}