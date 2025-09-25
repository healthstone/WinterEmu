#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellCastTimes {
    uint32_t id;
    int32_t base;
    int32_t perlevel;
    int32_t minimum;
};

template<>
struct PgRowMapper<DbcSpellCastTimes> {
    static DbcSpellCastTimes map(const pqxx::row& r) {
        DbcSpellCastTimes obj;
        obj.id       = r["id"].as<uint32_t>();
        obj.base     = r["base"].as<int32_t>();
        obj.perlevel = r["perlevel"].as<int32_t>();
        obj.minimum  = r["minimum"].as<int32_t>();
        return obj;
    }
};
