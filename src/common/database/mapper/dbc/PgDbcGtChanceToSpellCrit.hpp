#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtchancetospellcrit **/
struct DbcGtChanceToSpellCrit {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtChanceToSpellCrit> {
    static DbcGtChanceToSpellCrit map(const pqxx::row& r) {
        DbcGtChanceToSpellCrit row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
