#include "AuthHandlers.hpp"

#include "utils/HexUtils.hpp"

using namespace AuthHandlers;

boost::asio::awaitable<void>
AuthHandlers::handleAuthPacket(std::shared_ptr<GameSession> session, std::shared_ptr<WoWPacket> p) {
    auto log = Logger::get();
    log->debug("AuthHandlers::handleAuthPacket - CMSG_AUTH_SESSION");

    auto serverSeed = session->authSeed();
    log->trace("  ServerSeed: {}", HexUtils::byte_array_to_hex(serverSeed));

    auto authSessionData = ReadPacketFields(p);
    if (!authSessionData) {
        sendAuthResponse(session, ResponseCodes::AUTH_UNKNOWN_ACCOUNT);
        co_return;
    }

    session->setAccountName(authSessionData->accountName);

    auto account = co_await fetchFromDB(authSessionData.value(), session);
    if (!account) {
        sendAuthResponse(session, ResponseCodes::AUTH_UNKNOWN_ACCOUNT);
        co_return;
    }

    session->setAccountId(account->id);

    // Ключевое изменение: инициализация шифрования ДО проверки дайджеста
    session->initCrypt(account->sessionkey.value());

    // Проверка дайджеста
    if (!verifyClientDigest(authSessionData.value(), account.value(), serverSeed)) {
        log->error("Digest verification failed for account '{}'", authSessionData->accountName);
        sendAuthResponse(session, ResponseCodes::AUTH_FAILED);
        co_return;
    }

    co_await session->loadAccountData(GLOBAL_CACHE_MASK);
    co_await session->loadTutorialsData();
    //TODO
    //LoadInstanceTimeRestrictions(realmHolder.GetPreparedResult(AccountInfoQueryHolderPerRealm::INSTANCE_TIMES));

    sendAuthResponse(session, ResponseCodes::AUTH_OK, 2, 0);
    session->setAuthed(true);
    log->trace("Client '{}' authenticated successfully", authSessionData->accountName);

    session->readAddonsInfo(authSessionData->addonData);
    // Отправка ответа
    session->sendAddonsInfo();
    session->sendClientCacheVersion();
    session->sendTutorialsData();
    co_return;
}

boost::asio::awaitable<std::optional<AccountsRow>>
AuthHandlers::fetchFromDB(AuthSessionData asd, std::shared_ptr<GameSession> session) {
    try {
        PreparedStatement stmt("SELECT_ACCOUNT_BY_USERNAME");
        stmt.set_param(0, asd.accountName);
        auto user = co_await session->server()->db()->execute_async_one<AccountsRow>(stmt);
        co_return user;
    } catch (const std::exception &ex) {
        Logger::get()->error("[fetchFromDB] DB exception: {}", ex.what());
        co_return std::nullopt;
    }
}

std::optional<AuthSessionData> AuthHandlers::ReadPacketFields(const std::shared_ptr<WoWPacket> &p) {
    auto log = Logger::get();
    try {
        AuthSessionData asd;
        asd.client_build = p->read_uint32_le();
        asd.login_server_id = p->read_uint32_le();
        asd.accountName = p->read_string_nt_le();
        asd.login_server_type = p->read_uint32_le();
        asd.LocalChallenge = p->read_bytes_as_array<4>();
        asd.region_id = p->read_uint32_le();
        asd.battleground_id = p->read_uint32_le();
        asd.realm_id = p->read_uint32_le();
        asd.dos_response = p->read_uint64_le();
        asd.digest = p->read_bytes_as_array<20>();
        if (p->remaining() > 0) {
            asd.addonData = p->read_bytes(p->remaining());
        }

        std::string localChallenge_str = HexUtils::byte_array_to_hex(asd.LocalChallenge);
        std::string digest_str = HexUtils::byte_array_to_hex(asd.digest);
        log->trace("[AuthHandlers][ReadPacketFields] Parsed AuthSessionData:\n"
                   "  client_build: {}\n"
                   "  login_server_id: {}\n"
                   "  accountName: '{}'\n"
                   "  login_server_type: {}\n"
                   "  client_seed: {}\n"
                   "  region_id: {}\n"
                   "  battleground_id: {}\n"
                   "  realm_id: {}\n"
                   "  dos_response: {}\n"
                   "  digest: {}\n"
                   "  addonData size: {} bytes",
                   asd.client_build,
                   asd.login_server_id,
                   asd.accountName,
                   asd.login_server_type,
                   localChallenge_str,
                   asd.region_id,
                   asd.battleground_id,
                   asd.realm_id,
                   asd.dos_response,
                   digest_str,
                   asd.addonData.size());

        return asd;
    } catch (const std::exception &ex) {
        log->error("[AuthHandlers][ReadPacketFields] Exception while parsing: {}", ex.what());
        return std::nullopt;
    }
}

