#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_creaturefamily **/
struct DbcCreatureFamily {
    uint32_t ID;
    float MinScale;
    uint32_t MinScaleLevel;
    float MaxScale;
    uint32_t MaxScaleLevel;
    uint32_t SkillLine_1;
    uint32_t SkillLine_2;
    uint32_t PetFoodMask;
    int32_t PetTalentType;
    int32_t CategoryEnumID;

    // локализованные названия
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
    std::optional<std::string> IconFile;
};

template<>
struct PgRowMapper<DbcCreatureFamily> {
    static DbcCreatureFamily map(const pqxx::row& r) {
        DbcCreatureFamily row;

        row.ID             = r["id"].as<uint32_t>();
        row.MinScale       = r["minscale"].as<float>();
        row.MinScaleLevel  = r["minscalelevel"].as<uint32_t>();
        row.MaxScale       = r["maxscale"].as<float>();
        row.MaxScaleLevel  = r["maxscalelevel"].as<uint32_t>();
        row.SkillLine_1    = r["skillline_1"].as<uint32_t>();
        row.SkillLine_2    = r["skillline_2"].as<uint32_t>();
        row.PetFoodMask    = r["petfoodmask"].as<uint32_t>();
        row.PetTalentType  = r["pettalenttype"].as<int32_t>();
        row.CategoryEnumID = r["categoryenumid"].as<int32_t>();

        // локализованные строки
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
        row.Name_Lang_Unk  = get_optional_string(r, "name_lang_unk");

        row.Name_Lang_Mask = r["name_lang_mask"].as<uint32_t>();
        row.IconFile       = get_optional_string(r, "iconfile");

        return row;
    }
};
