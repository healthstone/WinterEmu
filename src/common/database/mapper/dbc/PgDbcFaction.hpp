#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_faction **/
struct DbcFaction {
    uint32_t id;
    int32_t reputationindex;

    uint32_t reputationracemask_1;
    uint32_t reputationracemask_2;
    uint32_t reputationracemask_3;
    uint32_t reputationracemask_4;

    uint32_t reputationclassmask_1;
    uint32_t reputationclassmask_2;
    uint32_t reputationclassmask_3;
    uint32_t reputationclassmask_4;

    int32_t reputationbase_1;
    int32_t reputationbase_2;
    int32_t reputationbase_3;
    int32_t reputationbase_4;

    uint32_t reputationflags_1;
    uint32_t reputationflags_2;
    uint32_t reputationflags_3;
    uint32_t reputationflags_4;

    uint32_t parentfactionid;
    float    parentfactionmod_1;
    float    parentfactionmod_2;
    int32_t parentfactioncap_1;
    int32_t parentfactioncap_2;

    // name localization (nullable)
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

    // description localization (nullable)
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

template<>
struct PgRowMapper<DbcFaction> {
    static DbcFaction map(const pqxx::row& r) {
        DbcFaction row{};

        row.id = r["id"].as<uint32_t>();
        row.reputationindex = r["reputationindex"].as<int32_t>();

        row.reputationracemask_1 = r["reputationracemask_1"].as<uint32_t>();
        row.reputationracemask_2 = r["reputationracemask_2"].as<uint32_t>();
        row.reputationracemask_3 = r["reputationracemask_3"].as<uint32_t>();
        row.reputationracemask_4 = r["reputationracemask_4"].as<uint32_t>();

        row.reputationclassmask_1 = r["reputationclassmask_1"].as<uint32_t>();
        row.reputationclassmask_2 = r["reputationclassmask_2"].as<uint32_t>();
        row.reputationclassmask_3 = r["reputationclassmask_3"].as<uint32_t>();
        row.reputationclassmask_4 = r["reputationclassmask_4"].as<uint32_t>();

        row.reputationbase_1 = r["reputationbase_1"].as<int32_t>();
        row.reputationbase_2 = r["reputationbase_2"].as<int32_t>();
        row.reputationbase_3 = r["reputationbase_3"].as<int32_t>();
        row.reputationbase_4 = r["reputationbase_4"].as<int32_t>();

        row.reputationflags_1 = r["reputationflags_1"].as<uint32_t>();
        row.reputationflags_2 = r["reputationflags_2"].as<uint32_t>();
        row.reputationflags_3 = r["reputationflags_3"].as<uint32_t>();
        row.reputationflags_4 = r["reputationflags_4"].as<uint32_t>();

        row.parentfactionid   = r["parentfactionid"].as<uint32_t>();
        row.parentfactionmod_1 = r["parentfactionmod_1"].as<float>();
        row.parentfactionmod_2 = r["parentfactionmod_2"].as<float>();
        row.parentfactioncap_1 = r["parentfactioncap_1"].as<int32_t>();
        row.parentfactioncap_2 = r["parentfactioncap_2"].as<int32_t>();

        // name localization (nullable)
        row.name_lang_enus = get_optional_string(r, "name_lang_enus");
        row.name_lang_engb = get_optional_string(r, "name_lang_engb");
        row.name_lang_kokr = get_optional_string(r, "name_lang_kokr");
        row.name_lang_frfr = get_optional_string(r, "name_lang_frfr");
        row.name_lang_dede = get_optional_string(r, "name_lang_dede");
        row.name_lang_encn = get_optional_string(r, "name_lang_encn");
        row.name_lang_zhcn = get_optional_string(r, "name_lang_zhcn");
        row.name_lang_entw = get_optional_string(r, "name_lang_entw");
        row.name_lang_zhtw = get_optional_string(r, "name_lang_zhtw");
        row.name_lang_eses = get_optional_string(r, "name_lang_eses");
        row.name_lang_esmx = get_optional_string(r, "name_lang_esmx");
        row.name_lang_ruru = get_optional_string(r, "name_lang_ruru");
        row.name_lang_ptpt = get_optional_string(r, "name_lang_ptpt");
        row.name_lang_ptbr = get_optional_string(r, "name_lang_ptbr");
        row.name_lang_itit = get_optional_string(r, "name_lang_itit");
        row.name_lang_unk  = get_optional_string(r, "name_lang_unk");
        row.name_lang_mask = r["name_lang_mask"].as<uint32_t>();

        // description localization (nullable)
        row.description_lang_enus = get_optional_string(r, "description_lang_enus");
        row.description_lang_engb = get_optional_string(r, "description_lang_engb");
        row.description_lang_kokr = get_optional_string(r, "description_lang_kokr");
        row.description_lang_frfr = get_optional_string(r, "description_lang_frfr");
        row.description_lang_dede = get_optional_string(r, "description_lang_dede");
        row.description_lang_encn = get_optional_string(r, "description_lang_encn");
        row.description_lang_zhcn = get_optional_string(r, "description_lang_zhcn");
        row.description_lang_entw = get_optional_string(r, "description_lang_entw");
        row.description_lang_zhtw = get_optional_string(r, "description_lang_zhtw");
        row.description_lang_eses = get_optional_string(r, "description_lang_eses");
        row.description_lang_esmx = get_optional_string(r, "description_lang_esmx");
        row.description_lang_ruru = get_optional_string(r, "description_lang_ruru");
        row.description_lang_ptpt = get_optional_string(r, "description_lang_ptpt");
        row.description_lang_ptbr = get_optional_string(r, "description_lang_ptbr");
        row.description_lang_itit = get_optional_string(r, "description_lang_itit");
        row.description_lang_unk  = get_optional_string(r, "description_lang_unk");
        row.description_lang_mask = r["description_lang_mask"].as<uint32_t>();

        return row;
    }
};
