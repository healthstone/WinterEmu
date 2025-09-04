#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_creaturespelldata **/
struct DbcCreatureSpellData {
    uint32_t ID;
    uint32_t Spells_1;
    uint32_t Spells_2;
    uint32_t Spells_3;
    uint32_t Spells_4;
    uint32_t Availability_1;
    uint32_t Availability_2;
    uint32_t Availability_3;
    uint32_t Availability_4;
};

template<>
struct PgRowMapper<DbcCreatureSpellData> {
    static DbcCreatureSpellData map(const pqxx::row& r) {
        DbcCreatureSpellData row;

        row.ID             = r["id"].as<uint32_t>();
        row.Spells_1       = r["spells_1"].as<uint32_t>();
        row.Spells_2       = r["spells_2"].as<uint32_t>();
        row.Spells_3       = r["spells_3"].as<uint32_t>();
        row.Spells_4       = r["spells_4"].as<uint32_t>();
        row.Availability_1 = r["availability_1"].as<uint32_t>();
        row.Availability_2 = r["availability_2"].as<uint32_t>();
        row.Availability_3 = r["availability_3"].as<uint32_t>();
        row.Availability_4 = r["availability_4"].as<uint32_t>();

        return row;
    }
};
