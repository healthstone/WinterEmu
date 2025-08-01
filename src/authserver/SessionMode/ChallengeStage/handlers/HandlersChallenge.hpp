#pragma once

#include <memory>
#include <vector>
#include "src/authserver/ClientSession/ClientSession.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "LogonChallenge.hpp"

/** AUTH_LOGON_CHALLENGE and AUTH_RECONNECT_CHALLENGE **/
namespace HandlersChallenge
{
    boost::asio::awaitable<void> HandleLogonChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);

    boost::asio::awaitable<void> HandleReconnectChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);

    std::optional<LogonChallenge> ReadPacketFields(const std::string &opcode_name, const std::vector<uint8_t>& payload);

    bool isPassedCommonLogic(AuthCmd cmd, const std::shared_ptr<ClientSession>& session, const std::vector<uint8_t>& payload);
    bool isPassedCache(AuthCmd cmd, const std::string &account_name, std::shared_ptr<ClientSession> session);

    boost::asio::awaitable<std::optional<AccountsRow>> fetchFromDB(AuthCmd cmd, std::shared_ptr<ClientSession> session);

    boost::asio::awaitable<void> LogonChallengeLogic(std::shared_ptr<ClientSession> session);
    boost::asio::awaitable<void> ReconnectChallengeLogic(const std::shared_ptr<ClientSession>& session);

    void send_auth_result(AuthCmd cmd, AuthResult result, std::shared_ptr<ClientSession> session);
}
