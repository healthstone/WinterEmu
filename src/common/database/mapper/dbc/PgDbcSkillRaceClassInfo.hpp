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
    uint32_t RaceMask;
    uint32_t ClassMask;
    uint32_t Flags;
    uint32_t MinLevel;
    uint32_t SkillTierID;
    uint32_t SkillCostIndex;
};

template<>
struct PgRowMapper<DbcSkillRaceClassInfo> {
    static DbcSkillRaceClassInfo map(const pqxx::row& r) {
        DbcSkillRaceClassInfo row;

        row.ID            = static_cast<uint32_t>(r["id"].as<int32_t>());
        row.SkillID       = static_cast<uint32_t>(r["skillid"].as<int32_t>());
        row.RaceMask      = static_cast<uint32_t>(r["racemask"].as<int32_t>());
        row.ClassMask     = static_cast<uint32_t>(r["classmask"].as<int32_t>());
        row.Flags         = static_cast<uint32_t>(r["flags"].as<int32_t>());
        row.MinLevel      = static_cast<uint32_t>(r["minlevel"].as<int32_t>());
        row.SkillTierID   = static_cast<uint32_t>(r["skilltierid"].as<int32_t>());
        row.SkillCostIndex= static_cast<uint32_t>(r["skillcostindex"].as<int32_t>());

        return row;
    }
};