#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcTaxiNodes {
    uint32_t id;
    uint32_t continentid;
    float x;
    float y;
    float z;

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
    uint32_t mountcreatureid_1;
    uint32_t mountcreatureid_2;
};

template<>
struct PgRowMapper<DbcTaxiNodes> {
    static DbcTaxiNodes map(const pqxx::row& r) {
        DbcTaxiNodes obj;
        obj.id                = r["id"].as<uint32_t>();
        obj.continentid       = r["continentid"].as<uint32_t>();
        obj.x                 = r["x"].as<float>();
        obj.y                 = r["y"].as<float>();
        obj.z                 = r["z"].as<float>();

        obj.name_lang_enus    = get_optional_string(r, "name_lang_enus");
        obj.name_lang_engb    = get_optional_string(r, "name_lang_engb");
        obj.name_lang_kokr    = get_optional_string(r, "name_lang_kokr");
        obj.name_lang_frfr    = get_optional_string(r, "name_lang_frfr");
        obj.name_lang_dede    = get_optional_string(r, "name_lang_dede");
        obj.name_lang_encn    = get_optional_string(r, "name_lang_encn");
        obj.name_lang_zhcn    = get_optional_string(r, "name_lang_zhcn");
        obj.name_lang_entw    = get_optional_string(r, "name_lang_entw");
        obj.name_lang_zhtw    = get_optional_string(r, "name_lang_zhtw");
        obj.name_lang_eses    = get_optional_string(r, "name_lang_eses");
        obj.name_lang_esmx    = get_optional_string(r, "name_lang_esmx");
        obj.name_lang_ruru    = get_optional_string(r, "name_lang_ruru");
        obj.name_lang_ptpt    = get_optional_string(r, "name_lang_ptpt");
        obj.name_lang_ptbr    = get_optional_string(r, "name_lang_ptbr");
        obj.name_lang_itit    = get_optional_string(r, "name_lang_itit");
        obj.name_lang_unk     = get_optional_string(r, "name_lang_unk");

        obj.name_lang_mask    = r["name_lang_mask"].as<uint64_t>();
        obj.mountcreatureid_1 = r["mountcreatureid_1"].as<uint32_t>();
        obj.mountcreatureid_2 = r["mountcreatureid_2"].as<uint32_t>();
        return obj;
    }
};