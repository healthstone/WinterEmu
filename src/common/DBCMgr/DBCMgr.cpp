#include "DBCMgr.hpp"
#include "src/relayserver/RelayServer.hpp"
#include "Time/Timer.hpp"
#include "Logger.hpp"

DBCMgr::~DBCMgr() {
    cleanUpBeforeDelete();
}

void DBCMgr::cleanUpBeforeDelete() {
    _chrClassesMap.clear();
}

void DBCMgr::initialize_for_relay() {
    load_ChrClasses();
}

void DBCMgr::load_ChrClasses() {
    auto log = Logger::get();
    _chrClassesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHRCLASSES");
        auto rows = server_->db()->execute_sync_many<DbcChrClassesRow>(stmt);
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
        log->error("RealmList::update failed: {}", ex.what());
    }
}