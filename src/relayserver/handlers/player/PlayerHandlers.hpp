#include <memory>
#include "packet/WoWPacket.hpp"
#include "src/relayserver/GameSession/GameSession.hpp"
#include "src/relayserver/enums/CharacterLoginErrorEnum.hpp"

namespace PlayerHandlers {
    /** CMSG_PLAYER_LOGIN **/
    boost::asio::awaitable<void> handlePlayerLogin(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    CharacterEnumRow const *checkCharacter(const std::shared_ptr<GameSession> &session, ObjectGuid characterGuid);

    void sendCharacterError(const std::shared_ptr<GameSession> &session, CharacterLoginErrorReason errorReason);

    void sendLoginVerifyWorld(const std::shared_ptr<GameSession> &session, CharacterEnumRow const *character);
}