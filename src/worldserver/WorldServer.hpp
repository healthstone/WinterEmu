#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex>

#include "Database.hpp"

class WorldSession;

class WorldServer : public std::enable_shared_from_this<WorldServer> {
public:
    WorldServer(boost::asio::io_context &io_context,
               std::shared_ptr<Database> db,
               int port);

    void start_accept();
    void stop();
    void remove_session(std::shared_ptr<WorldSession> session);
    void log_session_count();

    void init();

    std::shared_ptr<Database> db() { return db_; }

private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    const std::shared_ptr<Database> db_;

    std::unordered_set<std::shared_ptr<WorldSession>> sessions_;
    std::mutex sessions_mutex_;
};
