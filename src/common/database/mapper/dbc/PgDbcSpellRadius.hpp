#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellRadius {
    uint32_t id;
    float radius;
    float radius_per_level;
    float radius_max;
};

template<>
struct PgRowMapper<DbcSpellRadius> {
    static DbcSpellRadius map(const pqxx::row& r) {
        DbcSpellRadius obj;
        obj.id = r["id"].as<uint32_t>();
        obj.radius = r["radius"].as<float>();
        obj.radius_per_level = r["radius_per_level"].as<float>();
        obj.radius_max = r["radius_max"].as<float>();
        return obj;
    }
};