#include "PlayerInfoMgr.hpp"
#include "BaseServer.hpp"
#include "Logger.hpp"
#include "Time/Timer.hpp"
#include "src/game/enums/DBCStructure.hpp"

PlayerInfoMgr::~PlayerInfoMgr() {
    cleanUpBeforeDelete();
}

void PlayerInfoMgr::cleanUpBeforeDelete() {
    for (int race = 0; race < MAX_RACES; ++race) {
        for (int cls = 0; cls < MAX_CLASSES; ++cls) {
            _playerInfo[race][cls] = nullptr;
        }
    }
}

void PlayerInfoMgr::loadFromDB() {
    auto log = Logger::get();
    cleanUpBeforeDelete();

    try {
        auto dbcMgr = server_->getDBCMgr();
        uint32_t oldMSTime1 = getMSTime();

        auto stmt1 = PreparedStatement("SELECT_PLAYER_CREATE_INFO");
        auto rows1 = server_->db()->execute_sync_many<PlayerCreateInfoRow>(stmt1);
        for (const auto &row: rows1) {
            if (row.m_race >= MAX_RACES) {
                log->error("PlayerInfoMgr::loadFromDB: Wrong race {} in `playercreateinfo` table, ignoring.",
                           row.m_race);
                continue;
            }

            ChrRacesDBC const *rEntry = dbcMgr->getChrRacesDBC(row.m_race);
            if (!rEntry) {
                log->error("PlayerInfoMgr::loadFromDB: Wrong race {} in `playercreateinfo` table, ignoring.",
                           row.m_race);
                continue;
            }

            if (row.m_class >= MAX_CLASSES) {
                log->error("PlayerInfoMgr::loadFromDB: Wrong class {} in `playercreateinfo` table, ignoring.",
                           row.m_class);
                continue;
            }

            if (!dbcMgr->getChrClassesDBC(row.m_class)) {
                log->error("PlayerInfoMgr::loadFromDB: Wrong class {} in `playercreateinfo` table, ignoring.",
                           row.m_class);
                continue;
            }

            //TODO add checks
//            // accept DB data only for valid position (and non instanceable)
//            if (!MapManager::IsValidMapCoord(mapId, positionX, positionY, positionZ, orientation))
//            {
//                log->error("PlayerInfoMgr::loadFromDB: Wrong home position for class {} race {} pair in `playercreateinfo` table, ignoring.", current_class, current_race);
//                continue;
//            }
//
//            if (sDBCStoresMgr->GetMapDBC(mapId)->Instanceable())
//            {
//                log->error("PlayerInfoMgr::loadFromDB: Home position in instanceable map for class {} race {} pair in `playercreateinfo` table, ignoring.", current_class, current_race);
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
                  rows1.size(), GetMSTimeDiffToNow(oldMSTime1));
    }
    catch (const std::exception &ex) {
        log->error("PlayerInfoMgr::loadFromDB failed: {}", ex.what());
    }
}