#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcStableSlotPrices {
    uint32_t id;
    uint32_t cost;
};

template<>
struct PgRowMapper<DbcStableSlotPrices> {
    static DbcStableSlotPrices map(const pqxx::row& r) {
        DbcStableSlotPrices obj{};
        obj.id = r["id"].as<uint32_t>();
        obj.cost = r["cost"].as<uint32_t>();
        return obj;
    }
};