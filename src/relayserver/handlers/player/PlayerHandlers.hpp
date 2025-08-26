#include <memory>
#include "packet/WoWPacket.hpp"
#include "src/relayserver/GameSession/GameSession.hpp"

namespace PlayerHandlers {
    /** CMSG_PLAYER_LOGIN **/
    boost::asio::awaitable<void> handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    bool sendLoginVerifyWorld(const std::shared_ptr<GameSession> &session, ObjectGuid characterGuid);
}