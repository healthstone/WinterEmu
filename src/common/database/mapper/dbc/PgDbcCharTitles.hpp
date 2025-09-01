#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include <array>
#include <vector>

#include "QueryResults.hpp"

// === Структура для dbc_chartitles ===
struct DbcCharTitles {
    uint32_t ID;
    uint32_t ConditionID;

    // Name_Lang
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

    // Name1_Lang
    std::optional<std::string> Name1_Lang_enUS;
    std::optional<std::string> Name1_Lang_enGB;
    std::optional<std::string> Name1_Lang_koKR;
    std::optional<std::string> Name1_Lang_frFR;
    std::optional<std::string> Name1_Lang_deDE;
    std::optional<std::string> Name1_Lang_enCN;
    std::optional<std::string> Name1_Lang_zhCN;
    std::optional<std::string> Name1_Lang_enTW;
    std::optional<std::string> Name1_Lang_zhTW;
    std::optional<std::string> Name1_Lang_esES;
    std::optional<std::string> Name1_Lang_esMX;
    std::optional<std::string> Name1_Lang_ruRU;
    std::optional<std::string> Name1_Lang_ptPT;
    std::optional<std::string> Name1_Lang_ptBR;
    std::optional<std::string> Name1_Lang_itIT;
    std::optional<std::string> Name1_Lang_Unk;
    uint32_t Name1_Lang_Mask;

    uint32_t Mask_ID;
};

// === Маппер ===
template<>
struct PgRowMapper<DbcCharTitles> {
    static DbcCharTitles map(const pqxx::row& r) {
        DbcCharTitles row{};

        row.ID           = r["id"].as<uint32_t>();
        row.ConditionID  = r["condition_id"].as<uint32_t>();

        // Name_Lang
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

        // Name1_Lang
        row.Name1_Lang_enUS = get_optional_string(r, "name1_lang_enus");
        row.Name1_Lang_enGB = get_optional_string(r, "name1_lang_engb");
        row.Name1_Lang_koKR = get_optional_string(r, "name1_lang_kokr");
        row.Name1_Lang_frFR = get_optional_string(r, "name1_lang_frfr");
        row.Name1_Lang_deDE = get_optional_string(r, "name1_lang_dede");
        row.Name1_Lang_enCN = get_optional_string(r, "name1_lang_encn");
        row.Name1_Lang_zhCN = get_optional_string(r, "name1_lang_zhcn");
        row.Name1_Lang_enTW = get_optional_string(r, "name1_lang_entw");
        row.Name1_Lang_zhTW = get_optional_string(r, "name1_lang_zhtw");
        row.Name1_Lang_esES = get_optional_string(r, "name1_lang_eses");
        row.Name1_Lang_esMX = get_optional_string(r, "name1_lang_esmx");
        row.Name1_Lang_ruRU = get_optional_string(r, "name1_lang_ruru");
        row.Name1_Lang_ptPT = get_optional_string(r, "name1_lang_ptpt");
        row.Name1_Lang_ptBR = get_optional_string(r, "name1_lang_ptbr");
        row.Name1_Lang_itIT = get_optional_string(r, "name1_lang_itit");
        row.Name1_Lang_Unk  = get_optional_string(r, "name1_lang_unk");
        row.Name1_Lang_Mask = r["name1_lang_mask"].as<uint32_t>();

        row.Mask_ID = r["mask_id"].as<uint32_t>();

        return row;
    }
};
