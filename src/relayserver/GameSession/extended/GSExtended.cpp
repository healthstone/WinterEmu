#include "src/relayserver/GameSession/GameSession.hpp"
#include "Time/GameTime.hpp"
#include <zlib.h>

/** вызывается либо при логине (GLOBAL_CACHE_MASK), либо при входе персонажем (PER_CHARACTER_CACHE_MASK) **/
void GameSession::sendAccountDataTimes(uint32_t mask) {
    WoWPacket pkt(WoWOpcodes::SMSG_ACCOUNT_DATA_TIMES);
    pkt.write_uint32_le(GameTime::GetGameTime());    // Server time
    pkt.write_uint8(1);
    pkt.write_uint32_le(mask);                       // type mask
    for (uint32_t i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
        if (mask & (1 << i))
            pkt.write_uint32_le(getAccountData(AccountDataType(i))->Time); // also unix time

    send_packet(std::make_shared<WoWPacket>(pkt));
}

boost::asio::awaitable<void> GameSession::setAccountData(AccountDataType type, time_t tm, std::string const& data)
{
    try {
        if ((1 << type) & GLOBAL_CACHE_MASK) {
            PreparedStatement stmt("REPLACE_ACCOUNT_DATA");
            stmt.set_param(0, getAccountId());
            stmt.set_param(1, type);
            stmt.set_param(2, uint32_t(tm));
            stmt.set_param(3, data);
            co_await server()->db()->execute_async_one<NothingRow>(stmt);
        } else {
            if (!getCharacterId().is_nil()) {
                PreparedStatement stmt("REPLACE_CHARACTER_ACCOUNT_DATA");
                stmt.set_param(0, getCharacterId());
                stmt.set_param(1, type);
                stmt.set_param(2, uint32_t(tm));
                stmt.set_param(3, data);
                co_await server()->db()->execute_async_one<NothingRow>(stmt);
            }
        }

        m_accountData[type].Time = tm;
        m_accountData[type].Data = data;
        co_return;
    } catch (const std::exception &ex) {
        Logger::get()->error("[GameSession]::setAccountData DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void> GameSession::loadAccountData(uint32_t mask) {
    auto log = Logger::get();
    try {
        for (uint32_t i = 0; i < NUM_ACCOUNT_DATA_TYPES; ++i)
            if (mask & (1 << i))
                m_accountData[i] = AccountData();

        PreparedStatement stmt("SELECT_ACCOUNT_DATA");
        stmt.set_param(0, getAccountId());
        auto rows = co_await server()->db()->execute_async_many<AccountDataRow>(stmt);
        if (!rows.empty()) {
            for (const auto &row: rows) {
                if (row.type >= NUM_ACCOUNT_DATA_TYPES)
                {
                    log->error("Table `{}` have invalid account data type ({}), ignore.",
                               mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", row.type);
                    continue;
                }

                if ((mask & (1 << row.type)) == 0)
                {
                    log->error("Table `{}` have non appropriate for table account data type ({}), ignore.",
                               mask == GLOBAL_CACHE_MASK ? "account_data" : "character_account_data", row.type);
                    continue;
                }

                m_accountData[row.type].Time = time_t(row.time);
                std::string data(reinterpret_cast<const char*>(row.data.data()), row.data.size());
                m_accountData[row.type].Data = data;
            }
        }
        co_return;

    } catch (const std::exception &ex) {
        log->error("[GameSession]::loadAccountData DB exception: {}", ex.what());
        co_return;
    }
}

boost::asio::awaitable<void> GameSession::loadTutorialsData() {
    try {
        memset(m_Tutorials, 0, sizeof(uint32_t) * MAX_ACCOUNT_TUTORIAL_VALUES);

        PreparedStatement stmt("SELECT_ACCOUNT_TUTORIALS");
        stmt.set_param(0, getAccountId());
        auto row = co_await server()->db()->execute_async_one<AccountTutorialRow>(stmt);
        if (row) {
            m_Tutorials[0] = row->tut0;
            m_Tutorials[1] = row->tut1;
            m_Tutorials[2] = row->tut2;
            m_Tutorials[3] = row->tut3;
            m_Tutorials[4] = row->tut4;
            m_Tutorials[5] = row->tut5;
            m_Tutorials[6] = row->tut6;
            m_Tutorials[7] = row->tut7;
            m_TutorialsChanged |= TUTORIALS_FLAG_LOADED_FROM_DB;
        }
        m_TutorialsChanged &= ~TUTORIALS_FLAG_CHANGED;
        co_return;

    } catch (const std::exception &ex) {
        Logger::get()->error("[GameSession]::loadTutorialsData DB exception: {}", ex.what());
        co_return;
    }
}

void GameSession::sendTutorialsData() {
    WoWPacket pkt(WoWOpcodes::SMSG_TUTORIAL_FLAGS);
    for (uint8_t i = 0; i < MAX_ACCOUNT_TUTORIAL_VALUES; ++i)
        pkt.write_uint32_le(m_Tutorials[i]);

    send_packet(std::make_shared<WoWPacket>(pkt));
}

void GameSession::sendClientCacheVersion() {
    WoWPacket pkt(WoWOpcodes::SMSG_CLIENTCACHE_VERSION);
    pkt.write_uint32_le(server()->clientCacheVersion());
    send_packet(std::make_shared<WoWPacket>(pkt));
}

void GameSession::readAddonsInfo(const std::vector<uint8_t>& data) {
    auto log = Logger::get();

    if (data.empty()) {
        log->debug("Addon data is empty");
        return;
    }

    // Проверяем минимальный размер: uint32 size
    if (data.size() < sizeof(uint32_t)) {
        log->error("Addon packet too small: {}", data.size());
        return;
    }

    // Читаем размер сжатых данных с учетом little-endian
    uint32_t compressedSize;

    // Создаем временный вектор с первыми 4 байтами
    std::vector<uint8_t> sizeData(data.begin(), data.begin() + sizeof(uint32_t));
    ByteBuffer tempBuffer(sizeData);
    compressedSize = tempBuffer.read_uint32_le();

    if (compressedSize == 0) {
        log->debug("Addon packet size is zero");
        return;
    }

    // TrinityCore использует 0xFFFFF (1MB), а не 0xFFFFF
    constexpr uint32_t MAX_COMPRESSED_SIZE = 0xFFFFF;
    if (compressedSize > MAX_COMPRESSED_SIZE) {
        log->error("Addon packet too big: {} (max allowed: {})",
                   compressedSize, MAX_COMPRESSED_SIZE);
        return;
    }

    // TrinityCore не проверяет размер данных перед распаковкой
    // Вместо этого пытается распаковать и проверяет результат

    // Распаковываем данные
    uLongf destLen = compressedSize * 10; // Начальный размер буфера
    std::vector<uint8_t> decompressed(destLen);

    int z_res = uncompress(
            decompressed.data(),
            &destLen,
            data.data() + sizeof(uint32_t),
            compressedSize
    );

    if (z_res != Z_OK) {
        // TrinityCore просто логирует ошибку и продолжает работу
        log->error("Addon packet decompression failed: {} (Zlib error: {})",
                   z_res, zError(z_res));
        return;
    }
    decompressed.resize(destLen);

    try {
        ByteBuffer buffer(decompressed);

        // Читаем количество аддонов
        uint32_t addonsCount = buffer.read_uint32_le();
        addonsCount = std::min(addonsCount, Addons::MaxSecureAddons);

        _addons.SecureAddons.clear();
        _addons.SecureAddons.reserve(addonsCount);

        auto addonMgr = server()->getAddonMgr();
        if (!addonMgr) {
            log->error("AddonMgr not available!");
            return;
        }

        for (uint32_t i = 0; i < addonsCount; ++i) {
            Addons::SecureAddonInfo addon;
            addon.Name = buffer.read_string_nt_be();
            addon.HasKey = buffer.read_uint8() != 0;
            addon.PublicKeyCrc = buffer.read_uint32_le();
            addon.UrlCrc = buffer.read_uint32_le();

            // TrinityCore проверяет только зарегистрированные аддоны
            if (auto savedAddon = addonMgr->getAddonInfo(addon.Name)) {
                if (addon.PublicKeyCrc != savedAddon->CRC) {
                    if (addon.HasKey) {
                        addon.Status = Addons::SecureAddonInfo::BANNED;
                        log->warn("Addon: {}: modified (CRC: 0x{:x} != 0x{:x}) - accountID {}",
                                  addon.Name, savedAddon->CRC, addon.PublicKeyCrc,
                                  to_string(getAccountId()));
                    } else {
                        addon.Status = Addons::SecureAddonInfo::SECURE_HIDDEN;
                    }
                } else {
                    addon.Status = Addons::SecureAddonInfo::SECURE_HIDDEN;
                    log->trace("Addon: {}: validated (CRC: 0x{:x}) - accountID {}",
                               addon.Name, savedAddon->CRC, to_string(getAccountId()));
                }
            } else {
                // Незарегистрированные аддоны всегда баним
                addon.Status = Addons::SecureAddonInfo::BANNED;
                log->warn("Addon: {}: not registered - accountId {}",
                          addon.Name, to_string(getAccountId()));
            }

            _addons.SecureAddons.push_back(std::move(addon));
        }

        // Читаем таймстамп последнего бана
        if (buffer.remaining() >= sizeof(uint32_t)) {
            _addons.LastBannedAddOnTimestamp = buffer.read_uint32_le();
            log->trace("Newest banned addon timestamp: {}", _addons.LastBannedAddOnTimestamp);
        }
    } catch (const std::exception& e) {
        log->error("Error parsing addon data: {}", e.what());
    }
}

void GameSession::sendAddonsInfo() {
    WoWPacket pkt(WoWOpcodes::SMSG_ADDON_INFO);

    auto addonMgr = server()->getAddonMgr();
    if (!addonMgr) {
        auto log = Logger::get();
        log->error("AddonMgr not available during sendAddonsInfo!");
        return;
    }

    // Публичный ключ для аддонов (как в TrinityCore)
    static constexpr uint8_t addonPublicKey[256] = {
            0xC3, 0x5B, 0x50, 0x84, 0xB9, 0x3E, 0x32, 0x42, 0x8C, 0xD0, 0xC7, 0x48, 0xFA, 0x0E, 0x5D, 0x54,
            0x5A, 0xA3, 0x0E, 0x14, 0xBA, 0x9E, 0x0D, 0xB9, 0x5D, 0x8B, 0xEE, 0xB6, 0x84, 0x93, 0x45, 0x75,
            0xFF, 0x31, 0xFE, 0x2F, 0x64, 0x3F, 0x3D, 0x6D, 0x07, 0xD9, 0x44, 0x9B, 0x40, 0x85, 0x59, 0x34,
            0x4E, 0x10, 0xE1, 0xE7, 0x43, 0x69, 0xEF, 0x7C, 0x16, 0xFC, 0xB4, 0xED, 0x1B, 0x95, 0x28, 0xA8,
            0x23, 0x76, 0x51, 0x31, 0x57, 0x30, 0x2B, 0x79, 0x08, 0x50, 0x10, 0x1C, 0x4A, 0x1A, 0x2C, 0xC8,
            0x8B, 0x8F, 0x05, 0x2D, 0x22, 0x3D, 0xDB, 0x5A, 0x24, 0x7A, 0x0F, 0x13, 0x50, 0x37, 0x8F, 0x5A,
            0xCC, 0x9E, 0x04, 0x44, 0x0E, 0x87, 0x01, 0xD4, 0xA3, 0x15, 0x94, 0x16, 0x34, 0xC6, 0xC2, 0xC3,
            0xFB, 0x49, 0xFE, 0xE1, 0xF9, 0xDA, 0x8C, 0x50, 0x3C, 0xBE, 0x2C, 0xBB, 0x57, 0xED, 0x46, 0xB9,
            0xAD, 0x8B, 0xC6, 0xDF, 0x0E, 0xD6, 0x0F, 0xBE, 0x80, 0xB3, 0x8B, 0x1E, 0x77, 0xCF, 0xAD, 0x22,
            0xCF, 0xB7, 0x4B, 0xCF, 0xFB, 0xF0, 0x6B, 0x11, 0x45, 0x2D, 0x7A, 0x81, 0x18, 0xF2, 0x92, 0x7E,
            0x98, 0x56, 0x5D, 0x5E, 0x69, 0x72, 0x0A, 0x0D, 0x03, 0x0A, 0x85, 0xA2, 0x85, 0x9C, 0xCB, 0xFB,
            0x56, 0x6E, 0x8F, 0x44, 0xBB, 0x8F, 0x02, 0x22, 0x68, 0x63, 0x97, 0xBC, 0x85, 0xBA, 0xA8, 0xF7,
            0xB5, 0x40, 0x68, 0x3C, 0x77, 0x86, 0x6F, 0x4B, 0xD7, 0x88, 0xCA, 0x8A, 0xD7, 0xCE, 0x36, 0xF0,
            0x45, 0x6E, 0xD5, 0x64, 0x79, 0x0F, 0x17, 0xFC, 0x64, 0xDD, 0x10, 0x6F, 0xF3, 0xF5, 0xE0, 0xA6,
            0xC3, 0xFB, 0x1B, 0x8C, 0x29, 0xEF, 0x8E, 0xE5, 0x34, 0xCB, 0xD1, 0x2A, 0xCE, 0x79, 0xC3, 0x9A,
            0x0D, 0x36, 0xEA, 0x01, 0xE0, 0xAA, 0x91, 0x20, 0x54, 0xF0, 0x72, 0xD8, 0x1E, 0xC7, 0x89, 0xD2
    };

    // Отправляем информацию по каждому аддону
    for (const auto& addon : _addons.SecureAddons) {
        pkt.write_uint8(addon.Status);

        // TrinityCore: infoProvided = (status != BANNED) || addon.HasKey
        uint8_t infoProvided = (addon.Status != Addons::SecureAddonInfo::BANNED) || addon.HasKey;
        pkt.write_uint8(infoProvided);

        if (infoProvided) {
            // KeyProvided = !addon.HasKey
            pkt.write_uint8(!addon.HasKey);

            // Если ключ отсутствует, отправляем публичный ключ
            if (!addon.HasKey) {
                pkt.write_bytes(addonPublicKey, sizeof(addonPublicKey));
            }

            // Ревизия (всегда 0)
            pkt.write_uint32_le(0);
        }

        // UrlProvided (всегда 0)
        pkt.write_uint8(0);
    }

    // Отправляем новые забаненные аддоны
    const auto& bannedAddons = addonMgr->getBannedAddons();
    uint32_t lastBannedTimestamp = _addons.LastBannedAddOnTimestamp;

    // Проверка на читерство/смену сервера
    if (!bannedAddons.empty() && bannedAddons.back().Timestamp < lastBannedTimestamp) {
        lastBannedTimestamp = 0;
    }

    // Находим новые баны
    auto it = std::lower_bound(bannedAddons.begin(), bannedAddons.end(), lastBannedTimestamp,
                               [](const BannedAddon& a, uint32_t ts) {
                                   return a.Timestamp < ts;
                               });

    uint32_t newBansCount = std::distance(it, bannedAddons.end());
    pkt.write_uint32_le(newBansCount);

    for (; it != bannedAddons.end(); ++it) {
        pkt.write_uint32_le(it->Id);
        pkt.write_bytes(it->NameMD5.data(), it->NameMD5.size());
        pkt.write_bytes(it->VersionMD5.data(), it->VersionMD5.size());
        pkt.write_uint32_le(it->Timestamp);
        pkt.write_uint32_le(1); // IsBanned
    }

    send_packet(std::make_shared<WoWPacket>(pkt));
}