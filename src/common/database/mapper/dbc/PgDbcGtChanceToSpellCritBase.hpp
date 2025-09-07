#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtchancetospellcritbase **/
struct DbcGtChanceToSpellCritBase {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtChanceToSpellCritBase> {
    static DbcGtChanceToSpellCritBase map(const pqxx::row& r) {
        DbcGtChanceToSpellCritBase row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
