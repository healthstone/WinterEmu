#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSkillLineAbility {
    uint32_t id;
    uint32_t skillline;
    uint32_t spell;
    uint32_t racemask;
    uint32_t classmask;
    uint32_t excluderace;
    uint32_t excludeclass;
    uint32_t minskilllinerank;
    uint32_t supercededbyspell;
    uint32_t acquiremethod;
    uint32_t trivialskilllinerankhigh;
    uint32_t trivialskilllineranklow;
    uint32_t tradeskillcategoryid;
};

template<>
struct PgRowMapper<DbcSkillLineAbility> {
    static DbcSkillLineAbility map(const pqxx::row& r) {
        DbcSkillLineAbility row{};
        row.id                       = r["id"].as<uint32_t>();
        row.skillline                = r["skillline"].as<uint32_t>();
        row.spell                    = r["spell"].as<uint32_t>();
        row.racemask                 = r["racemask"].as<uint32_t>();
        row.classmask                = r["classmask"].as<uint32_t>();
        row.excluderace              = r["excluderace"].as<uint32_t>();
        row.excludeclass             = r["excludeclass"].as<uint32_t>();
        row.minskilllinerank         = r["minskilllinerank"].as<uint32_t>();
        row.supercededbyspell        = r["supercededbyspell"].as<uint32_t>();
        row.acquiremethod            = r["acquiremethod"].as<uint32_t>();
        row.trivialskilllinerankhigh = r["trivialskilllinerankhigh"].as<uint32_t>();
        row.trivialskilllineranklow  = r["trivialskilllineranklow"].as<uint32_t>();
        row.tradeskillcategoryid     = r["tradeskillcategoryid"].as<uint32_t>();
        return row;
    }
};