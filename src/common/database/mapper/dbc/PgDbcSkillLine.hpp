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
    uint64_t DisplayName_Lang_Mask;

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
    uint64_t Description_Lang_Mask;

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
    uint64_t AlternateVerb_Lang_Mask;

    uint32_t CanLink;
};

template<>
struct PgRowMapper<DbcSkillLine> {
    static DbcSkillLine map(const pqxx::row& r) {
        DbcSkillLine row;

        row.ID = static_cast<uint32_t>(r["id"].as<int32_t>());
        row.CategoryID = r["categoryid"].as<int32_t>();
        row.SkillCostsID = static_cast<uint32_t>(r["skillcostsid"].as<int32_t>());

        auto getOptString = [&r](const char* col) -> std::optional<std::string> {
            return r[col].is_null() ? std::nullopt : std::optional<std::string>{r[col].as<std::string>()};
        };

        // DisplayName
        row.DisplayName_Lang_enUS = getOptString("displayname_lang_enus");
        row.DisplayName_Lang_enGB = getOptString("displayname_lang_engb");
        row.DisplayName_Lang_koKR = getOptString("displayname_lang_kokr");
        row.DisplayName_Lang_frFR = getOptString("displayname_lang_frfr");
        row.DisplayName_Lang_deDE = getOptString("displayname_lang_dede");
        row.DisplayName_Lang_enCN = getOptString("displayname_lang_encn");
        row.DisplayName_Lang_zhCN = getOptString("displayname_lang_zhcn");
        row.DisplayName_Lang_enTW = getOptString("displayname_lang_entw");
        row.DisplayName_Lang_zhTW = getOptString("displayname_lang_zhtw");
        row.DisplayName_Lang_esES = getOptString("displayname_lang_eses");
        row.DisplayName_Lang_esMX = getOptString("displayname_lang_esmx");
        row.DisplayName_Lang_ruRU = getOptString("displayname_lang_ruru");
        row.DisplayName_Lang_ptPT = getOptString("displayname_lang_ptpt");
        row.DisplayName_Lang_ptBR = getOptString("displayname_lang_ptbr");
        row.DisplayName_Lang_itIT = getOptString("displayname_lang_itit");
        row.DisplayName_Lang_Unk = getOptString("displayname_lang_unk");
        row.DisplayName_Lang_Mask = static_cast<uint64_t>(r["displayname_lang_mask"].as<int64_t>());

        // Description
        row.Description_Lang_enUS = getOptString("description_lang_enus");
        row.Description_Lang_enGB = getOptString("description_lang_engb");
        row.Description_Lang_koKR = getOptString("description_lang_kokr");
        row.Description_Lang_frFR = getOptString("description_lang_frfr");
        row.Description_Lang_deDE = getOptString("description_lang_dede");
        row.Description_Lang_enCN = getOptString("description_lang_encn");
        row.Description_Lang_zhCN = getOptString("description_lang_zhcn");
        row.Description_Lang_enTW = getOptString("description_lang_entw");
        row.Description_Lang_zhTW = getOptString("description_lang_zhtw");
        row.Description_Lang_esES = getOptString("description_lang_eses");
        row.Description_Lang_esMX = getOptString("description_lang_esmx");
        row.Description_Lang_ruRU = getOptString("description_lang_ruru");
        row.Description_Lang_ptPT = getOptString("description_lang_ptpt");
        row.Description_Lang_ptBR = getOptString("description_lang_ptbr");
        row.Description_Lang_itIT = getOptString("description_lang_itit");
        row.Description_Lang_Unk = getOptString("description_lang_unk");
        row.Description_Lang_Mask = static_cast<uint64_t>(r["description_lang_mask"].as<int64_t>());

        row.SpellIconID = static_cast<uint32_t>(r["spelliconid"].as<int32_t>());

        // AlternateVerb
        row.AlternateVerb_Lang_enUS = getOptString("alternateverb_lang_enus");
        row.AlternateVerb_Lang_enGB = getOptString("alternateverb_lang_engb");
        row.AlternateVerb_Lang_koKR = getOptString("alternateverb_lang_kokr");
        row.AlternateVerb_Lang_frFR = getOptString("alternateverb_lang_frfr");
        row.AlternateVerb_Lang_deDE = getOptString("alternateverb_lang_dede");
        row.AlternateVerb_Lang_enCN = getOptString("alternateverb_lang_encn");
        row.AlternateVerb_Lang_zhCN = getOptString("alternateverb_lang_zhcn");
        row.AlternateVerb_Lang_enTW = getOptString("alternateverb_lang_entw");
        row.AlternateVerb_Lang_zhTW = getOptString("alternateverb_lang_zhtw");
        row.AlternateVerb_Lang_esES = getOptString("alternateverb_lang_eses");
        row.AlternateVerb_Lang_esMX = getOptString("alternateverb_lang_esmx");
        row.AlternateVerb_Lang_ruRU = getOptString("alternateverb_lang_ruru");
        row.AlternateVerb_Lang_ptPT = getOptString("alternateverb_lang_ptpt");
        row.AlternateVerb_Lang_ptBR = getOptString("alternateverb_lang_ptbr");
        row.AlternateVerb_Lang_itIT = getOptString("alternateverb_lang_itit");
        row.AlternateVerb_Lang_Unk = getOptString("alternateverb_lang_unk");
        row.AlternateVerb_Lang_Mask = static_cast<uint64_t>(r["alternateverb_lang_mask"].as<int64_t>());

        row.CanLink = static_cast<uint32_t>(r["canlink"].as<int32_t>());

        return row;
    }
};
