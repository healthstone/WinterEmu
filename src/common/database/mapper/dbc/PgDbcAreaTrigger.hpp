#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"

/** dbc_areatrigger **/
struct DbcAreaTrigger {
    uint32_t ID;
    uint32_t ContinentID;
    float X;
    float Y;
    float Z;
    float Radius;
    float BoxLength;
    float BoxWidth;
    float BoxHeight;
    float BoxYaw;
};

template<>
struct PgRowMapper<DbcAreaTrigger> {
    static DbcAreaTrigger map(const pqxx::row& r) {
        DbcAreaTrigger row;

        row.ID          = r["id"].as<uint32_t>();
        row.ContinentID = r["continent_id"].as<uint32_t>();
        row.X           = r["x"].as<float>();
        row.Y           = r["y"].as<float>();
        row.Z           = r["z"].as<float>();
        row.Radius      = r["radius"].as<float>();
        row.BoxLength   = r["box_length"].as<float>();
        row.BoxWidth    = r["box_width"].as<float>();
        row.BoxHeight   = r["box_height"].as<float>();
        row.BoxYaw      = r["box_yaw"].as<float>();

        return row;
    }
};
