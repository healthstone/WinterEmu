#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <pqxx/pqxx>
#include <mutex>
#include <queue>
#include <memory>
#include <optional>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <vector>
#include <functional>
#include <future>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/this_coro.hpp>

#include "QueryResults.hpp"

#include "database/mapper/PgRealm.hpp"
#include "database/mapper/auth/PgAccount.hpp"
#include "database/mapper/auth/PgBuildInfo.hpp"
#include "database/mapper/auth/PgBuildExeHash.hpp"
#include "database/mapper/auth/PgRealmCharacters.hpp"

#include "database/mapper/relay/PgAccountDataRow.hpp"
#include "database/mapper/relay/PgAccountTutorial.hpp"
#include "database/mapper/relay/PgAddon.hpp"
#include "database/mapper/relay/PgBannedAddonRow.hpp"
#include "database/mapper/relay/PgCharacterAccountData.hpp"
#include "database/mapper/relay/PgCharacterEnumRow.hpp"
#include "database/mapper/relay/PgPlayerCreateInfo.hpp"
#include "database/mapper/relay/PgPlayerCreateInfoItem.hpp"
#include "database/mapper/relay/PgPlayerCreateInfoSkills.hpp"

#include "database/mapper/dbc/PgDbcAchievement.hpp"
#include "database/mapper/dbc/PgDbcAchievementCriteria.hpp"
#include "database/mapper/dbc/PgDbcChrClasses.hpp"
#include "database/mapper/dbc/PgDbcChrRaces.hpp"
#include "database/mapper/dbc/PgDbcCharStartOutFit.hpp"
#include "database/mapper/dbc/PgDbcSkillRaceClassInfo.hpp"
#include "database/mapper/dbc/PgDbcSkillLine.hpp"

#include "database/mapper/world/PgItemTemplate.hpp"
#include "PreparedStatement.hpp"
#include "Logger.hpp"

class Database {
public:
    explicit Database(const std::string &conninfo, size_t sync_pool_size = 4, size_t async_threads = 2)
            : conninfo_(conninfo),
              thread_pool_(async_threads),
              work_guard_(boost::asio::make_work_guard(thread_pool_))
    {
        Logger::get()->info("[Database] Setting up Sync connection pool...");
        for (size_t i = 0; i < sync_pool_size; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            prepare_all(*conn);
            Logger::get()->trace("[Database] Sync connection {} established.", i + 1);
            {
                std::lock_guard<std::mutex> lock(mutex_);
                connections_.push(std::move(conn));
            }
        }
        Logger::get()->info("[Database] Sync connection pool established - size {}", sync_pool_size);

        Logger::get()->info("[Database] Setting up Async connection pool...");
        for (size_t i = 0; i < async_threads; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            prepare_all(*conn);
            Logger::get()->trace("[Database] Async connection {} established.", i + 1);
            {
                std::lock_guard<std::mutex> lock(async_mutex_);
                async_connections_.push(std::move(conn));
            }
        }
        Logger::get()->info("[Database] Async connection pool established - size {}", async_threads);

        Logger::get()->info("[Database] Starting async thread pool with {} threads...", async_threads);
        for (size_t i = 0; i < async_threads; ++i) {
            async_threads_.emplace_back([this]() {
                thread_pool_.run();
            });
        }
    }

    ~Database() {
        shutdown();
    }

    // === Синхронный интерфейс ===

    template<typename Struct>
    std::optional<Struct> execute_sync_one(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();
        pqxx::work txn(scoped.get());
        auto invoc = txn.prepared(stmt.name());
        for (const auto &param : stmt.params()) {
            if (param.has_value())
                invoc(param.value());
            else
                invoc(static_cast<const char *>(nullptr));
        }
        auto result = invoc.exec();
        txn.commit();

        if constexpr (std::is_same_v<Struct, NothingRow>)
            return Struct{};

        if (result.empty())
            return std::nullopt;

        return PgRowMapper<Struct>::map(result[0]);
    }

