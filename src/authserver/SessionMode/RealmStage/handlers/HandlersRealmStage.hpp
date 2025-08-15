#pragma once

#include <memory>
#include <vector>
#include "src/authserver/AuthSession/AuthSession.hpp"

/** only REALM_LIST **/
namespace HandlersRealmStage {
    boost::asio::awaitable<void>
    HandleRealmList(std::shared_ptr<AuthSession> session, std::shared_ptr<std::vector<uint8_t>> payload);

    boost::asio::awaitable<void>
    fillInitialRealmCharacters(std::shared_ptr<AuthSession> session);
}
