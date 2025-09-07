#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtoctclasscombatratingscalar **/
struct DbcGtoctClassCombatRatingScalar {
    uint32_t id;
    float data;
};

template<>
struct PgRowMapper<DbcGtoctClassCombatRatingScalar> {
    static DbcGtoctClassCombatRatingScalar map(const pqxx::row& r) {
        DbcGtoctClassCombatRatingScalar row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
