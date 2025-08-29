#pragma once

#include <pqxx/pqxx>
#include <cstdint>

#include "QueryResults.hpp"

/** dbc_areagroup **/
struct DbcAreaGroup {
    uint32_t ID;
    uint32_t AreaID_1;
    uint32_t AreaID_2;
    uint32_t AreaID_3;
    uint32_t AreaID_4;
    uint32_t AreaID_5;
    uint32_t AreaID_6;
    uint32_t NextAreaID;
};

template<>
struct PgRowMapper<DbcAreaGroup> {
    static DbcAreaGroup map(const pqxx::row& r) {
        DbcAreaGroup row;

        row.ID         = r["id"].as<uint32_t>();
        row.AreaID_1   = r["areaid_1"].as<uint32_t>();
        row.AreaID_2   = r["areaid_2"].as<uint32_t>();
        row.AreaID_3   = r["areaid_3"].as<uint32_t>();
        row.AreaID_4   = r["areaid_4"].as<uint32_t>();
        row.AreaID_5   = r["areaid_5"].as<uint32_t>();
        row.AreaID_6   = r["areaid_6"].as<uint32_t>();
        row.NextAreaID = r["nextareaid"].as<uint32_t>();

        return row;
    }
};
