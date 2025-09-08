#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_itemset **/
struct DbcItemSet {
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

    uint64_t name_lang_mask;

    uint32_t itemid_1;
    uint32_t itemid_2;
    uint32_t itemid_3;
    uint32_t itemid_4;
    uint32_t itemid_5;
    uint32_t itemid_6;
    uint32_t itemid_7;
    uint32_t itemid_8;
    uint32_t itemid_9;
    uint32_t itemid_10;
    uint32_t itemid_11;
    uint32_t itemid_12;
    uint32_t itemid_13;
    uint32_t itemid_14;
    uint32_t itemid_15;
    uint32_t itemid_16;
    uint32_t itemid_17;

    uint32_t setspellid_1;
    uint32_t setspellid_2;
    uint32_t setspellid_3;
    uint32_t setspellid_4;
    uint32_t setspellid_5;
    uint32_t setspellid_6;
    uint32_t setspellid_7;
    uint32_t setspellid_8;

    uint32_t setthreshold_1;
    uint32_t setthreshold_2;
    uint32_t setthreshold_3;
    uint32_t setthreshold_4;
    uint32_t setthreshold_5;
    uint32_t setthreshold_6;
    uint32_t setthreshold_7;
    uint32_t setthreshold_8;

    uint32_t requiredskill;
    uint32_t requiredskillrank;
};

template<>
struct PgRowMapper<DbcItemSet> {
    static DbcItemSet map(const pqxx::row &r) {
        DbcItemSet obj{
                r["id"].as<uint32_t>(),
                get_optional_string(r, "name_lang_enus"),
                get_optional_string(r, "name_lang_engb"),
                get_optional_string(r, "name_lang_kokr"),
                get_optional_string(r, "name_lang_frfr"),
                get_optional_string(r, "name_lang_dede"),
                get_optional_string(r, "name_lang_encn"),
                get_optional_string(r, "name_lang_zhcn"),
                get_optional_string(r, "name_lang_entw"),
                get_optional_string(r, "name_lang_zhtw"),
                get_optional_string(r, "name_lang_eses"),
                get_optional_string(r, "name_lang_esmx"),
                get_optional_string(r, "name_lang_ruru"),
                get_optional_string(r, "name_lang_ptpt"),
                get_optional_string(r, "name_lang_ptbr"),
                get_optional_string(r, "name_lang_itit"),
                get_optional_string(r, "name_lang_unk"),
                r["name_lang_mask"].as<uint64_t>(),

                r["itemid_1"].as<uint32_t>(),
                r["itemid_2"].as<uint32_t>(),
                r["itemid_3"].as<uint32_t>(),
                r["itemid_4"].as<uint32_t>(),
                r["itemid_5"].as<uint32_t>(),
                r["itemid_6"].as<uint32_t>(),
                r["itemid_7"].as<uint32_t>(),
                r["itemid_8"].as<uint32_t>(),
                r["itemid_9"].as<uint32_t>(),
                r["itemid_10"].as<uint32_t>(),
                r["itemid_11"].as<uint32_t>(),
                r["itemid_12"].as<uint32_t>(),
                r["itemid_13"].as<uint32_t>(),
                r["itemid_14"].as<uint32_t>(),
                r["itemid_15"].as<uint32_t>(),
                r["itemid_16"].as<uint32_t>(),
                r["itemid_17"].as<uint32_t>(),

                r["setspellid_1"].as<uint32_t>(),
                r["setspellid_2"].as<uint32_t>(),
                r["setspellid_3"].as<uint32_t>(),
                r["setspellid_4"].as<uint32_t>(),
                r["setspellid_5"].as<uint32_t>(),
                r["setspellid_6"].as<uint32_t>(),
                r["setspellid_7"].as<uint32_t>(),
                r["setspellid_8"].as<uint32_t>(),

                r["setthreshold_1"].as<uint32_t>(),
                r["setthreshold_2"].as<uint32_t>(),
                r["setthreshold_3"].as<uint32_t>(),
                r["setthreshold_4"].as<uint32_t>(),
                r["setthreshold_5"].as<uint32_t>(),
                r["setthreshold_6"].as<uint32_t>(),
                r["setthreshold_7"].as<uint32_t>(),
                r["setthreshold_8"].as<uint32_t>(),

                r["requiredskill"].as<uint32_t>(),
                r["requiredskillrank"].as<uint32_t>()
        };
        return obj;
    }
};