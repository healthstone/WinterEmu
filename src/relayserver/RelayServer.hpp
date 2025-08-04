#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex>

#include "Database.hpp"
#include "src/relayserver/Entity/NodeManager/NodeManager.hpp"

class GameSession;

class RelayServer : public std::enable_shared_from_this<RelayServer> {
public:
    RelayServer(boost::asio::io_context &io_context,
                std::shared_ptr<Database> db,
                int port);

    void start_accept();
    void stop();
    void remove_session(std::shared_ptr<GameSession> session);
    void log_session_count();

    void init(unsigned int network_threads);

    NodeManager* get_node_manager() const { return node_manager_.get(); }
    std::shared_ptr<Database> db() { return db_; }

private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    const std::shared_ptr<Database> db_;

    std::unique_ptr<NodeManager> node_manager_;
    std::unordered_set<std::shared_ptr<GameSession>> sessions_;
    std::mutex sessions_mutex_;
};
