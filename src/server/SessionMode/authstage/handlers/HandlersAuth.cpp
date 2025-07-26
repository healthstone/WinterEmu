#include "HandlersAuth.hpp"

#include <utility>
#include "src/server/SessionMode/authstage/opcodes/AuthPacket.hpp"
#include "utils/PacketUtils.hpp"
#include "utils/generators/GeneratorUtils.hpp"
#include "utils/utf8utils/UTF8Utils.hpp"

using namespace HandlersAuth;

void HandlersAuth::dispatch(std::shared_ptr<ClientSession> session, AuthPacket &p) {
    AuthOpcodes opcode = p.get_opcode();
    switch (opcode) {
        case AuthOpcodes::CMSG_PING:
            handle_ping(std::move(session), p);
            break;

        case AuthOpcodes::CMSG_AUTH_LOGON_CHALLENGE:
            boost::asio::co_spawn(
                    session->socket().get_executor(),
                    handle_logon_challenge(session, p),
                    boost::asio::detached
            );
            break;

        case AuthOpcodes::CMSG_AUTH_LOGON_PROOF:
            handle_logon_proof(std::move(session), p);
            break;

        default:
            Logger::get()->warn("[HandlersAuth] Unknown opcode: {}", static_cast<uint8_t>(opcode));
            break;
    }
}

void HandlersAuth::handle_ping(std::shared_ptr<ClientSession> session, AuthPacket &p) {
    // Считываем ping ID из клиента (4 байта LE)
    uint32_t ping = p.read_uint32_le();

    // Формируем PONG — возвращаем то же самое число
    AuthPacket reply(AuthOpcodes::SMSG_PONG);
    reply.write_uint32_le(ping);

    // Отправляем обратно клиенту
    Logger::get()->debug("[HandlersAuth] CMSG_PING");
    PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
}

boost::asio::awaitable<void>
HandlersAuth::handle_logon_challenge(std::shared_ptr<ClientSession> session, AuthPacket &p) {
    auto log = Logger::get();
    std::string username;
    try {
        // 1 - читаем поле с именем, проверяем на UTF8
        username = p.read_string_nt_le();
    }
    catch (const std::exception &ex) {
        log->error("[HandlersAuth] CMSG_AUTH_LOGON_CHALLENGE - Internal error: {}", ex.what());

        AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
        reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
        reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::INTERNAL_ERROR));
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }

    // 1 - проверка на UTF8
    if (!UTF8Utils::is_valid_utf8(username)) {
        log->error("[HandlersAuth] CMSG_AUTH_LOGON_CHALLENGE - Invalid UTF-8 username received: {}", username);
        // Обработка ошибки, например отказ
        AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
        reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
        reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::WRONG_USERNAME));
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }

    // 2 - делаем верхний регистр
    username = UTF8Utils::to_uppercase(username);

    // 2.5 - кикаем все сессии, где уже авторизованы под данным логином + откидываем этого с оповещением, что уже авторизован
    if (session->server()->disconnectSessionIfExists(username)) {
        log->warn("[HandlersAuth] Duplicate session for '{}'", username);

        AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
        reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
        reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::ALREADY_LOGGED_IN));
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }

    session->getAccountInfo()->setUserName(username);

    auto cache = session->server()->account_cache();
    auto cached_user_opt = cache->get(username);

    auto auth = session->getAccountInfo();
    auto srp = auth->srp();

    // 3 - пробуем взять из кэша
    if (cached_user_opt) {
        auto &cached_user = *cached_user_opt;

        srp->load_verifier(cached_user.salt, cached_user.verifier);
        srp->generate_server_ephemeral();

        log->debug(
                "[HandlersAuth] CMSG_AUTH_LOGON_CHALLENGE CACHED ENTRY used for '{}': B.size={}, g={}, N.size={}, salt.size={}",
                username, srp->get_B_bytes().size(), srp->get_generator(), srp->get_N_bytes().size(),
                cached_user.salt.size());

        AuthPacket reply(AuthOpcodes::SMSG_AUTH_LOGON_CHALLENGE);
        reply.write_bytes(srp->get_B_bytes());
        reply.write_uint8(srp->get_generator());
        reply.write_bytes(srp->get_N_bytes());
        reply.write_bytes(cached_user.salt);

        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }

    // 4 - лезем в бд
    try {
        PreparedStatement stmt("SELECT_ACCOUNT_BY_USERNAME");
        stmt.set_param(0, username);

        auto user = session->server()->db()->execute_sync<AccountsRow>(stmt);

        // 5 - если нет аккаунта
        if (!user) {
            log->error("[HandlersAuth] User '{}' not found", username);

            AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
            reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
            reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::WRONG_USERNAME));
            PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
            co_return;
        }

        // 6 --- Проверка salt и verifier ---
        if (!user->salt.has_value() || !user->verifier.has_value() ||
            user->salt->size() != 32 || user->verifier->size() != 32) {
            log->error("[HandlersAuth] User '{}' has invalid salt/verifier length", username);

            AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
            reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
            reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::WRONG_PASSWORD));
            PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
            co_return;
        }

        AccountCache::AccountCacheEntry cacheEntry;
        cacheEntry.salt = *user->salt;
        cacheEntry.verifier = *user->verifier;
        cache->put(username, cacheEntry);

        // 7 --- Инициализация SRP ---
        srp->load_verifier(*user->salt, *user->verifier);
        srp->generate_server_ephemeral();

        log->debug("[HandlersAuth] CMSG_AUTH_LOGON_CHALLENGE: B.size={}, g={}, N.size={}, salt.size={}",
                   srp->get_B_bytes().size(), srp->get_generator(), srp->get_N_bytes().size(), user->salt->size());
        AuthPacket reply(AuthOpcodes::SMSG_AUTH_LOGON_CHALLENGE);
        reply.write_bytes(srp->get_B_bytes());          // 32 байта B (публичный ключ сервера)
        reply.write_uint8(srp->get_generator());        // 1 байт g
        reply.write_bytes(srp->get_N_bytes());          // 32 байта N (большое простое число)
        reply.write_bytes(*user->salt);                 // 32 байта salt
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }
    catch (const std::exception &ex) {
        log->error("[HandlersAuth] CMSG_AUTH_LOGON_CHALLENGE: {}", ex.what());
        AuthPacket reply(AuthOpcodes::SMSG_AUTH_RESPONSE);
        reply.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
        reply.write_uint8(static_cast<uint8_t>(AuthErrorCode::DATABASE_BUSY));
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
        co_return;
    }
}

