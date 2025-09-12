#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_pvpdifficulty **/
struct DbcPvpdifficulty {
    uint32_t id;
    uint32_t map_id;
    uint32_t range_index;
    uint32_t min_level;
    uint32_t max_level;
    uint32_t difficulty;
};

template<>
struct PgRowMapper<DbcPvpdifficulty> {
    static DbcPvpdifficulty map(const pqxx::row& r) {
        DbcPvpdifficulty obj{};
        obj.id          = r["id"].as<uint32_t>();
        obj.map_id      = r["map_id"].as<uint32_t>();
        obj.range_index = r["range_index"].as<uint32_t>();
        obj.min_level   = r["min_level"].as<uint32_t>();
        obj.max_level   = r["max_level"].as<uint32_t>();
        obj.difficulty  = r["difficulty"].as<uint32_t>();
        return obj;
    }
};