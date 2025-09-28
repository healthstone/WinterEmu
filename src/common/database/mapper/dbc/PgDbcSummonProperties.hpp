#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSummonProperties {
    uint32_t id;
    uint32_t control;
    uint32_t faction;
    uint32_t title;
    uint32_t slot;
    uint32_t flags;
};

template<>
struct PgRowMapper<DbcSummonProperties> {
    static DbcSummonProperties map(const pqxx::row& r) {
        DbcSummonProperties obj{};
        obj.id = r["id"].as<uint32_t>();
        obj.control = r["control"].as<uint32_t>();
        obj.faction = r["faction"].as<uint32_t>();
        obj.title = r["title"].as<uint32_t>();
        obj.slot = r["slot"].as<uint32_t>();
        obj.flags = r["flags"].as<uint32_t>();
        return obj;
    }
};