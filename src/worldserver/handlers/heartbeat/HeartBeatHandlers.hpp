#include <memory>
#include "src/worldserver/WorldSession/WorldSession.hpp"
#include "src/worldserver/WorldSession/WoWPacket.hpp"

namespace HeartBeatHandlers {
    void handlePing(const std::shared_ptr<WorldSession>& session, const std::shared_ptr<WoWPacket> &p);
}