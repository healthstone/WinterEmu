#pragma once

#include <memory>
#include "src/worldserver/WorldSession/WorldSession.hpp"
#include "src/worldserver/WorldSession/WoWPacket.hpp"

namespace Handlers {
    void dispatch(const std::shared_ptr<WorldSession>& session, const std::shared_ptr<WoWPacket> &p);
}