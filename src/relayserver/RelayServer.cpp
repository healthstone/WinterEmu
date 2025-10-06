#include "RelayServer.hpp"

#include <utility>
#include "GameSession/GameSession.hpp"
#include "Logger.hpp"

using boost::asio::ip::tcp;

RelayServer::RelayServer(boost::asio::io_context &io_context,
                         std::shared_ptr<Database> db,
                         int port)
        : io_context_(io_context), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)),
          db_(std::move(db)) {
}

void RelayServer::start_accept() {
    if (!acceptor_.is_open()) return;

    acceptor_.async_accept(
            [self = shared_from_this()](boost::system::error_code ec, tcp::socket socket) {
                auto log = Logger::get();
                if (ec) {
                    if (ec != boost::asio::error::operation_aborted &&
                        ec != boost::asio::error::eof) {
                        log->error("[RelayServer] Accept failed: {}", ec.message());
                    }
                    return;
                }

                auto session = std::make_shared<GameSession>(std::move(socket), self);

                {
                    std::lock_guard<std::mutex> lock(self->sessions_mutex_);
                    self->sessions_.insert(session);
                    log->info("[RelayServer] New client connected.");
                    self->log_session_count();
                }

                session->start();
                self->start_accept();
            }
    );
}

void RelayServer::stop() {
    auto log = Logger::get();

    schedule_update_realm(RealmFlags::REALM_FLAG_OFFLINE, 0.0f);
    // Ждём все pending updates
    for (auto &p : pending_updates_) p->get_future().wait();

    if (realm_) {
        realm_.reset();
    }

    if (addon_manager_) {
        addon_manager_->cleanUpBeforeDelete();
        addon_manager_.reset();
    }

    if (dbc_manager_) {
        dbc_manager_->cleanUpBeforeDelete();
        dbc_manager_.reset();
    }

    if (playerInfo_manager_) {
        playerInfo_manager_->cleanUpBeforeDelete();
        playerInfo_manager_.reset();
    }

    if (itemTemplate_manager_) {
        itemTemplate_manager_->cleanUpBeforeDelete();
        itemTemplate_manager_.reset();
    }

    if (node_manager_) {
        node_manager_->stop_all();
        node_manager_->remove_connectors(1);
        node_manager_.reset();
    }

    boost::system::error_code ec;
    acceptor_.cancel(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[RelayServer] Failed to cancel acceptor: {}", ec.message());
    }

    acceptor_.close(ec);
    if (ec && ec != boost::asio::error::operation_aborted && ec != boost::asio::error::eof) {
        log->error("[RelayServer] Failed to close acceptor: {}", ec.message());
    }

    // Для избежания dead lock'a, нужно делать копию списка, закрыть открытые сокеты (где тоже мьютекс)
    {
        std::unordered_set<std::shared_ptr<GameSession>> sessions_copy;
        {
            std::lock_guard<std::mutex> lock(sessions_mutex_);
            sessions_copy = sessions_;
        }

        for (auto &s: sessions_copy) {
            if (s->isOpened()) s->close();
        }
    }

    // Теперь очищаем список
    {
        std::lock_guard<std::mutex> lock(sessions_mutex_);
        sessions_.clear();
    }
    playerSessionMap_.clear();
    log_session_count();

    io_context_.stop();
}

void RelayServer::remove_session(std::shared_ptr<GameSession> session) {
    std::lock_guard<std::mutex> lock(sessions_mutex_);
    sessions_.erase(session);
    log_session_count();
}

void RelayServer::log_session_count() {
    Logger::get()->info("[RelayServer] Active sessions: {}", sessions_.size());
}

void RelayServer::init(unsigned int network_threads, uint32_t realmID) {
    // Загружаем различные данные
    load_realm_by_id(realmID);

    dbc_manager_ = std::make_unique<DBCMgr>(shared_from_this());
    //dbc_manager_->initialize();

    addon_manager_ = std::make_unique<AddonMgr>(shared_from_this());
    //addon_manager_->loadFromDB();

    itemTemplate_manager_ = std::make_unique<ItemTemplateMgr>(shared_from_this());
    //itemTemplate_manager_->loadFromDB();

    playerInfo_manager_ = std::make_unique<PlayerInfoMgr>(shared_from_this());
    //playerInfo_manager_->loadFromDB();

    // Подключаемся к нодам
    node_manager_ = std::make_unique<NodeManager>(io_context_);
    node_manager_->add_connectors(1, "127.0.0.1", 8086, network_threads);
    node_manager_->start_all();

    schedule_update_realm(RealmFlags::REALM_FLAG_RECOMMENDED, 1.0f);
    Logger::get()->info("[RelayServer] Realm {} has been successfully started", realm_->Name);
}

