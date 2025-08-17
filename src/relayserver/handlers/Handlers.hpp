#pragma once

#include <memory>
#include "src/relayserver/GameSession/GameSession.hpp"
#include "packet/WoWPacket.hpp"

namespace Handlers {
    void dispatch(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p);

    void handleClientPingCommand(const std::shared_ptr<GameSession>& session, std::shared_ptr<WoWPacket> p);

    void handleReadyForAccountDataTimes(const std::shared_ptr<GameSession>& session);

    void HandleUpdateAccountData(const std::shared_ptr<GameSession>& session, std::shared_ptr<WoWPacket> p);

    void handleCharacterEnum(const std::shared_ptr<GameSession>& session);

    void HandleRealmSplitOpcode(const std::shared_ptr<GameSession>& session, std::shared_ptr<WoWPacket> p);
}