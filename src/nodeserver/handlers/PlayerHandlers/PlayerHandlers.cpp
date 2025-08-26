#include "PlayerHandlers.hpp"
#include "ObjectGuid/ObjectGuid.hpp"

void PlayerHandlers::handleDispatchWoWPacket(std::shared_ptr<NodeSession> session, std::shared_ptr<NodePacket> p) {
    auto log = Logger::get();

    NodeData nodeData = p->node_data().value();
    WoWPacket woWPacket = p->wow_packet().value();
    log->info("PlayerHandlers::handleDispatchWoWPacket handle nodeData.size={} and wowpacket.size={}", nodeData.size(),
              woWPacket.size());
}

void PlayerHandlers::handle_add_player(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p) {
    NodeData nodeData = p->node_data().value();
    uint64_t guid = nodeData.read_uint64_le();

    ObjectGuid playerGuid(guid);

    Logger::get()->warn("[MiscHandlers::handle_add_player] Player {} has been added in playerMap", playerGuid.ToString());
}

void PlayerHandlers::handle_del_player(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p) {
    NodeData nodeData = p->node_data().value();
    uint64_t guid = nodeData.read_uint64_le();

    ObjectGuid playerGuid(guid);

    Logger::get()->warn("[MiscHandlers::handle_del_player] Player {} has been deleted from playerMap", playerGuid.ToString());
}