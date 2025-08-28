#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"

/** dbc_skillline **/
struct DbcSkillLine {
    uint32_t ID;
    int32_t CategoryID;
    uint32_t SkillCostsID;

    // DisplayName
    std::optional<std::string> DisplayName_Lang_enUS;
    std::optional<std::string> DisplayName_Lang_enGB;
    std::optional<std::string> DisplayName_Lang_koKR;
    std::optional<std::string> DisplayName_Lang_frFR;
    std::optional<std::string> DisplayName_Lang_deDE;
    std::optional<std::string> DisplayName_Lang_enCN;
    std::optional<std::string> DisplayName_Lang_zhCN;
    std::optional<std::string> DisplayName_Lang_enTW;
    std::optional<std::string> DisplayName_Lang_zhTW;
    std::optional<std::string> DisplayName_Lang_esES;
    std::optional<std::string> DisplayName_Lang_esMX;
    std::optional<std::string> DisplayName_Lang_ruRU;
    std::optional<std::string> DisplayName_Lang_ptPT;
    std::optional<std::string> DisplayName_Lang_ptBR;
    std::optional<std::string> DisplayName_Lang_itIT;
    std::optional<std::string> DisplayName_Lang_Unk;
    uint32_t DisplayName_Lang_Mask;

    // Description
    std::optional<std::string> Description_Lang_enUS;
    std::optional<std::string> Description_Lang_enGB;
    std::optional<std::string> Description_Lang_koKR;
    std::optional<std::string> Description_Lang_frFR;
    std::optional<std::string> Description_Lang_deDE;
    std::optional<std::string> Description_Lang_enCN;
    std::optional<std::string> Description_Lang_zhCN;
    std::optional<std::string> Description_Lang_enTW;
    std::optional<std::string> Description_Lang_zhTW;
    std::optional<std::string> Description_Lang_esES;
    std::optional<std::string> Description_Lang_esMX;
    std::optional<std::string> Description_Lang_ruRU;
    std::optional<std::string> Description_Lang_ptPT;
    std::optional<std::string> Description_Lang_ptBR;
    std::optional<std::string> Description_Lang_itIT;
    std::optional<std::string> Description_Lang_Unk;
    uint32_t Description_Lang_Mask;

    uint32_t SpellIconID;

    // AlternateVerb
    std::optional<std::string> AlternateVerb_Lang_enUS;
    std::optional<std::string> AlternateVerb_Lang_enGB;
    std::optional<std::string> AlternateVerb_Lang_koKR;
    std::optional<std::string> AlternateVerb_Lang_frFR;
    std::optional<std::string> AlternateVerb_Lang_deDE;
    std::optional<std::string> AlternateVerb_Lang_enCN;
    std::optional<std::string> AlternateVerb_Lang_zhCN;
    std::optional<std::string> AlternateVerb_Lang_enTW;
    std::optional<std::string> AlternateVerb_Lang_zhTW;
    std::optional<std::string> AlternateVerb_Lang_esES;
    std::optional<std::string> AlternateVerb_Lang_esMX;
    std::optional<std::string> AlternateVerb_Lang_ruRU;
    std::optional<std::string> AlternateVerb_Lang_ptPT;
    std::optional<std::string> AlternateVerb_Lang_ptBR;
    std::optional<std::string> AlternateVerb_Lang_itIT;
    std::optional<std::string> AlternateVerb_Lang_Unk;
    uint32_t AlternateVerb_Lang_Mask;

    uint32_t CanLink;
};

