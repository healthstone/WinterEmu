#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex>

#include "Database.hpp"
#include "ClientSession/ClientSession.hpp"
#include "Entity/AccountCache/AccountCache.hpp"
#include "src/authserver/Entity/RealmList/RealmList.hpp"

class ClientSession;

class AuthServer : public std::enable_shared_from_this<AuthServer> {
public:
    AuthServer(boost::asio::io_context &io_context,
               std::shared_ptr<Database> db,
               int port);

    void start_accept();
    void stop();
    void remove_session(std::shared_ptr<ClientSession> session);
    void log_session_count();

    bool disconnectSessionIfExists(const std::string &username);

    void init();

    std::shared_ptr<Database> db() { return db_; }
    std::shared_ptr<AccountCache> account_cache() { return account_cache_; }

private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    const std::shared_ptr<Database> db_;

    std::shared_ptr<AccountCache> account_cache_;
    std::shared_ptr<RealmList> realmList_;

    std::unordered_set<std::shared_ptr<ClientSession>> sessions_;
    std::mutex sessions_mutex_;
};
