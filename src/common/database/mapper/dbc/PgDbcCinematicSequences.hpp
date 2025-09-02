#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_cinematicsequences **/
struct DbcCinematicSequences {
    uint32_t id;
    uint32_t soundid;
    uint32_t camera_1;
    uint32_t camera_2;
    uint32_t camera_3;
    uint32_t camera_4;
    uint32_t camera_5;
    uint32_t camera_6;
    uint32_t camera_7;
    uint32_t camera_8;
};

template<>
struct PgRowMapper<DbcCinematicSequences> {
    static DbcCinematicSequences map(const pqxx::row& r) {
        DbcCinematicSequences row;

        row.id       = r["id"].as<uint32_t>();
        row.soundid  = r["soundid"].as<uint32_t>();
        row.camera_1 = r["camera_1"].as<uint32_t>();
        row.camera_2 = r["camera_2"].as<uint32_t>();
        row.camera_3 = r["camera_3"].as<uint32_t>();
        row.camera_4 = r["camera_4"].as<uint32_t>();
        row.camera_5 = r["camera_5"].as<uint32_t>();
        row.camera_6 = r["camera_6"].as<uint32_t>();
        row.camera_7 = r["camera_7"].as<uint32_t>();
        row.camera_8 = r["camera_8"].as<uint32_t>();

        return row;
    }
};
