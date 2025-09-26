#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSpellDuration {
    uint32_t id;
    int32_t duration;
    int32_t duration_per_level;
    int32_t max_duration;
};

template<>
struct PgRowMapper<DbcSpellDuration> {
    static DbcSpellDuration map(const pqxx::row& r) {
        DbcSpellDuration obj{};
        obj.id = r["id"].as<uint32_t>();
        obj.duration = r["duration"].as<int32_t>();
        obj.duration_per_level = r["duration_per_level"].as<int32_t>();
        obj.max_duration = r["max_duration"].as<int32_t>();
        return obj;
    }
};