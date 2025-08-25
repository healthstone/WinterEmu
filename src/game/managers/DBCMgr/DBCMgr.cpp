#include "DBCMgr.hpp"
#include "Time/Timer.hpp"
#include "Logger.hpp"
#include "BaseServer.hpp"

DBCMgr::~DBCMgr() {
    cleanUpBeforeDelete();
}

void DBCMgr::cleanUpBeforeDelete() {
    _chrClassesMap.clear();
    _chrRacesMap.clear();
}

void DBCMgr::initialize_for_relay() {
    load_ChrClasses();
    load_ChrRaces();
}

void DBCMgr::initialize_for_node() {
    Logger::get()->info(">>> DBCMgr: initialized");
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