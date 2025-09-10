#include <memory>
#include "CharCreateData.hpp"
#include "packet/WoWPacket.hpp"
#include "src/relayserver/GameSession/GameSession.hpp"
#include "src/game/enums/ResponseCodes.hpp"

namespace CharHandlers {
    uint32_t constexpr MAX_CHARACTERS_PER_REALM = 10; // max supported by client in char enum

    /** CMSG_CHAR_ENUM **/
    boost::asio::awaitable<void>
    handleCharacterEnum(std::shared_ptr<GameSession> session);

    /** CMSG_CHAR_CREATE **/
    boost::asio::awaitable<void>
    handleCharacterCreate(std::shared_ptr<GameSession> session, const std::shared_ptr<WoWPacket> &p);

    ResponseCodes checkPlayerName(std::shared_ptr<GameSession> session, std::string_view name);
    void sendCharResponse(std::shared_ptr<GameSession> session, WoWOpcodes opcode, ResponseCodes result);

    std::optional<CharCreateData> ReadPacketFields(const std::shared_ptr<WoWPacket> &p);

    boost::asio::awaitable<std::vector<CharacterEnumRow>> fetchFromDB(std::shared_ptr<GameSession> session);

    boost::asio::awaitable<uint64_t> fetchUsernameCountFromDB(std::shared_ptr<GameSession> session, const std::string &charName);

    boost::asio::awaitable<void> handleInsertCharacter(std::shared_ptr<GameSession> session, const CharCreateData &ccd, PlayerInfo const* playerInfo);
    boost::asio::awaitable<void> handleUpdateRealmCharacters(std::shared_ptr<GameSession> session, uint8_t futureCharactersCount);
}