#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_gameobjectdisplayinfo **/
struct DbcGameObjectDisplayInfo {
    uint32_t id;

    // Model name (nullable)
    std::optional<std::string> modelname;

    uint32_t sound_1;
    uint32_t sound_2;
    uint32_t sound_3;
    uint32_t sound_4;
    uint32_t sound_5;
    uint32_t sound_6;
    uint32_t sound_7;
    uint32_t sound_8;
    uint32_t sound_9;
    uint32_t sound_10;

    float geoboxminx;
    float geoboxminy;
    float geoboxminz;
    float geoboxmaxx;
    float geoboxmaxy;
    float geoboxmaxz;

    uint32_t objecteffectpackageid;
};

template<>
struct PgRowMapper<DbcGameObjectDisplayInfo> {
    static DbcGameObjectDisplayInfo map(const pqxx::row& r) {
        DbcGameObjectDisplayInfo row{};

        row.id = r["id"].as<uint32_t>();
        row.modelname = get_optional_string(r, "modelname");

        row.sound_1 = r["sound_1"].as<uint32_t>();
        row.sound_2 = r["sound_2"].as<uint32_t>();
        row.sound_3 = r["sound_3"].as<uint32_t>();
        row.sound_4 = r["sound_4"].as<uint32_t>();
        row.sound_5 = r["sound_5"].as<uint32_t>();
        row.sound_6 = r["sound_6"].as<uint32_t>();
        row.sound_7 = r["sound_7"].as<uint32_t>();
        row.sound_8 = r["sound_8"].as<uint32_t>();
        row.sound_9 = r["sound_9"].as<uint32_t>();
        row.sound_10 = r["sound_10"].as<uint32_t>();

        row.geoboxminx = r["geoboxminx"].as<float>();
        row.geoboxminy = r["geoboxminy"].as<float>();
        row.geoboxminz = r["geoboxminz"].as<float>();
        row.geoboxmaxx = r["geoboxmaxx"].as<float>();
        row.geoboxmaxy = r["geoboxmaxy"].as<float>();
        row.geoboxmaxz = r["geoboxmaxz"].as<float>();

        row.objecteffectpackageid = r["objecteffectpackageid"].as<uint32_t>();

        return row;
    }
};
