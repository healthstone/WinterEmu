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

    if (auto srv = server_) {
        try {
            uint32_t oldMSTime = getMSTime();

            auto stmt = PreparedStatement("SELECT_ITEM_TEMPLATE");
            auto rows = srv->db()->execute_sync_many<ItemTemplate>(stmt);
            for (const auto &row: rows)
                itemTemplateStore_[row.ItemId] = row;

            log->info(">>> ItemTemplateMgr: loaded {} ItemTemplate in {} ms",
                      rows.size(), GetMSTimeDiffToNow(oldMSTime));
        }
        catch (const std::exception &ex) {
            log->error("ItemTemplateMgr::loadFromDB failed: {}", ex.what());
        }
    }
}