template<>
struct PgRowMapper<DbcSkillLine> {
    static DbcSkillLine map(const pqxx::row& r) {
        DbcSkillLine row;

        // Числовые поля
        row.ID = r["id"].as<uint32_t>();
        row.CategoryID = r["categoryid"].as<int32_t>();
        row.SkillCostsID = r["skillcostsid"].as<uint32_t>();
        row.DisplayName_Lang_Mask = r["displayname_lang_mask"].as<uint32_t>();
        row.Description_Lang_Mask = r["description_lang_mask"].as<uint32_t>();
        row.SpellIconID = r["spelliconid"].as<uint32_t>();
        row.AlternateVerb_Lang_Mask = r["alternateverb_lang_mask"].as<uint32_t>();
        row.CanLink = r["canlink"].as<uint32_t>();

        // DisplayName
        row.DisplayName_Lang_enUS = get_optional_string(r, "displayname_lang_enus");
        row.DisplayName_Lang_enGB = get_optional_string(r, "displayname_lang_engb");
        row.DisplayName_Lang_koKR = get_optional_string(r, "displayname_lang_kokr");
        row.DisplayName_Lang_frFR = get_optional_string(r, "displayname_lang_frfr");
        row.DisplayName_Lang_deDE = get_optional_string(r, "displayname_lang_dede");
        row.DisplayName_Lang_enCN = get_optional_string(r, "displayname_lang_encn");
        row.DisplayName_Lang_zhCN = get_optional_string(r, "displayname_lang_zhcn");
        row.DisplayName_Lang_enTW = get_optional_string(r, "displayname_lang_entw");
        row.DisplayName_Lang_zhTW = get_optional_string(r, "displayname_lang_zhtw");
        row.DisplayName_Lang_esES = get_optional_string(r, "displayname_lang_eses");
        row.DisplayName_Lang_esMX = get_optional_string(r, "displayname_lang_esmx");
        row.DisplayName_Lang_ruRU = get_optional_string(r, "displayname_lang_ruru");
        row.DisplayName_Lang_ptPT = get_optional_string(r, "displayname_lang_ptpt");
        row.DisplayName_Lang_ptBR = get_optional_string(r, "displayname_lang_ptbr");
        row.DisplayName_Lang_itIT = get_optional_string(r, "displayname_lang_itit");
        row.DisplayName_Lang_Unk = get_optional_string(r, "displayname_lang_unk");

        // Description
        row.Description_Lang_enUS = get_optional_string(r, "description_lang_enus");
        row.Description_Lang_enGB = get_optional_string(r, "description_lang_engb");
        row.Description_Lang_koKR = get_optional_string(r, "description_lang_kokr");
        row.Description_Lang_frFR = get_optional_string(r, "description_lang_frfr");
        row.Description_Lang_deDE = get_optional_string(r, "description_lang_dede");
        row.Description_Lang_enCN = get_optional_string(r, "description_lang_encn");
        row.Description_Lang_zhCN = get_optional_string(r, "description_lang_zhcn");
        row.Description_Lang_enTW = get_optional_string(r, "description_lang_entw");
        row.Description_Lang_zhTW = get_optional_string(r, "description_lang_zhtw");
        row.Description_Lang_esES = get_optional_string(r, "description_lang_eses");
        row.Description_Lang_esMX = get_optional_string(r, "description_lang_esmx");
        row.Description_Lang_ruRU = get_optional_string(r, "description_lang_ruru");
        row.Description_Lang_ptPT = get_optional_string(r, "description_lang_ptpt");
        row.Description_Lang_ptBR = get_optional_string(r, "description_lang_ptbr");
        row.Description_Lang_itIT = get_optional_string(r, "description_lang_itit");
        row.Description_Lang_Unk = get_optional_string(r, "description_lang_unk");

        // AlternateVerb
        row.AlternateVerb_Lang_enUS = get_optional_string(r, "alternateverb_lang_enus");
        row.AlternateVerb_Lang_enGB = get_optional_string(r, "alternateverb_lang_engb");
        row.AlternateVerb_Lang_koKR = get_optional_string(r, "alternateverb_lang_kokr");
        row.AlternateVerb_Lang_frFR = get_optional_string(r, "alternateverb_lang_frfr");
        row.AlternateVerb_Lang_deDE = get_optional_string(r, "alternateverb_lang_dede");
        row.AlternateVerb_Lang_enCN = get_optional_string(r, "alternateverb_lang_encn");
        row.AlternateVerb_Lang_zhCN = get_optional_string(r, "alternateverb_lang_zhcn");
        row.AlternateVerb_Lang_enTW = get_optional_string(r, "alternateverb_lang_entw");
        row.AlternateVerb_Lang_zhTW = get_optional_string(r, "alternateverb_lang_zhtw");
        row.AlternateVerb_Lang_esES = get_optional_string(r, "alternateverb_lang_eses");
        row.AlternateVerb_Lang_esMX = get_optional_string(r, "alternateverb_lang_esmx");
        row.AlternateVerb_Lang_ruRU = get_optional_string(r, "alternateverb_lang_ruru");
        row.AlternateVerb_Lang_ptPT = get_optional_string(r, "alternateverb_lang_ptpt");
        row.AlternateVerb_Lang_ptBR = get_optional_string(r, "alternateverb_lang_ptbr");
        row.AlternateVerb_Lang_itIT = get_optional_string(r, "alternateverb_lang_itit");
        row.AlternateVerb_Lang_Unk = get_optional_string(r, "alternateverb_lang_unk");

        return row;
    }
};