void RelayServer::load_realm_by_id(uint32_t id) {
    realm_ = std::make_unique<Realm>();
    try {
        auto stmt = PreparedStatement("SELECT_REALMLIST_BY_ID");
        stmt.set_param(0, id);
        auto row = db()->execute_sync_one<RealmRow>(stmt);
        if (row) {
            realm_->Id = row->id;
            realm_->Name = row->name;
            realm_->Build = row->build;
            realm_->Type = static_cast<RealmType>(row->icon);
            realm_->Flags = static_cast<RealmFlags>(row->flag);
            realm_->Timezone = row->timezone;
            realm_->AllowedSecurityLevel = static_cast<AccountTypes>(row->securityLevel);
            realm_->PopulationLevel = row->population;
            realm_->Port = row->port;
        }
    } catch (const std::exception &ex) {
        Logger::get()->error("RelayServer::load_realm_by_id failed: {}", ex.what());
    }
}

void RelayServer::addSessionInPlayerMap(ObjectGuid guid, std::shared_ptr<GameSession> session) {
    auto itr = playerSessionMap_.find(guid);
    if (itr == playerSessionMap_.end()) {
        session->setCurrentPlayerObjectGuid(guid);
        playerSessionMap_[guid] = std::move(session);

        NodeData nodeData;
        nodeData.write_uint64_le(guid.GetRawValue());
        NodePacket pkt(NodeOpcodes::REL_TO_NODE_ADD_PLAYER, nodeData);
        get_node_manager()->notify_all_nodes(pkt);

        Logger::get()->warn("[RelayServer::addSessionInPlayerMap] Player {} has been added in playerSessionMap", guid.ToString());
    }
    else
        Logger::get()->error("[RelayServer::addSessionInPlayerMap] Player {} already exist in playerSessionMap", guid.ToString());
}
void RelayServer::removeSessionFromPlayerMap(ObjectGuid guid, std::shared_ptr<GameSession> session) {
    auto itr = playerSessionMap_.find(guid);
    if (itr != playerSessionMap_.end()) {
        session->setCurrentPlayerObjectGuid(ObjectGuid::Empty);
        playerSessionMap_.erase(guid);

        NodeData nodeData;
        nodeData.write_uint64_le(guid.GetRawValue());
        NodePacket pkt(NodeOpcodes::REL_TO_NODE_DEL_PLAYER, nodeData);
        get_node_manager()->notify_all_nodes(pkt);

        Logger::get()->warn("[RelayServer::removeSessionFromPlayerMap] Player {} has been removed from playerSessionMap", guid.ToString());
    }
    else
        Logger::get()->error("[RelayServer::removeSessionFromPlayerMap] Player {} doesn't exist in playerSessionMap", guid.ToString());
}

std::shared_ptr<GameSession> RelayServer::getSessionByPlayerId(ObjectGuid guid) {
    auto itr = playerSessionMap_.find(guid);
    if (itr != playerSessionMap_.end()) {
        return itr->second;
    }
    return nullptr;
}

// Внутри RelayServer добавьте метод-обёртку:
void RelayServer::schedule_update_realm(RealmFlags flags, float population) {
    auto self = shared_from_this();
    auto p = std::make_shared<std::promise<void>>();
    pending_updates_.push_back(p);

    boost::asio::post(io_context_, [self, flags, population, p]() {
        boost::asio::co_spawn(
                self->io_context_,
                [self, flags, population, p]() -> boost::asio::awaitable<void> {
                    co_await self->updateRealm(flags, population);
                    p->set_value(); // сигнализируем о завершении
                    co_return;
                },
                boost::asio::detached
        );
    });
}

boost::asio::awaitable<void> RelayServer::updateRealm(RealmFlags flags, float population) {
    realm_->Flags = flags;
    realm_->PopulationLevel = population;

    try {
        PreparedStatement stmt("UPDATE_REALMLIST");
        stmt.set_param(0, static_cast<uint8_t>(flags));
        stmt.set_param(1, population);
        stmt.set_param(2, realm_->Id);
        co_await db()->execute_async_one<NothingRow>(stmt);
        co_return;
    } catch (const std::exception &ex) {
        Logger::get()->error("[RelayServer::updateRealm] DB exception: {}", ex.what());
        co_return;
    }
}