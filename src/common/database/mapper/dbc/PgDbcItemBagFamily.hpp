#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_itembagfamily **/
struct DbcItemBagFamily {
    uint32_t id;
    std::optional<std::string> name_lang_enUS;
    std::optional<std::string> name_lang_enGB;
    std::optional<std::string> name_lang_koKR;
    std::optional<std::string> name_lang_frFR;
    std::optional<std::string> name_lang_deDE;
    std::optional<std::string> name_lang_enCN;
    std::optional<std::string> name_lang_zhCN;
    std::optional<std::string> name_lang_enTW;
    std::optional<std::string> name_lang_zhTW;
    std::optional<std::string> name_lang_esES;
    std::optional<std::string> name_lang_esMX;
    std::optional<std::string> name_lang_ruRU;
    std::optional<std::string> name_lang_ptPT;
    std::optional<std::string> name_lang_ptBR;
    std::optional<std::string> name_lang_itIT;
    std::optional<std::string> name_lang_Unk;
    uint32_t name_lang_mask;
};

template<>
struct PgRowMapper<DbcItemBagFamily> {
    static DbcItemBagFamily map(const pqxx::row& r) {
        return DbcItemBagFamily{
                r["id"].as<uint32_t>(),
                get_optional_string(r, "name_lang_enUS"),
                get_optional_string(r, "name_lang_enGB"),
                get_optional_string(r, "name_lang_koKR"),
                get_optional_string(r, "name_lang_frFR"),
                get_optional_string(r, "name_lang_deDE"),
                get_optional_string(r, "name_lang_enCN"),
                get_optional_string(r, "name_lang_zhCN"),
                get_optional_string(r, "name_lang_enTW"),
                get_optional_string(r, "name_lang_zhTW"),
                get_optional_string(r, "name_lang_esES"),
                get_optional_string(r, "name_lang_esMX"),
                get_optional_string(r, "name_lang_ruRU"),
                get_optional_string(r, "name_lang_ptPT"),
                get_optional_string(r, "name_lang_ptBR"),
                get_optional_string(r, "name_lang_itIT"),
                get_optional_string(r, "name_lang_Unk"),
                r["name_lang_mask"].as<uint32_t>()
        };
    }
};