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
#include "PreparedStatement.hpp"
#include "Logger.hpp"
#include "AsyncQueryQueue.hpp"
#include "AsyncQueryExecutor.hpp"
#include "DatabasePreparer.hpp"

// ... инклюды мапперов ...
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
#include "database/mapper/dbc/PgDbcAreaTable.hpp"
#include "database/mapper/dbc/PgDbcAreaGroup.hpp"
#include "database/mapper/dbc/PgDbcAreaTrigger.hpp"
#include "database/mapper/dbc/PgDbcAuctionHouse.hpp"
#include "database/mapper/dbc/PgDbcBankBagSlotPrices.hpp"
#include "database/mapper/dbc/PgDbcBannedAddons.hpp"
#include "database/mapper/dbc/PgDbcBarberShopStyle.hpp"
#include "database/mapper/dbc/PgDbcBattleMasterList.hpp"
#include "database/mapper/dbc/PgDbcCharacterFacialHairstyles.hpp"
#include "database/mapper/dbc/PgDbcCharSections.hpp"
#include "database/mapper/dbc/PgDbcCharStartOutFit.hpp"
#include "database/mapper/dbc/PgDbcCharTitles.hpp"
#include "database/mapper/dbc/PgDbcChatChannels.hpp"
#include "database/mapper/dbc/PgDbcChrClasses.hpp"
#include "database/mapper/dbc/PgDbcChrRaces.hpp"
#include "database/mapper/dbc/PgDbcCinematicCamera.hpp"
#include "database/mapper/dbc/PgDbcCinematicSequences.hpp"
#include "database/mapper/dbc/PgDbcCreatureDisplayInfo.hpp"
#include "database/mapper/dbc/PgDbcCreatureDisplayInfoExtra.hpp"
#include "database/mapper/dbc/PgDbcCreatureFamily.hpp"
#include "database/mapper/dbc/PgDbcCreatureModelData.hpp"
#include "database/mapper/dbc/PgDbcCreatureSpellData.hpp"
#include "database/mapper/dbc/PgDbcCreatureType.hpp"
#include "database/mapper/dbc/PgDbcCurrencyCategory.hpp"
#include "database/mapper/dbc/PgDbcCurrencyTypes.hpp"
#include "database/mapper/dbc/PgDbcDestructibleModelData.hpp"
#include "database/mapper/dbc/PgDbcDungeonEncounter.hpp"
#include "database/mapper/dbc/PgDbcDurabilityCosts.hpp"
#include "database/mapper/dbc/PgDbcDurabilityQuality.hpp"
#include "database/mapper/dbc/PgDbcEmotes.hpp"
#include "database/mapper/dbc/PgDbcEmotesText.hpp"
#include "database/mapper/dbc/PgDbcEmotesTextSound.hpp"
#include "database/mapper/dbc/PgDbcFaction.hpp"
#include "database/mapper/dbc/PgDbcFactionTemplate.hpp"
#include "database/mapper/dbc/PgDbcGameObjectArtKit.hpp"
#include "database/mapper/dbc/PgDbcGameObjectDisplayInfo.hpp"
#include "database/mapper/dbc/PgDbcGemProperties.hpp"
#include "database/mapper/dbc/PgDbcGlyphProperties.hpp"
#include "database/mapper/dbc/PgDbcGlyphSlot.hpp"
#include "database/mapper/dbc/PgDbcGtBarberShopCostBase.hpp"
#include "database/mapper/dbc/PgDbcGtChanceToMeleeCrit.hpp"
#include "database/mapper/dbc/PgDbcGtChanceToMeleeCritBase.hpp"
#include "database/mapper/dbc/PgDbcGtChanceToSpellCrit.hpp"
#include "database/mapper/dbc/PgDbcGtChanceToSpellCritBase.hpp"
#include "database/mapper/dbc/PgDbcGtCombatRatings.hpp"
#include "database/mapper/dbc/PgDbcGtnpcManaCostScaler.hpp"
#include "database/mapper/dbc/PgDbcGtoctClassCombatRatingScalar.hpp"
#include "database/mapper/dbc/PgDbcGtoctRegenHP.hpp"
#include "database/mapper/dbc/PgDbcGtoctRegenMP.hpp"
#include "database/mapper/dbc/PgDbcGtRegenHpPerSpt.hpp"
#include "database/mapper/dbc/PgDbcGtRegenMpPerSpt.hpp"
#include "database/mapper/dbc/PgDbcHolidays.hpp"
#include "database/mapper/dbc/PgDbcItem.hpp"
#include "database/mapper/dbc/PgDbcItemBagFamily.hpp"
#include "database/mapper/dbc/PgDbcItemDisplayInfo.hpp"
#include "database/mapper/dbc/PgDbcItemExtendedCost.hpp"
#include "database/mapper/dbc/PgDbcItemLimitCategory.hpp"
#include "database/mapper/dbc/PgDbcItemRandomProperties.hpp"
#include "database/mapper/dbc/PgDbcItemRandomSuffix.hpp"
#include "database/mapper/dbc/PgDbcItemSet.hpp"
#include "database/mapper/dbc/PgDbcLfgDungeons.hpp"
#include "database/mapper/dbc/PgDbcLight.hpp"
#include "database/mapper/dbc/PgDbcLiquidType.hpp"
#include "database/mapper/dbc/PgDbcLock.hpp"
#include "database/mapper/dbc/PgDbcMailTemplate.hpp"
#include "database/mapper/dbc/PgDbcMap.hpp"
#include "database/mapper/dbc/PgDbcMapDifficulty.hpp"
#include "database/mapper/dbc/PgDbcMovie.hpp"
#include "database/mapper/dbc/PgDbcNamesProfanity.hpp"
#include "database/mapper/dbc/PgDbcNamesReserved.hpp"
#include "database/mapper/dbc/PgDbcOverridespellData.hpp"
#include "database/mapper/dbc/PgDbcPowerDisplay.hpp"
#include "database/mapper/dbc/PgDbcPvpdifficulty.hpp"
#include "database/mapper/dbc/PgDbcQuestFactionReward.hpp"
#include "database/mapper/dbc/PgDbcQuestsort.hpp"
#include "database/mapper/dbc/PgDbcQuestXp.hpp"
#include "database/mapper/dbc/PgDbcRandPropPoints.hpp"
#include "database/mapper/dbc/PgDbcScalingStatDistribution.hpp"
#include "database/mapper/dbc/PgDbcScalingStatValues.hpp"
#include "database/mapper/dbc/PgDbcSkillLine.hpp"
#include "database/mapper/dbc/PgDbcSkillLineAbility.hpp"
#include "database/mapper/dbc/PgDbcSkillRaceClassInfo.hpp"


