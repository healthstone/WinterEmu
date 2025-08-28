#include "DBCMgr.hpp"
#include "Time/Timer.hpp"
#include "Logger.hpp"
#include "BaseServer.hpp"

DBCMgr::~DBCMgr() {
    cleanUpBeforeDelete();
}

void DBCMgr::cleanUpBeforeDelete() {
    // Сначала чистим мультимапы и вторичные контейнеры
    _charStartOutfitByTripple.clear();
    _skillRaceClassInfoBySkill.clear();

    // Потом уже сами основные мапы
    _achievementMap.clear();
    _chrClassesMap.clear();
    _chrRacesMap.clear();
    _charStartOutfitMap.clear();
    _skillRaceClassInfoMap.clear();
    _skillLineMap.clear();
}

void DBCMgr::initialize() {
    load_Achievement();
    load_ChrClasses();
    load_ChrRaces();
    load_CharStartOutfit();
    load_SkillRaceClassInfo();
    load_SkillLine();

    initialize_Additional_Data();
}

Team DBCMgr::teamForRace(uint8_t race)
{
    auto log = Logger::get();
    if (ChrRacesDBC const* rEntry = getChrRacesDBC(race))
    {
        switch (rEntry->BaseLanguage)
        {
            case 1: return Team::HORDE;
            case 7: return Team::ALLIANCE;
        }
        log->error("DBCMgr::teamForRace: Race ({}) has wrong teamid ({}) in DBC: wrong DBC files?", race, rEntry->BaseLanguage);
    }
    else
        log->error("DBCMgr::teamForRace: Race ({}) not found in DBC: wrong DBC files?", race);

    return Team::ALLIANCE;
}

void DBCMgr::load_Achievement() {
    auto log = Logger::get();
    _chrClassesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ACHIEVEMENT");
        auto rows = server_->db()->execute_sync_many<DbcAchievement>(stmt);
        for (const auto &row: rows) {
            AchievementDBC ach;
            ach.ID              = row.ID;
            ach.Faction         = row.Faction;
            ach.InstanceID      = row.InstanceID;

            // Title
            ach.Title[LOCALE_enUS] = row.Title_Lang_enUS.value_or("");
            ach.Title[LOCALE_enGB] = row.Title_Lang_enGB.value_or("");
            ach.Title[LOCALE_koKR] = row.Title_Lang_koKR.value_or("");
            ach.Title[LOCALE_frFR] = row.Title_Lang_frFR.value_or("");
            ach.Title[LOCALE_deDE] = row.Title_Lang_deDE.value_or("");
            ach.Title[LOCALE_enCN] = row.Title_Lang_enCN.value_or("");
            ach.Title[LOCALE_zhCN] = row.Title_Lang_zhCN.value_or("");
            ach.Title[LOCALE_enTW] = row.Title_Lang_enTW.value_or("");
            ach.Title[LOCALE_zhTW] = row.Title_Lang_zhTW.value_or("");
            ach.Title[LOCALE_esES] = row.Title_Lang_esES.value_or("");
            ach.Title[LOCALE_esMX] = row.Title_Lang_esMX.value_or("");
            ach.Title[LOCALE_ruRU] = row.Title_Lang_ruRU.value_or("");
            ach.Title[LOCALE_ptPT] = row.Title_Lang_ptPT.value_or("");
            ach.Title[LOCALE_ptBR] = row.Title_Lang_ptBR.value_or("");
            ach.Title[LOCALE_itIT] = row.Title_Lang_itIT.value_or("");

            ach.Category        = row.Category;
            ach.Points          = row.Points;
            ach.Flags           = row.Flags;
            ach.MinimumCriteria = row.Minimum_Criteria;
            ach.SharesCriteria  = row.Shares_Criteria;

            _achievementMap[row.ID] = ach;
        }
        log->info(">>> DBCMgr: loaded {} Achievement in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Achievement failed: {}", ex.what());
    }
}

