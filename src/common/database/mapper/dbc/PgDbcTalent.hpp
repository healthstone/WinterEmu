#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcTalent {
    uint32_t id;
    uint32_t tabid;
    uint32_t tierid;
    uint32_t columnindex;

    uint32_t spellrank_1;
    uint32_t spellrank_2;
    uint32_t spellrank_3;
    uint32_t spellrank_4;
    uint32_t spellrank_5;
    uint32_t spellrank_6;
    uint32_t spellrank_7;
    uint32_t spellrank_8;
    uint32_t spellrank_9;

    uint32_t prereqtalent_1;
    uint32_t prereqtalent_2;
    uint32_t prereqtalent_3;

    uint32_t prerekrank_1;
    uint32_t prerekrank_2;
    uint32_t prerekrank_3;

    uint32_t flags;
    uint32_t requiredspellid;
    int32_t categorymask_1;
    int32_t categorymask_2;
};

template<>
struct PgRowMapper<DbcTalent> {
    static DbcTalent map(const pqxx::row& r) {
        DbcTalent t;
        t.id              = r["id"].as<uint32_t>();
        t.tabid           = r["tabid"].as<uint32_t>();
        t.tierid          = r["tierid"].as<uint32_t>();
        t.columnindex     = r["columnindex"].as<uint32_t>();

        t.spellrank_1     = r["spellrank_1"].as<uint32_t>();
        t.spellrank_2     = r["spellrank_2"].as<uint32_t>();
        t.spellrank_3     = r["spellrank_3"].as<uint32_t>();
        t.spellrank_4     = r["spellrank_4"].as<uint32_t>();
        t.spellrank_5     = r["spellrank_5"].as<uint32_t>();
        t.spellrank_6     = r["spellrank_6"].as<uint32_t>();
        t.spellrank_7     = r["spellrank_7"].as<uint32_t>();
        t.spellrank_8     = r["spellrank_8"].as<uint32_t>();
        t.spellrank_9     = r["spellrank_9"].as<uint32_t>();

        t.prereqtalent_1  = r["prereqtalent_1"].as<uint32_t>();
        t.prereqtalent_2  = r["prereqtalent_2"].as<uint32_t>();
        t.prereqtalent_3  = r["prereqtalent_3"].as<uint32_t>();

        t.prerekrank_1    = r["prerekrank_1"].as<uint32_t>();
        t.prerekrank_2    = r["prerekrank_2"].as<uint32_t>();
        t.prerekrank_3    = r["prerekrank_3"].as<uint32_t>();

        t.flags           = r["flags"].as<uint32_t>();
        t.requiredspellid = r["requiredspellid"].as<uint32_t>();
        t.categorymask_1  = r["categorymask_1"].as<int32_t>();
        t.categorymask_2  = r["categorymask_2"].as<int32_t>();

        return t;
    }
};