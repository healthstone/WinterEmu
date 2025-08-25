#pragma once

#include <unordered_map>
#include <mutex>
#include <memory>
#include <boost/asio.hpp>

#include "Database.hpp"
#include "Logger.hpp"
#include "src/game/Entity/Realm/Realm.hpp"
#include "src/game/Entity/Realm/Resolver.hpp"

class AuthServer;

class RealmList : public std::enable_shared_from_this<RealmList> {
public:
    RealmList(boost::asio::io_context &io_context,
              std::shared_ptr<AuthServer> server,
              std::chrono::seconds update_interval = std::chrono::seconds(20));

    ~RealmList();

    void start();
    void stop();

    void load_realms(bool isFirst = false);
    boost::asio::awaitable<void> update();

    std::shared_ptr<Realm> get(uint32_t id) const;

    const std::unordered_map<uint32_t, std::shared_ptr<Realm>>& getRealmsMap() const {
        return realms_;
    }

private:
    void start_update_timer();

    std::unordered_map<uint32_t, std::shared_ptr<Realm>> realms_;

    boost::asio::io_context &io_context_;
    boost::asio::steady_timer update_timer_;
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    std::unique_ptr<Resolver> resolver_;
    std::shared_ptr<AuthServer> server_;
    const std::chrono::seconds update_interval_;

    mutable std::mutex mutex_;
};
