#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_itemrandomsuffix **/
struct DbcItemRandomSuffix {
    uint32_t id;
    std::optional<std::string> name_lang_enus;
    std::optional<std::string> name_lang_engb;
    std::optional<std::string> name_lang_kokr;
    std::optional<std::string> name_lang_frfr;
    std::optional<std::string> name_lang_dede;
    std::optional<std::string> name_lang_encn;
    std::optional<std::string> name_lang_zhcn;
    std::optional<std::string> name_lang_entw;
    std::optional<std::string> name_lang_zhtw;
    std::optional<std::string> name_lang_eses;
    std::optional<std::string> name_lang_esmx;
    std::optional<std::string> name_lang_ruru;
    std::optional<std::string> name_lang_ptpt;
    std::optional<std::string> name_lang_ptbr;
    std::optional<std::string> name_lang_itit;
    std::optional<std::string> name_lang_unk;
    uint32_t name_lang_mask;
    std::optional<std::string> internalname;
    uint32_t enchantment_1;
    uint32_t enchantment_2;
    uint32_t enchantment_3;
    uint32_t enchantment_4;
    uint32_t enchantment_5;
    uint32_t allocationpct_1;
    uint32_t allocationpct_2;
    uint32_t allocationpct_3;
    uint32_t allocationpct_4;
    uint32_t allocationpct_5;
};

template<>
struct PgRowMapper<DbcItemRandomSuffix> {
    static DbcItemRandomSuffix map(const pqxx::row& r) {
        DbcItemRandomSuffix row{
                r["id"].as<uint32_t>(),
                get_optional_string(r, "name_lang_enus"),
                get_optional_string(r, "name_lang_engb"),
                get_optional_string(r, "name_lang_kokr"),
                get_optional_string(r, "name_lang_frfr"),
                get_optional_string(r, "name_lang_dede"),
                get_optional_string(r, "name_lang_encn"),
                get_optional_string(r, "name_lang_zhcn"),
                get_optional_string(r, "name_lang_entw"),
                get_optional_string(r, "name_lang_zhtw"),
                get_optional_string(r, "name_lang_eses"),
                get_optional_string(r, "name_lang_esmx"),
                get_optional_string(r, "name_lang_ruru"),
                get_optional_string(r, "name_lang_ptpt"),
                get_optional_string(r, "name_lang_ptbr"),
                get_optional_string(r, "name_lang_itit"),
                get_optional_string(r, "name_lang_unk"),
                r["name_lang_mask"].as<uint32_t>(),
                get_optional_string(r, "internalname"),
                r["enchantment_1"].as<uint32_t>(),
                r["enchantment_2"].as<uint32_t>(),
                r["enchantment_3"].as<uint32_t>(),
                r["enchantment_4"].as<uint32_t>(),
                r["enchantment_5"].as<uint32_t>(),
                r["allocationpct_1"].as<uint32_t>(),
                r["allocationpct_2"].as<uint32_t>(),
                r["allocationpct_3"].as<uint32_t>(),
                r["allocationpct_4"].as<uint32_t>(),
                r["allocationpct_5"].as<uint32_t>()
        };
        return row;
    }
};