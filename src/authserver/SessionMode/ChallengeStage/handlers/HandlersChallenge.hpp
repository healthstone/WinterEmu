#pragma once

#include <memory>
#include <vector>
#include "src/authserver/ClientSession/ClientSession.hpp"

/** AUTH_LOGON_CHALLENGE and AUTH_RECONNECT_CHALLENGE **/
namespace HandlersChallenge
{
    boost::asio::awaitable<void> HandleLogonChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);
    boost::asio::awaitable<void> HandleReconnectChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);
}
