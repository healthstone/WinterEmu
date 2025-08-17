#pragma once

#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace Handlers {
    void dispatch(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    void HandleReadyForAccountDataTimes(const std::shared_ptr<GameSession>& session);
}