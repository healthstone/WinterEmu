#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_battlemasterlist **/
struct DbcBattleMasterList {
    uint32_t id;

    int32_t mapid_1;
    int32_t mapid_2;
    int32_t mapid_3;
    int32_t mapid_4;
    int32_t mapid_5;
    int32_t mapid_6;
    int32_t mapid_7;
    int32_t mapid_8;

    uint32_t instance_type;
    uint32_t groups_allowed;

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

    uint32_t max_group_size;
    uint32_t holiday_worldstate;
    uint32_t minlevel;
    uint32_t maxlevel;
};

template<>
struct PgRowMapper<DbcBattleMasterList> {
    static DbcBattleMasterList map(const pqxx::row& r) {
        DbcBattleMasterList row;

        row.id                  = r["id"].as<uint32_t>();
        row.mapid_1             = r["mapid_1"].as<int32_t>();
        row.mapid_2             = r["mapid_2"].as<int32_t>();
        row.mapid_3             = r["mapid_3"].as<int32_t>();
        row.mapid_4             = r["mapid_4"].as<int32_t>();
        row.mapid_5             = r["mapid_5"].as<int32_t>();
        row.mapid_6             = r["mapid_6"].as<int32_t>();
        row.mapid_7             = r["mapid_7"].as<int32_t>();
        row.mapid_8             = r["mapid_8"].as<int32_t>();

        row.instance_type       = r["instance_type"].as<uint32_t>();
        row.groups_allowed      = r["groups_allowed"].as<uint32_t>();

        row.name_lang_enus      = get_optional_string(r, "name_lang_enus");
        row.name_lang_engb      = get_optional_string(r, "name_lang_engb");
        row.name_lang_kokr      = get_optional_string(r, "name_lang_kokr");
        row.name_lang_frfr      = get_optional_string(r, "name_lang_frfr");
        row.name_lang_dede      = get_optional_string(r, "name_lang_dede");
        row.name_lang_encn      = get_optional_string(r, "name_lang_encn");
        row.name_lang_zhcn      = get_optional_string(r, "name_lang_zhcn");
        row.name_lang_entw      = get_optional_string(r, "name_lang_entw");
        row.name_lang_zhtw      = get_optional_string(r, "name_lang_zhtw");
        row.name_lang_eses      = get_optional_string(r, "name_lang_eses");
        row.name_lang_esmx      = get_optional_string(r, "name_lang_esmx");
        row.name_lang_ruru      = get_optional_string(r, "name_lang_ruru");
        row.name_lang_ptpt      = get_optional_string(r, "name_lang_ptpt");
        row.name_lang_ptbr      = get_optional_string(r, "name_lang_ptbr");
        row.name_lang_itit      = get_optional_string(r, "name_lang_itit");
        row.name_lang_unk       = get_optional_string(r, "name_lang_unk");
        row.name_lang_mask      = r["name_lang_mask"].as<uint32_t>();

        row.max_group_size      = r["max_group_size"].as<uint32_t>();
        row.holiday_worldstate  = r["holiday_worldstate"].as<uint32_t>();
        row.minlevel            = r["minlevel"].as<uint32_t>();
        row.maxlevel            = r["maxlevel"].as<uint32_t>();

        return row;
    }
};
