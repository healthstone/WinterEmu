#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_itemrandomproperties **/
struct DbcItemRandomProperties {
    uint32_t id;
    std::optional<std::string> name;
    uint32_t enchantment_1;
    uint32_t enchantment_2;
    uint32_t enchantment_3;
    uint32_t enchantment_4;
    uint32_t enchantment_5;

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
};

template<>
struct PgRowMapper<DbcItemRandomProperties> {
    static DbcItemRandomProperties map(const pqxx::row& r) {
        DbcItemRandomProperties obj;
        obj.id               = r["id"].as<uint32_t>();
        obj.name             = get_optional_string(r, "name");
        obj.enchantment_1    = r["enchantment_1"].as<uint32_t>();
        obj.enchantment_2    = r["enchantment_2"].as<uint32_t>();
        obj.enchantment_3    = r["enchantment_3"].as<uint32_t>();
        obj.enchantment_4    = r["enchantment_4"].as<uint32_t>();
        obj.enchantment_5    = r["enchantment_5"].as<uint32_t>();

        obj.name_lang_enus   = get_optional_string(r, "name_lang_enus");
        obj.name_lang_engb   = get_optional_string(r, "name_lang_engb");
        obj.name_lang_kokr   = get_optional_string(r, "name_lang_kokr");
        obj.name_lang_frfr   = get_optional_string(r, "name_lang_frfr");
        obj.name_lang_dede   = get_optional_string(r, "name_lang_dede");
        obj.name_lang_encn   = get_optional_string(r, "name_lang_encn");
        obj.name_lang_zhcn   = get_optional_string(r, "name_lang_zhcn");
        obj.name_lang_entw   = get_optional_string(r, "name_lang_entw");
        obj.name_lang_zhtw   = get_optional_string(r, "name_lang_zhtw");
        obj.name_lang_eses   = get_optional_string(r, "name_lang_eses");
        obj.name_lang_esmx   = get_optional_string(r, "name_lang_esmx");
        obj.name_lang_ruru   = get_optional_string(r, "name_lang_ruru");
        obj.name_lang_ptpt   = get_optional_string(r, "name_lang_ptpt");
        obj.name_lang_ptbr   = get_optional_string(r, "name_lang_ptbr");
        obj.name_lang_itit   = get_optional_string(r, "name_lang_itit");
        obj.name_lang_unk    = get_optional_string(r, "name_lang_unk");

        obj.name_lang_mask   = r["name_lang_mask"].as<uint32_t>();
        return obj;
    }
};
