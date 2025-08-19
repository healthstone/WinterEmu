#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace CharHandlers {
    boost::asio::awaitable<void>
    handleCharacterEnum(const std::shared_ptr<GameSession> &session);

    boost::asio::awaitable<std::vector<CharacterEnumRow>> fetchFromDB(std::shared_ptr<GameSession> session);
}