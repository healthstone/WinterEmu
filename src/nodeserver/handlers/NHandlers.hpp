#pragma once

#include <memory>
#include "src/nodeserver/NodeSession/NodeSession.hpp"
#include "packet/NodePacket.hpp"

namespace NHandlers {
    void dispatch(std::shared_ptr<NodeSession> session, std::shared_ptr<NodePacket> p);
}