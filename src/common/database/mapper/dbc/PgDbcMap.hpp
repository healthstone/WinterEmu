#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_map **/
struct DbcMap {
    uint32_t id;
    std::optional<std::string> directory;
    uint32_t instance_type;
    uint32_t flags;
    uint32_t pvp;

    std::optional<std::string> mapname_lang_en_us;
    std::optional<std::string> mapname_lang_en_gb;
    std::optional<std::string> mapname_lang_ko_kr;
    std::optional<std::string> mapname_lang_fr_fr;
    std::optional<std::string> mapname_lang_de_de;
    std::optional<std::string> mapname_lang_en_cn;
    std::optional<std::string> mapname_lang_zh_cn;
    std::optional<std::string> mapname_lang_en_tw;
    std::optional<std::string> mapname_lang_zh_tw;
    std::optional<std::string> mapname_lang_es_es;
    std::optional<std::string> mapname_lang_es_mx;
    std::optional<std::string> mapname_lang_ru_ru;
    std::optional<std::string> mapname_lang_pt_pt;
    std::optional<std::string> mapname_lang_pt_br;
    std::optional<std::string> mapname_lang_it_it;
    std::optional<std::string> mapname_lang_unk;
    uint32_t mapname_lang_mask;

    uint32_t area_table_id;

    std::optional<std::string> mapdescription0_lang_en_us;
    std::optional<std::string> mapdescription0_lang_en_gb;
    std::optional<std::string> mapdescription0_lang_ko_kr;
    std::optional<std::string> mapdescription0_lang_fr_fr;
    std::optional<std::string> mapdescription0_lang_de_de;
    std::optional<std::string> mapdescription0_lang_en_cn;
    std::optional<std::string> mapdescription0_lang_zh_cn;
    std::optional<std::string> mapdescription0_lang_en_tw;
    std::optional<std::string> mapdescription0_lang_zh_tw;
    std::optional<std::string> mapdescription0_lang_es_es;
    std::optional<std::string> mapdescription0_lang_es_mx;
    std::optional<std::string> mapdescription0_lang_ru_ru;
    std::optional<std::string> mapdescription0_lang_pt_pt;
    std::optional<std::string> mapdescription0_lang_pt_br;
    std::optional<std::string> mapdescription0_lang_it_it;
    std::optional<std::string> mapdescription0_lang_unk;
    uint32_t mapdescription0_lang_mask;

    std::optional<std::string> mapdescription1_lang_en_us;
    std::optional<std::string> mapdescription1_lang_en_gb;
    std::optional<std::string> mapdescription1_lang_ko_kr;
    std::optional<std::string> mapdescription1_lang_fr_fr;
    std::optional<std::string> mapdescription1_lang_de_de;
    std::optional<std::string> mapdescription1_lang_en_cn;
    std::optional<std::string> mapdescription1_lang_zh_cn;
    std::optional<std::string> mapdescription1_lang_en_tw;
    std::optional<std::string> mapdescription1_lang_zh_tw;
    std::optional<std::string> mapdescription1_lang_es_es;
    std::optional<std::string> mapdescription1_lang_es_mx;
    std::optional<std::string> mapdescription1_lang_ru_ru;
    std::optional<std::string> mapdescription1_lang_pt_pt;
    std::optional<std::string> mapdescription1_lang_pt_br;
    std::optional<std::string> mapdescription1_lang_it_it;
    std::optional<std::string> mapdescription1_lang_unk;
    uint32_t mapdescription1_lang_mask;

    uint32_t loading_screen_id;
    float minimap_icon_scale;
    int32_t corpse_map_id;
    float corpse_x;
    float corpse_y;
    int32_t time_of_day_override;
    uint32_t expansion_id;
    uint32_t raid_offset;
    uint32_t max_players;
};

