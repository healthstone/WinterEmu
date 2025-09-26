#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

struct DbcSpellFocusObject {
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
    uint64_t name_lang_mask;
};

template<>
struct PgRowMapper<DbcSpellFocusObject> {
    static DbcSpellFocusObject map(const pqxx::row& r) {
        DbcSpellFocusObject row{};
        row.id              = r["id"].as<uint32_t>();
        row.name_lang_enus  = get_optional_string(r, "name_lang_enus");
        row.name_lang_engb  = get_optional_string(r, "name_lang_engb");
        row.name_lang_kokr  = get_optional_string(r, "name_lang_kokr");
        row.name_lang_frfr  = get_optional_string(r, "name_lang_frfr");
        row.name_lang_dede  = get_optional_string(r, "name_lang_dede");
        row.name_lang_encn  = get_optional_string(r, "name_lang_encn");
        row.name_lang_zhcn  = get_optional_string(r, "name_lang_zhcn");
        row.name_lang_entw  = get_optional_string(r, "name_lang_entw");
        row.name_lang_zhtw  = get_optional_string(r, "name_lang_zhtw");
        row.name_lang_eses  = get_optional_string(r, "name_lang_eses");
        row.name_lang_esmx  = get_optional_string(r, "name_lang_esmx");
        row.name_lang_ruru  = get_optional_string(r, "name_lang_ruru");
        row.name_lang_ptpt  = get_optional_string(r, "name_lang_ptpt");
        row.name_lang_ptbr  = get_optional_string(r, "name_lang_ptbr");
        row.name_lang_itit  = get_optional_string(r, "name_lang_itit");
        row.name_lang_unk   = get_optional_string(r, "name_lang_unk");
        row.name_lang_mask  = r["name_lang_mask"].as<uint64_t>();
        return row;
    }
};
