#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <boost/asio.hpp>
#include <random>
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"
#include "Logger.hpp"

class NodeManager {
public:
    explicit NodeManager(boost::asio::io_context& io) : io_(io) {}

    /// Добавить N коннекторов для данного NodeID.
    void add_connectors(uint8_t node_id, const std::string& host, int port, size_t count) {
        auto& pool = connectors_[node_id];
        for (size_t i = 0; i < count; ++i) {
            auto connector = std::make_shared<NodeConnector>(io_, host, port, node_id);
            pool.push_back(connector);
        }
        Logger::get()->info("[NodeManager] Added {} connector(s) for NodeID {}", count, node_id);
    }

    /// Получить случайный коннектор для NodeID
    std::shared_ptr<NodeConnector> get_first_connector(uint8_t node_id) const {
        auto it = connectors_.find(node_id);
        if (it != connectors_.end() && !it->second.empty()) {
            const auto& pool = it->second;
            if (pool.size() == 1) {
                return pool.front();
            } else {
                static thread_local std::mt19937 rng{ std::random_device{}() };
                std::uniform_int_distribution<size_t> dist(0, pool.size() - 1);
                size_t index = dist(rng);
                Logger::get()->debug("[NodeManager] Selected connector index {} for NodeID {}", index, node_id);
                return pool[index];
            }
        }
        return nullptr;
    }

    /// Отдать весь пул коннекторов для NodeID
    const std::vector<std::shared_ptr<NodeConnector>>& get_connectors(uint8_t node_id) const {
        static const std::vector<std::shared_ptr<NodeConnector>> empty;
        auto it = connectors_.find(node_id);
        return (it != connectors_.end()) ? it->second : empty;
    }

    /// Запустить все коннекторы
    void start_all() {
        for (auto& [node_id, pool] : connectors_) {
            for (auto& connector : pool) {
                connector->start();
            }
        }
    }

    /// Остановить все коннекторы
    void stop_all() {
        for (auto& [node_id, pool] : connectors_) {
            for (auto& connector : pool) {
                connector->stop();
            }
        }
    }

    /// Удалить все коннекторы для NodeID
    void remove_connectors(uint8_t node_id) {
        auto it = connectors_.find(node_id);
        if (it != connectors_.end()) {
            for (auto& connector : it->second) {
                connector->stop();
            }
            connectors_.erase(it);
            Logger::get()->info("[NodeManager] Removed all connectors for NodeID {}", node_id);
        }
    }

private:
    boost::asio::io_context& io_;
    /// Пул коннекторов для каждого NodeID
    std::unordered_map<uint8_t, std::vector<std::shared_ptr<NodeConnector>>> connectors_;
};
