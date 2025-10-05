#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <mutex>
#include <boost/asio.hpp>
#include <random>
#include "src/relayserver/Entity/NodeConnector/NodeConnector.hpp"
#include "Logger.hpp"

class NodeManager {
public:
    explicit NodeManager(boost::asio::io_context& io) : io_(io) {}

    /// Добавить N коннекторов для данного NodeID.
    void add_connectors(uint8_t node_id, const std::string& host, int port, size_t count) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& pool = connectors_[node_id];
        for (size_t i = 0; i < count; ++i) {
            auto connector = std::make_shared<NodeConnector>(io_, host, port, node_id);
            pool.push_back(connector);
        }
        Logger::get()->info("[NodeManager] Added {} connector(s) for NodeID {}", count, node_id);
    }

    /// Получить случайный коннектор для NodeID
    std::shared_ptr<NodeConnector> get_first_connector(uint8_t node_id) const {
        std::lock_guard<std::mutex> lock(mutex_);
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
    std::vector<std::shared_ptr<NodeConnector>> get_connectors(uint8_t node_id) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connectors_.find(node_id);
        if (it != connectors_.end()) {
            return it->second;
        }
        return {};
    }

    /// Запустить все коннекторы
    void start_all() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [node_id, pool] : connectors_) {
            for (auto& connector : pool) {
                connector->start();
            }
        }
    }

    /// Остановить все коннекторы
    void stop_all() {
        std::lock_guard<std::mutex> lock(mutex_);
        for (auto& [node_id, pool] : connectors_) {
            for (auto& connector : pool) {
                connector->stop();
            }
        }
    }

    /// Удалить все коннекторы для NodeID
    void remove_connectors(uint8_t node_id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = connectors_.find(node_id);
        if (it != connectors_.end()) {
            for (auto& connector : it->second) {
                connector->stop();
            }
            connectors_.erase(it);
            Logger::get()->info("[NodeManager] Removed all connectors for NodeID {}", node_id);
        }
    }

    /// Отправить пакет на все ноды (по одному случайному коннектору для каждой ноды)
    void notify_all_nodes(const NodePacket& packet) {
        std::lock_guard<std::mutex> lock(mutex_);
        for (const auto& [node_id, connectors] : connectors_) {
            if (!connectors.empty()) {
                static thread_local std::mt19937 rng{std::random_device{}()};
                std::uniform_int_distribution<size_t> dist(0, connectors.size() - 1);
                size_t index = dist(rng);
                connectors[index]->send_packet(packet);
                Logger::get()->debug("[NodeManager] Sent packet to NodeID {} via connector {}", node_id, index);
            }
        }
    }

private:
    boost::asio::io_context& io_;
    /// Пул коннекторов для каждого NodeID
    std::unordered_map<uint8_t, std::vector<std::shared_ptr<NodeConnector>>> connectors_;
    mutable std::mutex mutex_;
};