#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcWmoAreaTable {
    uint32_t id;
    int32_t wmo_id;
    int32_t name_set_id;
    int32_t wmo_group_id;
    int32_t sound_provider_pref;
    uint32_t sound_provider_pref_underwater;
    uint32_t ambience_id;
    uint32_t zone_music;
    int32_t intro_sound;
    uint32_t flags;
    uint32_t area_table_id;

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
struct PgRowMapper<DbcWmoAreaTable> {
    static DbcWmoAreaTable map(const pqxx::row& r) {
        DbcWmoAreaTable obj;
        obj.id = r["id"].as<uint32_t>();
        obj.wmo_id = r["wmoid"].as<int32_t>();
        obj.name_set_id = r["namesetid"].as<int32_t>();
        obj.wmo_group_id = r["wmogroupid"].as<int32_t>();
        obj.sound_provider_pref = r["soundproviderpref"].as<int32_t>();
        obj.sound_provider_pref_underwater = r["soundproviderprefunderwater"].as<uint32_t>();
        obj.ambience_id = r["ambienceid"].as<uint32_t>();
        obj.zone_music = r["zonemusic"].as<uint32_t>();
        obj.intro_sound = r["introsound"].as<int32_t>();
        obj.flags = r["flags"].as<uint32_t>();
        obj.area_table_id = r["areatableid"].as<uint32_t>();

        obj.area_name_lang_enus = get_optional_string(r, "areaname_lang_enus");
        obj.area_name_lang_engb = get_optional_string(r, "areaname_lang_engb");
        obj.area_name_lang_kokr = get_optional_string(r, "areaname_lang_kokr");
        obj.area_name_lang_frfr = get_optional_string(r, "areaname_lang_frfr");
        obj.area_name_lang_dede = get_optional_string(r, "areaname_lang_dede");
        obj.area_name_lang_encn = get_optional_string(r, "areaname_lang_encn");
        obj.area_name_lang_zhcn = get_optional_string(r, "areaname_lang_zhcn");
        obj.area_name_lang_entw = get_optional_string(r, "areaname_lang_entw");
        obj.area_name_lang_zhtw = get_optional_string(r, "areaname_lang_zhtw");
        obj.area_name_lang_eses = get_optional_string(r, "areaname_lang_eses");
        obj.area_name_lang_esmx = get_optional_string(r, "areaname_lang_esmx");
        obj.area_name_lang_ruru = get_optional_string(r, "areaname_lang_ruru");
        obj.area_name_lang_ptpt = get_optional_string(r, "areaname_lang_ptpt");
        obj.area_name_lang_ptbr = get_optional_string(r, "areaname_lang_ptbr");
        obj.area_name_lang_itit = get_optional_string(r, "areaname_lang_itit");
        obj.area_name_lang_unk = get_optional_string(r, "areaname_lang_unk");

        obj.area_name_lang_mask = r["areaname_lang_mask"].as<uint32_t>();
        return obj;
    }
};
