#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcTaxiPath {
    uint32_t id;
    int32_t fromtaxinode;
    int32_t totaxinode;
    uint32_t cost;
};

template<>
struct PgRowMapper<DbcTaxiPath> {
    static DbcTaxiPath map(const pqxx::row& r) {
        DbcTaxiPath obj;
        obj.id           = r["id"].as<uint32_t>();
        obj.fromtaxinode = r["fromtaxinode"].as<int32_t>();
        obj.totaxinode   = r["totaxinode"].as<int32_t>();
        obj.cost         = r["cost"].as<uint32_t>();
        return obj;
    }
};