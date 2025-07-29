#include <memory>
#include "src/authserver/ClientSession/ClientSession.hpp"

namespace PacketUtils {

    template <typename PacketType, typename... Args>
    void send_packet_as(std::shared_ptr<ClientSession> session, Args&&... args) {
        session->send_packet(std::make_shared<PacketType>(std::forward<Args>(args)...));
    }

}