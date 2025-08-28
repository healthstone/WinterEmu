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

        // Числовые поля
        row.ID = r["id"].as<uint32_t>();
        row.Flags = r["flags"].as<uint32_t>();
        row.FactionID = r["factionid"].as<uint32_t>();
        row.ExplorationSoundID = r["explorationsoundid"].as<uint32_t>();
        row.MaleDisplayId = r["maledisplayid"].as<uint32_t>();
        row.FemaleDisplayId = r["femaledisplayid"].as<uint32_t>();
        row.BaseLanguage = r["baselanguage"].as<uint32_t>();
        row.CreatureType = r["creaturetype"].as<uint32_t>();
        row.ResSicknessSpellID = r["ressicknessspellid"].as<uint32_t>();
        row.SplashSoundID = r["splashsoundid"].as<uint32_t>();
        row.CinematicSequenceID = r["cinematicsequenceid"].as<uint32_t>();
        row.Alliance = r["alliance"].as<uint32_t>();
        row.Name_Lang_Mask = r["name_lang_mask"].as<uint32_t>();
        row.Name_Female_Lang_Mask = r["name_female_lang_mask"].as<uint32_t>();
        row.Name_Male_Lang_Mask = r["name_male_lang_mask"].as<uint32_t>();
        row.Required_Expansion = r["required_expansion"].as<uint32_t>();

        // Optional строковые поля через QueryResults.hpp
        row.ClientPrefix = get_optional_string(r, "clientprefix");
        row.ClientFilestring = get_optional_string(r, "clientfilestring");

        // Name fields
        row.Name_Lang_enUS = get_optional_string(r, "name_lang_enus");
        row.Name_Lang_enGB = get_optional_string(r, "name_lang_engb");
        row.Name_Lang_koKR = get_optional_string(r, "name_lang_kokr");
        row.Name_Lang_frFR = get_optional_string(r, "name_lang_frfr");
        row.Name_Lang_deDE = get_optional_string(r, "name_lang_dede");
        row.Name_Lang_enCN = get_optional_string(r, "name_lang_encn");
        row.Name_Lang_zhCN = get_optional_string(r, "name_lang_zhcn");
        row.Name_Lang_enTW = get_optional_string(r, "name_lang_entw");
        row.Name_Lang_zhTW = get_optional_string(r, "name_lang_zhtw");
        row.Name_Lang_esES = get_optional_string(r, "name_lang_eses");
        row.Name_Lang_esMX = get_optional_string(r, "name_lang_esmx");
        row.Name_Lang_ruRU = get_optional_string(r, "name_lang_ruru");
        row.Name_Lang_ptPT = get_optional_string(r, "name_lang_ptpt");
        row.Name_Lang_ptBR = get_optional_string(r, "name_lang_ptbr");
        row.Name_Lang_itIT = get_optional_string(r, "name_lang_itit");
        row.Name_Lang_Unk = get_optional_string(r, "name_lang_unk");

        // Female names
        row.Name_Female_Lang_enUS = get_optional_string(r, "name_female_lang_enus");
        row.Name_Female_Lang_enGB = get_optional_string(r, "name_female_lang_engb");
        row.Name_Female_Lang_koKR = get_optional_string(r, "name_female_lang_kokr");
        row.Name_Female_Lang_frFR = get_optional_string(r, "name_female_lang_frfr");
        row.Name_Female_Lang_deDE = get_optional_string(r, "name_female_lang_dede");
        row.Name_Female_Lang_enCN = get_optional_string(r, "name_female_lang_encn");
        row.Name_Female_Lang_zhCN = get_optional_string(r, "name_female_lang_zhcn");
        row.Name_Female_Lang_enTW = get_optional_string(r, "name_female_lang_entw");
        row.Name_Female_Lang_zhTW = get_optional_string(r, "name_female_lang_zhtw");
        row.Name_Female_Lang_esES = get_optional_string(r, "name_female_lang_eses");
        row.Name_Female_Lang_esMX = get_optional_string(r, "name_female_lang_esmx");
        row.Name_Female_Lang_ruRU = get_optional_string(r, "name_female_lang_ruru");
        row.Name_Female_Lang_ptPT = get_optional_string(r, "name_female_lang_ptpt");
        row.Name_Female_Lang_ptBR = get_optional_string(r, "name_female_lang_ptbr");
        row.Name_Female_Lang_itIT = get_optional_string(r, "name_female_lang_itit");
        row.Name_Female_Lang_Unk = get_optional_string(r, "name_female_lang_unk");

        // Male names
        row.Name_Male_Lang_enUS = get_optional_string(r, "name_male_lang_enus");
        row.Name_Male_Lang_enGB = get_optional_string(r, "name_male_lang_engb");
        row.Name_Male_Lang_koKR = get_optional_string(r, "name_male_lang_kokr");
        row.Name_Male_Lang_frFR = get_optional_string(r, "name_male_lang_frfr");
        row.Name_Male_Lang_deDE = get_optional_string(r, "name_male_lang_dede");
        row.Name_Male_Lang_enCN = get_optional_string(r, "name_male_lang_encn");
        row.Name_Male_Lang_zhCN = get_optional_string(r, "name_male_lang_zhcn");
        row.Name_Male_Lang_enTW = get_optional_string(r, "name_male_lang_entw");
        row.Name_Male_Lang_zhTW = get_optional_string(r, "name_male_lang_zhtw");
        row.Name_Male_Lang_esES = get_optional_string(r, "name_male_lang_eses");
        row.Name_Male_Lang_esMX = get_optional_string(r, "name_male_lang_esmx");
        row.Name_Male_Lang_ruRU = get_optional_string(r, "name_male_lang_ruru");
        row.Name_Male_Lang_ptPT = get_optional_string(r, "name_male_lang_ptpt");
        row.Name_Male_Lang_ptBR = get_optional_string(r, "name_male_lang_ptbr");
        row.Name_Male_Lang_itIT = get_optional_string(r, "name_male_lang_itit");
        row.Name_Male_Lang_Unk = get_optional_string(r, "name_male_lang_unk");

        // Customization fields
        row.FacialHairCustomization_1 = get_optional_string(r, "facialhaircustomization_1");
        row.FacialHairCustomization_2 = get_optional_string(r, "facialhaircustomization_2");
        row.HairCustomization = get_optional_string(r, "haircustomization");

        return row;
    }
};