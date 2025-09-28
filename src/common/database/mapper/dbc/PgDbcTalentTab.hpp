#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcTalentTab {
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
    uint32_t spell_icon_id;
    int32_t race_mask;
    uint32_t class_mask;
    uint32_t pet_talent_mask;
    uint32_t order_index;
    std::optional<std::string> background_file;
};

template<>
struct PgRowMapper<DbcTalentTab> {
    static DbcTalentTab map(const pqxx::row& r) {
        DbcTalentTab dbc;
        dbc.id = r["id"].as<uint32_t>();
        dbc.name_lang_enus = get_optional_string(r, "name_lang_enus");
        dbc.name_lang_engb = get_optional_string(r, "name_lang_engb");
        dbc.name_lang_kokr = get_optional_string(r, "name_lang_kokr");
        dbc.name_lang_frfr = get_optional_string(r, "name_lang_frfr");
        dbc.name_lang_dede = get_optional_string(r, "name_lang_dede");
        dbc.name_lang_encn = get_optional_string(r, "name_lang_encn");
        dbc.name_lang_zhcn = get_optional_string(r, "name_lang_zhcn");
        dbc.name_lang_entw = get_optional_string(r, "name_lang_entw");
        dbc.name_lang_zhtw = get_optional_string(r, "name_lang_zhtw");
        dbc.name_lang_eses = get_optional_string(r, "name_lang_eses");
        dbc.name_lang_esmx = get_optional_string(r, "name_lang_esmx");
        dbc.name_lang_ruru = get_optional_string(r, "name_lang_ruru");
        dbc.name_lang_ptpt = get_optional_string(r, "name_lang_ptpt");
        dbc.name_lang_ptbr = get_optional_string(r, "name_lang_ptbr");
        dbc.name_lang_itit = get_optional_string(r, "name_lang_itit");
        dbc.name_lang_unk = get_optional_string(r, "name_lang_unk");

        dbc.name_lang_mask = r["name_lang_mask"].as<uint32_t>();
        dbc.spell_icon_id = r["spelliconid"].as<uint32_t>();
        dbc.race_mask = r["racemask"].as<int32_t>();
        dbc.class_mask = r["classmask"].as<uint32_t>();
        dbc.pet_talent_mask = r["pettalentmask"].as<uint32_t>();
        dbc.order_index = r["orderindex"].as<uint32_t>();
        dbc.background_file = get_optional_string(r, "backgroundfile");
        return dbc;
    }
};