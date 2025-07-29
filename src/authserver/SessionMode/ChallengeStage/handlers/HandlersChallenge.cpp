#include "HandlersChallenge.hpp"
#include "src/authserver/enums/AuthResult.hpp"
#include "src/authserver/enums/AuthCmd.hpp"
#include "src/authserver/enums/LoginResult.hpp"

using namespace HandlersChallenge;

void HandlersChallenge::HandleLogonChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {

}

void HandlersChallenge::HandleReconnectChallenge(std::shared_ptr<ClientSession> session, const std::vector<uint8_t>& payload) {

}
