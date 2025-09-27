#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

struct DbcSpellRange {
    uint32_t id;
    float rangemin_1;
    float rangemin_2;
    float rangemax_1;
    float rangemax_2;
    uint32_t flags;

    std::optional<std::string> displayname_lang_enus;
    std::optional<std::string> displayname_lang_engb;
    std::optional<std::string> displayname_lang_kokr;
    std::optional<std::string> displayname_lang_frfr;
    std::optional<std::string> displayname_lang_dede;
    std::optional<std::string> displayname_lang_encn;
    std::optional<std::string> displayname_lang_zhcn;
    std::optional<std::string> displayname_lang_entw;
    std::optional<std::string> displayname_lang_zhtw;
    std::optional<std::string> displayname_lang_eses;
    std::optional<std::string> displayname_lang_esmx;
    std::optional<std::string> displayname_lang_ruru;
    std::optional<std::string> displayname_lang_ptpt;
    std::optional<std::string> displayname_lang_ptbr;
    std::optional<std::string> displayname_lang_itit;
    std::optional<std::string> displayname_lang_unk;
    uint32_t displayname_lang_mask;

    std::optional<std::string> displaynameshort_lang_enus;
    std::optional<std::string> displaynameshort_lang_engb;
    std::optional<std::string> displaynameshort_lang_kokr;
    std::optional<std::string> displaynameshort_lang_frfr;
    std::optional<std::string> displaynameshort_lang_dede;
    std::optional<std::string> displaynameshort_lang_encn;
    std::optional<std::string> displaynameshort_lang_zhcn;
    std::optional<std::string> displaynameshort_lang_entw;
    std::optional<std::string> displaynameshort_lang_zhtw;
    std::optional<std::string> displaynameshort_lang_eses;
    std::optional<std::string> displaynameshort_lang_esmx;
    std::optional<std::string> displaynameshort_lang_ruru;
    std::optional<std::string> displaynameshort_lang_ptpt;
    std::optional<std::string> displaynameshort_lang_ptbr;
    std::optional<std::string> displaynameshort_lang_itit;
    std::optional<std::string> displaynameshort_lang_unk;
    uint32_t displaynameshort_lang_mask;
};

template<>
struct PgRowMapper<DbcSpellRange> {
    static DbcSpellRange map(const pqxx::row& r) {
        DbcSpellRange obj;
        obj.id = r["id"].as<uint32_t>();
        obj.rangemin_1 = r["rangemin_1"].as<float>();
        obj.rangemin_2 = r["rangemin_2"].as<float>();
        obj.rangemax_1 = r["rangemax_1"].as<float>();
        obj.rangemax_2 = r["rangemax_2"].as<float>();
        obj.flags = r["flags"].as<uint32_t>();

        obj.displayname_lang_enus  = get_optional_string(r, "displayname_lang_enus");
        obj.displayname_lang_engb  = get_optional_string(r, "displayname_lang_engb");
        obj.displayname_lang_kokr  = get_optional_string(r, "displayname_lang_kokr");
        obj.displayname_lang_frfr  = get_optional_string(r, "displayname_lang_frfr");
        obj.displayname_lang_dede  = get_optional_string(r, "displayname_lang_dede");
        obj.displayname_lang_encn  = get_optional_string(r, "displayname_lang_encn");
        obj.displayname_lang_zhcn  = get_optional_string(r, "displayname_lang_zhcn");
        obj.displayname_lang_entw  = get_optional_string(r, "displayname_lang_entw");
        obj.displayname_lang_zhtw  = get_optional_string(r, "displayname_lang_zhtw");
        obj.displayname_lang_eses  = get_optional_string(r, "displayname_lang_eses");
        obj.displayname_lang_esmx  = get_optional_string(r, "displayname_lang_esmx");
        obj.displayname_lang_ruru  = get_optional_string(r, "displayname_lang_ruru");
        obj.displayname_lang_ptpt  = get_optional_string(r, "displayname_lang_ptpt");
        obj.displayname_lang_ptbr  = get_optional_string(r, "displayname_lang_ptbr");
        obj.displayname_lang_itit  = get_optional_string(r, "displayname_lang_itit");
        obj.displayname_lang_unk   = get_optional_string(r, "displayname_lang_unk");
        obj.displayname_lang_mask  = r["displayname_lang_mask"].as<uint32_t>();

        obj.displaynameshort_lang_enus = get_optional_string(r, "displaynameshort_lang_enus");
        obj.displaynameshort_lang_engb = get_optional_string(r, "displaynameshort_lang_engb");
        obj.displaynameshort_lang_kokr = get_optional_string(r, "displaynameshort_lang_kokr");
        obj.displaynameshort_lang_frfr = get_optional_string(r, "displaynameshort_lang_frfr");
        obj.displaynameshort_lang_dede = get_optional_string(r, "displaynameshort_lang_dede");
        obj.displaynameshort_lang_encn = get_optional_string(r, "displaynameshort_lang_encn");
        obj.displaynameshort_lang_zhcn = get_optional_string(r, "displaynameshort_lang_zhcn");
        obj.displaynameshort_lang_entw = get_optional_string(r, "displaynameshort_lang_entw");
        obj.displaynameshort_lang_zhtw = get_optional_string(r, "displaynameshort_lang_zhtw");
        obj.displaynameshort_lang_eses = get_optional_string(r, "displaynameshort_lang_eses");
        obj.displaynameshort_lang_esmx = get_optional_string(r, "displaynameshort_lang_esmx");
        obj.displaynameshort_lang_ruru = get_optional_string(r, "displaynameshort_lang_ruru");
        obj.displaynameshort_lang_ptpt = get_optional_string(r, "displaynameshort_lang_ptpt");
        obj.displaynameshort_lang_ptbr = get_optional_string(r, "displaynameshort_lang_ptbr");
        obj.displaynameshort_lang_itit = get_optional_string(r, "displaynameshort_lang_itit");
        obj.displaynameshort_lang_unk  = get_optional_string(r, "displaynameshort_lang_unk");
        obj.displaynameshort_lang_mask = r["displaynameshort_lang_mask"].as<uint32_t>();

        return obj;
    }
};