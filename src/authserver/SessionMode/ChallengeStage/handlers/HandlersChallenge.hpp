#pragma once

#include <memory>
#include <vector>
#include "src/authserver/ClientSession/ClientSession.hpp"

/** AUTH_LOGON_CHALLENGE and AUTH_RECONNECT_CHALLENGE **/
namespace HandlersChallenge
{
    boost::asio::awaitable<void> HandleLogonChallenge(const std::string &opcode_name, std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);
}