void DBCMgr::load_ChrClasses() {
    auto log = Logger::get();
    _chrClassesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHRCLASSES");
        auto rows = server_->db()->execute_sync_many<DbcChrClasses>(stmt);
        for (const auto &row: rows) {
            ChrClassesDBC cc;
            cc.ID           = row.ID;
            cc.DisplayPower = row.DisplayPower;

            // Заполняем локализованные названия
            if (row.Name_Lang_enUS.has_value()) cc.Name[LOCALE_enUS] = *row.Name_Lang_enUS;
            if (row.Name_Lang_enGB.has_value()) cc.Name[LOCALE_enGB] = *row.Name_Lang_enGB;
            if (row.Name_Lang_koKR.has_value()) cc.Name[LOCALE_koKR] = *row.Name_Lang_koKR;
            if (row.Name_Lang_frFR.has_value()) cc.Name[LOCALE_frFR] = *row.Name_Lang_frFR;
            if (row.Name_Lang_deDE.has_value()) cc.Name[LOCALE_deDE] = *row.Name_Lang_deDE;
            if (row.Name_Lang_enCN.has_value()) cc.Name[LOCALE_enCN] = *row.Name_Lang_enCN;
            if (row.Name_Lang_zhCN.has_value()) cc.Name[LOCALE_zhCN] = *row.Name_Lang_zhCN;
            if (row.Name_Lang_enTW.has_value()) cc.Name[LOCALE_enTW] = *row.Name_Lang_enTW;
            if (row.Name_Lang_zhTW.has_value()) cc.Name[LOCALE_zhTW] = *row.Name_Lang_zhTW;
            if (row.Name_Lang_esES.has_value()) cc.Name[LOCALE_esES] = *row.Name_Lang_esES;
            if (row.Name_Lang_esMX.has_value()) cc.Name[LOCALE_esMX] = *row.Name_Lang_esMX;
            if (row.Name_Lang_ruRU.has_value()) cc.Name[LOCALE_ruRU] = *row.Name_Lang_ruRU;
            if (row.Name_Lang_ptPT.has_value()) cc.Name[LOCALE_ptPT] = *row.Name_Lang_ptPT;
            if (row.Name_Lang_ptBR.has_value()) cc.Name[LOCALE_ptBR] = *row.Name_Lang_ptBR;
            if (row.Name_Lang_itIT.has_value()) cc.Name[LOCALE_itIT] = *row.Name_Lang_itIT;

            cc.SpellClassSet = row.SpellClassSet;
            cc.CinematicSequenceID = row.CinematicSequenceID;
            cc.RequiredExpansion = row.Required_Expansion;

            _chrClassesMap[cc.ID] = cc;
        }
        log->info(">>> DBCMgr: loaded {} ChrClasses in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));

    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ChrClasses failed: {}", ex.what());
    }
}

void DBCMgr::load_ChrRaces() {
    auto log = Logger::get();
    _chrRacesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHRRACES");
        auto rows = server_->db()->execute_sync_many<DbcChrRaces>(stmt);
        for (const auto &row: rows) {
            ChrRacesDBC cr;
            cr.ID                  = row.ID;
            cr.Flags               = row.Flags;
            cr.FactionID           = row.FactionID;
            cr.MaleDisplayID       = row.MaleDisplayId;
            cr.FemaleDisplayID     = row.FemaleDisplayId;
            cr.BaseLanguage        = row.BaseLanguage;
            cr.CreatureType        = row.CreatureType;
            cr.ResSicknessSpellID  = row.ResSicknessSpellID;
            cr.CinematicSequenceID = row.CinematicSequenceID;
            cr.Alliance            = row.Alliance;

            // Заполняем локализованные названия
            if (row.Name_Lang_enUS.has_value()) cr.Name[LOCALE_enUS] = *row.Name_Lang_enUS;
            if (row.Name_Lang_enGB.has_value()) cr.Name[LOCALE_enGB] = *row.Name_Lang_enGB;
            if (row.Name_Lang_koKR.has_value()) cr.Name[LOCALE_koKR] = *row.Name_Lang_koKR;
            if (row.Name_Lang_frFR.has_value()) cr.Name[LOCALE_frFR] = *row.Name_Lang_frFR;
            if (row.Name_Lang_deDE.has_value()) cr.Name[LOCALE_deDE] = *row.Name_Lang_deDE;
            if (row.Name_Lang_enCN.has_value()) cr.Name[LOCALE_enCN] = *row.Name_Lang_enCN;
            if (row.Name_Lang_zhCN.has_value()) cr.Name[LOCALE_zhCN] = *row.Name_Lang_zhCN;
            if (row.Name_Lang_enTW.has_value()) cr.Name[LOCALE_enTW] = *row.Name_Lang_enTW;
            if (row.Name_Lang_zhTW.has_value()) cr.Name[LOCALE_zhTW] = *row.Name_Lang_zhTW;
            if (row.Name_Lang_esES.has_value()) cr.Name[LOCALE_esES] = *row.Name_Lang_esES;
            if (row.Name_Lang_esMX.has_value()) cr.Name[LOCALE_esMX] = *row.Name_Lang_esMX;
            if (row.Name_Lang_ruRU.has_value()) cr.Name[LOCALE_ruRU] = *row.Name_Lang_ruRU;
            if (row.Name_Lang_ptPT.has_value()) cr.Name[LOCALE_ptPT] = *row.Name_Lang_ptPT;
            if (row.Name_Lang_ptBR.has_value()) cr.Name[LOCALE_ptBR] = *row.Name_Lang_ptBR;
            if (row.Name_Lang_itIT.has_value()) cr.Name[LOCALE_itIT] = *row.Name_Lang_itIT;

            cr.RequiredExpansion = row.Required_Expansion;

            _chrRacesMap[cr.ID] = cr;
        }
        log->info(">>> DBCMgr: loaded {} ChrRaces in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));

    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ChrRaces failed: {}", ex.what());
    }
}

