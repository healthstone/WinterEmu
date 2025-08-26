#include <memory>
#include "packet/WoWPacket.hpp"
#include "src/relayserver/GameSession/GameSession.hpp"

namespace PlayerHandlers {
    /** CMSG_PLAYER_LOGIN **/
    void handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);
}