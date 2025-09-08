#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

// === Структура, соответствующая таблице dbc.dbc_itemlimitcategory ===
struct DbcItemLimitCategory {
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
    uint32_t quantity;
    uint32_t flags;
};

// === PgRowMapper specialization ===
template<>
struct PgRowMapper<DbcItemLimitCategory> {
    static DbcItemLimitCategory map(const pqxx::row &r) {
        DbcItemLimitCategory o;

        o.id = r["id"].as<uint32_t>();

        // Строковые поля — используем get_optional_string
        o.name_lang_enus = get_optional_string(r, "name_lang_enus");
        o.name_lang_engb = get_optional_string(r, "name_lang_engb");
        o.name_lang_kokr = get_optional_string(r, "name_lang_kokr");
        o.name_lang_frfr = get_optional_string(r, "name_lang_frfr");
        o.name_lang_dede = get_optional_string(r, "name_lang_dede");
        o.name_lang_encn = get_optional_string(r, "name_lang_encn");
        o.name_lang_zhcn = get_optional_string(r, "name_lang_zhcn");
        o.name_lang_entw = get_optional_string(r, "name_lang_entw");
        o.name_lang_zhtw = get_optional_string(r, "name_lang_zhtw");
        o.name_lang_eses = get_optional_string(r, "name_lang_eses");
        o.name_lang_esmx = get_optional_string(r, "name_lang_esmx");
        o.name_lang_ruru = get_optional_string(r, "name_lang_ruru");
        o.name_lang_ptpt = get_optional_string(r, "name_lang_ptpt");
        o.name_lang_ptbr = get_optional_string(r, "name_lang_ptbr");
        o.name_lang_itit = get_optional_string(r, "name_lang_itit");
        o.name_lang_unk  = get_optional_string(r, "name_lang_unk");

        // Числовые поля (NOT NULL) — читаем напрямую
        o.name_lang_mask = r["name_lang_mask"].as<uint32_t>();
        o.quantity       = r["quantity"].as<uint32_t>();
        o.flags          = r["flags"].as<uint32_t>();

        return o;
    }
};
