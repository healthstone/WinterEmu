#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_currencycategory **/
struct DbcCurrencyCategory {
    uint32_t ID;
    uint32_t Flags;
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
};

template<>
struct PgRowMapper<DbcCurrencyCategory> {
    static DbcCurrencyCategory map(const pqxx::row& r) {
        DbcCurrencyCategory row;

        row.ID            = r["id"].as<uint32_t>();
        row.Flags         = r["flags"].as<uint32_t>();
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

        return row;
    }
};
