#pragma once

#include <queue>
#include <unordered_map>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <functional>
#include <chrono>
#include <string>
#include "PreparedStatement.hpp"
#include "Logger.hpp"

class AsyncQueryQueue {
public:
    using Callback = std::function<void(std::unique_ptr<pqxx::connection>&)>;

    struct QueuedQuery {
        PreparedStatement stmt;
        Callback callback;
        std::chrono::steady_clock::time_point enqueue_time;
        std::string description;
    };

    AsyncQueryQueue() = default;
    ~AsyncQueryQueue() { shutdown(); }

    void enqueue(const std::string& queue_key, PreparedStatement stmt,
                 Callback callback, const std::string& description = "") {
        std::lock_guard<std::mutex> lock(mutex_);

        // Нормализуем имя очереди
        const std::string& actual_queue = queue_key.empty() ? "default" : queue_key;

        queues_[actual_queue].push({std::move(stmt), std::move(callback),
                                    std::chrono::steady_clock::now(), description});
        cv_.notify_one();

        Logger::get()->trace("[AsyncQueryQueue] Enqueued query to queue '{}': {}",
                             actual_queue, description.empty() ? "no description" : description);
    }

    std::optional<std::pair<std::string, QueuedQuery>> dequeue(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);

        if (!cv_.wait_for(lock, timeout, [this] {
            return shutdown_ || has_queries();
        })) {
            return std::nullopt; // timeout
        }

        if (shutdown_) {
            return std::nullopt;
        }

        // Round-robin по очередям для fairness
        static size_t last_queue_index = 0;
        size_t start_index = last_queue_index;
        bool found = false;

        if (queues_.empty()) {
            return std::nullopt;
        }

        auto it = queues_.begin();
        std::advance(it, start_index % queues_.size());

        for (size_t i = 0; i < queues_.size(); ++i) {
            if (!it->second.empty()) {
                auto query = std::move(it->second.front());
                it->second.pop();

                std::string queue_key = it->first;

                // Если очередь пуста, можно удалить ее из мапы
                if (it->second.empty()) {
                    it = queues_.erase(it);
                } else {
                    ++it;
                }

                last_queue_index = (start_index + i + 1) % (queues_.empty() ? 1 : queues_.size());

                Logger::get()->trace("[AsyncQueryQueue] Dequeued query from queue '{}'", queue_key);
                return std::make_pair(queue_key, std::move(query));
            }

            if (++it == queues_.end()) {
                it = queues_.begin();
            }
        }

        return std::nullopt;
    }

    size_t get_queue_size(const std::string& queue_key) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = queues_.find(queue_key);
        return it != queues_.end() ? it->second.size() : 0;
    }

    size_t get_total_queues() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queues_.size();
    }

    size_t get_total_queries() const {
        std::lock_guard<std::mutex> lock(mutex_);
        size_t total = 0;
        for (const auto& [key, queue] : queues_) {
            total += queue.size();
        }
        return total;
    }

    void shutdown() {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
        cv_.notify_all();

        Logger::get()->info("[AsyncQueryQueue] Shutdown completed");
    }

    bool is_shutdown() const {
        return shutdown_;
    }

private:
    bool has_queries() const {
        for (const auto& [key, queue] : queues_) {
            if (!queue.empty()) return true;
        }
        return false;
    }

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::unordered_map<std::string, std::queue<QueuedQuery>> queues_;
    std::atomic<bool> shutdown_{false};
};