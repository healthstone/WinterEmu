#include "HandlersRealmStage.hpp"

using namespace HandlersRealmStage;

boost::asio::awaitable<void> HandlersRealmStage::HandleRealmList(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {
    Logger::get()->debug("[HandleRealmList] HandleRealmList");

    co_return;
}