#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellShapeshiftForm {
    uint32_t id;
    uint32_t bonusactionbar;

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
    uint32_t flags;
    int32_t creaturetype;
    uint32_t attackiconid;
    uint32_t combatroundtime;
    uint32_t creaturedisplayid_1;
    uint32_t creaturedisplayid_2;
    uint32_t creaturedisplayid_3;
    uint32_t creaturedisplayid_4;
    uint32_t presetspellid_1;
    uint32_t presetspellid_2;
    uint32_t presetspellid_3;
    uint32_t presetspellid_4;
    uint32_t presetspellid_5;
    uint32_t presetspellid_6;
    uint32_t presetspellid_7;
    uint32_t presetspellid_8;
};

template<>
struct PgRowMapper<DbcSpellShapeshiftForm> {
    static DbcSpellShapeshiftForm map(const pqxx::row& r) {
        DbcSpellShapeshiftForm obj;
        obj.id               = r["id"].as<uint32_t>();
        obj.bonusactionbar   = r["bonusactionbar"].as<uint32_t>();

        obj.name_lang_enus   = get_optional_string(r, "name_lang_enus");
        obj.name_lang_engb   = get_optional_string(r, "name_lang_engb");
        obj.name_lang_kokr   = get_optional_string(r, "name_lang_kokr");
        obj.name_lang_frfr   = get_optional_string(r, "name_lang_frfr");
        obj.name_lang_dede   = get_optional_string(r, "name_lang_dede");
        obj.name_lang_encn   = get_optional_string(r, "name_lang_encn");
        obj.name_lang_zhcn   = get_optional_string(r, "name_lang_zhcn");
        obj.name_lang_entw   = get_optional_string(r, "name_lang_entw");
        obj.name_lang_zhtw   = get_optional_string(r, "name_lang_zhtw");
        obj.name_lang_eses   = get_optional_string(r, "name_lang_eses");
        obj.name_lang_esmx   = get_optional_string(r, "name_lang_esmx");
        obj.name_lang_ruru   = get_optional_string(r, "name_lang_ruru");
        obj.name_lang_ptpt   = get_optional_string(r, "name_lang_ptpt");
        obj.name_lang_ptbr   = get_optional_string(r, "name_lang_ptbr");
        obj.name_lang_itit   = get_optional_string(r, "name_lang_itit");
        obj.name_lang_unk    = get_optional_string(r, "name_lang_unk");

        obj.name_lang_mask   = r["name_lang_mask"].as<uint32_t>();
        obj.flags            = r["flags"].as<uint32_t>();
        obj.creaturetype     = r["creaturetype"].as<int32_t>();
        obj.attackiconid     = r["attackiconid"].as<uint32_t>();
        obj.combatroundtime  = r["combatroundtime"].as<uint32_t>();
        obj.creaturedisplayid_1 = r["creaturedisplayid_1"].as<uint32_t>();
        obj.creaturedisplayid_2 = r["creaturedisplayid_2"].as<uint32_t>();
        obj.creaturedisplayid_3 = r["creaturedisplayid_3"].as<uint32_t>();
        obj.creaturedisplayid_4 = r["creaturedisplayid_4"].as<uint32_t>();
        obj.presetspellid_1  = r["presetspellid_1"].as<uint32_t>();
        obj.presetspellid_2  = r["presetspellid_2"].as<uint32_t>();
        obj.presetspellid_3  = r["presetspellid_3"].as<uint32_t>();
        obj.presetspellid_4  = r["presetspellid_4"].as<uint32_t>();
        obj.presetspellid_5  = r["presetspellid_5"].as<uint32_t>();
        obj.presetspellid_6  = r["presetspellid_6"].as<uint32_t>();
        obj.presetspellid_7  = r["presetspellid_7"].as<uint32_t>();
        obj.presetspellid_8  = r["presetspellid_8"].as<uint32_t>();
        return obj;
    }
};