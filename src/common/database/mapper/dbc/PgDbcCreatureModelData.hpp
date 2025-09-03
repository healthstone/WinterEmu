#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_creaturemodeldata **/
struct DbcCreatureModelData {
    uint32_t ID;
    uint32_t Flags;
    std::optional<std::string> ModelName;
    uint32_t SizeClass;
    float ModelScale;
    int32_t BloodID;
    int32_t FootprintTextureID;
    float FootprintTextureLength;
    float FootprintTextureWidth;
    float FootprintParticleScale;
    uint32_t FoleyMaterialID;
    uint32_t FootstepShakeSize;
    uint32_t DeathThudShakeSize;
    uint32_t SoundID;
    float CollisionWidth;
    float CollisionHeight;
    float MountHeight;
    float GeoBoxMinX;
    float GeoBoxMinY;
    float GeoBoxMinZ;
    float GeoBoxMaxX;
    float GeoBoxMaxY;
    float GeoBoxMaxZ;
    float WorldEffectScale;
    float AttachedEffectScale;
    float MissileCollisionRadius;
    float MissileCollisionPush;
    float MissileCollisionRaise;
};

template<>
struct PgRowMapper<DbcCreatureModelData> {
    static DbcCreatureModelData map(const pqxx::row& r) {
        DbcCreatureModelData row;

        row.ID                     = r["id"].as<uint32_t>();
        row.Flags                  = r["flags"].as<uint32_t>();
        row.ModelName              = get_optional_string(r, "modelname");
        row.SizeClass              = r["sizeclass"].as<uint32_t>();
        row.ModelScale             = r["modelscale"].as<float>();
        row.BloodID                = r["bloodid"].as<int32_t>();
        row.FootprintTextureID     = r["footprinttextureid"].as<int32_t>();
        row.FootprintTextureLength = r["footprinttexturelength"].as<float>();
        row.FootprintTextureWidth  = r["footprinttexturewidth"].as<float>();
        row.FootprintParticleScale = r["footprintparticlescale"].as<float>();
        row.FoleyMaterialID        = r["foleymaterialid"].as<uint32_t>();
        row.FootstepShakeSize      = r["footstepshakesize"].as<uint32_t>();
        row.DeathThudShakeSize     = r["deaththudshakesize"].as<uint32_t>();
        row.SoundID                = r["soundid"].as<uint32_t>();
        row.CollisionWidth         = r["collisionwidth"].as<float>();
        row.CollisionHeight        = r["collisionheight"].as<float>();
        row.MountHeight            = r["mountheight"].as<float>();
        row.GeoBoxMinX             = r["geoboxminx"].as<float>();
        row.GeoBoxMinY             = r["geoboxminy"].as<float>();
        row.GeoBoxMinZ             = r["geoboxminz"].as<float>();
        row.GeoBoxMaxX             = r["geoboxmaxx"].as<float>();
        row.GeoBoxMaxY             = r["geoboxmaxy"].as<float>();
        row.GeoBoxMaxZ             = r["geoboxmaxz"].as<float>();
        row.WorldEffectScale       = r["worldeffectscale"].as<float>();
        row.AttachedEffectScale    = r["attachedeffectscale"].as<float>();
        row.MissileCollisionRadius = r["missilecollisionradius"].as<float>();
        row.MissileCollisionPush   = r["missilecollisionpush"].as<float>();
        row.MissileCollisionRaise  = r["missilecollisionraise"].as<float>();

        return row;
    }
};
