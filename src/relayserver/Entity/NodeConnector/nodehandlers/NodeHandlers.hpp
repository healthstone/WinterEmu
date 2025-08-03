#include <memory>
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"

namespace NodeHandlers {
    void dispatch(const std::shared_ptr<NodeConnector>& connector, const std::shared_ptr<NodePacket> &p);

    void handle_ping(const std::shared_ptr<NodeConnector>& connector, const std::shared_ptr<NodePacket> &p);
}