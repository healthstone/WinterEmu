#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtnpcmanacostscaler **/
struct DbcGtnpcManaCostScaler {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtnpcManaCostScaler> {
    static DbcGtnpcManaCostScaler map(const pqxx::row& r) {
        DbcGtnpcManaCostScaler row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
