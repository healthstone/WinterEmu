#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"
#include "src/relayserver/enums/ResponseCodes.hpp"

namespace CharHandlers {
    boost::asio::awaitable<void>
    handleCharacterEnum(std::shared_ptr<GameSession> session);

    boost::asio::awaitable<std::vector<CharacterEnumRow>> fetchFromDB(std::shared_ptr<GameSession> session);

    void handleCharacterCreate(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket> &p);

    void sendCharCreate(std::shared_ptr<GameSession> session, WoWOpcodes opcode, ResponseCodes result);
}