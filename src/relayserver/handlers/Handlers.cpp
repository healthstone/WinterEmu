#include "Handlers.hpp"
#include "enums/WoWOpcodes.hpp"
#include "src/relayserver/handlers/heartbeat/HeartBeatHandlers.hpp"
#include "src/relayserver/handlers/authsession/AuthHandlers.hpp"

using namespace Handlers;

void Handlers::dispatch(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    WoWOpcodes opcode = p->get_opcode();
    switch (opcode) {
        case WoWOpcodes::CMSG_PING: {
            HeartBeatHandlers::handlePing(session, p);
            break;
        }
        case WoWOpcodes::CMSG_AUTH_SESSION: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await AuthHandlers::handleAuthPacket(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_READY_FOR_ACCOUNT_DATA_TIMES: Logger::get()->info("CMSG_READY_FOR_ACCOUNT_DATA_TIMES called"); break;
        case WoWOpcodes::CMSG_CHAR_ENUM: Logger::get()->info("CMSG_CHAR_ENUM called"); break;
        case WoWOpcodes::CMSG_REALM_SPLIT: Logger::get()->info("CMSG_REALM_SPLIT called"); break;
        default:
            Logger::get()->warn("[Handlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}