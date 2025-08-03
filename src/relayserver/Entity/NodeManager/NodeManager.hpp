#pragma once

#include <unordered_map>
#include <memory>
#include <boost/asio.hpp>
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"
#include "Logger.hpp"

class NodeManager {
public:
    explicit NodeManager(boost::asio::io_context& io) : io_(io) {}

    std::shared_ptr<NodeConnector> get_connector(uint8_t node_id) const {
        auto it = connectors_.find(node_id);
        return (it != connectors_.end()) ? it->second : nullptr;
    }

    void add_connector(uint8_t node_id, const std::string& host, int port) {
        if (connectors_.count(node_id)) {
            Logger::get()->warn("[NodeManager] Connector for NodeID {} already exists!", node_id);
            return;
        }
        connectors_.emplace(node_id, std::make_shared<NodeConnector>(io_, host, port, node_id));
    }

    void remove_connector(uint8_t node_id) {
        auto it = connectors_.find(node_id);
        if (it != connectors_.end()) {
            it->second->stop();
            connectors_.erase(it);
        }
    }

    void start_all() {
        for (auto& [_, connector] : connectors_) {
            connector->start();
        }
    }

    void stop_all() {
        for (auto& [_, connector] : connectors_) {
            connector->stop();
        }
    }

private:
    boost::asio::io_context& io_;
    std::unordered_map<uint8_t, std::shared_ptr<NodeConnector>> connectors_;
};
