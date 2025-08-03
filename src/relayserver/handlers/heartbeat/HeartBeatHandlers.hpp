#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "src/relayserver/GameSession/WoWPacket.hpp"

namespace HeartBeatHandlers {
    void handlePing(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p);
}