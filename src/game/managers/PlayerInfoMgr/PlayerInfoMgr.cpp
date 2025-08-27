#include "PlayerInfoMgr.hpp"
#include "BaseServer.hpp"
#include "Logger.hpp"
#include "Time/Timer.hpp"
#include "src/game/enums/DBCStructure.hpp"
#include "src/game/enums/Gender.hpp"

PlayerInfoMgr::~PlayerInfoMgr() {
    cleanUpBeforeDelete();
}

void PlayerInfoMgr::cleanUpBeforeDelete() {
    // Очистка _playerInfo
    for (int race = 0; race < MAX_RACES; ++race) {
        for (int cls = 0; cls < MAX_CLASSES; ++cls) {
            _playerInfo[race][cls] = nullptr;
        }
    }

    // Очистка _playerClassInfo
    for (int cls = 0; cls < MAX_CLASSES; ++cls) {
        _playerClassInfo[cls] = nullptr;
    }
}

void PlayerInfoMgr::loadFromDB() {
    cleanUpBeforeDelete();

    load_playercreateinfo();
    load_playercreateinfo_item();
}

void PlayerInfoMgr::load_playercreateinfo() {
    auto log = Logger::get();
    try {
        auto dbcMgr = server_->getDBCMgr();
        uint32_t oldMSTime1 = getMSTime();

        auto stmt = PreparedStatement("SELECT_PLAYER_CREATE_INFO");
        auto rows = server_->db()->execute_sync_many<PlayerCreateInfoRow>(stmt);
        for (const auto &row: rows) {
            if (row.m_race >= MAX_RACES) {
                log->error("[PlayerInfoMgr::load_playercreateinfo] Wrong race {} in `playercreateinfo` table, ignoring.",
                           row.m_race);
                continue;
            }

            ChrRacesDBC const *rEntry = dbcMgr->getChrRacesDBC(row.m_race);
            if (!rEntry) {
                log->error("[PlayerInfoMgr::load_playercreateinfo] Wrong race {} in `playercreateinfo` table, ignoring.",
                           row.m_race);
                continue;
            }

            if (row.m_class >= MAX_CLASSES) {
                log->error("[PlayerInfoMgr::load_playercreateinfo] Wrong class {} in `playercreateinfo` table, ignoring.",
                           row.m_class);
                continue;
            }

            if (!dbcMgr->getChrClassesDBC(row.m_class)) {
                log->error("[PlayerInfoMgr::load_playercreateinfo] Wrong class {} in `playercreateinfo` table, ignoring.",
                           row.m_class);
                continue;
            }

            //TODO add checks
//            // accept DB data only for valid position (and non instanceable)
//            if (!MapManager::IsValidMapCoord(mapId, positionX, positionY, positionZ, orientation))
//            {
//                log->error("[PlayerInfoMgr::load_playercreateinfo] Wrong home position for class {} race {} pair in `playercreateinfo` table, ignoring.", current_class, current_race);
//                continue;
//            }
//
//            if (sDBCStoresMgr->GetMapDBC(mapId)->Instanceable())
//            {
//                log->error("[PlayerInfoMgr::load_playercreateinfo] Home position in instanceable map for class {} race {} pair in `playercreateinfo` table, ignoring.", current_class, current_race);
//                continue;
//            }

            std::unique_ptr<PlayerInfo> info = std::make_unique<PlayerInfo>();
            info->mapId = row.m_map;
            info->areaId = row.m_zone;
            info->positionX = row.position_x;
            info->positionY = row.position_y;
            info->positionZ = row.position_z;
            info->orientation = row.orientation;
            info->displayId_m = rEntry->MaleDisplayID;
            info->displayId_f = rEntry->FemaleDisplayID;
            _playerInfo[row.m_race][row.m_class] = std::move(info);
        }
        log->info(">>> PlayerInfoMgr: loaded {} playerInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime1));
    }
    catch (const std::exception &ex) {
        log->error("[PlayerInfoMgr::load_playercreateinfo] failed: {}", ex.what());
    }
}

void PlayerInfoMgr::load_playercreateinfo_item() {
    auto log = Logger::get();
    try {
        uint32_t oldMSTime = getMSTime();
        auto itemTemplateMgr = server_->getItemTemplateMgr();
        auto stmt = PreparedStatement("SELECT_PLAYER_CREATE_INFO_ITEM");
        auto rows = server_->db()->execute_sync_many<PlayerCreateInfoItemRow>(stmt);
        for (const auto &row: rows) {
            if (row.race >= MAX_RACES)
            {
                log->error("[PlayerInfoMgr::load_playercreateinfo_item] Wrong race {} in `playercreateinfo_item` table, ignoring.", row.race);
                continue;
            }

            if (row.class_ >= MAX_CLASSES)
            {
                log->error("[PlayerInfoMgr::load_playercreateinfo_item] Wrong class {} in `playercreateinfo_item` table, ignoring.", row.class_);
                continue;
            }

            if (!itemTemplateMgr->getItemTemplate(row.itemid))
            {
                log->error("[PlayerInfoMgr::load_playercreateinfo_item] Item id {} (race {} class {}) in `playercreateinfo_item` table but not listed in `item_template`, ignoring.",
                           row.itemid, row.race, row.class_);
                continue;
            }

            if (!row.amount)
            {
                log->error("sql.sql", "Item id {} (class {} race {}) have amount == 0 in `playercreateinfo_item` table, ignoring.", row.itemid, row.race, row.class_);
                continue;
            }

            if (!row.race || !row.class_)
            {
                uint8_t min_race = row.race ? row.race : 1;
                uint8_t max_race = row.race ? row.race + 1 : MAX_RACES;
                uint8_t min_class = row.class_ ? row.class_ : 1;
                uint8_t max_class = row.class_ ? row.class_ + 1 : MAX_CLASSES;
                for (uint8_t r = min_race; r < max_race; ++r)
                    for (uint8_t c = min_class; c < max_class; ++c)
                        playerCreateInfoAddItemHelper(r, c, row.itemid, row.amount);
            }
            else
                playerCreateInfoAddItemHelper(row.race, row.class_, row.itemid, row.amount);
        }
        log->info(">>> PlayerInfoMgr: loaded {} playercreateinfo_item in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    }
    catch (const std::exception &ex) {
        log->error("[PlayerInfoMgr::load_playercreateinfo_item] failed: {}", ex.what());
    }
}

void PlayerInfoMgr::playerCreateInfoAddItemHelper(uint8_t race_, uint8_t class_, uint32_t itemId, int16_t count) {
    if (!_playerInfo[race_][class_])
        return;

    if (count > 0)
        _playerInfo[race_][class_]->item.push_back(PlayerCreateInfoItem(itemId, count));
    else
    {
        auto log = Logger::get();
        if (count < -1)
            log->error("[PlayerInfoMgr::playerCreateInfoAddItemHelper] Invalid count {} specified on item {} be removed from original player create info (use -1)!", count, itemId);

//        for (uint8_t gender = 0; gender < static_cast<uint8_t>(Gender::GENDER_NONE); ++gender)
//        {
//            if (CharStartOutfitEntry const* entry = GetCharStartOutfitEntry(race_, class_, gender))
//            {
//                bool found = false;
//                for (uint8 x = 0; x < MAX_OUTFIT_ITEMS; ++x)
//                {
//                    if (entry->ItemID[x] > 0 && uint32(entry->ItemID[x]) == itemId)
//                    {
//                        found = true;
//                        const_cast<CharStartOutfitEntry*>(entry)->ItemID[x] = 0;
//                        break;
//                    }
//                }
//
//                if (!found)
//                    log->error("[PlayerInfoMgr::playerCreateInfoAddItemHelper] Item {} specified to be removed from original create info not found in dbc!", itemId);
//            }
//        }
    }
}