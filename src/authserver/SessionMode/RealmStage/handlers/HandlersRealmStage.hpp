#pragma once

#include <memory>
#include <vector>
#include "src/authserver/ClientSession/ClientSession.hpp"

/** only REALM_LIST **/
namespace HandlersRealmStage {
    boost::asio::awaitable<void>
    HandleRealmList(std::shared_ptr<ClientSession> session, const std::vector<uint8_t> &payload);

    boost::asio::awaitable<void> fillInitialRealmCharacters(const std::shared_ptr<ClientSession> &session);
}
