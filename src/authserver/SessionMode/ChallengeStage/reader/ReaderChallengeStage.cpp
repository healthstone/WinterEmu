#include "ReaderChallengeStage.hpp"
#include "src/authserver/SessionMode/ChallengeStage/handlers/HandlersChallenge.hpp"
#include "src/authserver/enums/AuthCmd.hpp"

using namespace ReaderChallengeStage;

void ReaderChallengeStage::process_read_buffer(std::shared_ptr<AuthSession> session) {
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

    auto payload = std::make_shared<std::vector<uint8_t>>(data, data + 4 + size);
    buffer.read_completed(4 + size);

    switch (static_cast<AuthCmd>(cmd)) {
        case AuthCmd::AUTH_LOGON_CHALLENGE: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, payload]() -> boost::asio::awaitable<void> {
                        co_await HandlersChallenge::HandleLogonChallenge(session, payload);
                    },
                    boost::asio::detached
            );
            break;
        }
        case AuthCmd::AUTH_RECONNECT_CHALLENGE: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, payload]() -> boost::asio::awaitable<void> {
                        co_await HandlersChallenge::HandleReconnectChallenge(session, payload);
                    },
                    boost::asio::detached
            );
            break;
        }
        default:
            Logger::get()->warn("ReaderChallengeStage: Unexpected cmd 0x{:02X}, expected AUTH_LOGON_CHALLENGE or AUTH_RECONNECT_CHALLENGE", data[0]);
            break;
    }
}