#include "database/mapper/world/PgItemTemplate.hpp"

class Database {
public:
    explicit Database(const std::string &conninfo, size_t sync_pool_size = 4, size_t async_threads = 2)
            : conninfo_(conninfo),
              async_query_queue_(),
              async_query_executor_(*this, async_query_queue_, async_threads),
              thread_pool_(async_threads),
              work_guard_(boost::asio::make_work_guard(thread_pool_)) {

        Logger::get()->info("[Database] Setting up Sync connection pool...");
        for (size_t i = 0; i < sync_pool_size; ++i) {
            auto conn = std::make_unique<pqxx::connection>(conninfo_);
            DatabasePreparer::prepare_all(*conn);
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
            DatabasePreparer::prepare_all(*conn);
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

        // Запускаем исполнитель асинхронных очередей
        async_query_executor_.start();

        // Настраиваем очередь по умолчанию
        async_query_executor_.set_queue_connection_limit("default", async_threads);

        Logger::get()->info("[Database] Async query executor started with {} threads", async_threads);
        Logger::get()->info("[Database] Default queue configured with {} connections", async_threads);
    }

    ~Database() {
        shutdown();
    }

    // === НОВЫЕ АСИНХРОННЫЕ МЕТОДЫ С ОЧЕРЕДЯМИ ===

    template<typename Struct>
    boost::asio::awaitable<std::optional<Struct>> execute_async_one_queued(
            const std::string& queue_key, const PreparedStatement& stmt,
            const std::string& description = "") {

        auto promise = std::make_shared<std::promise<std::optional<Struct>>>();
        auto future = promise->get_future();

        // Добавляем запрос в очередь
        async_query_queue_.enqueue(queue_key, stmt,
                                   [promise, stmt](std::unique_ptr<pqxx::connection>& conn) {
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

                                           if constexpr (std::is_same_v<Struct, NothingRow>) {
                                               promise->set_value(Struct{});
                                           } else {
                                               promise->set_value(result.empty() ? std::nullopt
                                                                                 : std::make_optional(PgRowMapper<Struct>::map(result[0])));
                                           }
                                       } catch (const std::exception& e) {
                                           Logger::get()->error("[Database] Queued async query failed: {}", e.what());
                                           promise->set_exception(std::current_exception());
                                       }
                                   }, description);

        // Ожидаем результат через таймер
        auto executor = co_await boost::asio::this_coro::executor;
        boost::asio::steady_timer timer(executor);

        while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            timer.expires_after(std::chrono::milliseconds(10));
            co_await timer.async_wait(boost::asio::use_awaitable);
        }

