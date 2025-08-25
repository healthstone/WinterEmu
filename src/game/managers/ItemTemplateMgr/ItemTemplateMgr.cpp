#include "ItemTemplateMgr.hpp"
#include "BaseServer.hpp"
#include "Logger.hpp"
#include "Time/Timer.hpp"

ItemTemplateMgr::~ItemTemplateMgr() {
    cleanUpBeforeDelete();
}

void ItemTemplateMgr::cleanUpBeforeDelete() {
    itemTemplateStore_.clear();
}

void ItemTemplateMgr::loadFromDB() {
    auto log = Logger::get();
    cleanUpBeforeDelete();

    try {
        auto dbcMgr = server_->getDBCMgr();
        uint32_t oldMSTime1 = getMSTime();

        auto stmt1 = PreparedStatement("SELECT_PLAYER_CREATE_INFO");
        auto rows1 = server_->db()->execute_sync_many<PlayerCreateInfoRow>(stmt1);
        for (const auto &row: rows1) {

        }
    }
    catch (const std::exception &ex) {
        log->error("ItemTemplateMgr::loadFromDB failed: {}", ex.what());
    }
}