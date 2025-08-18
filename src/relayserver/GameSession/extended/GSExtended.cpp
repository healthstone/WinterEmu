#include "src/relayserver/GameSession/GameSession.hpp"
#include "Time/GameTime.hpp"

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

void GameSession::sendAddonsInfo(const std::vector<uint8_t> &addonData) {
    // Формат пакета: [uint8: count][repeated: uint32: crc, uint8: flags]
    WoWPacket pkt(WoWOpcodes::SMSG_ADDON_INFO);
    // Простейшая реализация - без поддержки аддонов
    pkt.write_uint8(0); // Количество аддонов = 0
    send_packet(std::make_shared<WoWPacket>(pkt));
}

void GameSession::sendClientCacheVersion() {
    WoWPacket pkt(WoWOpcodes::SMSG_CLIENTCACHE_VERSION);
    pkt.write_uint32_le(server()->clientCacheVersion());
    send_packet(std::make_shared<WoWPacket>(pkt));
}