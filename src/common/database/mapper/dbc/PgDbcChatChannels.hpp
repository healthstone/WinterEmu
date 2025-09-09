#pragma once

#include <string>
#include <optional>

#include "QueryResults.hpp"

/** dbc_chatchannels **/
struct DbcChatChannels
{
    uint32_t ID;
    uint32_t Flags;
    uint32_t FactionGroup;

    // локализованные имена
    std::optional<std::string> Name_Lang_enUS;
    std::optional<std::string> Name_Lang_enGB;
    std::optional<std::string> Name_Lang_koKR;
    std::optional<std::string> Name_Lang_frFR;
    std::optional<std::string> Name_Lang_deDE;
    std::optional<std::string> Name_Lang_enCN;
    std::optional<std::string> Name_Lang_zhCN;
    std::optional<std::string> Name_Lang_enTW;
    std::optional<std::string> Name_Lang_zhTW;
    std::optional<std::string> Name_Lang_esES;
    std::optional<std::string> Name_Lang_esMX;
    std::optional<std::string> Name_Lang_ruRU;
    std::optional<std::string> Name_Lang_ptPT;
    std::optional<std::string> Name_Lang_ptBR;
    std::optional<std::string> Name_Lang_itIT;
    std::optional<std::string> Name_Lang_Unk;
    uint32_t Name_Lang_Mask;

    // локализованные сокращения
    std::optional<std::string> Shortcut_Lang_enUS;
    std::optional<std::string> Shortcut_Lang_enGB;
    std::optional<std::string> Shortcut_Lang_koKR;
    std::optional<std::string> Shortcut_Lang_frFR;
    std::optional<std::string> Shortcut_Lang_deDE;
    std::optional<std::string> Shortcut_Lang_enCN;
    std::optional<std::string> Shortcut_Lang_zhCN;
    std::optional<std::string> Shortcut_Lang_enTW;
    std::optional<std::string> Shortcut_Lang_zhTW;
    std::optional<std::string> Shortcut_Lang_esES;
    std::optional<std::string> Shortcut_Lang_esMX;
    std::optional<std::string> Shortcut_Lang_ruRU;
    std::optional<std::string> Shortcut_Lang_ptPT;
    std::optional<std::string> Shortcut_Lang_ptBR;
    std::optional<std::string> Shortcut_Lang_itIT;
    std::optional<std::string> Shortcut_Lang_Unk;
    uint32_t Shortcut_Lang_Mask;
};

template<>
struct PgRowMapper<DbcChatChannels>
{
    static DbcChatChannels map(const pqxx::row &r)
    {
        DbcChatChannels row{};

        row.ID           = r["id"].as<uint32_t>();
        row.Flags        = r["flags"].as<uint32_t>();
        row.FactionGroup = r["factiongroup"].as<uint32_t>();

        row.Name_Lang_enUS = get_optional_string(r, "name_lang_enus");
        row.Name_Lang_enGB = get_optional_string(r, "name_lang_engb");
        row.Name_Lang_koKR = get_optional_string(r, "name_lang_kokr");
        row.Name_Lang_frFR = get_optional_string(r, "name_lang_frfr");
        row.Name_Lang_deDE = get_optional_string(r, "name_lang_dede");
        row.Name_Lang_enCN = get_optional_string(r, "name_lang_encn");
        row.Name_Lang_zhCN = get_optional_string(r, "name_lang_zhcn");
        row.Name_Lang_enTW = get_optional_string(r, "name_lang_entw");
        row.Name_Lang_zhTW = get_optional_string(r, "name_lang_zhtw");
        row.Name_Lang_esES = get_optional_string(r, "name_lang_eses");
        row.Name_Lang_esMX = get_optional_string(r, "name_lang_esmx");
        row.Name_Lang_ruRU = get_optional_string(r, "name_lang_ruru");
        row.Name_Lang_ptPT = get_optional_string(r, "name_lang_ptpt");
        row.Name_Lang_ptBR = get_optional_string(r, "name_lang_ptbr");
        row.Name_Lang_itIT = get_optional_string(r, "name_lang_itit");
        row.Name_Lang_Unk  = get_optional_string(r, "name_lang_unk");
        row.Name_Lang_Mask = r["name_lang_mask"].as<uint32_t>();

        row.Shortcut_Lang_enUS = get_optional_string(r, "shortcut_lang_enus");
        row.Shortcut_Lang_enGB = get_optional_string(r, "shortcut_lang_engb");
        row.Shortcut_Lang_koKR = get_optional_string(r, "shortcut_lang_kokr");
        row.Shortcut_Lang_frFR = get_optional_string(r, "shortcut_lang_frfr");
        row.Shortcut_Lang_deDE = get_optional_string(r, "shortcut_lang_dede");
        row.Shortcut_Lang_enCN = get_optional_string(r, "shortcut_lang_encn");
        row.Shortcut_Lang_zhCN = get_optional_string(r, "shortcut_lang_zhcn");
        row.Shortcut_Lang_enTW = get_optional_string(r, "shortcut_lang_entw");
        row.Shortcut_Lang_zhTW = get_optional_string(r, "shortcut_lang_zhtw");
        row.Shortcut_Lang_esES = get_optional_string(r, "shortcut_lang_eses");
        row.Shortcut_Lang_esMX = get_optional_string(r, "shortcut_lang_esmx");
        row.Shortcut_Lang_ruRU = get_optional_string(r, "shortcut_lang_ruru");
        row.Shortcut_Lang_ptPT = get_optional_string(r, "shortcut_lang_ptpt");
        row.Shortcut_Lang_ptBR = get_optional_string(r, "shortcut_lang_ptbr");
        row.Shortcut_Lang_itIT = get_optional_string(r, "shortcut_lang_itit");
        row.Shortcut_Lang_Unk  = get_optional_string(r, "shortcut_lang_unk");
        row.Shortcut_Lang_Mask = r["shortcut_lang_mask"].as<uint32_t>();

        return row;
    }
};
