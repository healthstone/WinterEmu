#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellVisual {
    uint32_t id;
    uint32_t precastkit;
    uint32_t castkit;
    uint32_t impactkit;
    uint32_t statekit;
    uint32_t statedonekit;
    uint32_t channelkit;
    uint32_t hasmissile;
    int32_t missilemodel;
    uint32_t missilepathtype;
    uint32_t missiledestinationattachment;
    int32_t missilesound;
    int32_t animeventsoundid;
    uint32_t flags;
    uint32_t casterimpactkit;
    uint32_t targetimpactkit;
    int32_t missileattachment;
    int32_t missilefollowgroundheight;
    uint32_t missilefollowgrounddropspeed;
    uint32_t missilefollowgroundapproach;
    uint32_t missilefollowgroundflags;
    uint32_t misslemotion;
    uint32_t missiletargetingkit;
    uint32_t instantareakit;
    uint32_t impactareakit;
    uint32_t persistentareakit;
    float missilecastoffsetx;
    float missilecastoffsety;
    float missilecastoffsetz;
    float missileimpactoffsetx;
    float missileimpactoffsety;
    float missileimpactoffsetz;
};

template<>
struct PgRowMapper<DbcSpellVisual> {
    static DbcSpellVisual map(const pqxx::row& r) {
        DbcSpellVisual obj{};
        obj.id = r["id"].as<uint32_t>();
        obj.precastkit = r["precastkit"].as<uint32_t>();
        obj.castkit = r["castkit"].as<uint32_t>();
        obj.impactkit = r["impactkit"].as<uint32_t>();
        obj.statekit = r["statekit"].as<uint32_t>();
        obj.statedonekit = r["statedonekit"].as<uint32_t>();
        obj.channelkit = r["channelkit"].as<uint32_t>();
        obj.hasmissile = r["hasmissile"].as<uint32_t>();
        obj.missilemodel = r["missilemodel"].as<int32_t>();
        obj.missilepathtype = r["missilepathtype"].as<uint32_t>();
        obj.missiledestinationattachment = r["missiledestinationattachment"].as<uint32_t>();
        obj.missilesound = r["missilesound"].as<int32_t>();
        obj.animeventsoundid = r["animeventsoundid"].as<int32_t>();
        obj.flags = r["flags"].as<uint32_t>();
        obj.casterimpactkit = r["casterimpactkit"].as<uint32_t>();
        obj.targetimpactkit = r["targetimpactkit"].as<uint32_t>();
        obj.missileattachment = r["missileattachment"].as<int32_t>();
        obj.missilefollowgroundheight = r["missilefollowgroundheight"].as<int32_t>();
        obj.missilefollowgrounddropspeed = r["missilefollowgrounddropspeed"].as<uint32_t>();
        obj.missilefollowgroundapproach = r["missilefollowgroundapproach"].as<uint32_t>();
        obj.missilefollowgroundflags = r["missilefollowgroundflags"].as<uint32_t>();
        obj.misslemotion = r["misslemotion"].as<uint32_t>();
        obj.missiletargetingkit = r["missiletargetingkit"].as<uint32_t>();
        obj.instantareakit = r["instantareakit"].as<uint32_t>();
        obj.impactareakit = r["impactareakit"].as<uint32_t>();
        obj.persistentareakit = r["persistentareakit"].as<uint32_t>();
        obj.missilecastoffsetx = r["missilecastoffsetx"].as<float>();
        obj.missilecastoffsety = r["missilecastoffsety"].as<float>();
        obj.missilecastoffsetz = r["missilecastoffsetz"].as<float>();
        obj.missileimpactoffsetx = r["missileimpactoffsetx"].as<float>();
        obj.missileimpactoffsety = r["missileimpactoffsety"].as<float>();
        obj.missileimpactoffsetz = r["missileimpactoffsetz"].as<float>();
        return obj;
    }
};