    template<typename Struct>
    std::vector<Struct> execute_sync_many(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();
        pqxx::work txn(scoped.get());
        auto invoc = txn.prepared(stmt.name());
        for (const auto &param : stmt.params()) {
            if (param.has_value())
                invoc(param.value());
            else
                invoc(static_cast<const char *>(nullptr));
        }
        auto result = invoc.exec();
        txn.commit();

        std::vector<Struct> rows;
        for (const auto &row : result)
            rows.push_back(PgRowMapper<Struct>::map(row));

        return rows;
    }

    // === Асинхронный интерфейс ===

    template<typename Struct>
    boost::asio::awaitable<std::optional<Struct>> execute_async_one(const PreparedStatement &stmt) {
        co_return co_await post_to_thread_pool<std::optional<Struct>>([this, stmt]() -> std::optional<Struct> {
            auto conn = acquire_async_connection(std::chrono::seconds(5));
            if (!conn) {
                Logger::get()->error("[Database] Async connection unavailable.");
                return std::nullopt;
            }

            try {
                pqxx::work txn(*conn);
                auto invoc = txn.prepared(stmt.name());
                for (const auto &param : stmt.params()) {
                    if (param.has_value())
                        invoc(param.value());
                    else
                        invoc(static_cast<const char *>(nullptr));
                }
                auto result = invoc.exec();
                txn.commit();

                release_async_connection(std::move(conn));

                if constexpr (std::is_same_v<Struct, NothingRow>)
                    return Struct{};

                if (result.empty())
                    return std::nullopt;

                return PgRowMapper<Struct>::map(result[0]);
            } catch (const std::exception &e) {
                Logger::get()->error("[Database] Async query failed: {}", e.what());
                release_async_connection(std::move(conn));
                return std::nullopt;
            }
        });
    }

    template<typename Struct>
    boost::asio::awaitable<std::vector<Struct>> execute_async_many(const PreparedStatement &stmt) {
        co_return co_await post_to_thread_pool<std::vector<Struct>>([this, stmt]() -> std::vector<Struct> {
            auto conn = acquire_async_connection(std::chrono::seconds(5));
            if (!conn) {
                Logger::get()->error("[Database] Async connection unavailable.");
                return {};
            }

            try {
                pqxx::work txn(*conn);
                auto invoc = txn.prepared(stmt.name());
                for (const auto &param : stmt.params()) {
                    if (param.has_value())
                        invoc(param.value());
                    else
                        invoc(static_cast<const char *>(nullptr));
                }
                auto result = invoc.exec();
                txn.commit();

                release_async_connection(std::move(conn));

                std::vector<Struct> rows;
                for (const auto &row : result)
                    rows.push_back(PgRowMapper<Struct>::map(row));

                return rows;
            } catch (const std::exception &e) {
                Logger::get()->error("[Database] Async query failed: {}", e.what());
                release_async_connection(std::move(conn));
                return {};
            }
        });
    }

    void shutdown() {
        Logger::get()->info("[Database] Shutting down...");
        work_guard_.reset();
        thread_pool_.stop();

        for (auto &t : async_threads_) {
            if (t.joinable()) t.join();
        }
        async_threads_.clear();

        {
            std::lock_guard<std::mutex> lock(mutex_);
            while (!connections_.empty()) {
                auto &c = connections_.front();
                if (c && c->is_open()) {
                    c->disconnect();
                    Logger::get()->info("[Database] Sync connection closed.");
                }
                connections_.pop();
            }
        }

        {
            std::lock_guard<std::mutex> lock(async_mutex_);
            while (!async_connections_.empty()) {
                auto &c = async_connections_.front();
                if (c && c->is_open()) {
                    c->disconnect();
                    Logger::get()->info("[Database] Async connection closed.");
                }
                async_connections_.pop();
            }
        }
    }

    class ScopedConnection {
    public:
        ScopedConnection(Database &db, std::unique_ptr<pqxx::connection> conn)
                : db_(db), conn_(std::move(conn)) {}

        ~ScopedConnection() {
            if (conn_) db_.release_connection(std::move(conn_));
        }

        pqxx::connection &get() { return *conn_; }

        ScopedConnection(const ScopedConnection &) = delete;
        ScopedConnection &operator=(const ScopedConnection &) = delete;
        ScopedConnection(ScopedConnection &&) = delete;
        ScopedConnection &operator=(ScopedConnection &&) = delete;

    private:
        Database &db_;
        std::unique_ptr<pqxx::connection> conn_;
    };

