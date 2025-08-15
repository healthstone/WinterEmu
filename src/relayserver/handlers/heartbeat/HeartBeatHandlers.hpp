#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace HeartBeatHandlers {
    void handlePing(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket>& p);
}