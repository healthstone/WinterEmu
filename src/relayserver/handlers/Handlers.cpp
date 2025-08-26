#include "Handlers.hpp"
#include "enums/WoWOpcodes.hpp"
#include "Time/GameTime.hpp"
#include "src/relayserver/handlers/authsession/AuthHandlers.hpp"
#include "src/relayserver/handlers/misc/MiscHandlers.hpp"
#include "src/relayserver/handlers/character/CharHandlers.hpp"
#include "src/relayserver/handlers/player/PlayerHandlers.hpp"

void Handlers::dispatch(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    WoWOpcodes opcode = p->get_opcode();
    switch (opcode) {
        case WoWOpcodes::CMSG_PING: {
            MiscHandlers::handleClientPingCommand(session, p);
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
        case WoWOpcodes::CMSG_READY_FOR_ACCOUNT_DATA_TIMES: {
            MiscHandlers::handleReadyForAccountDataTimes(session);
            break;
        }
        case WoWOpcodes::CMSG_UPDATE_ACCOUNT_DATA: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await MiscHandlers::handleUpdateAccountData(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_CHAR_ENUM: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session]() -> boost::asio::awaitable<void> {
                        co_await CharHandlers::handleCharacterEnum(session);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_CHAR_CREATE: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await CharHandlers::handleCharacterCreate(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        case WoWOpcodes::CMSG_REALM_SPLIT: {
            MiscHandlers::handleRealmSplitOpcode(session, p);
            break;
        }
        case WoWOpcodes::CMSG_SET_ACTIVE_VOICE_CHANNEL: {
            MiscHandlers::handleActiveVoiceChannel(session, p);
            break;
        }
        case WoWOpcodes::CMSG_CANCEL_TRADE: {
            MiscHandlers::handleCancelTrade(session, p);
            break;
        }
        case WoWOpcodes::CMSG_PLAYER_LOGIN: {
            auto ex = boost::asio::make_strand(session->socket().get_executor());
            boost::asio::co_spawn(
                    ex,
                    [session, p]() -> boost::asio::awaitable<void> {
                        co_await PlayerHandlers::handlePlayerLogin(session, p);
                    },
                    boost::asio::detached
            );
            break;
        }
        default:
            Logger::get()->warn("[Handlers] Unknown opcode: {}", static_cast<uint16_t>(opcode));
            break;
    }
}