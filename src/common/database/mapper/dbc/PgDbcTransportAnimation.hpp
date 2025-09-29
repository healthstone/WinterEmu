#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcTransportAnimation {
    uint32_t id;
    uint32_t transportid;
    uint32_t timeindex;
    float posx;
    float posy;
    float posz;
    uint32_t sequenceid;
};

template<>
struct PgRowMapper<DbcTransportAnimation> {
    static DbcTransportAnimation map(const pqxx::row& r) {
        return DbcTransportAnimation{
                .id = r["id"].as<uint32_t>(),
                .transportid = r["transportid"].as<uint32_t>(),
                .timeindex = r["timeindex"].as<uint32_t>(),
                .posx = r["posx"].as<float>(),
                .posy = r["posy"].as<float>(),
                .posz = r["posz"].as<float>(),
                .sequenceid = r["sequenceid"].as<uint32_t>()
        };
    }
};