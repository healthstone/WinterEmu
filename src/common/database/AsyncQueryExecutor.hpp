#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <pqxx/connection>
#include "AsyncQueryQueue.hpp"
#include "Logger.hpp"

// Forward declaration
class Database;

class AsyncQueryExecutor {
public:
    AsyncQueryExecutor(Database& db, AsyncQueryQueue& queue, size_t num_threads = 2);
    ~AsyncQueryExecutor();

    void start();
    void stop();

    void set_queue_connection_limit(const std::string& queue_key, size_t limit);
    size_t get_queue_connection_count(const std::string& queue_key) const;

    struct Statistics {
        size_t total_queries_processed = 0;
        size_t total_errors = 0;
        std::unordered_map<std::string, size_t> queue_processing_stats;
    };

    Statistics get_statistics() const;

private:
    void worker_thread();
    std::unique_ptr<pqxx::connection> acquire_connection_for_queue(const std::string& queue_key);
    void release_connection_to_queue(const std::string& queue_key,
                                     std::unique_ptr<pqxx::connection> conn);

    Database& db_;
    AsyncQueryQueue& queue_;
    size_t num_threads_;
    std::atomic<bool> running_{false};
    std::vector<std::thread> workers_;

    // Connection pool per queue
    mutable std::mutex connections_mutex_;
    std::unordered_map<std::string, std::queue<std::unique_ptr<pqxx::connection>>> queue_connections_;
    std::unordered_map<std::string, size_t> queue_connection_limits_;

    // Statistics
    mutable std::mutex stats_mutex_;
    Statistics stats_;
};