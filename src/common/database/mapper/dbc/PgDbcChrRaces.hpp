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

/** dbc_chrraces **/
struct DbcChrRaces {
    uint32_t ID;
    uint32_t Flags;
    uint32_t FactionID;
    uint32_t ExplorationSoundID;
    uint32_t MaleDisplayId;
    uint32_t FemaleDisplayId;
    std::optional<std::string> ClientPrefix;
    uint32_t BaseLanguage;
    uint32_t CreatureType;
    uint32_t ResSicknessSpellID;
    uint32_t SplashSoundID;
    std::optional<std::string> ClientFilestring;
    uint32_t CinematicSequenceID;
    uint32_t Alliance;

    // Name fields
    std::optional<std::string> Name_Lang_enUS;
    std::optional<std::string> Name_Lang_enGB;
    std::optional<std::string> Name_Lang_koKR;
    std::optional<std::string> Name_Lang_frFR;
    std::optional<std::string> Name_Lang_deDE;
    std::optional<std::string> Name_Lang_enCN;
    std::optional<std::string> Name_Lang_zhCN;
    std::optional<std::string> Name_Lang_enTW;
    std::optional<std::string> Name_Lang_zhTW;
    std::optional<std::string> Name_Lang_esES;
    std::optional<std::string> Name_Lang_esMX;
    std::optional<std::string> Name_Lang_ruRU;
    std::optional<std::string> Name_Lang_ptPT;
    std::optional<std::string> Name_Lang_ptBR;
    std::optional<std::string> Name_Lang_itIT;
    std::optional<std::string> Name_Lang_Unk;
    uint32_t Name_Lang_Mask;

    // Female name fields
    std::optional<std::string> Name_Female_Lang_enUS;
    std::optional<std::string> Name_Female_Lang_enGB;
    std::optional<std::string> Name_Female_Lang_koKR;
    std::optional<std::string> Name_Female_Lang_frFR;
    std::optional<std::string> Name_Female_Lang_deDE;
    std::optional<std::string> Name_Female_Lang_enCN;
    std::optional<std::string> Name_Female_Lang_zhCN;
    std::optional<std::string> Name_Female_Lang_enTW;
    std::optional<std::string> Name_Female_Lang_zhTW;
    std::optional<std::string> Name_Female_Lang_esES;
    std::optional<std::string> Name_Female_Lang_esMX;
    std::optional<std::string> Name_Female_Lang_ruRU;
    std::optional<std::string> Name_Female_Lang_ptPT;
    std::optional<std::string> Name_Female_Lang_ptBR;
    std::optional<std::string> Name_Female_Lang_itIT;
    std::optional<std::string> Name_Female_Lang_Unk;
    uint32_t Name_Female_Lang_Mask;

    // Male name fields
    std::optional<std::string> Name_Male_Lang_enUS;
    std::optional<std::string> Name_Male_Lang_enGB;
    std::optional<std::string> Name_Male_Lang_koKR;
    std::optional<std::string> Name_Male_Lang_frFR;
    std::optional<std::string> Name_Male_Lang_deDE;
    std::optional<std::string> Name_Male_Lang_enCN;
    std::optional<std::string> Name_Male_Lang_zhCN;
    std::optional<std::string> Name_Male_Lang_enTW;
    std::optional<std::string> Name_Male_Lang_zhTW;
    std::optional<std::string> Name_Male_Lang_esES;
    std::optional<std::string> Name_Male_Lang_esMX;
    std::optional<std::string> Name_Male_Lang_ruRU;
    std::optional<std::string> Name_Male_Lang_ptPT;
    std::optional<std::string> Name_Male_Lang_ptBR;
    std::optional<std::string> Name_Male_Lang_itIT;
    std::optional<std::string> Name_Male_Lang_Unk;
    uint32_t Name_Male_Lang_Mask;

    // Customization fields
    std::optional<std::string> FacialHairCustomization_1;
    std::optional<std::string> FacialHairCustomization_2;
    std::optional<std::string> HairCustomization;

    uint32_t Required_Expansion;
};

