#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_dungeonencounter **/
struct DbcDungeonEncounter {
    uint32_t id;
    uint32_t mapid;
    uint32_t difficulty;
    int32_t orderindex;
    uint32_t bit;
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
    uint32_t spelliconid;
};

template<>
struct PgRowMapper<DbcDungeonEncounter> {
    static DbcDungeonEncounter map(const pqxx::row& r) {
        DbcDungeonEncounter row;

        row.id             = r["id"].as<uint32_t>();
        row.mapid          = r["mapid"].as<uint32_t>();
        row.difficulty     = r["difficulty"].as<uint32_t>();
        row.orderindex     = r["orderindex"].as<int32_t>();
        row.bit            = r["bit"].as<uint32_t>();
        row.name_lang_enus = r["name_lang_enus"].is_null() ? std::nullopt : std::optional(r["name_lang_enus"].as<std::string>());
        row.name_lang_engb = r["name_lang_engb"].is_null() ? std::nullopt : std::optional(r["name_lang_engb"].as<std::string>());
        row.name_lang_kokr = r["name_lang_kokr"].is_null() ? std::nullopt : std::optional(r["name_lang_kokr"].as<std::string>());
        row.name_lang_frfr = r["name_lang_frfr"].is_null() ? std::nullopt : std::optional(r["name_lang_frfr"].as<std::string>());
        row.name_lang_dede = r["name_lang_dede"].is_null() ? std::nullopt : std::optional(r["name_lang_dede"].as<std::string>());
        row.name_lang_encn = r["name_lang_encn"].is_null() ? std::nullopt : std::optional(r["name_lang_encn"].as<std::string>());
        row.name_lang_zhcn = r["name_lang_zhcn"].is_null() ? std::nullopt : std::optional(r["name_lang_zhcn"].as<std::string>());
        row.name_lang_entw = r["name_lang_entw"].is_null() ? std::nullopt : std::optional(r["name_lang_entw"].as<std::string>());
        row.name_lang_zhtw = r["name_lang_zhtw"].is_null() ? std::nullopt : std::optional(r["name_lang_zhtw"].as<std::string>());
        row.name_lang_eses = r["name_lang_eses"].is_null() ? std::nullopt : std::optional(r["name_lang_eses"].as<std::string>());
        row.name_lang_esmx = r["name_lang_esmx"].is_null() ? std::nullopt : std::optional(r["name_lang_esmx"].as<std::string>());
        row.name_lang_ruru = r["name_lang_ruru"].is_null() ? std::nullopt : std::optional(r["name_lang_ruru"].as<std::string>());
        row.name_lang_ptpt = r["name_lang_ptpt"].is_null() ? std::nullopt : std::optional(r["name_lang_ptpt"].as<std::string>());
        row.name_lang_ptbr = r["name_lang_ptbr"].is_null() ? std::nullopt : std::optional(r["name_lang_ptbr"].as<std::string>());
        row.name_lang_itit = r["name_lang_itit"].is_null() ? std::nullopt : std::optional(r["name_lang_itit"].as<std::string>());
        row.name_lang_unk  = r["name_lang_unk"].is_null() ? std::nullopt : std::optional(r["name_lang_unk"].as<std::string>());
        row.name_lang_mask = r["name_lang_mask"].as<uint32_t>();
        row.spelliconid    = r["spelliconid"].as<uint32_t>();

        return row;
    }
};