    ScopedConnection acquire_scoped_connection(std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        auto conn = acquire_connection(timeout);
        if (!conn) throw std::runtime_error("No DB connection available after timeout");
        return ScopedConnection(*this, std::move(conn));
    }

private:
    // --- Helper: Post task to async thread pool и получить результат awaitable ---
    template<typename ResultType, typename Func>
    boost::asio::awaitable<ResultType> post_to_thread_pool(Func func) {
        auto executor = co_await boost::asio::this_coro::executor;

        auto prom = std::make_shared<std::promise<ResultType>>();
        auto fut = prom->get_future();

        boost::asio::post(thread_pool_, [prom, func = std::move(func)]() mutable {
            try {
                if constexpr (std::is_void_v<ResultType>) {
                    func();
                    prom->set_value();
                } else {
                    prom->set_value(func());
                }
            } catch (...) {
                try {
                    prom->set_exception(std::current_exception());
                } catch (...) {}
            }
        });

        boost::asio::steady_timer timer(executor);

        while (fut.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            timer.expires_after(std::chrono::milliseconds(10));
            co_await timer.async_wait(boost::asio::use_awaitable);
        }

        co_return fut.get();
    }

    std::unique_ptr<pqxx::connection> acquire_connection(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (connections_.empty()) {
            if (!cond_.wait_for(lock, timeout, [this] { return !connections_.empty(); })) {
                return nullptr;
            }
        }

        auto conn = std::move(connections_.front());
        connections_.pop();

        if (!conn->is_open()) {
            Logger::get()->warn("[Database] Sync connection was closed. Reconnecting...");
            conn = reconnect_connection();
        }

        return conn;
    }

