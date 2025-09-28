#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellRuneCost {
    uint32_t id;
    uint32_t blood;
    uint32_t unholy;
    uint32_t frost;
    uint32_t runicpower;
};

template<>
struct PgRowMapper<DbcSpellRuneCost> {
    static DbcSpellRuneCost map(const pqxx::row& r) {
        DbcSpellRuneCost obj;
        obj.id        = r["id"].as<uint32_t>();
        obj.blood     = r["blood"].as<uint32_t>();
        obj.unholy    = r["unholy"].as<uint32_t>();
        obj.frost     = r["frost"].as<uint32_t>();
        obj.runicpower= r["runicpower"].as<uint32_t>();
        return obj;
    }
};