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

/** dbc_chrclasses **/
struct DbcChrClasses {
    uint32_t ID;
    uint32_t DamageBonusStat;
    uint32_t DisplayPower;
    uint32_t PetNameToken;
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
    std::optional<std::string> Filename;
    uint32_t SpellClassSet;
    uint32_t Flags;
    uint32_t CinematicSequenceID;
    uint32_t Required_Expansion;
};

template<>
struct PgRowMapper<DbcChrClasses> {
    static DbcChrClasses map(const pqxx::row& r) {
        DbcChrClasses row;

        // Обязательные числовые поля
        row.ID               = r["id"].as<uint32_t>();
        row.DamageBonusStat  = r["field01"].as<uint32_t>();
        row.DisplayPower     = r["displaypower"].as<uint32_t>();
        row.PetNameToken     = r["petnametoken"].as<uint32_t>();
        row.Name_Lang_Mask   = r["name_lang_mask"].as<uint32_t>();
        row.Name_Female_Lang_Mask = r["name_female_lang_mask"].as<uint32_t>();
        row.Name_Male_Lang_Mask   = r["name_male_lang_mask"].as<uint32_t>();
        row.SpellClassSet    = r["spellclassset"].as<uint32_t>();
        row.Flags            = r["flags"].as<uint32_t>();
        row.CinematicSequenceID = r["cinematicsequenceid"].as<uint32_t>();
        row.Required_Expansion  = r["required_expansion"].as<uint32_t>();

        // Опциональные строковые поля
        row.Name_Lang_enUS   = get_optional_string(r, "name_lang_enus");
        row.Name_Lang_enGB   = get_optional_string(r, "name_lang_engb");
        row.Name_Lang_koKR   = get_optional_string(r, "name_lang_kokr");
        row.Name_Lang_frFR   = get_optional_string(r, "name_lang_frfr");
        row.Name_Lang_deDE   = get_optional_string(r, "name_lang_dede");
        row.Name_Lang_enCN   = get_optional_string(r, "name_lang_encn");
        row.Name_Lang_zhCN   = get_optional_string(r, "name_lang_zhcn");
        row.Name_Lang_enTW   = get_optional_string(r, "name_lang_entw");
        row.Name_Lang_zhTW   = get_optional_string(r, "name_lang_zhtw");
        row.Name_Lang_esES   = get_optional_string(r, "name_lang_eses");
        row.Name_Lang_esMX   = get_optional_string(r, "name_lang_esmx");
        row.Name_Lang_ruRU   = get_optional_string(r, "name_lang_ruru");
        row.Name_Lang_ptPT   = get_optional_string(r, "name_lang_ptpt");
        row.Name_Lang_ptBR   = get_optional_string(r, "name_lang_ptbr");
        row.Name_Lang_itIT   = get_optional_string(r, "name_lang_itit");
        row.Name_Lang_Unk    = get_optional_string(r, "name_lang_unk");

        row.Name_Female_Lang_enUS = get_optional_string(r, "name_female_lang_enus");
        row.Name_Female_Lang_enGB = get_optional_string(r, "name_female_lang_engb");
        row.Name_Female_Lang_koKR = get_optional_string(r, "name_female_lang_kokr");
        row.Name_Female_Lang_frFR = get_optional_string(r, "name_female_lang_frfr");
        row.Name_Female_Lang_deDE = get_optional_string(r, "name_female_lang_dede");
        row.Name_Female_Lang_enCN = get_optional_string(r, "name_female_lang_encn");
        row.Name_Female_Lang_zhCN = get_optional_string(r, "name_female_lang_zhcn");
        row.Name_Female_Lang_enTW = get_optional_string(r, "name_female_lang_entw");
        row.Name_Female_Lang_zhTW = get_optional_string(r, "name_female_lang_zhTW");
        row.Name_Female_Lang_esES = get_optional_string(r, "name_female_lang_eses");
        row.Name_Female_Lang_esMX = get_optional_string(r, "name_female_lang_esmx");
        row.Name_Female_Lang_ruRU = get_optional_string(r, "name_female_lang_ruru");
        row.Name_Female_Lang_ptPT = get_optional_string(r, "name_female_lang_ptpt");
        row.Name_Female_Lang_ptBR = get_optional_string(r, "name_female_lang_ptbr");
        row.Name_Female_Lang_itIT = get_optional_string(r, "name_female_lang_itit");
        row.Name_Female_Lang_Unk  = get_optional_string(r, "name_female_lang_unk");

        row.Name_Male_Lang_enUS = get_optional_string(r, "name_male_lang_enus");
        row.Name_Male_Lang_enGB = get_optional_string(r, "name_male_lang_engb");
        row.Name_Male_Lang_koKR = get_optional_string(r, "name_male_lang_kokr");
        row.Name_Male_Lang_frFR = get_optional_string(r, "name_male_lang_frfr");
        row.Name_Male_Lang_deDE = get_optional_string(r, "name_male_lang_dede");
        row.Name_Male_Lang_enCN = get_optional_string(r, "name_male_lang_encn");
        row.Name_Male_Lang_zhCN = get_optional_string(r, "name_male_lang_zhcn");
        row.Name_Male_Lang_enTW = get_optional_string(r, "name_male_lang_entw");
        row.Name_Male_Lang_zhTW = get_optional_string(r, "name_male_lang_zhTW");
        row.Name_Male_Lang_esES = get_optional_string(r, "name_male_lang_eses");
        row.Name_Male_Lang_esMX = get_optional_string(r, "name_male_lang_esmx");
        row.Name_Male_Lang_ruRU = get_optional_string(r, "name_male_lang_ruru");
        row.Name_Male_Lang_ptPT = get_optional_string(r, "name_male_lang_ptpt");
        row.Name_Male_Lang_ptBR = get_optional_string(r, "name_male_lang_ptbr");
        row.Name_Male_Lang_itIT = get_optional_string(r, "name_male_lang_itit");
        row.Name_Male_Lang_Unk  = get_optional_string(r, "name_male_lang_unk");

        // Опциональная строка Filename
        row.Filename = get_optional_string(r, "filename");

        return row;
    }
};
