#include <memory>
#include "src/authserver/AuthSession/AuthSession.hpp"
#include "src/worldserver/WorldSession/WorldSession.hpp"

namespace PacketUtils {

    template <typename PacketType, typename... Args>
    void send_packet_as(std::shared_ptr<AuthSession> session, Args&&... args) {
        session->send_packet(std::make_shared<PacketType>(std::forward<Args>(args)...));
    }

    template <typename PacketType, typename... Args>
    void send_packet_as(std::shared_ptr<WorldSession> session, Args&&... args) {
        session->send_packet(std::make_shared<PacketType>(std::forward<Args>(args)...));
    }
}