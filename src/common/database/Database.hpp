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

#include "QueryResults.hpp"
#include "PreparedStatement.hpp"
#include "Logger.hpp"

class Database {
public:
    explicit Database(const std::string &conninfo, size_t pool_size = 4)
            : conninfo_(conninfo) {
        for (size_t i = 0; i < pool_size; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            prepare_all(*conn);
            Logger::get()->info("[Database] Connection {} established", i + 1);
            connections_.push(std::move(conn));
        }
    }

    ~Database() {
        shutdown();
    }

    void shutdown() {
        std::unique_lock<std::mutex> lock(mutex_);
        while (!connections_.empty()) {
            auto &c = connections_.front();
            if (c && c->is_open()) {
                c->disconnect();
                Logger::get()->info("[Database] Connection closed.");
            }
            connections_.pop();
        }
    }

    /// RAII-хелпер
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

    /// Получить RAII обертку с таймаутом
    ScopedConnection acquire_scoped_connection(std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        auto conn = acquire_connection(timeout);
        if (!conn) throw std::runtime_error("No DB connection available after timeout");
        return ScopedConnection(*this, std::move(conn));
    }

    /// Выполнить запрос одиночный запрос( возврат 0 или 1 строки)
    template<typename Struct>
    std::optional<Struct> execute_sync_one(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();

        try {
            pqxx::work txn(scoped.get());
            auto invoc = txn.prepared(stmt.name());
            for (const auto &param: stmt.params()) {
                if (param.has_value()) {
                    invoc(param.value());
                } else {
                    invoc(static_cast<const char *>(nullptr));
                }
            }

            auto result = invoc.exec();
            txn.commit();

            if constexpr (std::is_same_v<Struct, NothingRow>) {
                return Struct{};
            }

            if (result.empty()) {
                return std::nullopt;
            }

            return PgRowMapper<Struct>::map(result[0]);
        }
        catch (const pqxx::broken_connection &) {
            Logger::get()->error("[Database] Connection broken. Attempting to reconnect.");
            auto reconnect = reconnect_connection();
            if (!reconnect) throw std::runtime_error("Reconnection failed");
            throw;
        }
    }

    template<typename Struct>
    std::vector<Struct> execute_sync_many(const PreparedStatement &stmt) {
        auto scoped = acquire_scoped_connection();

        try {
            pqxx::work txn(scoped.get());
            auto invoc = txn.prepared(stmt.name());
            for (const auto &param: stmt.params()) {
                if (param.has_value()) {
                    invoc(param.value());
                } else {
                    invoc(static_cast<const char *>(nullptr));
                }
            }

            auto result = invoc.exec();
            txn.commit();

            std::vector<Struct> rows;
            for (auto const& row : result) {
                rows.push_back(PgRowMapper<Struct>::map(row));
            }
            return rows;
        }
        catch (const pqxx::broken_connection &) {
            Logger::get()->error("[Database] Connection broken. Attempting to reconnect.");
            auto reconnect = reconnect_connection();
            if (!reconnect) throw std::runtime_error("Reconnection failed");
            throw;
        }
    }

private:
    std::unique_ptr<pqxx::connection> acquire_connection(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (connections_.empty()) {
            if (!cond_.wait_for(lock, timeout, [this] { return !connections_.empty(); })) {
                // timeout
                return nullptr;
            }
        }

        auto conn = std::move(connections_.front());
        connections_.pop();

        if (!conn->is_open()) {
            Logger::get()->warn("[Database] Connection was closed. Reconnecting.");
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

    std::unique_ptr<pqxx::connection> reconnect_connection() {
        auto conn = std::make_unique<pqxx::connection>(conninfo_);
        prepare_all(*conn);
        Logger::get()->info("[Database] Connection re-established.");
        return conn;
    }

    void prepare_all(pqxx::connection &conn) {
        pqxx::work txn(conn);
        conn.prepare("SELECT_ACCOUNT_BY_USERNAME",
                     "SELECT id, username, salt, verifier, email, created_at FROM accounts WHERE username = $1");
        conn.prepare("SELECT_BUILD_INFO",
                     "SELECT majorVersion, minorVersion, bugfixVersion, hotfixVersion, build FROM build_info ORDER BY build ASC");
        conn.prepare("SELECT_BUILD_EXECUTABLE_HASH",
                     "SELECT build, platform, executableHash FROM build_executable_hash");

        conn.prepare("UPDATE_LOGIN_LOGONPROOF",
                     "UPDATE accounts SET session_key_auth = decode($1, 'hex'), last_ip = $2, last_login = NOW() WHERE username = $3");

        conn.prepare("INSERT_ACCOUNT_BY_USERNAME",
                     "INSERT INTO accounts (username, salt, verifier) VALUES ($1, $2, $3) RETURNING id");
        txn.commit();
    }

    std::string conninfo_;
    std::queue<std::unique_ptr<pqxx::connection>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#pragma GCC diagnostic pop