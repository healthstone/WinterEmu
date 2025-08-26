#include "PlayerHandlers.hpp"

#define PER_CHARACTER_CACHE_MASK    0xEA

void PlayerHandlers::handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    Logger::get()->debug("PlayerHandlers::handlePlayerLogin - CMSG_PLAYER_LOGIN");
    uint64_t guid =p->read_uint64_le();

    ObjectGuid playerGuid(guid);
    Logger::get()->debug("PlayerHandlers::handlePlayerLogin - playerGuid = {}", playerGuid.ToString());


}