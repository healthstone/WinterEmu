#pragma once

#include <boost/asio.hpp>
#include <unordered_map>
#include <mutex>
#include <chrono>
#include <optional>

class AccountCache : public std::enable_shared_from_this<AccountCache> {
public:
    explicit AccountCache(boost::asio::io_context &io_context,
                          std::chrono::seconds ttl = std::chrono::minutes(5),
                          std::chrono::seconds cleanup_interval = std::chrono::minutes(1))
            : io_context_(io_context),
              cleanup_timer_(io_context),
              ttl_(ttl),
              cleanup_interval_(cleanup_interval) {
    }

    ~AccountCache() {
        stop();
    }

    void start() {
        start_cleanup_timer();
    }

    void stop() {
        boost::system::error_code ec;
        cleanup_timer_.cancel(ec);
        cache_.clear();
    }

    struct AccountCacheEntry {
        //ttl
        std::chrono::steady_clock::time_point last_access; // скользящий TTL

        //account entity
        AccountsRow account;
    };

    std::shared_ptr<AccountCacheEntry> get(const std::string &username) {
        std::lock_guard lock(mutex_);
        auto it = cache_.find(username);
        if (it == cache_.end()) return nullptr;

        auto now = std::chrono::steady_clock::now();
        if (now - it->second.last_access > ttl_) {
            return nullptr;
        }

        it->second.last_access = now;
        return std::make_shared<AccountCacheEntry>(it->second);
    }

    void put(const std::string &username, const AccountCacheEntry &entry) {
        std::lock_guard lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        auto new_entry = entry;
        new_entry.last_access = now;
        cache_[username] = std::move(new_entry);
    }

    void invalidate(const std::string &username) {
        std::lock_guard lock(mutex_);
        cache_.erase(username);
    }

    void update_session_key(const std::string &accountName, const std::array<uint8_t, 40> &K) {
        std::lock_guard lock(mutex_);
        auto it = cache_.find(accountName);
        if (it != cache_.end()) {
            it->second.account.session_key_auth = K; // Простое присваивание
        }
    }

private:
    void start_cleanup_timer() {
        cleanup_timer_.expires_after(cleanup_interval_);
        cleanup_timer_.async_wait([self = shared_from_this()](const boost::system::error_code &ec) {
            if (!ec) {
                self->cleanup_expired_entries();
                self->start_cleanup_timer();
            }
            // Если ошибка — можно залогировать
        });
    }

    void cleanup_expired_entries() {
        std::lock_guard lock(mutex_);
        auto now = std::chrono::steady_clock::now();

        for (auto it = cache_.begin(); it != cache_.end();) {
            if (now - it->second.last_access > ttl_) {
                it = cache_.erase(it);
            } else {
                ++it;
            }
        }
    }

    boost::asio::io_context &io_context_;
    boost::asio::steady_timer cleanup_timer_;

    std::unordered_map<std::string, AccountCacheEntry> cache_;
    std::mutex mutex_;

    const std::chrono::seconds ttl_;
    const std::chrono::seconds cleanup_interval_;
};
