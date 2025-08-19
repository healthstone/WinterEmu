#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace MiscHandlers {
    void handleClientPingCommand(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);

    void handleReadyForAccountDataTimes(const std::shared_ptr<GameSession> &session);

    boost::asio::awaitable<void>
    handleUpdateAccountData(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    void handleRealmSplitOpcode(const std::shared_ptr<GameSession> &session, std::shared_ptr<WoWPacket> p);
}