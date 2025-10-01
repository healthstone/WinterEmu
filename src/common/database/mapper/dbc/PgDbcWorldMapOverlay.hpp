#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcWorldMapOverlay {
    uint32_t id;
    uint32_t mapareaid;
    uint32_t areaid_1;
    uint32_t areaid_2;
    uint32_t areaid_3;
    uint32_t areaid_4;
    uint32_t mappointx;
    uint32_t mappointy;
    std::optional<std::string> texturename;
    uint32_t texturewidth;
    uint32_t textureheight;
    uint32_t offsetx;
    uint32_t offsety;
    uint32_t hitrecttop;
    uint32_t hitrectleft;
    uint32_t hitrectbottom;
    uint32_t hitrectright;
};

template<>
struct PgRowMapper<DbcWorldMapOverlay> {
    static DbcWorldMapOverlay map(const pqxx::row& r) {
        DbcWorldMapOverlay obj{};
        obj.id = r["id"].as<uint32_t>();
        obj.mapareaid = r["mapareaid"].as<uint32_t>();
        obj.areaid_1 = r["areaid_1"].as<uint32_t>();
        obj.areaid_2 = r["areaid_2"].as<uint32_t>();
        obj.areaid_3 = r["areaid_3"].as<uint32_t>();
        obj.areaid_4 = r["areaid_4"].as<uint32_t>();
        obj.mappointx = r["mappointx"].as<uint32_t>();
        obj.mappointy = r["mappointy"].as<uint32_t>();
        obj.texturename = get_optional_string(r, "texturename");
        obj.texturewidth = r["texturewidth"].as<uint32_t>();
        obj.textureheight = r["textureheight"].as<uint32_t>();
        obj.offsetx = r["offsetx"].as<uint32_t>();
        obj.offsety = r["offsety"].as<uint32_t>();
        obj.hitrecttop = r["hitrecttop"].as<uint32_t>();
        obj.hitrectleft = r["hitrectleft"].as<uint32_t>();
        obj.hitrectbottom = r["hitrectbottom"].as<uint32_t>();
        obj.hitrectright = r["hitrectright"].as<uint32_t>();
        return obj;
    }
};
