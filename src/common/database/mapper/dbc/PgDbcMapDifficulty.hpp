#pragma once

#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_mapdifficulty **/
struct DbcMapDifficulty
{
    uint32_t id;
    uint32_t map_id;
    uint32_t difficulty;

    // локализованные сообщения
    std::optional<std::string> message_lang_en_us;
    std::optional<std::string> message_lang_en_gb;
    std::optional<std::string> message_lang_ko_kr;
    std::optional<std::string> message_lang_fr_fr;
    std::optional<std::string> message_lang_de_de;
    std::optional<std::string> message_lang_en_cn;
    std::optional<std::string> message_lang_zh_cn;
    std::optional<std::string> message_lang_en_tw;
    std::optional<std::string> message_lang_zh_tw;
    std::optional<std::string> message_lang_es_es;
    std::optional<std::string> message_lang_es_mx;
    std::optional<std::string> message_lang_ru_ru;
    std::optional<std::string> message_lang_pt_pt;
    std::optional<std::string> message_lang_pt_br;
    std::optional<std::string> message_lang_it_it;
    std::optional<std::string> message_lang_unk;
    uint32_t message_lang_mask;

    uint32_t raid_duration;
    uint32_t max_players;

    std::optional<std::string> difficulty_string;
};

template<>
struct PgRowMapper<DbcMapDifficulty>
{
    static DbcMapDifficulty map(const pqxx::row &r)
    {
        DbcMapDifficulty row{};

        row.id            = r["id"].as<uint32_t>();
        row.map_id        = r["map_id"].as<uint32_t>();
        row.difficulty    = r["difficulty"].as<uint32_t>();

        row.message_lang_en_us = get_optional_string(r, "message_lang_en_us");
        row.message_lang_en_gb = get_optional_string(r, "message_lang_en_gb");
        row.message_lang_ko_kr = get_optional_string(r, "message_lang_ko_kr");
        row.message_lang_fr_fr = get_optional_string(r, "message_lang_fr_fr");
        row.message_lang_de_de = get_optional_string(r, "message_lang_de_de");
        row.message_lang_en_cn = get_optional_string(r, "message_lang_en_cn");
        row.message_lang_zh_cn = get_optional_string(r, "message_lang_zh_cn");
        row.message_lang_en_tw = get_optional_string(r, "message_lang_en_tw");
        row.message_lang_zh_tw = get_optional_string(r, "message_lang_zh_tw");
        row.message_lang_es_es = get_optional_string(r, "message_lang_es_es");
        row.message_lang_es_mx = get_optional_string(r, "message_lang_es_mx");
        row.message_lang_ru_ru = get_optional_string(r, "message_lang_ru_ru");
        row.message_lang_pt_pt = get_optional_string(r, "message_lang_pt_pt");
        row.message_lang_pt_br = get_optional_string(r, "message_lang_pt_br");
        row.message_lang_it_it = get_optional_string(r, "message_lang_it_it");
        row.message_lang_unk   = get_optional_string(r, "message_lang_unk");
        row.message_lang_mask  = r["message_lang_mask"].as<uint32_t>();

        row.raid_duration     = r["raid_duration"].as<uint32_t>();
        row.max_players       = r["max_players"].as<uint32_t>();

        row.difficulty_string = get_optional_string(r, "difficulty_string");

        return row;
    }
};