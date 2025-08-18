#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"
#include "AuthSessionData.hpp"
#include "src/relayserver/enums/ResponseCodes.hpp"

namespace AuthHandlers {
    boost::asio::awaitable<void>
    handleAuthPacket(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    std::optional<AuthSessionData> ReadPacketFields(const std::shared_ptr<WoWPacket> &p);

    boost::asio::awaitable<std::optional<AccountsRow>>
    fetchFromDB(AuthSessionData asd, std::shared_ptr<GameSession> session);

    void sendAuthResponse(std::shared_ptr<GameSession> session, ResponseCodes code);

    void sendAuthResponse(std::shared_ptr<GameSession> session, ResponseCodes code,
                          uint8_t expansion, uint32_t queuePos);

    bool verifyClientDigest(const AuthSessionData &asd,
                            const AccountsRow &account,
                            const std::array<uint8_t, 4> &authSeed);
}