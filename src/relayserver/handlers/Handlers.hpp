#pragma once

#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "src/relayserver/GameSession/WoWPacket.hpp"

namespace Handlers {
    void dispatch(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p);
}