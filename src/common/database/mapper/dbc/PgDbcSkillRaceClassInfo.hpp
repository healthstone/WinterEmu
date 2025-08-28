#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"

/** dbc_skillraceclassinfo **/
struct DbcSkillRaceClassInfo {
    uint32_t ID;
    uint32_t SkillID;
    int32_t RaceMask;
    int32_t ClassMask;
    uint32_t Flags;
    uint32_t MinLevel;
    uint32_t SkillTierID;
    uint32_t SkillCostIndex;
};

template<>
struct PgRowMapper<DbcSkillRaceClassInfo> {
    static DbcSkillRaceClassInfo map(const pqxx::row& r) {
        DbcSkillRaceClassInfo row;

        row.ID            = r["id"].as<uint32_t>();
        row.SkillID       = r["skillid"].as<uint32_t>();
        row.RaceMask      = r["racemask"].as<int32_t>();
        row.ClassMask     = r["classmask"].as<int32_t>();
        row.Flags         = r["flags"].as<uint32_t>();
        row.MinLevel      = r["minlevel"].as<uint32_t>();
        row.SkillTierID   = r["skilltierid"].as<uint32_t>();
        row.SkillCostIndex= r["skillcostindex"].as<uint32_t>();

        return row;
    }
};