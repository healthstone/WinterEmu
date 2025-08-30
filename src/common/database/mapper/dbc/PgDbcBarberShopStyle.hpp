#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_barbershopstyle **/
struct DbcBarberShopStyle {
    uint32_t ID;
    uint32_t Type;

    std::optional<std::string> DisplayName_Lang_enUS;
    std::optional<std::string> DisplayName_Lang_enGB;
    std::optional<std::string> DisplayName_Lang_koKR;
    std::optional<std::string> DisplayName_Lang_frFR;
    std::optional<std::string> DisplayName_Lang_deDE;
    std::optional<std::string> DisplayName_Lang_enCN;
    std::optional<std::string> DisplayName_Lang_zhCN;
    std::optional<std::string> DisplayName_Lang_enTW;
    std::optional<std::string> DisplayName_Lang_zhTW;
    std::optional<std::string> DisplayName_Lang_esES;
    std::optional<std::string> DisplayName_Lang_esMX;
    std::optional<std::string> DisplayName_Lang_ruRU;
    std::optional<std::string> DisplayName_Lang_ptPT;
    std::optional<std::string> DisplayName_Lang_ptBR;
    std::optional<std::string> DisplayName_Lang_itIT;
    std::optional<std::string> DisplayName_Lang_Unk;
    uint32_t DisplayName_Lang_Mask;

    std::optional<std::string> Description_Lang_enUS;
    std::optional<std::string> Description_Lang_enGB;
    std::optional<std::string> Description_Lang_koKR;
    std::optional<std::string> Description_Lang_frFR;
    std::optional<std::string> Description_Lang_deDE;
    std::optional<std::string> Description_Lang_enCN;
    std::optional<std::string> Description_Lang_zhCN;
    std::optional<std::string> Description_Lang_enTW;
    std::optional<std::string> Description_Lang_zhTW;
    std::optional<std::string> Description_Lang_esES;
    std::optional<std::string> Description_Lang_esMX;
    std::optional<std::string> Description_Lang_ruRU;
    std::optional<std::string> Description_Lang_ptPT;
    std::optional<std::string> Description_Lang_ptBR;
    std::optional<std::string> Description_Lang_itIT;
    std::optional<std::string> Description_Lang_Unk;
    uint32_t Description_Lang_Mask;

    float Cost_Modifier;
    uint8_t Race;
    uint8_t Sex;
    uint32_t Data;
};

template<>
struct PgRowMapper<DbcBarberShopStyle> {
    static DbcBarberShopStyle map(const pqxx::row& r) {
        DbcBarberShopStyle row;

        row.ID   = r["id"].as<uint32_t>();
        row.Type = r["type"].as<uint32_t>();

        row.DisplayName_Lang_enUS = get_optional_string(r, "displayname_lang_enus");
        row.DisplayName_Lang_enGB = get_optional_string(r, "displayname_lang_engb");
        row.DisplayName_Lang_koKR = get_optional_string(r, "displayname_lang_kokr");
        row.DisplayName_Lang_frFR = get_optional_string(r, "displayname_lang_frfr");
        row.DisplayName_Lang_deDE = get_optional_string(r, "displayname_lang_dede");
        row.DisplayName_Lang_enCN = get_optional_string(r, "displayname_lang_encn");
        row.DisplayName_Lang_zhCN = get_optional_string(r, "displayname_lang_zhcn");
        row.DisplayName_Lang_enTW = get_optional_string(r, "displayname_lang_entw");
        row.DisplayName_Lang_zhTW = get_optional_string(r, "displayname_lang_zhtw");
        row.DisplayName_Lang_esES = get_optional_string(r, "displayname_lang_eses");
        row.DisplayName_Lang_esMX = get_optional_string(r, "displayname_lang_esmx");
        row.DisplayName_Lang_ruRU = get_optional_string(r, "displayname_lang_ruru");
        row.DisplayName_Lang_ptPT = get_optional_string(r, "displayname_lang_ptpt");
        row.DisplayName_Lang_ptBR = get_optional_string(r, "displayname_lang_ptbr");
        row.DisplayName_Lang_itIT = get_optional_string(r, "displayname_lang_itit");
        row.DisplayName_Lang_Unk  = get_optional_string(r, "displayname_lang_unk");
        row.DisplayName_Lang_Mask  = r["displayname_lang_mask"].as<uint32_t>();

        row.Description_Lang_enUS = get_optional_string(r, "description_lang_enus");
        row.Description_Lang_enGB = get_optional_string(r, "description_lang_engb");
        row.Description_Lang_koKR = get_optional_string(r, "description_lang_kokr");
        row.Description_Lang_frFR = get_optional_string(r, "description_lang_frfr");
        row.Description_Lang_deDE = get_optional_string(r, "description_lang_dede");
        row.Description_Lang_enCN = get_optional_string(r, "description_lang_encn");
        row.Description_Lang_zhCN = get_optional_string(r, "description_lang_zhcn");
        row.Description_Lang_enTW = get_optional_string(r, "description_lang_entw");
        row.Description_Lang_zhTW = get_optional_string(r, "description_lang_zhtw");
        row.Description_Lang_esES = get_optional_string(r, "description_lang_eses");
        row.Description_Lang_esMX = get_optional_string(r, "description_lang_esmx");
        row.Description_Lang_ruRU = get_optional_string(r, "description_lang_ruru");
        row.Description_Lang_ptPT = get_optional_string(r, "description_lang_ptpt");
        row.Description_Lang_ptBR = get_optional_string(r, "description_lang_ptbr");
        row.Description_Lang_itIT = get_optional_string(r, "description_lang_itit");
        row.Description_Lang_Unk  = get_optional_string(r, "description_lang_unk");
        row.Description_Lang_Mask  = r["description_lang_mask"].as<uint32_t>();

        row.Cost_Modifier = r["cost_modifier"].as<float>();
        row.Race          = static_cast<uint8_t>(r["race"].as<int>());
        row.Sex           = static_cast<uint8_t>(r["sex"].as<int>());
        row.Data          = r["data"].as<uint32_t>();

        return row;
    }
};
