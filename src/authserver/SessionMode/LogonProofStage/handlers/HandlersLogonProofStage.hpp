#pragma once

#include <memory>
#include <vector>
#include "src/authserver/ClientSession/ClientSession.hpp"

/** only AUTH_LOGON_PROOF **/
namespace HandlersLogonProofStage
{
    boost::asio::awaitable<void> HandleLogonProof(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload);
}
