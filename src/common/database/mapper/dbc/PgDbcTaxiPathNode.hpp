#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcTaxiPathNode {
    uint32_t id;
    uint32_t pathid;
    uint32_t nodeindex;
    uint32_t continentid;
    float locx;
    float locy;
    float locz;
    uint32_t flags;
    uint32_t delay;
    uint32_t arrivaleventid;
    uint32_t departureeventid;
};

template<>
struct PgRowMapper<DbcTaxiPathNode> {
    static DbcTaxiPathNode map(const pqxx::row& r) {
        DbcTaxiPathNode x;
        x.id               = r["id"].as<uint32_t>();
        x.pathid           = r["pathid"].as<uint32_t>();
        x.nodeindex        = r["nodeindex"].as<uint32_t>();
        x.continentid      = r["continentid"].as<uint32_t>();
        x.locx             = r["locx"].as<float>();
        x.locy             = r["locy"].as<float>();
        x.locz             = r["locz"].as<float>();
        x.flags            = r["flags"].as<uint32_t>();
        x.delay            = r["delay"].as<uint32_t>();
        x.arrivaleventid   = r["arrivaleventid"].as<uint32_t>();
        x.departureeventid = r["departureeventid"].as<uint32_t>();
        return x;
    }
};