void HandlersAuth::handle_logon_proof(std::shared_ptr<ClientSession> session, AuthPacket &p) {
    auto log = Logger::get();
    try {
        std::vector<uint8_t> A_bytes = p.read_bytes(32);
        std::vector<uint8_t> M1_client = p.read_bytes(20);

        std::vector<uint8_t> M2;
        auto srp = session->getAccountInfo()->srp();

        if (!srp->verify_client_proof(A_bytes, M1_client, M2)) {
            log->warn("[HandlersAuth] CMSG_AUTH_LOGON_PROOF: SRP6 M1 verification failed");
            AuthPacket fail(AuthOpcodes::SMSG_AUTH_RESPONSE);
            fail.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
            fail.write_uint8(static_cast<uint8_t>(AuthErrorCode::WRONG_PASSWORD));
            PacketUtils::send_packet_as<AuthPacket>(std::move(session), fail);
            return;
        }

        log->debug("[HandlersAuth] CMSG_AUTH_LOGON_PROOF: SRP6 OK — Sent SMSG_AUTH_LOGON_PROOF");
        session->set_session_mode(SessionMode::WORK_SESSION);
        session->getAccountInfo()->handle_auth_state();

        AuthPacket reply(AuthOpcodes::SMSG_AUTH_LOGON_PROOF);
        reply.write_bytes(M2);
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), reply);
    }
    catch (const std::exception &ex) {
        log->error("[HandlersAuth] CMSG_AUTH_LOGON_PROOF exception: {}", ex.what());
        AuthPacket fail(AuthOpcodes::SMSG_AUTH_RESPONSE);
        fail.write_uint8(static_cast<uint8_t>(AuthStatusCode::AUTH_FAILED));
        fail.write_uint8(static_cast<uint8_t>(AuthErrorCode::INTERNAL_ERROR));
        PacketUtils::send_packet_as<AuthPacket>(std::move(session), fail);
    }
}