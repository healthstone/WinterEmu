#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_overridespelldata **/
struct DbcOverridespellData {
    uint32_t id;
    uint32_t spells_1;
    uint32_t spells_2;
    uint32_t spells_3;
    uint32_t spells_4;
    uint32_t spells_5;
    uint32_t spells_6;
    uint32_t spells_7;
    uint32_t spells_8;
    uint32_t spells_9;
    uint32_t spells_10;
    uint32_t flags;
};

template<>
struct PgRowMapper<DbcOverridespellData> {
    static DbcOverridespellData map(const pqxx::row &r) {
        DbcOverridespellData obj;
        obj.id        = r["id"].as<uint32_t>();
        obj.spells_1  = r["spells_1"].as<uint32_t>();
        obj.spells_2  = r["spells_2"].as<uint32_t>();
        obj.spells_3  = r["spells_3"].as<uint32_t>();
        obj.spells_4  = r["spells_4"].as<uint32_t>();
        obj.spells_5  = r["spells_5"].as<uint32_t>();
        obj.spells_6  = r["spells_6"].as<uint32_t>();
        obj.spells_7  = r["spells_7"].as<uint32_t>();
        obj.spells_8  = r["spells_8"].as<uint32_t>();
        obj.spells_9  = r["spells_9"].as<uint32_t>();
        obj.spells_10 = r["spells_10"].as<uint32_t>();
        obj.flags     = r["flags"].as<uint32_t>();
        return obj;
    }
};