        co_return future.get();
    }

    template<typename Struct>
    boost::asio::awaitable<std::vector<Struct>> execute_async_many_queued(
            const std::string& queue_key, const PreparedStatement& stmt,
            const std::string& description = "") {

        auto promise = std::make_shared<std::promise<std::vector<Struct>>>();
        auto future = promise->get_future();

        async_query_queue_.enqueue(queue_key, stmt,
                                   [promise, stmt](std::unique_ptr<pqxx::connection>& conn) {
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

                                           std::vector<Struct> rows;
                                           for (const auto &row : result) {
                                               rows.push_back(PgRowMapper<Struct>::map(row));
                                           }
                                           promise->set_value(std::move(rows));
                                       } catch (const std::exception& e) {
                                           Logger::get()->error("[Database] Queued async query failed: {}", e.what());
                                           promise->set_exception(std::current_exception());
                                       }
                                   }, description);

        auto executor = co_await boost::asio::this_coro::executor;
        boost::asio::steady_timer timer(executor);

        while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            timer.expires_after(std::chrono::milliseconds(10));
            co_await timer.async_wait(boost::asio::use_awaitable);
        }

        co_return future.get();
    }

    // === ОБНОВЛЕННЫЕ АСИНХРОННЫЕ МЕТОДЫ (С ОЧЕРЕДЯМИ) ===

    template<typename Struct>
    boost::asio::awaitable<std::optional<Struct>> execute_async_one(const PreparedStatement &stmt) {
        // Используем очередь "default" для обратной совместимости
        co_return co_await execute_async_one_queued<Struct>("default", stmt, "Legacy async query");
    }

    template<typename Struct>
    boost::asio::awaitable<std::vector<Struct>> execute_async_many(const PreparedStatement &stmt) {
        // Используем очередь "default" для обратной совместимости
        co_return co_await execute_async_many_queued<Struct>("default", stmt, "Legacy async query");
    }

    // === НОВЫЕ ПЕРЕГРУЗКИ С ВОЗМОЖНОСТЬЮ УКАЗАТЬ ОЧЕРЕДЬ ===

    template<typename Struct>
    boost::asio::awaitable<std::optional<Struct>> execute_async_one(
            const PreparedStatement &stmt,
            const std::string& queue_key,
            const std::string& description = "") {

        co_return co_await execute_async_one_queued<Struct>(queue_key, stmt, description);
    }

    template<typename Struct>
    boost::asio::awaitable<std::vector<Struct>> execute_async_many(
            const PreparedStatement &stmt,
            const std::string& queue_key,
            const std::string& description = "") {

        co_return co_await execute_async_many_queued<Struct>(queue_key, stmt, description);
    }

    // === УТИЛИТЫ ДЛЯ УПРАВЛЕНИЯ ОЧЕРЕДЯМИ ===

    size_t get_queue_size(const std::string& queue_key) const {
        return async_query_queue_.get_queue_size(queue_key);
    }

    size_t get_default_queue_size() const {
        return async_query_queue_.get_queue_size("default");
    }

    size_t get_total_queued_queries() const {
        return async_query_queue_.get_total_queries();
    }

    void set_queue_connection_limit(const std::string& queue_key, size_t limit) {
        async_query_executor_.set_queue_connection_limit(queue_key, limit);
    }

    void set_default_queue_connection_limit(size_t limit) {
        async_query_executor_.set_queue_connection_limit("default", limit);
    }

    size_t get_default_queue_connection_count() const {
        return async_query_executor_.get_queue_connection_count("default");
    }

    AsyncQueryExecutor::Statistics get_queue_statistics() const {
        return async_query_executor_.get_statistics();
    }

    // === СУЩЕСТВУЮЩИЕ СИНХРОННЫЕ МЕТОДЫ ===

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

    void shutdown() {
        Logger::get()->info("[Database] Shutting down...");

        // Останавливаем исполнитель очередей первым
        async_query_executor_.stop();

        // Затем останавливаем существующий асинхронный пул
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

    // === ДОПОЛНИТЕЛЬНЫЕ МЕТОДЫ ДЛЯ AsyncQueryExecutor ===
    const std::string& get_connection_string() const { return conninfo_; }

    void prepare_connection(pqxx::connection& conn) {
        DatabasePreparer::prepare_all(conn);
    }

private:
    std::string conninfo_;

    // Новые поля для управления очередями
    AsyncQueryQueue async_query_queue_;
    AsyncQueryExecutor async_query_executor_;

    // Существующие поля
    std::queue<std::unique_ptr<pqxx::connection>> connections_;
    std::mutex mutex_;
    std::condition_variable cond_;

    boost::asio::io_context thread_pool_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;
    std::vector<std::thread> async_threads_;

    std::queue<std::unique_ptr<pqxx::connection>> async_connections_;
    std::mutex async_mutex_;
    std::condition_variable async_cond_;

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
        DatabasePreparer::prepare_all(*conn);
        Logger::get()->info("[Database] Connection re-established.");
        return conn;
    }
};

#pragma GCC diagnostic pop