#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtchancetomeleecritbase **/
struct DbcGtChanceToMeleeCritBase {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtChanceToMeleeCritBase> {
    static DbcGtChanceToMeleeCritBase map(const pqxx::row& r) {
        DbcGtChanceToMeleeCritBase row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};