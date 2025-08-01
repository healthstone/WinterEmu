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
#include <boost/asio.hpp>

#include "QueryResults.hpp"
#include "PreparedStatement.hpp"
#include "Logger.hpp"

class Database {
public:
    explicit Database(const std::string &conninfo, size_t sync_pool_size = 4, size_t async_threads = 2)
            : conninfo_(conninfo),
              async_pool_(),
              async_threads_count_(async_threads)
    {
        Logger::get()->info("[Database] Setup Sync and Async connection pools...");
        // Инициализация sync-пула соединений
        for (size_t i = 0; i < sync_pool_size; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            prepare_all(*conn);
            Logger::get()->trace("[Database] Sync connection {} established.", i + 1);
            connections_.push(std::move(conn));
        }
        Logger::get()->info("[Database] Sync  connection pool established - size {}", sync_pool_size);

        // Инициализация async-пула соединений
        for (size_t i = 0; i < async_threads_count_; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            prepare_all(*conn);
            Logger::get()->trace("[Database] Async connection {} established.", i + 1);
            async_connections_.push(std::move(conn));
        }
        Logger::get()->info("[Database] Async connection pool established - size {}", async_threads_count_);
    }

    ~Database() {
        shutdown();
    }

    /// Запустить async-пул потоков
    void start_async_pool() {
        Logger::get()->info("[Database] Starting async pool with {} threads...", async_threads_count_);
        for (size_t i = 0; i < async_threads_count_; ++i) {
            async_threads_.emplace_back([this]() {
                async_pool_.run();
            });
        }
    }

    /// Остановить async-пул
    void stop_async_pool() {
        Logger::get()->info("[Database] Stopping async pool...");
        async_pool_.stop();
    }

    /// Дождаться завершения async-потоков
    void join_async_pool() {
        for (auto &t : async_threads_) {
            if (t.joinable()) t.join();
        }
        Logger::get()->info("[Database] All async threads joined.");
    }

    /// Доступ к io_context async-пула
    boost::asio::io_context &async_pool() { return async_pool_; }

    /// RAII обёртка для sync соединений
    class ScopedConnection {
    public:
        ScopedConnection(Database &db, std::unique_ptr<pqxx::connection> conn)
                : db_(db), conn_(std::move(conn)) {}

        ~ScopedConnection() {
            if (conn_) db_.release_connection(std::move(conn_));
        }

        pqxx::connection &get() { return *conn_; }

    private:
        Database &db_;
        std::unique_ptr<pqxx::connection> conn_;
    };

    ScopedConnection acquire_scoped_connection(std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        auto conn = acquire_connection(timeout);
        if (!conn) throw std::runtime_error("No DB connection available after timeout");
        return ScopedConnection(*this, std::move(conn));
    }

    /// Синхронный запрос один результат
    template<typename Struct>
    std::optional<Struct> execute_sync_one(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();
        pqxx::work txn(scoped.get());
        auto invoc = txn.prepared(stmt.name());
        for (const auto &param : stmt.params()) {
            if (param.has_value()) invoc(param.value());
            else invoc(static_cast<const char *>(nullptr));
        }
        auto result = invoc.exec();
        txn.commit();

        if constexpr (std::is_same_v<Struct, NothingRow>) return Struct{};
        if (result.empty()) return std::nullopt;
        return PgRowMapper<Struct>::map(result[0]);
    }

    /// Синхронный запрос много результатов
    template<typename Struct>
    std::vector<Struct> execute_sync_many(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();
        pqxx::work txn(scoped.get());
        auto invoc = txn.prepared(stmt.name());
        for (const auto &param : stmt.params()) {
            if (param.has_value()) invoc(param.value());
            else invoc(static_cast<const char *>(nullptr));
        }
        auto result = invoc.exec();
        txn.commit();

        std::vector<Struct> rows;
        for (auto const &row : result) rows.push_back(PgRowMapper<Struct>::map(row));
        return rows;
    }

