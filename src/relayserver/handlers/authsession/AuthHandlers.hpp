#include <memory>
#include "AuthSessionData.hpp"
#include "packet/WoWPacket.hpp"
#include "src/relayserver/GameSession/GameSession.hpp"
#include "src/relayserver/enums/ResponseCodes.hpp"

namespace AuthHandlers {
    boost::asio::awaitable<void>
    handleAuthPacket(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    std::optional<AuthSessionData> ReadPacketFields(const std::shared_ptr<WoWPacket> &p);

    boost::asio::awaitable<std::optional<AccountsRow>> fetchFromDB(std::shared_ptr<GameSession> session, const std::string &accName);

    void sendAuthResponse(std::shared_ptr<GameSession> session, ResponseCodes code);

    void sendAuthResponse(std::shared_ptr<GameSession> session, ResponseCodes code,
                          uint8_t expansion, uint32_t queuePos);

    bool verifyClientDigest(const AuthSessionData &asd,
                            const AccountsRow &account,
                            const std::array<uint8_t, 4> &authSeed);
}