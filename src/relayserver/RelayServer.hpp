#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <unordered_set>
#include <mutex>

#include "Database.hpp"
#include "BaseServer.hpp"
#include "src/game/Entity/Realm/Realm.hpp"
#include "src/relayserver/Entity/NodeManager/NodeManager.hpp"
#include "src/relayserver/Entity/AddonMgr/AddonMgr.hpp"
#include "src/game/managers/DBCMgr/DBCMgr.hpp"
#include "src/game/managers/ItemTemplateMgr/ItemTemplateMgr.hpp"

class GameSession;

class RelayServer : public std::enable_shared_from_this<RelayServer>, public BaseServer {
public:
    RelayServer(boost::asio::io_context &io_context,
                std::shared_ptr<Database> db,
                int port);

    void start_accept();
    void stop();
    void remove_session(std::shared_ptr<GameSession> session);
    void log_session_count();

    void init(unsigned int network_threads, uint32_t realmID);
    void load_realm_by_id(uint32_t id);

    // from BaseServer
    std::shared_ptr<Database> db() const override { return db_; }
    DBCMgr* getDBCMgr() const override { return dbc_manager_.get(); }
    PlayerInfoMgr* getPlayerInfoMgr() const override { return playerInfo_manager_.get(); }

    // own ptr
    Realm* getRealm() const { return realm_.get(); }
    NodeManager* get_node_manager() const  { return node_manager_.get(); }
    AddonMgr* getAddonMgr() const { return addon_manager_.get(); }
    ItemTemplateMgr* getItemTemplateMgr() const { return itemTemplate_manager_.get(); }

private:
    boost::asio::io_context &io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    const std::shared_ptr<Database> db_;

    std::unique_ptr<Realm> realm_;
    std::unique_ptr<NodeManager> node_manager_;
    std::unique_ptr<AddonMgr> addon_manager_;
    std::unique_ptr<DBCMgr> dbc_manager_;
    std::unique_ptr<PlayerInfoMgr> playerInfo_manager_;
    std::unique_ptr<ItemTemplateMgr> itemTemplate_manager_;
    std::unordered_set<std::shared_ptr<GameSession>> sessions_;
    std::mutex sessions_mutex_;
};
