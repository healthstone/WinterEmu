#pragma once

#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace Handlers {
    void dispatch(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket> &p);
}