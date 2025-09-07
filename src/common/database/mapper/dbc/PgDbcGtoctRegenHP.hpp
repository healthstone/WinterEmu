#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtoctregenhp **/
struct DbcGtoctRegenHP {
    uint32_t id;
    float data;
};

template<>
struct PgRowMapper<DbcGtoctRegenHP> {
    static DbcGtoctRegenHP map(const pqxx::row& r) {
        DbcGtoctRegenHP row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};