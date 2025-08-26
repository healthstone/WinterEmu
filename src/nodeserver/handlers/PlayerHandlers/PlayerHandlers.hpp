#include <memory>
#include "src/nodeserver/NodeSession/NodeSession.hpp"
#include "packet/NodePacket.hpp"

namespace PlayerHandlers {

    void handleDispatchWoWPacket(std::shared_ptr<NodeSession> session, std::shared_ptr<NodePacket> p);

    void handle_add_player(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p);

    void handle_del_player(const std::shared_ptr<NodeSession>& session, std::shared_ptr<NodePacket> p);
}