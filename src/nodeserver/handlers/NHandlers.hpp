#pragma once

#include <memory>
#include "src/nodeserver/NodeSession/NodeSession.hpp"
#include "packet/NodePacket.hpp"

namespace NHandlers {
    void dispatch(const std::shared_ptr<NodeSession>& session, const std::shared_ptr<NodePacket> &p);

    void handle_ping(const std::shared_ptr<NodeSession>& session, const std::shared_ptr<NodePacket> &p);
}