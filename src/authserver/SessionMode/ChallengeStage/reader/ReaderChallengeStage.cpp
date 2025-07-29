#include "ReaderChallengeStage.hpp"
#include "src/authserver/SessionMode/ChallengeStage/handlers/HandlersChallenge.hpp"
#include "src/authserver/enums/AuthCmd.hpp"

using namespace ReaderChallengeStage;

void ReaderChallengeStage::process_read_buffer(std::shared_ptr<ClientSession> session) {
    auto log = Logger::get();
    auto& buffer = session->read_buffer();

    if (buffer.get_active_size() < 4)
        return;

    const uint8_t* data = buffer.read_ptr();
    uint8_t cmd = data[0];
    uint8_t error = data[1];
    uint16_t size = static_cast<uint16_t>(data[2]) | (static_cast<uint16_t>(data[3]) << 8);

    if (buffer.get_active_size() < 4 + size)
        return;

    std::vector<uint8_t> payload(data, data + 4 + size);
    buffer.read_completed(4 + size);

    if (static_cast<AuthCmd>(cmd) == AuthCmd::AUTH_LOGON_CHALLENGE)
        HandlersChallenge::HandleLogonChallenge(session, payload);
    if (static_cast<AuthCmd>(cmd) == AuthCmd::AUTH_RECONNECT_CHALLENGE)
        HandlersChallenge::HandleReconnectChallenge(session, payload);
}