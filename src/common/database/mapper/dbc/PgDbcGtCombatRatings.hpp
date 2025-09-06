#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtcombatratings **/
struct DbcGtCombatRatings {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtCombatRatings> {
    static DbcGtCombatRatings map(const pqxx::row& r) {
        DbcGtCombatRatings row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
