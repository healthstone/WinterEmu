#include <memory>
#include "src/nodeserver/NodeSession/NodeSession.hpp"
#include "packet/NodePacket.hpp"

namespace MiscHandlers {
    void handle_ping(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p);
}