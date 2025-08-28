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

/** dbc_achievement **/
struct DbcAchievement {
    uint32_t ID;
    int32_t Faction;
    int32_t InstanceID;
    uint32_t Supercedes;

    // Title
    std::optional<std::string> Title_Lang_enUS;
    std::optional<std::string> Title_Lang_enGB;
    std::optional<std::string> Title_Lang_koKR;
    std::optional<std::string> Title_Lang_frFR;
    std::optional<std::string> Title_Lang_deDE;
    std::optional<std::string> Title_Lang_enCN;
    std::optional<std::string> Title_Lang_zhCN;
    std::optional<std::string> Title_Lang_enTW;
    std::optional<std::string> Title_Lang_zhTW;
    std::optional<std::string> Title_Lang_esES;
    std::optional<std::string> Title_Lang_esMX;
    std::optional<std::string> Title_Lang_ruRU;
    std::optional<std::string> Title_Lang_ptPT;
    std::optional<std::string> Title_Lang_ptBR;
    std::optional<std::string> Title_Lang_itIT;
    std::optional<std::string> Title_Lang_Unk;
    uint32_t Title_Lang_Mask;

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

    uint32_t Category;
    uint32_t Points;
    uint32_t UiOrder;
    uint32_t Flags;
    uint32_t IconID;

    // Reward
    std::optional<std::string> Reward_Lang_enUS;
    std::optional<std::string> Reward_Lang_enGB;
    std::optional<std::string> Reward_Lang_koKR;
    std::optional<std::string> Reward_Lang_frFR;
    std::optional<std::string> Reward_Lang_deDE;
    std::optional<std::string> Reward_Lang_enCN;
    std::optional<std::string> Reward_Lang_zhCN;
    std::optional<std::string> Reward_Lang_enTW;
    std::optional<std::string> Reward_Lang_zhTW;
    std::optional<std::string> Reward_Lang_esES;
    std::optional<std::string> Reward_Lang_esMX;
    std::optional<std::string> Reward_Lang_ruRU;
    std::optional<std::string> Reward_Lang_ptPT;
    std::optional<std::string> Reward_Lang_ptBR;
    std::optional<std::string> Reward_Lang_itIT;
    std::optional<std::string> Reward_Lang_Unk;
    uint32_t Reward_Lang_Mask;

    uint32_t Minimum_Criteria;
    uint32_t Shares_Criteria;
};

template<>
struct PgRowMapper<DbcAchievement> {

    static DbcAchievement map(const pqxx::row& r) {
        DbcAchievement row;

        row.ID = r["id"].as<uint32_t>();
        row.Faction = r["faction"].as<int32_t>();
        row.InstanceID = r["instance_id"].as<int32_t>();
        row.Supercedes = r["supercedes"].as<uint32_t>();

        // Title
        row.Title_Lang_enUS = get_optional_string(r, "title_lang_enus");
        row.Title_Lang_enGB = get_optional_string(r, "title_lang_engb");
        row.Title_Lang_koKR = get_optional_string(r, "title_lang_kokr");
        row.Title_Lang_frFR = get_optional_string(r, "title_lang_frfr");
        row.Title_Lang_deDE = get_optional_string(r, "title_lang_dede");
        row.Title_Lang_enCN = get_optional_string(r, "title_lang_encn");
        row.Title_Lang_zhCN = get_optional_string(r, "title_lang_zhcn");
        row.Title_Lang_enTW = get_optional_string(r, "title_lang_entw");
        row.Title_Lang_zhTW = get_optional_string(r, "title_lang_zhtw");
        row.Title_Lang_esES = get_optional_string(r, "title_lang_eses");
        row.Title_Lang_esMX = get_optional_string(r, "title_lang_esmx");
        row.Title_Lang_ruRU = get_optional_string(r, "title_lang_ruru");
        row.Title_Lang_ptPT = get_optional_string(r, "title_lang_ptpt");
        row.Title_Lang_ptBR = get_optional_string(r, "title_lang_ptbr");
        row.Title_Lang_itIT = get_optional_string(r, "title_lang_itit");
        row.Title_Lang_Unk = get_optional_string(r, "title_lang_unk");
        row.Title_Lang_Mask = r["title_lang_mask"].as<uint32_t>();

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
        row.Description_Lang_Mask = r["description_lang_mask"].as<uint32_t>();

        row.Category = r["category"].as<uint32_t>();
        row.Points = r["points"].as<uint32_t>();
        row.UiOrder = r["ui_order"].as<uint32_t>();
        row.Flags = r["flags"].as<uint32_t>();
        row.IconID = r["iconid"].as<uint32_t>();

        // Reward
        row.Reward_Lang_enUS = get_optional_string(r, "reward_lang_enus");
        row.Reward_Lang_enGB = get_optional_string(r, "reward_lang_engb");
        row.Reward_Lang_koKR = get_optional_string(r, "reward_lang_kokr");
        row.Reward_Lang_frFR = get_optional_string(r, "reward_lang_frfr");
        row.Reward_Lang_deDE = get_optional_string(r, "reward_lang_dede");
        row.Reward_Lang_enCN = get_optional_string(r, "reward_lang_encn");
        row.Reward_Lang_zhCN = get_optional_string(r, "reward_lang_zhcn");
        row.Reward_Lang_enTW = get_optional_string(r, "reward_lang_entw");
        row.Reward_Lang_zhTW = get_optional_string(r, "reward_lang_zhtw");
        row.Reward_Lang_esES = get_optional_string(r, "reward_lang_eses");
        row.Reward_Lang_esMX = get_optional_string(r, "reward_lang_esmx");
        row.Reward_Lang_ruRU = get_optional_string(r, "reward_lang_ruru");
        row.Reward_Lang_ptPT = get_optional_string(r, "reward_lang_ptpt");
        row.Reward_Lang_ptBR = get_optional_string(r, "reward_lang_ptbr");
        row.Reward_Lang_itIT = get_optional_string(r, "reward_lang_itit");
        row.Reward_Lang_Unk = get_optional_string(r, "reward_lang_unk");
        row.Reward_Lang_Mask = r["reward_lang_mask"].as<uint32_t>();

        row.Minimum_Criteria = r["minimum_criteria"].as<uint32_t>();
        row.Shares_Criteria = r["shares_criteria"].as<uint32_t>();

        return row;
    }
};
