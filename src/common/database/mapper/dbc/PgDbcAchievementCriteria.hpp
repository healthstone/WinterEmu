#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "QueryResults.hpp"

/** dbc_achievement_criteria **/
struct DbcAchievementCriteria {
    uint32_t ID;
    uint32_t AchievementId;
    uint32_t Type;
    uint32_t AssetId;
    uint32_t Quantity;
    uint32_t StartEvent;
    uint32_t StartAsset;
    uint32_t FailEvent;
    uint32_t FailAsset;

    // Description
    std::optional<std::string> DescriptionLang_enUS;
    std::optional<std::string> DescriptionLang_enGB;
    std::optional<std::string> DescriptionLang_koKR;
    std::optional<std::string> DescriptionLang_frFR;
    std::optional<std::string> DescriptionLang_deDE;
    std::optional<std::string> DescriptionLang_enCN;
    std::optional<std::string> DescriptionLang_zhCN;
    std::optional<std::string> DescriptionLang_enTW;
    std::optional<std::string> DescriptionLang_zhTW;
    std::optional<std::string> DescriptionLang_esES;
    std::optional<std::string> DescriptionLang_esMX;
    std::optional<std::string> DescriptionLang_ruRU;
    std::optional<std::string> DescriptionLang_ptPT;
    std::optional<std::string> DescriptionLang_ptBR;
    std::optional<std::string> DescriptionLang_itIT;
    std::optional<std::string> DescriptionLang_Unk;
    uint32_t DescriptionLangMask;

    uint32_t Flags;
    uint32_t TimerStartEvent;
    uint32_t TimerAssetId;
    uint32_t TimerTime;
    uint32_t UiOrder;
};

template<>
struct PgRowMapper<DbcAchievementCriteria> {
    static DbcAchievementCriteria map(const pqxx::row& r) {
        DbcAchievementCriteria row;

        row.ID              = r["id"].as<uint32_t>();
        row.AchievementId   = r["achievement_id"].as<uint32_t>();
        row.Type            = r["type"].as<int32_t>();
        row.AssetId         = r["asset_id"].as<uint32_t>();
        row.Quantity        = r["quantity"].as<uint32_t>();
        row.StartEvent      = r["start_event"].as<uint32_t>();
        row.StartAsset      = r["start_asset"].as<uint32_t>();
        row.FailEvent       = r["fail_event"].as<uint32_t>();
        row.FailAsset       = r["fail_asset"].as<uint32_t>();

        row.DescriptionLang_enUS = get_optional_string(r, "description_lang_enus");
        row.DescriptionLang_enGB = get_optional_string(r, "description_lang_engb");
        row.DescriptionLang_koKR = get_optional_string(r, "description_lang_kokr");
        row.DescriptionLang_frFR = get_optional_string(r, "description_lang_frfr");
        row.DescriptionLang_deDE = get_optional_string(r, "description_lang_dede");
        row.DescriptionLang_enCN = get_optional_string(r, "description_lang_encn");
        row.DescriptionLang_zhCN = get_optional_string(r, "description_lang_zhcn");
        row.DescriptionLang_enTW = get_optional_string(r, "description_lang_entw");
        row.DescriptionLang_zhTW = get_optional_string(r, "description_lang_zhtw");
        row.DescriptionLang_esES = get_optional_string(r, "description_lang_eses");
        row.DescriptionLang_esMX = get_optional_string(r, "description_lang_esmx");
        row.DescriptionLang_ruRU = get_optional_string(r, "description_lang_ruru");
        row.DescriptionLang_ptPT = get_optional_string(r, "description_lang_ptpt");
        row.DescriptionLang_ptBR = get_optional_string(r, "description_lang_ptbr");
        row.DescriptionLang_itIT = get_optional_string(r, "description_lang_itit");
        row.DescriptionLang_Unk  = get_optional_string(r, "description_lang_unk");
        row.DescriptionLangMask  = r["description_lang_mask"].as<uint32_t>();

        row.Flags           = r["flags"].as<uint32_t>();
        row.TimerStartEvent = r["timer_start_event"].as<uint32_t>();
        row.TimerAssetId    = r["timer_asset_id"].as<uint32_t>();
        row.TimerTime       = r["timer_time"].as<uint32_t>();
        row.UiOrder         = r["ui_order"].as<uint32_t>();

        return row;
    }
};
