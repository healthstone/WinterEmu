#include "HandlersLogonProofStage.hpp"

using namespace HandlersLogonProofStage;

boost::asio::awaitable<void> HandlersLogonProofStage::HandleLogonProof(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {
    Logger::get()->debug("HandlersLogonProofStage: HandleLogonProof");
    co_return;
}