void AuthHandlers::sendAuthResponse(std::shared_ptr<GameSession> session, ResponseCodes code) {
    WoWPacket pkt(WoWOpcodes::SMSG_AUTH_RESPONSE);
    pkt.write_uint8(static_cast<uint8_t>(code));
    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

void AuthHandlers::sendAuthResponse(
        std::shared_ptr<GameSession> session,
        ResponseCodes code,
        uint8_t expansion,
        uint32_t queuePos) {
    WoWPacket pkt(WoWOpcodes::SMSG_AUTH_RESPONSE);

    // Основные поля
    pkt.write_uint8(static_cast<uint8_t>(code));       // Код ответа
    pkt.write_uint32_le(0);                            // BillingTimeRemaining
    pkt.write_uint8(0);                                // BillingPlanFlags
    pkt.write_uint32_le(0);                            // BillingTimeRested
    pkt.write_uint8(expansion);                        // Уровень расширения

    // Дополнительные поля для AUTH_OK
    if (code == ResponseCodes::AUTH_OK) {
        pkt.write_uint32_le(queuePos);                 // Позиция в очереди (0)
        pkt.write_uint8(0);                            // Флаг миграции персонажей
    }

    session->send_packet(std::make_shared<WoWPacket>(pkt));
}

bool AuthHandlers::verifyClientDigest(const AuthSessionData &asd,
                                      const AccountsRow &account,
                                      const std::array<uint8_t, 4> &authSeed) {
    using namespace Crypto;
    auto log = Logger::get();

    // 1. Проверка размера сессионного ключа
    if (account.sessionkey.value().size() != 40) {
        log->error("Invalid session key size: {}", account.sessionkey.value().size());
        return false;
    }

    // Подготовка данных
    std::array<uint8_t, 4> zero = {0, 0, 0, 0};

    // Ключевое изменение: используем исходное имя аккаунта (без преобразования в верхний регистр)
    const std::string& accountName = asd.accountName;

    // 3. Вычисление хеша (точная последовательность как в TrinityCore)
    SHA1 sha;
    sha.UpdateData(accountName);           // Имя аккаунта как есть
    sha.UpdateData(zero);                  // 4 нулевых байта
    sha.UpdateData(asd.LocalChallenge);    // Клиентский seed (4 байта)
    sha.UpdateData(authSeed);              // Серверный seed (4 байта)
    sha.UpdateData(account.sessionkey.value());  // Сессионный ключ из БД

    sha.Finalize();
    SHA1::Digest computedDigest = sha.GetDigest();

    // 4. Логирование для отладки
    log->trace("VerifyClientDigest details:");
    log->trace("  Account: {}", asd.accountName);
    log->trace("  LocalChallenge: {}", HexUtils::byte_array_to_hex(asd.LocalChallenge));
    log->trace("  ServerSeed: {}", HexUtils::byte_array_to_hex(authSeed));
    log->trace("  SessionKey: {}", HexUtils::byte_array_to_hex(account.sessionkey.value()));
    log->trace("  Computed: {}", HexUtils::byte_array_to_hex(computedDigest));
    log->trace("  Received: {}", HexUtils::byte_array_to_hex(asd.digest));

    return computedDigest == asd.digest;
}