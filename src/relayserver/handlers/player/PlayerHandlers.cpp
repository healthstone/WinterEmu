#include "PlayerHandlers.hpp"
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"

#define PER_CHARACTER_CACHE_MASK    0xEA

void PlayerHandlers::handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    Logger::get()->debug("PlayerHandlers::handlePlayerLogin - CMSG_PLAYER_LOGIN");
    uint64_t guid =p->read_uint64_le();

    ObjectGuid playerGuid(guid);
    Logger::get()->debug("PlayerHandlers::handlePlayerLogin - playerGuid = {}", playerGuid.ToString());

    // Заносим
    session->server()->addSessionInPlayerMap(playerGuid, session);

    // А теперь транслируем сам пакет
    NodeData data;
    data.write_uint64_le(guid);

    NodePacket pkt(NodeOpcodes::REL_TO_NODE_WOWPACKET, data, *p);
    auto nodeSession = session->server()->get_node_manager()->get_first_connector(1);
    nodeSession->send_packet(pkt);
}