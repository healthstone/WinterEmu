#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellItemEnchantment {
    uint32_t id;
    uint32_t charges;
    uint32_t effect_1;
    uint32_t effect_2;
    uint32_t effect_3;
    int32_t effectpointsmin_1;
    int32_t effectpointsmin_2;
    int32_t effectpointsmin_3;
    int32_t effectpointsmax_1;
    int32_t effectpointsmax_2;
    int32_t effectpointsmax_3;
    uint32_t effectarg_1;
    uint32_t effectarg_2;
    uint32_t effectarg_3;
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
    int32_t itemvisual;
    uint32_t flags;
    uint32_t src_itemid;
    uint32_t condition_id;
    uint32_t requiredskillid;
    uint32_t requiredskillrank;
    uint32_t minlevel;
};

template<>
struct PgRowMapper<DbcSpellItemEnchantment> {
    static DbcSpellItemEnchantment map(const pqxx::row& r) {
        DbcSpellItemEnchantment row{};
        row.id                = r["id"].as<uint32_t>();
        row.charges           = r["charges"].as<uint32_t>();
        row.effect_1          = r["effect_1"].as<uint32_t>();
        row.effect_2          = r["effect_2"].as<uint32_t>();
        row.effect_3          = r["effect_3"].as<uint32_t>();
        row.effectpointsmin_1 = r["effectpointsmin_1"].as<int32_t>();
        row.effectpointsmin_2 = r["effectpointsmin_2"].as<int32_t>();
        row.effectpointsmin_3 = r["effectpointsmin_3"].as<int32_t>();
        row.effectpointsmax_1 = r["effectpointsmax_1"].as<int32_t>();
        row.effectpointsmax_2 = r["effectpointsmax_2"].as<int32_t>();
        row.effectpointsmax_3 = r["effectpointsmax_3"].as<int32_t>();
        row.effectarg_1       = r["effectarg_1"].as<uint32_t>();
        row.effectarg_2       = r["effectarg_2"].as<uint32_t>();
        row.effectarg_3       = r["effectarg_3"].as<uint32_t>();

        row.name_lang_enus    = get_optional_string(r, "name_lang_enus");
        row.name_lang_engb    = get_optional_string(r, "name_lang_engb");
        row.name_lang_kokr    = get_optional_string(r, "name_lang_kokr");
        row.name_lang_frfr    = get_optional_string(r, "name_lang_frfr");
        row.name_lang_dede    = get_optional_string(r, "name_lang_dede");
        row.name_lang_encn    = get_optional_string(r, "name_lang_encn");
        row.name_lang_zhcn    = get_optional_string(r, "name_lang_zhcn");
        row.name_lang_entw    = get_optional_string(r, "name_lang_entw");
        row.name_lang_zhtw    = get_optional_string(r, "name_lang_zhtw");
        row.name_lang_eses    = get_optional_string(r, "name_lang_eses");
        row.name_lang_esmx    = get_optional_string(r, "name_lang_esmx");
        row.name_lang_ruru    = get_optional_string(r, "name_lang_ruru");
        row.name_lang_ptpt    = get_optional_string(r, "name_lang_ptpt");
        row.name_lang_ptbr    = get_optional_string(r, "name_lang_ptbr");
        row.name_lang_itit    = get_optional_string(r, "name_lang_itit");
        row.name_lang_unk     = get_optional_string(r, "name_lang_unk");

        row.name_lang_mask    = r["name_lang_mask"].as<uint32_t>();
        row.itemvisual        = r["itemvisual"].as<int32_t>();
        row.flags             = r["flags"].as<uint32_t>();
        row.src_itemid        = r["src_itemid"].as<uint32_t>();
        row.condition_id      = r["condition_id"].as<uint32_t>();
        row.requiredskillid   = r["requiredskillid"].as<uint32_t>();
        row.requiredskillrank = r["requiredskillrank"].as<uint32_t>();
        row.minlevel          = r["minlevel"].as<uint32_t>();
        return row;
    }
};