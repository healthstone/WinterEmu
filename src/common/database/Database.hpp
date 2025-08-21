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
#include "AuthQueryResults.hpp"
#include "DBCQueryResults.hpp"
#include "RelayQueryResults.hpp"
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

        txn.commit();
    }

    void prepareAuthSchema(pqxx::connection &conn) {
        std::string auth_schema = std::getenv("AUTH_SCHEMA") ? std::string(std::getenv("AUTH_SCHEMA")) : "auth_server";

        conn.prepare("SELECT_ACCOUNT_BY_USERNAME",
                     fmt::format("SELECT id, username, salt, verifier, session_key_auth, email, created_at FROM {}.accounts WHERE username = $1", auth_schema));
        conn.prepare("SELECT_BUILD_INFO",
                     fmt::format("SELECT majorVersion, minorVersion, bugfixVersion, hotfixVersion, build FROM {}.build_info ORDER BY build ASC", auth_schema));
        conn.prepare("SELECT_BUILD_EXECUTABLE_HASH",
                     fmt::format("SELECT build, platform, executableHash FROM {}.build_executable_hash", auth_schema));
        conn.prepare("SELECT_REALMLIST",
                     fmt::format("SELECT id, name, address, local_address, local_subnet_mask, port, icon, flag, timezone, allowed_security_level, population, gamebuild FROM {}.realmlist WHERE flag <> 3 ORDER BY name", auth_schema));
        conn.prepare("SELECT_REALM_CHARACTERS",
                     fmt::format("SELECT realmid, numchars FROM {}.realmcharacters WHERE acctid = $1", auth_schema));
        conn.prepare("UPDATE_LOGIN_LOGONPROOF",
                     fmt::format("UPDATE {}.accounts SET session_key_auth = decode($1, 'hex'), last_ip = $2, last_login = NOW() WHERE username = $3", auth_schema));
        conn.prepare("INSERT_REALM_CHARACTERS",
                     fmt::format("INSERT INTO {}.realmcharacters (realmid, acctid, numchars) VALUES ($1, $2, $3)", auth_schema));
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

        conn.prepare("SELECT_ACCOUNT_TUTORIALS",
                     fmt::format("SELECT account_id, tut0, tut1, tut2, tut3, tut4, tut5, tut6, tut7 FROM {}.account_tutorial WHERE account_id = $1", relay_schema));

        conn.prepare("SELECT_CHAR_ENUM",
                     fmt::format("SELECT c.guid, c.name, c.race, c.class, c.gender, c.skin, c.face, c.hairStyle, c.hairColor, c.facialStyle, c.level, c.zone, c.map, c.position_x, c.position_y, c.position_z, "
                                 "gm.guildid, c.playerFlags, c.at_login, cp.entry, cp.modelid, cp.level, c.equipmentCache, cb.guid "
                                 "FROM {}.characters AS c "
                                 "LEFT JOIN {}.character_pet AS cp ON c.guid = cp.owner AND cp.slot = $1 "
                                 "LEFT JOIN {}.guild_member AS gm ON c.guid = gm.guid "
                                 "LEFT JOIN {}.character_banned AS cb ON c.guid = cb.guid AND cb.active = 1 "
                                 "WHERE c.account = $2 AND c.deleteInfos_Name IS NULL "
                                 "ORDER BY c.guid",
                                 relay_schema, relay_schema, relay_schema, relay_schema));
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
