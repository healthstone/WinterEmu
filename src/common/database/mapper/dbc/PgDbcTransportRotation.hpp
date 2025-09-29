#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcTransportRotation {
    uint32_t id;
    uint32_t gameobjectsid;
    uint32_t timeindex;
    float rotx;
    float roty;
    float rotz;
    float rotw;
};

template<>
struct PgRowMapper<DbcTransportRotation> {
    static DbcTransportRotation map(const pqxx::row& r) {
        DbcTransportRotation obj;
        obj.id = r["id"].as<uint32_t>();
        obj.gameobjectsid = r["gameobjectsid"].as<uint32_t>();
        obj.timeindex = r["timeindex"].as<uint32_t>();
        obj.rotx = r["rotx"].as<float>();
        obj.roty = r["roty"].as<float>();
        obj.rotz = r["rotz"].as<float>();
        obj.rotw = r["rotw"].as<float>();
        return obj;
    }
};