    /// Асинхронный запрос один результат (использует async пул соединений)
    template<typename Struct>
    void execute_async_one(const PreparedStatement &stmt, std::function<void(std::optional<Struct>)> callback) {
        boost::asio::post(async_pool_, [this, stmt, callback]() {
            auto conn = acquire_async_connection(std::chrono::seconds(5));
            if (!conn) {
                Logger::get()->error("[Database] Async connection unavailable.");
                callback(std::nullopt);
                return;
            }
            try {
                pqxx::work txn(*conn);
                auto invoc = txn.prepared(stmt.name());
                for (const auto &param : stmt.params()) {
                    if (param.has_value()) invoc(param.value());
                    else invoc(static_cast<const char *>(nullptr));
                }
                auto result = invoc.exec();
                txn.commit();

                if constexpr (std::is_same_v<Struct, NothingRow>) {
                    callback(Struct{});
                } else if (result.empty()) {
                    callback(std::nullopt);
                } else {
                    callback(PgRowMapper<Struct>::map(result[0]));
                }
            } catch (const std::exception &e) {
                Logger::get()->error("[Database] Async query failed: {}", e.what());
                callback(std::nullopt);
            }
            release_async_connection(std::move(conn));
        });
    }

    /// Асинхронный запрос много результатов (использует async пул соединений)
    template<typename Struct>
    void execute_async_many(const PreparedStatement &stmt, std::function<void(std::vector<Struct>)> callback) {
        boost::asio::post(async_pool_, [this, stmt, callback]() {
            auto conn = acquire_async_connection(std::chrono::seconds(5));
            if (!conn) {
                Logger::get()->error("[Database] Async connection unavailable.");
                callback({});
                return;
            }
            try {
                pqxx::work txn(*conn);
                auto invoc = txn.prepared(stmt.name());
                for (const auto &param : stmt.params()) {
                    if (param.has_value()) invoc(param.value());
                    else invoc(static_cast<const char *>(nullptr));
                }
                auto result = invoc.exec();
                txn.commit();

                std::vector<Struct> rows;
                for (auto const &row : result) rows.push_back(PgRowMapper<Struct>::map(row));
                callback(std::move(rows));
            } catch (const std::exception &e) {
                Logger::get()->error("[Database] Async query failed: {}", e.what());
                callback({});
            }
            release_async_connection(std::move(conn));
        });
    }

    /// Полная остановка и очистка всех ресурсов
    void shutdown() {
        Logger::get()->info("[Database] Shutting down...");
        stop_async_pool();
        join_async_pool();

        {
            std::unique_lock<std::mutex> lock(mutex_);
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
            std::unique_lock<std::mutex> lock(async_mutex_);
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

private:
    // Для sync соединений
    std::unique_ptr<pqxx::connection> acquire_connection(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (connections_.empty()) {
            if (!cond_.wait_for(lock, timeout, [this] { return !connections_.empty(); })) {
                return nullptr; // timeout
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

    // Для async соединений
    std::unique_ptr<pqxx::connection> acquire_async_connection(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(async_mutex_);
        if (async_connections_.empty()) {
            if (!async_cond_.wait_for(lock, timeout, [this] { return !async_connections_.empty(); })) {
                return nullptr; // timeout
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
        std::string auth_schema = std::getenv("AUTH_SCHEMA") ?: "auth_server";

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
        txn.commit();
    }

    std::string conninfo_;

    // Sync pool
    std::queue<std::unique_ptr<pqxx::connection>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;

    // Async pool
    boost::asio::io_context async_pool_;
    size_t async_threads_count_;
    std::vector<std::thread> async_threads_;

    std::queue<std::unique_ptr<pqxx::connection>> async_connections_;
    std::mutex async_mutex_;
    std::condition_variable async_cond_;
};

#pragma GCC diagnostic pop
