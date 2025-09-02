#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "QueryResults.hpp"

/** dbc_creaturedisplayinfo **/
struct DbcCreatureDisplayInfo {
    uint32_t ID;
    uint32_t ModelID;
    uint32_t SoundID;
    uint32_t ExtendedDisplayInfoID;
    float CreatureModelScale;
    uint32_t CreatureModelAlpha;
    std::optional<std::string> TextureVariation_1;
    std::optional<std::string> TextureVariation_2;
    std::optional<std::string> TextureVariation_3;
    std::optional<std::string> PortraitTextureName;
    int32_t BloodLevel;
    uint32_t BloodID;
    uint32_t NPCSoundID;
    uint32_t ParticleColorID;
    uint32_t CreatureGeosetData;
    uint32_t ObjectEffectPackageID;
};

template<>
struct PgRowMapper<DbcCreatureDisplayInfo> {
    static DbcCreatureDisplayInfo map(const pqxx::row& r) {
        DbcCreatureDisplayInfo row;

        row.ID = r["id"].as<uint32_t>();
        row.ModelID = r["modelid"].as<uint32_t>();
        row.SoundID = r["soundid"].as<uint32_t>();
        row.ExtendedDisplayInfoID = r["extendeddisplayinfoid"].as<uint32_t>();
        row.CreatureModelScale = r["creaturemodelscale"].as<float>();
        row.CreatureModelAlpha = r["creaturemodelalpha"].as<uint32_t>();
        row.BloodLevel = r["bloodlevel"].as<int32_t>();
        row.BloodID = r["bloodid"].as<uint32_t>();
        row.NPCSoundID = r["npcsoundid"].as<uint32_t>();
        row.ParticleColorID = r["particlecolorid"].as<uint32_t>();
        row.CreatureGeosetData = r["creaturegeosetdata"].as<uint32_t>();
        row.ObjectEffectPackageID = r["objecteffectpackageid"].as<uint32_t>();

        row.TextureVariation_1 = get_optional_string(r, "texturevariation_1");
        row.TextureVariation_2 = get_optional_string(r, "texturevariation_2");
        row.TextureVariation_3 = get_optional_string(r, "texturevariation_3");
        row.PortraitTextureName = get_optional_string(r, "portraittexturename");

        return row;
    }
};
