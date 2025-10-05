#include "AsyncQueryExecutor.hpp"
#include "Database.hpp"
#include <chrono>

AsyncQueryExecutor::AsyncQueryExecutor(Database &db, AsyncQueryQueue &queue, size_t num_threads)
        : db_(db), queue_(queue), num_threads_(num_threads) {
    Logger::get()->info("[AsyncQueryExecutor] Created with {} threads", num_threads);
}

AsyncQueryExecutor::~AsyncQueryExecutor() {
    if (running_) {
        Logger::get()->error("[AsyncQueryExecutor] Destructor called while still running!");
        stop(); // Принудительная остановка
    }
}

void AsyncQueryExecutor::start() {
    if (running_) return;

    running_ = true;
    workers_.reserve(num_threads_);

    for (size_t i = 0; i < num_threads_; ++i) {
        workers_.emplace_back([this]() { worker_thread(); });
        Logger::get()->trace("[AsyncQueryExecutor] Started worker thread {}", i + 1);
    }

    Logger::get()->info("[AsyncQueryExecutor] Started {} worker threads", num_threads_);
}

void AsyncQueryExecutor::stop() {
    if (!running_) return;

    running_ = false;

    for (auto &worker: workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    workers_.clear();

    // Cleanup connection pools - явное закрытие всех соединений
    std::lock_guard<std::mutex> lock(connections_mutex_);
    for (auto &[queue_name, queue]: queue_connections_) {
        while (!queue.empty()) {
            auto conn = std::move(queue.front());
            queue.pop();
            if (conn && conn->is_open()) {
                conn->disconnect();
                Logger::get()->trace("[AsyncQueryExecutor] Closed connection for queue '{}'", queue_name);
            }
        }
    }
    queue_connections_.clear();

    Logger::get()->info("[AsyncQueryExecutor] Stopped");
}

void AsyncQueryExecutor::worker_thread() {
    Logger::get()->trace("[AsyncQueryExecutor] Worker thread started");

    while (running_) {
        try {
            auto item = queue_.dequeue(std::chrono::milliseconds(100));
            if (!item) {
                continue;
            }

            auto &[queue_key, query] = *item;

            auto conn = acquire_connection_for_queue(queue_key);
            if (!conn) {
                Logger::get()->error("[AsyncQueryExecutor] Failed to acquire connection for queue '{}'",
                                     queue_key);
                continue;
            }

            try {
                query.callback(conn);

                // Update statistics
                {
                    std::lock_guard<std::mutex> lock(stats_mutex_);
                    stats_.total_queries_processed++;
                    stats_.queue_processing_stats[queue_key]++;
                }

                Logger::get()->trace("[AsyncQueryExecutor] Successfully processed query for queue '{}'",
                                     queue_key);
            } catch (const std::exception &e) {
                Logger::get()->error("[AsyncQueryExecutor] Query failed for queue '{}': {}",
                                     queue_key, e.what());

                {
                    std::lock_guard<std::mutex> lock(stats_mutex_);
                    stats_.total_errors++;
                }
            }

            release_connection_to_queue(queue_key, std::move(conn));

        } catch (const std::exception &e) {
            Logger::get()->error("[AsyncQueryExecutor] Worker thread error: {}", e.what());
        }
    }

    Logger::get()->trace("[AsyncQueryExecutor] Worker thread finished");
}

std::unique_ptr<pqxx::connection> AsyncQueryExecutor::acquire_connection_for_queue(const std::string &queue_key) {
    std::lock_guard<std::mutex> lock(connections_mutex_);

    // Нормализуем имя очереди
    const std::string &actual_queue = queue_key.empty() ? "default" : queue_key;

    auto &queue = queue_connections_[actual_queue];

    if (!queue.empty()) {
        auto conn = std::move(queue.front());
        queue.pop();

        if (conn->is_open()) {
            Logger::get()->trace("[AsyncQueryExecutor] Reusing existing connection for queue '{}'", actual_queue);
            return conn;
        } else {
            Logger::get()->warn("[AsyncQueryExecutor] Connection for queue '{}' was closed, recreating", actual_queue);
            // Продолжаем выполнение чтобы создать новое соединение
        }
    }

    // Create new connection
    try {
        auto conn = std::make_unique<pqxx::connection>(db_.get_connection_string());
        db_.prepare_connection(*conn);

        Logger::get()->trace("[AsyncQueryExecutor] Created new connection for queue '{}'", actual_queue);
        return conn;
    } catch (const std::exception &e) {
        Logger::get()->error("[AsyncQueryExecutor] Failed to create connection for queue '{}': {}",
                             actual_queue, e.what());
        return nullptr;
    }
}

void AsyncQueryExecutor::release_connection_to_queue(const std::string &queue_key,
                                                     std::unique_ptr<pqxx::connection> conn) {
    if (!conn) return;

    std::lock_guard<std::mutex> lock(connections_mutex_);

    // Нормализуем имя очереди
    const std::string &actual_queue = queue_key.empty() ? "default" : queue_key;

    // Check if we should keep this connection based on queue limit
    auto limit_it = queue_connection_limits_.find(actual_queue);
    size_t limit = (limit_it != queue_connection_limits_.end()) ? limit_it->second : 5;

    auto &queue = queue_connections_[actual_queue];

    if (queue.size() < limit && conn->is_open()) {
        queue.push(std::move(conn));
        Logger::get()->trace("[AsyncQueryExecutor] Released connection to queue '{}' (size: {}/{})",
                             actual_queue, queue.size(), limit);
    } else {
        // Явно закрываем соединение перед уничтожением
        if (conn->is_open()) {
            conn->disconnect();
        }
        Logger::get()->trace("[AsyncQueryExecutor] Discarding connection for queue '{}' (limit reached: {})",
                             actual_queue, limit);
    }
    // else: connection will be destroyed when unique_ptr goes out of scope
}

void AsyncQueryExecutor::set_queue_connection_limit(const std::string &queue_key, size_t limit) {
    std::lock_guard<std::mutex> lock(connections_mutex_);
    queue_connection_limits_[queue_key] = limit;
    Logger::get()->info("[AsyncQueryExecutor] Set connection limit for queue '{}' to {}",
                        queue_key, limit);
}

size_t AsyncQueryExecutor::get_queue_connection_count(const std::string &queue_key) const {
    std::lock_guard<std::mutex> lock(connections_mutex_);
    auto it = queue_connections_.find(queue_key);
    return it != queue_connections_.end() ? it->second.size() : 0;
}

AsyncQueryExecutor::Statistics AsyncQueryExecutor::get_statistics() const {
    std::lock_guard<std::mutex> lock(stats_mutex_);
    return stats_;
}