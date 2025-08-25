#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex>

#include "Database.hpp"
#include "BaseServer.hpp"

class NodeSession;

class NodeServer : public std::enable_shared_from_this<NodeServer>, public BaseServer {
public:
    NodeServer(boost::asio::io_context &io_context,
                std::shared_ptr<Database> db,
                int port);

    void start_accept();
    void stop();
    void remove_session(std::shared_ptr<NodeSession> session);
    void log_session_count();

    void init();

    // from BaseServer
    std::shared_ptr<Database> db() const override { return db_; }
    DBCMgr* getDBCMgr() const override { return dbc_manager_.get(); }
    PlayerInfoMgr* getPlayerInfoMgr() const override { return playerInfo_manager_.get(); }

private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    const std::shared_ptr<Database> db_;

    std::unique_ptr<DBCMgr> dbc_manager_;
    std::unique_ptr<PlayerInfoMgr> playerInfo_manager_;

    std::unordered_set<std::shared_ptr<NodeSession>> sessions_;
    std::mutex sessions_mutex_;
};
