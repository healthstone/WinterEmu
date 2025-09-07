#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtoctregenmp **/
struct DbcGtoctRegenMP {
    uint32_t id;
    float data;
};

template<>
struct PgRowMapper<DbcGtoctRegenMP> {
    static DbcGtoctRegenMP map(const pqxx::row& r) {
        DbcGtoctRegenMP row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};