void DBCMgr::load_CharStartOutfit() {
    auto log = Logger::get();
    _charStartOutfitMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHARSTARTOUTFIT");
        auto rows = server_->db()->execute_sync_many<DbcCharStartOutfit>(stmt);
        for (const auto &row: rows) {
            CharStartOutfitDBC cso;
            cso.ID      = row.ID;
            cso.RaceID  = row.RaceID;
            cso.ClassID = row.ClassID;
            cso.SexID   = row.SexID;

            for (uint8_t i = 0; i < MAX_OUTFIT_ITEMS; i++)
                cso.ItemID[i] = row.ItemID[i];

            _charStartOutfitMap[row.ID] = cso;
        }
        log->info(">>> DBCMgr: loaded {} CharStartOutfit in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CharStartOutfit failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillRaceClassInfo() {
    auto log = Logger::get();
    _skillRaceClassInfoMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLRACECLASSINFO");
        auto rows = server_->db()->execute_sync_many<DbcSkillRaceClassInfo>(stmt);
        for (const auto &row: rows) {
            SkillRaceClassInfoDBC srci;
            srci.ID = row.ID;
            srci.SkillID     = row.SkillID;
            srci.RaceMask    = row.RaceMask;
            srci.ClassMask   = row.ClassMask;
            srci.Flags       = row.Flags;
            srci.SkillTierID = row.SkillTierID;

            _skillRaceClassInfoMap[row.ID] = srci;
        }
        log->info(">>> DBCMgr: loaded {} SkillRaceClassInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillRaceClassInfo failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillLine() {
    auto log = Logger::get();
    _skillLineMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLLINE");
        auto rows = server_->db()->execute_sync_many<DbcSkillLine>(stmt);
        for (const auto &row: rows) {
            SkillLineDBC sl;
            sl.ID = row.ID;
            sl.CategoryID = row.CategoryID;

            // DisplayName
            sl.DisplayName[LOCALE_enUS] = row.DisplayName_Lang_enUS.value_or("");
            sl.DisplayName[LOCALE_enGB] = row.DisplayName_Lang_enGB.value_or("");
            sl.DisplayName[LOCALE_koKR] = row.DisplayName_Lang_koKR.value_or("");
            sl.DisplayName[LOCALE_frFR] = row.DisplayName_Lang_frFR.value_or("");
            sl.DisplayName[LOCALE_deDE] = row.DisplayName_Lang_deDE.value_or("");
            sl.DisplayName[LOCALE_enCN] = row.DisplayName_Lang_enCN.value_or("");
            sl.DisplayName[LOCALE_zhCN] = row.DisplayName_Lang_zhCN.value_or("");
            sl.DisplayName[LOCALE_enTW] = row.DisplayName_Lang_enTW.value_or("");
            sl.DisplayName[LOCALE_zhTW] = row.DisplayName_Lang_zhTW.value_or("");
            sl.DisplayName[LOCALE_esES] = row.DisplayName_Lang_esES.value_or("");
            sl.DisplayName[LOCALE_esMX] = row.DisplayName_Lang_esMX.value_or("");
            sl.DisplayName[LOCALE_ruRU] = row.DisplayName_Lang_ruRU.value_or("");
            sl.DisplayName[LOCALE_ptPT] = row.DisplayName_Lang_ptPT.value_or("");
            sl.DisplayName[LOCALE_ptBR] = row.DisplayName_Lang_ptBR.value_or("");
            sl.DisplayName[LOCALE_itIT] = row.DisplayName_Lang_itIT.value_or("");

            sl.SpellIconID = row.SpellIconID;
            sl.CanLink     = row.CanLink;

            _skillLineMap[row.ID] = sl;
        }
        log->info(">>> DBCMgr: loaded {} SkillLine in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillLine failed: {}", ex.what());
    }
}

void DBCMgr::initialize_Additional_Data() {
    handle_CharStartOutfitByTripple();
    handle_SkillRaceClassInfo();
}

void DBCMgr::handle_CharStartOutfitByTripple() {
    for (const auto& csoID : _charStartOutfitMap)
    {
        if (CharStartOutfitDBC const* entry = &csoID.second)
            _charStartOutfitByTripple[CharStartOutfitKey(entry->RaceID, entry->ClassID, entry->SexID)] = entry;
    }
}

void DBCMgr::handle_SkillRaceClassInfo() {
    for (const auto& srciID : _skillRaceClassInfoMap)
    {
        if (SkillRaceClassInfoDBC const* entry = &srciID.second)
            if (getSkillLineDBC(entry->SkillID))
                _skillRaceClassInfoBySkill.emplace(entry->SkillID, entry);
    }
}