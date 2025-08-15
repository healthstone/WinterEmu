#pragma once

#include <memory>
#include <vector>
#include "src/authserver/AuthSession/AuthSession.hpp"

/** only AUTH_LOGON_PROOF **/
namespace HandlersLogonProofStage {
    boost::asio::awaitable<void>
    HandleLogonProof(std::shared_ptr<AuthSession> session, std::shared_ptr<std::vector<uint8_t>> payload);
}
