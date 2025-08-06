#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"
#include "AuthSessionData.hpp"
#include "src/relayserver/enums/ResponseCodes.hpp"

namespace AuthHandlers {
    void handleAuthPacket(const std::shared_ptr<GameSession>& session, const std::shared_ptr<WoWPacket>& p);

    std::optional<AuthSessionData> ReadPacketFields(const std::shared_ptr<WoWPacket>& p);

    void sendAuthResponse(const std::shared_ptr<GameSession>& session, ResponseCodes code);
}