    void release_connection(std::unique_ptr<pqxx::connection> conn) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            connections_.push(std::move(conn));
        }
        cond_.notify_one();
    }

    std::unique_ptr<pqxx::connection> acquire_async_connection(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(async_mutex_);
        if (async_connections_.empty()) {
            if (!async_cond_.wait_for(lock, timeout, [this] { return !async_connections_.empty(); })) {
                return nullptr;
            }
        }

        auto conn = std::move(async_connections_.front());
        async_connections_.pop();

        if (!conn->is_open()) {
            Logger::get()->warn("[Database] Async connection was closed. Reconnecting...");
            conn = reconnect_connection();
        }

        return conn;
    }

    void release_async_connection(std::unique_ptr<pqxx::connection> conn) {
        {
            std::lock_guard<std::mutex> lock(async_mutex_);
            async_connections_.push(std::move(conn));
        }
        async_cond_.notify_one();
    }

    std::unique_ptr<pqxx::connection> reconnect_connection() {
        auto conn = std::make_unique<pqxx::connection>(conninfo_);
        prepare_all(*conn);
        Logger::get()->info("[Database] Connection re-established.");
        return conn;
    }

    void prepare_all(pqxx::connection &conn) {
        pqxx::work txn(conn);

        prepareAuthSchema(conn);
        prepareRelaySchema(conn);
        prepareDBCSchema(conn);
        prepareWorldSchema(conn);

        txn.commit();
    }

    void prepareAuthSchema(pqxx::connection &conn) {
        std::string auth_schema = std::getenv("AUTH_SCHEMA") ? std::string(std::getenv("AUTH_SCHEMA")) : "auth_server";

        conn.prepare("SELECT_ACCOUNT_BY_USERNAME",
                     fmt::format("SELECT "
                                 "id, username, salt, verifier, session_key_auth, session_key_bnet, "
                                 "totp_secret, email, reg_mail, joindate, last_ip, last_attempt_ip, "
                                 "failed_logins, locked, lock_country, last_login, online, expansion, "
                                 "mutetime, mutereason, muteby, locale, os, timezone_offset, recruiter, coins "
                                 "FROM {}.accounts WHERE username = $1", auth_schema));
        conn.prepare("SELECT_BUILD_INFO",
                     fmt::format("SELECT majorVersion, minorVersion, bugfixVersion, hotfixVersion, build FROM {}.build_info ORDER BY build ASC", auth_schema));
        conn.prepare("SELECT_BUILD_EXECUTABLE_HASH",
                     fmt::format("SELECT build, platform, executableHash FROM {}.build_executable_hash", auth_schema));
        conn.prepare("SELECT_REALMLIST",
                     fmt::format("SELECT id, name, address, local_address, local_subnet_mask, port, icon, flag, timezone, allowed_security_level, population, gamebuild FROM {}.realmlist WHERE flag <> 3 ORDER BY name", auth_schema));
        conn.prepare("SELECT_REALMLIST_BY_ID",
                     fmt::format("SELECT id, name, address, local_address, local_subnet_mask, port, icon, flag, timezone, allowed_security_level, population, gamebuild FROM {}.realmlist WHERE id = $1", auth_schema));
        conn.prepare("SELECT_REALM_CHARACTERS",
                     fmt::format("SELECT realmid, numchars FROM {}.realmcharacters WHERE acctid = $1", auth_schema));
        conn.prepare("UPDATE_LOGIN_LOGONPROOF",
                     fmt::format("UPDATE {}.accounts SET session_key_auth = decode($1, 'hex'), last_ip = $2, last_login = NOW(), locale = $3, os = $4, timezone_offset = $5 WHERE username = $6", auth_schema));
        conn.prepare("UPDATE_REALMLIST",
                     fmt::format("UPDATE {}.realmlist SET flag = $1, population = $2 WHERE id = $3", auth_schema));
        conn.prepare("INSERT_REALM_CHARACTERS",
                     fmt::format("INSERT INTO {}.realmcharacters (realmid, acctid, numchars) VALUES ($1, $2, $3)", auth_schema));
        conn.prepare("UPDATE_REALM_CHARACTERS",
                     fmt::format("UPDATE {}.realmcharacters SET numchars = $1 WHERE acctid = $2", auth_schema));
    }

    void prepareRelaySchema(pqxx::connection &conn) {
        std::string relay_schema = std::getenv("RELAY_SCHEMA") ? std::string(std::getenv("RELAY_SCHEMA")) : "relay_server";

        conn.prepare("REPLACE_ACCOUNT_DATA",
                     fmt::format("INSERT INTO {}.account_data (account_id, type, time, data) "
                                 "VALUES ($1, $2, $3, $4) "
                                 "ON CONFLICT (account_id, type) DO UPDATE SET "
                                 "time = EXCLUDED.time, data = EXCLUDED.data",
                                 relay_schema));
        conn.prepare("SELECT_ACCOUNT_DATA",
                     fmt::format("SELECT account_id, type, time, data FROM {}.account_data WHERE account_id = $1", relay_schema));

        conn.prepare("SELECT_ADDONS",
                     fmt::format("SELECT name, crc FROM {}.addons", relay_schema));
        conn.prepare("SELECT_BANNED_ADDONS",
                     fmt::format("SELECT id, name, version, timestamp FROM {}.banned_addons ORDER BY timestamp", relay_schema));

        conn.prepare("REPLACE_CHARACTER_ACCOUNT_DATA",
                     fmt::format("INSERT INTO {}.character_account_data (guid, type, time, data) "
                                 "VALUES ($1, $2, $3, $4) "
                                 "ON CONFLICT (guid, type) DO UPDATE SET "
                                 "time = EXCLUDED.time, data = EXCLUDED.data",
                                 relay_schema));
        conn.prepare("SELECT_CHARACTER_ACCOUNT_DATA",
                     fmt::format("SELECT guid, type, time, data FROM {}.character_account_data WHERE guid = $1", relay_schema));

        conn.prepare("SELECT_ACCOUNT_TUTORIALS",
                     fmt::format("SELECT account_id, tut0, tut1, tut2, tut3, tut4, tut5, tut6, tut7 FROM {}.account_tutorial WHERE account_id = $1", relay_schema));

        conn.prepare("SELECT_CHAR_ENUM",
                     fmt::format("SELECT c.guid, c.name, c.race, c.class, c.gender, c.skin, c.face, c.hairStyle, c.hairColor, c.facialStyle, "
                                 "c.level, c.zone, c.map, c.position_x, c.position_y, c.position_z, c.orientation, c.equipmentCache, c.playerFlags, c.at_login, c.istransfer, "
                                 "gm.guildid as guild_guildid, "
                                 "cp.entry as pet_entry, cp.modelid as pet_modelid, cp.level as pet_level, "
                                 "cb.guid as banned_guid "
                                 "FROM {}.characters AS c "
                                 "LEFT JOIN {}.character_pet AS cp ON c.guid = cp.owner AND cp.slot = $1 "
                                 "LEFT JOIN {}.guild_member AS gm ON c.guid = gm.guid "
                                 "LEFT JOIN {}.character_banned AS cb ON c.guid = cb.guid AND cb.active = 1 "
                                 "WHERE c.account = $2 AND c.deleteInfos_Name IS NULL "
                                 "ORDER BY c.guid",
                                 relay_schema, relay_schema, relay_schema, relay_schema));
        conn.prepare("SELECT_COUNT_CHARS_BY_USERNAME",
                     fmt::format("SELECT COUNT(*) "
                                 "FROM {}.characters WHERE name = $1", relay_schema));

        conn.prepare("INSERT_CHARACTER",
                     //                                         1       2    3      4      5       6     7    8     9     10      11         12          13          14         15          16
                     fmt::format("INSERT INTO {}.characters (account, name, race, class, gender, level, xp, money, skin, face, hairstyle, haircolor, facialstyle, bankslots, reststate, playerflags, "
                                 //17       18              19              20          21          22          23         24      25        26       27        28
                                 "map, instance_id, instance_mode_mask, position_x, position_y, position_z, orientation, trans_x, trans_y, trans_z, trans_o, transguid, "
                                 // 29         30
                                 "taximask, cinematic, "
                                 //   31        32          33          34              35                 36                  37
                                 "totaltime, leveltime, rest_bonus, logout_time, is_logout_resting, resettalents_cost, resettalents_time, "
                                 //   38             39         40      41
                                 "extra_flags, stable_slots, at_login, zone, "
                                 //     42              43          44              45                46                47                 48
                                 "death_expire_time, taxi_path, arenapoints, totalhonorpoints, todayhonorpoints, yesterdayhonorpoints, totalkills, "
                                 //    49           50            51              52              53         54     55       56      57     58
                                 "todaykills, yesterdaykills, chosentitle, knowncurrencies, watchedfaction, drunk, health, power1, power2, power3, "
                                 // 59      60      61      62      63              64                65               66             67          68         69          70            71
                                 "power4, power5, power6, power7, latency, talentgroupscount, activetalentgroup, exploredzones, equipmentcache, ammoid, knowntitles, actionbars, grantablelevels) "
                                 "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12, $13, $14, $15, $16, $17, $18, $19, $20, $21, $22, $23, $24, $25, $26, $27, $28, $29, $30, $31, $32, $33, $34, $35, $36, $37, $38, $39, $40, $41, $42, $43, $44, $45, $46, $47, $48, $49, $50, $51, $52, $53, $54, $55, $56, $57, $58, $59, $60, $61, $62, $63, $64, $65, $66, $67, $68, $69, $70, $71)",
                                 relay_schema));

        conn.prepare("SELECT_PLAYER_CREATE_INFO",
                     fmt::format("SELECT race, class, map, zone, position_x, position_y, position_z, orientation FROM {}.playercreateinfo", relay_schema));
        conn.prepare("SELECT_PLAYER_CREATE_INFO_ITEM",
                     fmt::format("SELECT race, class, itemid, amount FROM {}.playercreateinfo_item", relay_schema));
        conn.prepare("SELECT_PLAYER_CREATE_INFO_SKILLS",
                     fmt::format("SELECT racemask, classmask, skill, rank, comment FROM {}.playercreateinfo_skills", relay_schema));

    }

    void prepareDBCSchema(pqxx::connection &conn) {
        std::string dbc_schema = std::getenv("DBC_SCHEMA") ? std::string(std::getenv("DBC_SCHEMA")) : "dbc";

        conn.prepare("SELECT_DBC_CHRCLASSES",
                     fmt::format("SELECT "
                                 "id, field01, displaypower, petnametoken, "
                                 "name_lang_enus, name_lang_engb, name_lang_kokr, name_lang_frfr, name_lang_dede, "
                                 "name_lang_encn, name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                                 "name_lang_eses, name_lang_esmx, name_lang_ruru, name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                                 "name_lang_unk, name_lang_mask, "
                                 "name_female_lang_enus, name_female_lang_engb, name_female_lang_kokr, name_female_lang_frfr, name_female_lang_dede, "
                                 "name_female_lang_encn, name_female_lang_zhcn, name_female_lang_entw, name_female_lang_zhtw, "
                                 "name_female_lang_eses, name_female_lang_esmx, name_female_lang_ruru, name_female_lang_ptpt, name_female_lang_ptbr, name_female_lang_itit, "
                                 "name_female_lang_unk, name_female_lang_mask, "
                                 "name_male_lang_enus, name_male_lang_engb, name_male_lang_kokr, name_male_lang_frfr, name_male_lang_dede, "
                                 "name_male_lang_encn, name_male_lang_zhcn, name_male_lang_entw, name_male_lang_zhtw, "
                                 "name_male_lang_eses, name_male_lang_esmx, name_male_lang_ruru, name_male_lang_ptpt, name_male_lang_ptbr, name_male_lang_itit, "
                                 "name_male_lang_unk, name_male_lang_mask, "
                                 "filename, spellclassset, flags, cinematicsequenceid, required_expansion "
                                 "FROM {}.dbc_chrclasses", dbc_schema));

        conn.prepare("SELECT_DBC_CHRRACES",
                     fmt::format("SELECT "
                                 "id, flags, factionid, explorationsoundid, "
                                 "maledisplayid, femaledisplayid, clientprefix, "
                                 "baselanguage, creaturetype, ressicknessspellid, "
                                 "splashsoundid, clientfilestring, cinematicsequenceid, "
                                 "alliance, "
                                 "name_lang_enus, name_lang_engb, name_lang_kokr, "
                                 "name_lang_frfr, name_lang_dede, name_lang_encn, "
                                 "name_lang_zhcn, name_lang_entw, name_lang_zhtw, "
                                 "name_lang_eses, name_lang_esmx, name_lang_ruru, "
                                 "name_lang_ptpt, name_lang_ptbr, name_lang_itit, "
                                 "name_lang_unk, name_lang_mask, "
                                 "name_female_lang_enus, name_female_lang_engb, "
                                 "name_female_lang_kokr, name_female_lang_frfr, "
                                 "name_female_lang_dede, name_female_lang_encn, "
                                 "name_female_lang_zhcn, name_female_lang_entw, "
                                 "name_female_lang_zhtw, name_female_lang_eses, "
                                 "name_female_lang_esmx, name_female_lang_ruru, "
                                 "name_female_lang_ptpt, name_female_lang_ptbr, "
                                 "name_female_lang_itit, name_female_lang_unk, "
                                 "name_female_lang_mask, "
                                 "name_male_lang_enus, name_male_lang_engb, "
                                 "name_male_lang_kokr, name_male_lang_frfr, "
                                 "name_male_lang_dede, name_male_lang_encn, "
                                 "name_male_lang_zhcn, name_male_lang_entw, "
                                 "name_male_lang_zhtw, name_male_lang_eses, "
                                 "name_male_lang_esmx, name_male_lang_ruru, "
                                 "name_male_lang_ptpt, name_male_lang_ptbr, "
                                 "name_male_lang_itit, name_male_lang_unk, "
                                 "name_male_lang_mask, "
                                 "facialhaircustomization_1, facialhaircustomization_2, "
                                 "haircustomization, required_expansion "
                                 "FROM {}.dbc_chrraces", dbc_schema));

        conn.prepare("SELECT_DBC_CHARSTARTOUTFIT",
                     fmt::format("SELECT "
                                 "id, raceid, classid, sexid, outfitid, "
                                 "itemid_1, itemid_2, itemid_3, itemid_4, itemid_5, itemid_6, "
                                 "itemid_7, itemid_8, itemid_9, itemid_10, itemid_11, itemid_12, "
                                 "itemid_13, itemid_14, itemid_15, itemid_16, itemid_17, itemid_18, "
                                 "itemid_19, itemid_20, itemid_21, itemid_22, itemid_23, itemid_24, "
                                 "displayitemid_1, displayitemid_2, displayitemid_3, displayitemid_4, displayitemid_5, displayitemid_6, "
                                 "displayitemid_7, displayitemid_8, displayitemid_9, displayitemid_10, displayitemid_11, displayitemid_12, "
                                 "displayitemid_13, displayitemid_14, displayitemid_15, displayitemid_16, displayitemid_17, displayitemid_18, "
                                 "displayitemid_19, displayitemid_20, displayitemid_21, displayitemid_22, displayitemid_23, displayitemid_24, "
                                 "inventorytype_1, inventorytype_2, inventorytype_3, inventorytype_4, inventorytype_5, inventorytype_6, "
                                 "inventorytype_7, inventorytype_8, inventorytype_9, inventorytype_10, inventorytype_11, inventorytype_12, "
                                 "inventorytype_13, inventorytype_14, inventorytype_15, inventorytype_16, inventorytype_17, inventorytype_18, "
                                 "inventorytype_19, inventorytype_20, inventorytype_21, inventorytype_22, inventorytype_23, inventorytype_24 "
                                 "FROM {}.dbc_charstartoutfit",
                                 dbc_schema));

        conn.prepare("SELECT_DBC_SKILLRACECLASSINFO",
                     fmt::format("SELECT "
                                 "id, skillid, racemask, classmask, "
                                 "flags, minlevel, skilltierid, skillcostindex "
                                 "FROM {}.dbc_skillraceclassinfo",
                                 dbc_schema));

        conn.prepare("SELECT_DBC_SKILLLINE",
                     fmt::format("SELECT "
                                 "id, categoryid, skillcostsid, "
                                 "displayname_lang_enus, displayname_lang_engb, displayname_lang_kokr, displayname_lang_frfr, displayname_lang_dede, "
                                 "displayname_lang_encn, displayname_lang_zhcn, displayname_lang_entw, displayname_lang_zhtw, "
                                 "displayname_lang_eses, displayname_lang_esmx, displayname_lang_ruru, displayname_lang_ptpt, "
                                 "displayname_lang_ptbr, displayname_lang_itit, displayname_lang_unk, displayname_lang_mask, "
                                 "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                                 "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                                 "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                                 "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                                 "spelliconid, "
                                 "alternateverb_lang_enus, alternateverb_lang_engb, alternateverb_lang_kokr, alternateverb_lang_frfr, alternateverb_lang_dede, "
                                 "alternateverb_lang_encn, alternateverb_lang_zhcn, alternateverb_lang_entw, alternateverb_lang_zhtw, "
                                 "alternateverb_lang_eses, alternateverb_lang_esmx, alternateverb_lang_ruru, alternateverb_lang_ptpt, "
                                 "alternateverb_lang_ptbr, alternateverb_lang_itit, alternateverb_lang_unk, alternateverb_lang_mask, "
                                 "canlink "
                                 "FROM {}.dbc_skillline",
                                 dbc_schema));

        conn.prepare("SELECT_DBC_ACHIEVEMENT",
                     fmt::format(
                             "SELECT "
                             "id, faction, instance_id, supercedes, "
                             "title_lang_enus, title_lang_engb, title_lang_kokr, title_lang_frfr, title_lang_dede, "
                             "title_lang_encn, title_lang_zhcn, title_lang_entw, title_lang_zhtw, "
                             "title_lang_eses, title_lang_esmx, title_lang_ruru, title_lang_ptpt, "
                             "title_lang_ptbr, title_lang_itit, title_lang_unk, title_lang_mask, "
                             "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                             "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                             "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                             "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                             "category, points, ui_order, flags, iconid, "
                             "reward_lang_enus, reward_lang_engb, reward_lang_kokr, reward_lang_frfr, reward_lang_dede, "
                             "reward_lang_encn, reward_lang_zhcn, reward_lang_entw, reward_lang_zhtw, "
                             "reward_lang_eses, reward_lang_esmx, reward_lang_ruru, reward_lang_ptpt, "
                             "reward_lang_ptbr, reward_lang_itit, reward_lang_unk, reward_lang_mask, "
                             "minimum_criteria, shares_criteria "
                             "FROM {}.dbc_achievement",
                             dbc_schema
                     )
        );

        conn.prepare("SELECT_DBC_ACHIEVEMENT_CRITERIA",
                     fmt::format(
                             "SELECT "
                             "id, achievement_id, \"type\", asset_id, quantity, start_event, start_asset, fail_event, fail_asset, "
                             "description_lang_enus, description_lang_engb, description_lang_kokr, description_lang_frfr, description_lang_dede, "
                             "description_lang_encn, description_lang_zhcn, description_lang_entw, description_lang_zhtw, "
                             "description_lang_eses, description_lang_esmx, description_lang_ruru, description_lang_ptpt, "
                             "description_lang_ptbr, description_lang_itit, description_lang_unk, description_lang_mask, "
                             "flags, timer_start_event, timer_asset_id, timer_time, ui_order "
                             "FROM {}.dbc_achievement_criteria",
                             dbc_schema
                     )
        );
    }

    void prepareWorldSchema(pqxx::connection &conn) {
        std::string world_schema = std::getenv("WORLD_SCHEMA") ? std::string(std::getenv("DBC_SCHEMA")) : "world";

        conn.prepare("SELECT_ITEM_TEMPLATE",
                     fmt::format("SELECT "
                                 "entry, class, subclass, soundoverridesubclass, "
                                 "name, displayid, quality, flags, flagsextra, "
                                 "buycount, buyprice, sellprice, inventorytype, "
                                 "allowableclass, allowablerace, itemlevel, "
                                 "requiredlevel, requiredskill, requiredskillrank, "
                                 "requiredspell, requiredhonorrank, requiredcityrank, "
                                 "requiredreputationfaction, requiredreputationrank, "
                                 "maxcount, stackable, containerslots, statscount, "
                                 "stat_type1, stat_value1, stat_type2, stat_value2, "
                                 "stat_type3, stat_value3, stat_type4, stat_value4, "
                                 "stat_type5, stat_value5, stat_type6, stat_value6, "
                                 "stat_type7, stat_value7, stat_type8, stat_value8, "
                                 "stat_type9, stat_value9, stat_type10, stat_value10, "
                                 "scalingstatdistribution, scalingstatvalue, "
                                 "dmg_min1, dmg_max1, dmg_type1, dmg_min2, dmg_max2, dmg_type2, "
                                 "armor, holy_res, fire_res, nature_res, frost_res, shadow_res, arcane_res, "
                                 "delay, ammo_type, rangedmodrange, "
                                 "spellid_1, spelltrigger_1, spellcharges_1, spellppmrate_1, spellcooldown_1, spellcategory_1, spellcategorycooldown_1, "
                                 "spellid_2, spelltrigger_2, spellcharges_2, spellppmrate_2, spellcooldown_2, spellcategory_2, spellcategorycooldown_2, "
                                 "spellid_3, spelltrigger_3, spellcharges_3, spellppmrate_3, spellcooldown_3, spellcategory_3, spellcategorycooldown_3, "
                                 "spellid_4, spelltrigger_4, spellcharges_4, spellppmrate_4, spellcooldown_4, spellcategory_4, spellcategorycooldown_4, "
                                 "spellid_5, spelltrigger_5, spellcharges_5, spellppmrate_5, spellcooldown_5, spellcategory_5, spellcategorycooldown_5, "
                                 "bonding, description, pagetext, languageid, pagematerial, "
                                 "startquest, lockid, material, sheath, "
                                 "randomproperty, randomsuffix, block, itemset, maxdurability, "
                                 "area, map, bagfamily, totemcategory, "
                                 "socketcolor_1, socketcontent_1, socketcolor_2, socketcontent_2, socketcolor_3, socketcontent_3, "
                                 "socketbonus, gemproperties, requireddisenchantskill, armordamagemodifier, "
                                 "duration, itemlimitcategory, holidayid, scriptname, "
                                 "disenchantid, foodtype, minmoneyloot, maxmoneyloot, flagscustom, verifiedbuild "
                                 "FROM {}.item_template", world_schema));
    }

private:
    std::string conninfo_;

    std::queue<std::unique_ptr<pqxx::connection>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;

    boost::asio::io_context thread_pool_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::vector<std::thread> async_threads_;

    std::queue<std::unique_ptr<pqxx::connection>> async_connections_;
    std::mutex async_mutex_;
    std::condition_variable async_cond_;
};

#pragma GCC diagnostic pop
