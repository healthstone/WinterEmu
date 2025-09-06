#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtbarbershopcostbase **/
struct DbcGtBarberShopCostBase {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtBarberShopCostBase> {
    static DbcGtBarberShopCostBase map(const pqxx::row& r) {
        DbcGtBarberShopCostBase row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
