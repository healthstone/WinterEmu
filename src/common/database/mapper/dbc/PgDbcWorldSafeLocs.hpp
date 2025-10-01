#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcWorldSafeLocs {
    uint32_t id;
    int32_t continent;
    float loc_x;
    float loc_y;
    float loc_z;

    std::optional<std::string> area_name_lang_enus;
    std::optional<std::string> area_name_lang_engb;
    std::optional<std::string> area_name_lang_kokr;
    std::optional<std::string> area_name_lang_frfr;
    std::optional<std::string> area_name_lang_dede;
    std::optional<std::string> area_name_lang_encn;
    std::optional<std::string> area_name_lang_zhcn;
    std::optional<std::string> area_name_lang_entw;
    std::optional<std::string> area_name_lang_zhtw;
    std::optional<std::string> area_name_lang_eses;
    std::optional<std::string> area_name_lang_esmx;
    std::optional<std::string> area_name_lang_ruru;
    std::optional<std::string> area_name_lang_ptpt;
    std::optional<std::string> area_name_lang_ptbr;
    std::optional<std::string> area_name_lang_itit;
    std::optional<std::string> area_name_lang_unk;

    uint32_t area_name_lang_mask;
};

template<>
struct PgRowMapper<DbcWorldSafeLocs> {
    static DbcWorldSafeLocs map(const pqxx::row& r) {
        DbcWorldSafeLocs obj;
        obj.id = r["id"].as<uint32_t>();
        obj.continent = r["continent"].as<int32_t>();
        obj.loc_x = r["loc_x"].as<float>();
        obj.loc_y = r["loc_y"].as<float>();
        obj.loc_z = r["loc_z"].as<float>();

        obj.area_name_lang_enus = get_optional_string(r, "area_name_lang_enus");
        obj.area_name_lang_engb = get_optional_string(r, "area_name_lang_engb");
        obj.area_name_lang_kokr = get_optional_string(r, "area_name_lang_kokr");
        obj.area_name_lang_frfr = get_optional_string(r, "area_name_lang_frfr");
        obj.area_name_lang_dede = get_optional_string(r, "area_name_lang_dede");
        obj.area_name_lang_encn = get_optional_string(r, "area_name_lang_encn");
        obj.area_name_lang_zhcn = get_optional_string(r, "area_name_lang_zhcn");
        obj.area_name_lang_entw = get_optional_string(r, "area_name_lang_entw");
        obj.area_name_lang_zhtw = get_optional_string(r, "area_name_lang_zhtw");
        obj.area_name_lang_eses = get_optional_string(r, "area_name_lang_eses");
        obj.area_name_lang_esmx = get_optional_string(r, "area_name_lang_esmx");
        obj.area_name_lang_ruru = get_optional_string(r, "area_name_lang_ruru");
        obj.area_name_lang_ptpt = get_optional_string(r, "area_name_lang_ptpt");
        obj.area_name_lang_ptbr = get_optional_string(r, "area_name_lang_ptbr");
        obj.area_name_lang_itit = get_optional_string(r, "area_name_lang_itit");
        obj.area_name_lang_unk = get_optional_string(r, "area_name_lang_unk");

        obj.area_name_lang_mask = r["area_name_lang_mask"].as<uint32_t>();
        return obj;
    }
};