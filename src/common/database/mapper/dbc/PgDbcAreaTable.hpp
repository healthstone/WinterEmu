#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>

#include "QueryResults.hpp"

/** dbc_areatable **/
struct DbcAreaTable {
    uint32_t ID;
    uint32_t ContinentID;
    uint32_t ParentAreaID;
    uint32_t AreaBit;
    uint32_t Flags;
    int32_t SoundProviderPref;
    int32_t SoundProviderPrefUnderwater;
    int32_t AmbienceID;
    int32_t ZoneMusic;
    int32_t IntroSound;
    int32_t ExplorationLevel;

    // AreaName
    std::optional<std::string> AreaName_Lang_enUS;
    std::optional<std::string> AreaName_Lang_enGB;
    std::optional<std::string> AreaName_Lang_koKR;
    std::optional<std::string> AreaName_Lang_frFR;
    std::optional<std::string> AreaName_Lang_deDE;
    std::optional<std::string> AreaName_Lang_enCN;
    std::optional<std::string> AreaName_Lang_zhCN;
    std::optional<std::string> AreaName_Lang_enTW;
    std::optional<std::string> AreaName_Lang_zhTW;
    std::optional<std::string> AreaName_Lang_esES;
    std::optional<std::string> AreaName_Lang_esMX;
    std::optional<std::string> AreaName_Lang_ruRU;
    std::optional<std::string> AreaName_Lang_ptPT;
    std::optional<std::string> AreaName_Lang_ptBR;
    std::optional<std::string> AreaName_Lang_itIT;
    std::optional<std::string> AreaName_Lang_Unk;

    uint32_t AreaName_Lang_Mask;

    uint32_t FactionGroupMask;
    uint32_t LiquidTypeID_1;
    uint32_t LiquidTypeID_2;
    uint32_t LiquidTypeID_3;
    uint32_t LiquidTypeID_4;
    float MinElevation;
    float AmbientMultiplier;
    int32_t LightID;
};

template<>
struct PgRowMapper<DbcAreaTable> {
    static DbcAreaTable map(const pqxx::row& r) {
        DbcAreaTable row;

        // Числовые поля
        row.ID = r["id"].as<uint32_t>();
        row.ContinentID = r["continent_id"].as<uint32_t>();
        row.ParentAreaID = r["parent_area_id"].as<uint32_t>();
        row.AreaBit = r["area_bit"].as<uint32_t>();
        row.Flags = r["flags"].as<uint32_t>();
        row.SoundProviderPref = r["sound_provider_pref"].as<int32_t>();
        row.SoundProviderPrefUnderwater = r["sound_provider_pref_underwater"].as<int32_t>();
        row.AmbienceID = r["ambience_id"].as<int32_t>();
        row.ZoneMusic = r["zone_music"].as<int32_t>();
        row.IntroSound = r["intro_sound"].as<int32_t>();
        row.ExplorationLevel = r["exploration_level"].as<int32_t>();
        row.AreaName_Lang_Mask = r["area_name_lang_mask"].as<uint32_t>();
        row.FactionGroupMask = r["faction_group_mask"].as<uint32_t>();
        row.LiquidTypeID_1 = r["liquid_type_id_1"].as<uint32_t>();
        row.LiquidTypeID_2 = r["liquid_type_id_2"].as<uint32_t>();
        row.LiquidTypeID_3 = r["liquid_type_id_3"].as<uint32_t>();
        row.LiquidTypeID_4 = r["liquid_type_id_4"].as<uint32_t>();
        row.MinElevation = r["min_elevation"].as<float>();
        row.AmbientMultiplier = r["ambient_multiplier"].as<float>();
        row.LightID = r["lightid"].as<int32_t>();

        // AreaName (все языки)
        row.AreaName_Lang_enUS = get_optional_string(r, "area_name_lang_enus");
        row.AreaName_Lang_enGB = get_optional_string(r, "area_name_lang_engb");
        row.AreaName_Lang_koKR = get_optional_string(r, "area_name_lang_kokr");
        row.AreaName_Lang_frFR = get_optional_string(r, "area_name_lang_frfr");
        row.AreaName_Lang_deDE = get_optional_string(r, "area_name_lang_dede");
        row.AreaName_Lang_enCN = get_optional_string(r, "area_name_lang_encn");
        row.AreaName_Lang_zhCN = get_optional_string(r, "area_name_lang_zhcn");
        row.AreaName_Lang_enTW = get_optional_string(r, "area_name_lang_entw");
        row.AreaName_Lang_zhTW = get_optional_string(r, "area_name_lang_zhtw");
        row.AreaName_Lang_esES = get_optional_string(r, "area_name_lang_eses");
        row.AreaName_Lang_esMX = get_optional_string(r, "area_name_lang_esmx");
        row.AreaName_Lang_ruRU = get_optional_string(r, "area_name_lang_ruru");
        row.AreaName_Lang_ptPT = get_optional_string(r, "area_name_lang_ptpt");
        row.AreaName_Lang_ptBR = get_optional_string(r, "area_name_lang_ptbr");
        row.AreaName_Lang_itIT = get_optional_string(r, "area_name_lang_itit");
        row.AreaName_Lang_Unk = get_optional_string(r, "area_name_lang_unk");

        return row;
    }
};
