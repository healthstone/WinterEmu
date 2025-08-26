#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace MiscHandlers {
    /** CMSG_PING **/
    void handleClientPingCommand(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);

    /** CMSG_READY_FOR_ACCOUNT_DATA_TIMES **/
    void handleReadyForAccountDataTimes(const std::shared_ptr<GameSession> &session);

    /** CMSG_UPDATE_ACCOUNT_DATA **/
    boost::asio::awaitable<void>
    handleUpdateAccountData(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    /** CMSG_REALM_SPLIT **/
    void handleRealmSplitOpcode(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);

    /** CMSG_SET_ACTIVE_VOICE_CHANNEL **/
    void handleActiveVoiceChannel(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);

    /** CMSG_CANCEL_TRADE **/
    void handleCancelTrade(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);
}