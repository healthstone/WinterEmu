#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_light **/
struct DbcLight {
    uint32_t id;
    uint32_t continent_id;
    float x;
    float y;
    float z;
    float falloff_start;
    float falloff_end;
    uint32_t lightparamsid_1;
    uint32_t lightparamsid_2;
    uint32_t lightparamsid_3;
    uint32_t lightparamsid_4;
    uint32_t lightparamsid_5;
    uint32_t lightparamsid_6;
    uint32_t lightparamsid_7;
    uint32_t lightparamsid_8;
};

template<>
struct PgRowMapper<DbcLight> {
    static DbcLight map(const pqxx::row& r) {
        DbcLight obj;
        obj.id              = r["id"].as<uint32_t>();
        obj.continent_id    = r["continent_id"].as<uint32_t>();
        obj.x               = r["x"].as<float>();
        obj.y               = r["y"].as<float>();
        obj.z               = r["z"].as<float>();
        obj.falloff_start   = r["falloff_start"].as<float>();
        obj.falloff_end     = r["falloff_end"].as<float>();
        obj.lightparamsid_1 = r["lightparamsid_1"].as<uint32_t>();
        obj.lightparamsid_2 = r["lightparamsid_2"].as<uint32_t>();
        obj.lightparamsid_3 = r["lightparamsid_3"].as<uint32_t>();
        obj.lightparamsid_4 = r["lightparamsid_4"].as<uint32_t>();
        obj.lightparamsid_5 = r["lightparamsid_5"].as<uint32_t>();
        obj.lightparamsid_6 = r["lightparamsid_6"].as<uint32_t>();
        obj.lightparamsid_7 = r["lightparamsid_7"].as<uint32_t>();
        obj.lightparamsid_8 = r["lightparamsid_8"].as<uint32_t>();
        return obj;
    }
};