#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

struct PgDbcLfgDungeons {
    uint32_t id;

    // Name локализации
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

    // Уровни и карта
    uint32_t min_level;
    uint32_t max_level;
    uint32_t target_level;
    uint32_t target_level_min;
    uint32_t target_level_max;
    int32_t mapid;
    uint32_t difficulty;
    uint32_t flags;
    uint32_t typeId;
    int32_t faction;
    std::optional<std::string> texturefilename;
    uint32_t expansionlevel;
    uint32_t order_index;
    uint32_t group_id;

    // Description локализации
    std::optional<std::string> description_lang_enus;
    std::optional<std::string> description_lang_engb;
    std::optional<std::string> description_lang_kokr;
    std::optional<std::string> description_lang_frfr;
    std::optional<std::string> description_lang_dede;
    std::optional<std::string> description_lang_encn;
    std::optional<std::string> description_lang_zhcn;
    std::optional<std::string> description_lang_entw;
    std::optional<std::string> description_lang_zhtw;
    std::optional<std::string> description_lang_eses;
    std::optional<std::string> description_lang_esmx;
    std::optional<std::string> description_lang_ruru;
    std::optional<std::string> description_lang_ptpt;
    std::optional<std::string> description_lang_ptbr;
    std::optional<std::string> description_lang_itit;
    std::optional<std::string> description_lang_unk;
    uint32_t description_lang_mask;
};

// === PgRowMapper ===
template<>
struct PgRowMapper<PgDbcLfgDungeons> {
    static PgDbcLfgDungeons map(const pqxx::row& r) {
        PgDbcLfgDungeons d;

        d.id = r["id"].as<uint32_t>();

        d.name_lang_enus = get_optional_string(r, "name_lang_enus");
        d.name_lang_engb = get_optional_string(r, "name_lang_engb");
        d.name_lang_kokr = get_optional_string(r, "name_lang_kokr");
        d.name_lang_frfr = get_optional_string(r, "name_lang_frfr");
        d.name_lang_dede = get_optional_string(r, "name_lang_dede");
        d.name_lang_encn = get_optional_string(r, "name_lang_encn");
        d.name_lang_zhcn = get_optional_string(r, "name_lang_zhcn");
        d.name_lang_entw = get_optional_string(r, "name_lang_entw");
        d.name_lang_zhtw = get_optional_string(r, "name_lang_zhtw");
        d.name_lang_eses = get_optional_string(r, "name_lang_eses");
        d.name_lang_esmx = get_optional_string(r, "name_lang_esmx");
        d.name_lang_ruru = get_optional_string(r, "name_lang_ruru");
        d.name_lang_ptpt = get_optional_string(r, "name_lang_ptpt");
        d.name_lang_ptbr = get_optional_string(r, "name_lang_ptbr");
        d.name_lang_itit = get_optional_string(r, "name_lang_itit");
        d.name_lang_unk  = get_optional_string(r, "name_lang_unk");
        d.name_lang_mask = r["name_lang_mask"].as<uint32_t>();

        d.min_level = r["min_level"].as<uint32_t>();
        d.max_level = r["max_level"].as<uint32_t>();
        d.target_level = r["target_level"].as<uint32_t>();
        d.target_level_min = r["target_level_min"].as<uint32_t>();
        d.target_level_max = r["target_level_max"].as<uint32_t>();
        d.mapid = r["mapid"].as<int32_t>();
        d.difficulty = r["difficulty"].as<uint32_t>();
        d.flags = r["flags"].as<uint32_t>();
        d.typeId = r["typeid"].as<uint32_t>();
        d.faction = r["faction"].as<int32_t>();
        d.texturefilename = get_optional_string(r, "texturefilename");
        d.expansionlevel = r["expansionlevel"].as<uint32_t>();
        d.order_index = r["order_index"].as<uint32_t>();
        d.group_id = r["group_id"].as<uint32_t>();

        d.description_lang_enus = get_optional_string(r, "description_lang_enus");
        d.description_lang_engb = get_optional_string(r, "description_lang_engb");
        d.description_lang_kokr = get_optional_string(r, "description_lang_kokr");
        d.description_lang_frfr = get_optional_string(r, "description_lang_frfr");
        d.description_lang_dede = get_optional_string(r, "description_lang_dede");
        d.description_lang_encn = get_optional_string(r, "description_lang_encn");
        d.description_lang_zhcn = get_optional_string(r, "description_lang_zhcn");
        d.description_lang_entw = get_optional_string(r, "description_lang_entw");
        d.description_lang_zhtw = get_optional_string(r, "description_lang_zhtw");
        d.description_lang_eses = get_optional_string(r, "description_lang_eses");
        d.description_lang_esmx = get_optional_string(r, "description_lang_esmx");
        d.description_lang_ruru = get_optional_string(r, "description_lang_ruru");
        d.description_lang_ptpt = get_optional_string(r, "description_lang_ptpt");
        d.description_lang_ptbr = get_optional_string(r, "description_lang_ptbr");
        d.description_lang_itit = get_optional_string(r, "description_lang_itit");
        d.description_lang_unk = get_optional_string(r, "description_lang_unk");
        d.description_lang_mask = r["description_lang_mask"].as<uint32_t>();

        return d;
    }
};