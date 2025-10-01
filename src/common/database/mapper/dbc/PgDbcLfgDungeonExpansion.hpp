#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcLfgDungeonExpansion {
    uint32_t id;
    uint32_t lfg_id;
    uint32_t expansion_level;
    uint32_t random_id;
    uint32_t hard_level_min;
    uint32_t hard_level_max;
    uint32_t target_level_min;
    uint32_t target_level_max;
};

template<>
struct PgRowMapper<DbcLfgDungeonExpansion> {
    static DbcLfgDungeonExpansion map(const pqxx::row& r) {
        DbcLfgDungeonExpansion obj{};
        obj.id               = r["id"].as<uint32_t>();
        obj.lfg_id           = r["lfg_id"].as<uint32_t>();
        obj.expansion_level  = r["expansion_level"].as<uint32_t>();
        obj.random_id        = r["random_id"].as<uint32_t>();
        obj.hard_level_min   = r["hard_level_min"].as<uint32_t>();
        obj.hard_level_max   = r["hard_level_max"].as<uint32_t>();
        obj.target_level_min = r["target_level_min"].as<uint32_t>();
        obj.target_level_max = r["target_level_max"].as<uint32_t>();
        return obj;
    }
};