template<>
struct PgRowMapper<DbcChrRaces> {
    static DbcChrRaces map(const pqxx::row& r) {
        DbcChrRaces row;

        row.ID = r["id"].as<uint32_t>();
        row.Flags = r["flags"].as<uint32_t>();
        row.FactionID = r["factionid"].as<uint32_t>();
        row.ExplorationSoundID = r["explorationsoundid"].as<uint32_t>();
        row.MaleDisplayId = r["maledisplayid"].as<uint32_t>();
        row.FemaleDisplayId = r["femaledisplayid"].as<uint32_t>();

        // Обработка optional строковых полей
        if (!r["clientprefix"].is_null()) row.ClientPrefix = r["clientprefix"].as<std::string>();

        row.BaseLanguage = r["baselanguage"].as<uint32_t>();
        row.CreatureType = r["creaturetype"].as<uint32_t>();
        row.ResSicknessSpellID = r["ressicknessspellid"].as<uint32_t>();
        row.SplashSoundID = r["splashsoundid"].as<uint32_t>();

        if (!r["clientfilestring"].is_null()) row.ClientFilestring = r["clientfilestring"].as<std::string>();

        row.CinematicSequenceID = r["cinematicsequenceid"].as<uint32_t>();
        row.Alliance = r["alliance"].as<uint32_t>();

        // Name fields
        if (!r["name_lang_enus"].is_null()) row.Name_Lang_enUS = r["name_lang_enus"].as<std::string>();
        if (!r["name_lang_engb"].is_null()) row.Name_Lang_enGB = r["name_lang_engb"].as<std::string>();
        if (!r["name_lang_kokr"].is_null()) row.Name_Lang_koKR = r["name_lang_kokr"].as<std::string>();
        if (!r["name_lang_frfr"].is_null()) row.Name_Lang_frFR = r["name_lang_frfr"].as<std::string>();
        if (!r["name_lang_dede"].is_null()) row.Name_Lang_deDE = r["name_lang_dede"].as<std::string>();
        if (!r["name_lang_encn"].is_null()) row.Name_Lang_enCN = r["name_lang_encn"].as<std::string>();
        if (!r["name_lang_zhcn"].is_null()) row.Name_Lang_zhCN = r["name_lang_zhcn"].as<std::string>();
        if (!r["name_lang_entw"].is_null()) row.Name_Lang_enTW = r["name_lang_entw"].as<std::string>();
        if (!r["name_lang_zhtw"].is_null()) row.Name_Lang_zhTW = r["name_lang_zhtw"].as<std::string>();
        if (!r["name_lang_eses"].is_null()) row.Name_Lang_esES = r["name_lang_eses"].as<std::string>();
        if (!r["name_lang_esmx"].is_null()) row.Name_Lang_esMX = r["name_lang_esmx"].as<std::string>();
        if (!r["name_lang_ruru"].is_null()) row.Name_Lang_ruRU = r["name_lang_ruru"].as<std::string>();
        if (!r["name_lang_ptpt"].is_null()) row.Name_Lang_ptPT = r["name_lang_ptpt"].as<std::string>();
        if (!r["name_lang_ptbr"].is_null()) row.Name_Lang_ptBR = r["name_lang_ptbr"].as<std::string>();
        if (!r["name_lang_itit"].is_null()) row.Name_Lang_itIT = r["name_lang_itit"].as<std::string>();
        if (!r["name_lang_unk"].is_null()) row.Name_Lang_Unk = r["name_lang_unk"].as<std::string>();

        row.Name_Lang_Mask = r["name_lang_mask"].as<uint32_t>();

        // Female name fields
        if (!r["name_female_lang_enus"].is_null()) row.Name_Female_Lang_enUS = r["name_female_lang_enus"].as<std::string>();
        if (!r["name_female_lang_engb"].is_null()) row.Name_Female_Lang_enGB = r["name_female_lang_engb"].as<std::string>();
        if (!r["name_female_lang_kokr"].is_null()) row.Name_Female_Lang_koKR = r["name_female_lang_kokr"].as<std::string>();
        if (!r["name_female_lang_frfr"].is_null()) row.Name_Female_Lang_frFR = r["name_female_lang_frfr"].as<std::string>();
        if (!r["name_female_lang_dede"].is_null()) row.Name_Female_Lang_deDE = r["name_female_lang_dede"].as<std::string>();
        if (!r["name_female_lang_encn"].is_null()) row.Name_Female_Lang_enCN = r["name_female_lang_encn"].as<std::string>();
        if (!r["name_female_lang_zhcn"].is_null()) row.Name_Female_Lang_zhCN = r["name_female_lang_zhcn"].as<std::string>();
        if (!r["name_female_lang_entw"].is_null()) row.Name_Female_Lang_enTW = r["name_female_lang_entw"].as<std::string>();
        if (!r["name_female_lang_zhtw"].is_null()) row.Name_Female_Lang_zhTW = r["name_female_lang_zhtw"].as<std::string>();
        if (!r["name_female_lang_eses"].is_null()) row.Name_Female_Lang_esES = r["name_female_lang_eses"].as<std::string>();
        if (!r["name_female_lang_esmx"].is_null()) row.Name_Female_Lang_esMX = r["name_female_lang_esmx"].as<std::string>();
        if (!r["name_female_lang_ruru"].is_null()) row.Name_Female_Lang_ruRU = r["name_female_lang_ruru"].as<std::string>();
        if (!r["name_female_lang_ptpt"].is_null()) row.Name_Female_Lang_ptPT = r["name_female_lang_ptpt"].as<std::string>();
        if (!r["name_female_lang_ptbr"].is_null()) row.Name_Female_Lang_ptBR = r["name_female_lang_ptbr"].as<std::string>();
        if (!r["name_female_lang_itit"].is_null()) row.Name_Female_Lang_itIT = r["name_female_lang_itit"].as<std::string>();
        if (!r["name_female_lang_unk"].is_null()) row.Name_Female_Lang_Unk = r["name_female_lang_unk"].as<std::string>();

        row.Name_Female_Lang_Mask = r["name_female_lang_mask"].as<uint32_t>();

        // Male name fields
        if (!r["name_male_lang_enus"].is_null()) row.Name_Male_Lang_enUS = r["name_male_lang_enus"].as<std::string>();
        if (!r["name_male_lang_engb"].is_null()) row.Name_Male_Lang_enGB = r["name_male_lang_engb"].as<std::string>();
        if (!r["name_male_lang_kokr"].is_null()) row.Name_Male_Lang_koKR = r["name_male_lang_kokr"].as<std::string>();
        if (!r["name_male_lang_frfr"].is_null()) row.Name_Male_Lang_frFR = r["name_male_lang_frfr"].as<std::string>();
        if (!r["name_male_lang_dede"].is_null()) row.Name_Male_Lang_deDE = r["name_male_lang_dede"].as<std::string>();
        if (!r["name_male_lang_encn"].is_null()) row.Name_Male_Lang_enCN = r["name_male_lang_encn"].as<std::string>();
        if (!r["name_male_lang_zhcn"].is_null()) row.Name_Male_Lang_zhCN = r["name_male_lang_zhcn"].as<std::string>();
        if (!r["name_male_lang_entw"].is_null()) row.Name_Male_Lang_enTW = r["name_male_lang_entw"].as<std::string>();
        if (!r["name_male_lang_zhtw"].is_null()) row.Name_Male_Lang_zhTW = r["name_male_lang_zhtw"].as<std::string>();
        if (!r["name_male_lang_eses"].is_null()) row.Name_Male_Lang_esES = r["name_male_lang_eses"].as<std::string>();
        if (!r["name_male_lang_esmx"].is_null()) row.Name_Male_Lang_esMX = r["name_male_lang_esmx"].as<std::string>();
        if (!r["name_male_lang_ruru"].is_null()) row.Name_Male_Lang_ruRU = r["name_male_lang_ruru"].as<std::string>();
        if (!r["name_male_lang_ptpt"].is_null()) row.Name_Male_Lang_ptPT = r["name_male_lang_ptpt"].as<std::string>();
        if (!r["name_male_lang_ptbr"].is_null()) row.Name_Male_Lang_ptBR = r["name_male_lang_ptbr"].as<std::string>();
        if (!r["name_male_lang_itit"].is_null()) row.Name_Male_Lang_itIT = r["name_male_lang_itit"].as<std::string>();
        if (!r["name_male_lang_unk"].is_null()) row.Name_Male_Lang_Unk = r["name_male_lang_unk"].as<std::string>();

        row.Name_Male_Lang_Mask = r["name_male_lang_mask"].as<uint32_t>();

        // Customization fields
        if (!r["facialhaircustomization_1"].is_null()) row.FacialHairCustomization_1 = r["facialhaircustomization_1"].as<std::string>();
        if (!r["facialhaircustomization_2"].is_null()) row.FacialHairCustomization_2 = r["facialhaircustomization_2"].as<std::string>();
        if (!r["haircustomization"].is_null()) row.HairCustomization = r["haircustomization"].as<std::string>();

        row.Required_Expansion = r["required_expansion"].as<uint32_t>();

        return row;
    }
};