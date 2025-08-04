#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace AuthHandlers {
    void handleAuthPacket(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p);
}