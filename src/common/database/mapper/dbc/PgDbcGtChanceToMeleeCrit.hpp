#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtchancetomeleecrit **/
struct DbcGtChanceToMeleeCrit {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtChanceToMeleeCrit> {
    static DbcGtChanceToMeleeCrit map(const pqxx::row& r) {
        DbcGtChanceToMeleeCrit row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};

