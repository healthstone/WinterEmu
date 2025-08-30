#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_characterfacialhairstyles **/
struct DbcCharacterFacialHairstyles {
    uint32_t id;
    uint8_t raceid;
    uint8_t sexid;
    uint8_t variationid;
    int32_t geoset_1;
    int32_t geoset_2;
    int32_t geoset_3;
    int32_t geoset_4;
    int32_t geoset_5;
};

template<>
struct PgRowMapper<DbcCharacterFacialHairstyles> {
    static DbcCharacterFacialHairstyles map(const pqxx::row& r) {
        DbcCharacterFacialHairstyles row;

        row.id          = r["id"].as<uint32_t>();
        row.raceid      = static_cast<uint8_t>(r["raceid"].as<int>());
        row.sexid       = static_cast<uint8_t>(r["sexid"].as<int>());
        row.variationid = static_cast<uint8_t>(r["variationid"].as<int>());
        row.geoset_1    = r["geoset_1"].as<int32_t>();
        row.geoset_2    = r["geoset_2"].as<int32_t>();
        row.geoset_3    = r["geoset_3"].as<int32_t>();
        row.geoset_4    = r["geoset_4"].as<int32_t>();
        row.geoset_5    = r["geoset_5"].as<int32_t>();

        return row;
    }
};