template<>
struct PgRowMapper<DbcMap> {
    static DbcMap map(const pqxx::row& r) {
        DbcMap m{};
        m.id = r["id"].as<uint32_t>();
        m.directory = get_optional_string(r, "directory");
        m.instance_type = r["instance_type"].as<uint32_t>();
        m.flags = r["flags"].as<uint32_t>();
        m.pvp = r["pvp"].as<uint32_t>();

        m.mapname_lang_en_us = get_optional_string(r, "mapname_lang_en_us");
        m.mapname_lang_en_gb = get_optional_string(r, "mapname_lang_en_gb");
        m.mapname_lang_ko_kr = get_optional_string(r, "mapname_lang_ko_kr");
        m.mapname_lang_fr_fr = get_optional_string(r, "mapname_lang_fr_fr");
        m.mapname_lang_de_de = get_optional_string(r, "mapname_lang_de_de");
        m.mapname_lang_en_cn = get_optional_string(r, "mapname_lang_en_cn");
        m.mapname_lang_zh_cn = get_optional_string(r, "mapname_lang_zh_cn");
        m.mapname_lang_en_tw = get_optional_string(r, "mapname_lang_en_tw");
        m.mapname_lang_zh_tw = get_optional_string(r, "mapname_lang_zh_tw");
        m.mapname_lang_es_es = get_optional_string(r, "mapname_lang_es_es");
        m.mapname_lang_es_mx = get_optional_string(r, "mapname_lang_es_mx");
        m.mapname_lang_ru_ru = get_optional_string(r, "mapname_lang_ru_ru");
        m.mapname_lang_pt_pt = get_optional_string(r, "mapname_lang_pt_pt");
        m.mapname_lang_pt_br = get_optional_string(r, "mapname_lang_pt_br");
        m.mapname_lang_it_it = get_optional_string(r, "mapname_lang_it_it");
        m.mapname_lang_unk = get_optional_string(r, "mapname_lang_unk");
        m.mapname_lang_mask = r["mapname_lang_mask"].as<uint32_t>();

        m.area_table_id = r["area_table_id"].as<uint32_t>();

        m.mapdescription0_lang_en_us = get_optional_string(r, "mapdescription0_lang_en_us");
        m.mapdescription0_lang_en_gb = get_optional_string(r, "mapdescription0_lang_en_gb");
        m.mapdescription0_lang_ko_kr = get_optional_string(r, "mapdescription0_lang_ko_kr");
        m.mapdescription0_lang_fr_fr = get_optional_string(r, "mapdescription0_lang_fr_fr");
        m.mapdescription0_lang_de_de = get_optional_string(r, "mapdescription0_lang_de_de");
        m.mapdescription0_lang_en_cn = get_optional_string(r, "mapdescription0_lang_en_cn");
        m.mapdescription0_lang_zh_cn = get_optional_string(r, "mapdescription0_lang_zh_cn");
        m.mapdescription0_lang_en_tw = get_optional_string(r, "mapdescription0_lang_en_tw");
        m.mapdescription0_lang_zh_tw = get_optional_string(r, "mapdescription0_lang_zh_tw");
        m.mapdescription0_lang_es_es = get_optional_string(r, "mapdescription0_lang_es_es");
        m.mapdescription0_lang_es_mx = get_optional_string(r, "mapdescription0_lang_es_mx");
        m.mapdescription0_lang_ru_ru = get_optional_string(r, "mapdescription0_lang_ru_ru");
        m.mapdescription0_lang_pt_pt = get_optional_string(r, "mapdescription0_lang_pt_pt");
        m.mapdescription0_lang_pt_br = get_optional_string(r, "mapdescription0_lang_pt_br");
        m.mapdescription0_lang_it_it = get_optional_string(r, "mapdescription0_lang_it_it");
        m.mapdescription0_lang_unk = get_optional_string(r, "mapdescription0_lang_unk");
        m.mapdescription0_lang_mask = r["mapdescription0_lang_mask"].as<uint32_t>();

        m.mapdescription1_lang_en_us = get_optional_string(r, "mapdescription1_lang_en_us");
        m.mapdescription1_lang_en_gb = get_optional_string(r, "mapdescription1_lang_en_gb");
        m.mapdescription1_lang_ko_kr = get_optional_string(r, "mapdescription1_lang_ko_kr");
        m.mapdescription1_lang_fr_fr = get_optional_string(r, "mapdescription1_lang_fr_fr");
        m.mapdescription1_lang_de_de = get_optional_string(r, "mapdescription1_lang_de_de");
        m.mapdescription1_lang_en_cn = get_optional_string(r, "mapdescription1_lang_en_cn");
        m.mapdescription1_lang_zh_cn = get_optional_string(r, "mapdescription1_lang_zh_cn");
        m.mapdescription1_lang_en_tw = get_optional_string(r, "mapdescription1_lang_en_tw");
        m.mapdescription1_lang_zh_tw = get_optional_string(r, "mapdescription1_lang_zh_tw");
        m.mapdescription1_lang_es_es = get_optional_string(r, "mapdescription1_lang_es_es");
        m.mapdescription1_lang_es_mx = get_optional_string(r, "mapdescription1_lang_es_mx");
        m.mapdescription1_lang_ru_ru = get_optional_string(r, "mapdescription1_lang_ru_ru");
        m.mapdescription1_lang_pt_pt = get_optional_string(r, "mapdescription1_lang_pt_pt");
        m.mapdescription1_lang_pt_br = get_optional_string(r, "mapdescription1_lang_pt_br");
        m.mapdescription1_lang_it_it = get_optional_string(r, "mapdescription1_lang_it_it");
        m.mapdescription1_lang_unk = get_optional_string(r, "mapdescription1_lang_unk");
        m.mapdescription1_lang_mask = r["mapdescription1_lang_mask"].as<uint32_t>();

        m.loading_screen_id = r["loading_screen_id"].as<uint32_t>();
        m.minimap_icon_scale = r["minimap_icon_scale"].as<float>();
        m.corpse_map_id = r["corpse_map_id"].as<int32_t>();
        m.corpse_x = r["corpse_x"].as<float>();
        m.corpse_y = r["corpse_y"].as<float>();
        m.time_of_day_override = r["time_of_day_override"].as<int32_t>();
        m.expansion_id = r["expansion_id"].as<uint32_t>();
        m.raid_offset = r["raid_offset"].as<uint32_t>();
        m.max_players = r["max_players"].as<uint32_t>();

        return m;
    }
};