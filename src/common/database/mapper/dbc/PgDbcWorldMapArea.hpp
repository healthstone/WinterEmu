#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcWorldMapArea {
    uint32_t id;
    uint32_t mapid;
    uint32_t areaid;
    std::optional<std::string> areaname;
    float locleft;
    float locright;
    float loctop;
    float locbottom;
    int32_t displaymapid;
    int32_t defaultdungeonfloor;
    uint32_t parentworldmapid;
};

template <>
struct PgRowMapper<DbcWorldMapArea> {
    static DbcWorldMapArea map(const pqxx::row& r) {
        DbcWorldMapArea row{};
        row.id = r["id"].as<uint32_t>();
        row.mapid = r["mapid"].as<uint32_t>();
        row.areaid = r["areaid"].as<uint32_t>();
        row.areaname = get_optional_string(r, "areaname");
        row.locleft = r["locleft"].as<float>();
        row.locright = r["locright"].as<float>();
        row.loctop = r["loctop"].as<float>();
        row.locbottom = r["locbottom"].as<float>();
        row.displaymapid = r["displaymapid"].as<int32_t>();
        row.defaultdungeonfloor = r["defaultdungeonfloor"].as<int32_t>();
        row.parentworldmapid = r["parentworldmapid"].as<uint32_t>();
        return row;
    }
};
