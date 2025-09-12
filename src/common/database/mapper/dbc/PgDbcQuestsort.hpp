#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_questsort **/
struct DbcQuestsort {
    uint32_t id;
    std::optional<std::string> sortname_lang_enus;
    std::optional<std::string> sortname_lang_engb;
    std::optional<std::string> sortname_lang_kokr;
    std::optional<std::string> sortname_lang_frfr;
    std::optional<std::string> sortname_lang_dede;
    std::optional<std::string> sortname_lang_encn;
    std::optional<std::string> sortname_lang_zhcn;
    std::optional<std::string> sortname_lang_entw;
    std::optional<std::string> sortname_lang_zhtw;
    std::optional<std::string> sortname_lang_eses;
    std::optional<std::string> sortname_lang_esmx;
    std::optional<std::string> sortname_lang_ruru;
    std::optional<std::string> sortname_lang_ptpt;
    std::optional<std::string> sortname_lang_ptbr;
    std::optional<std::string> sortname_lang_itit;
    std::optional<std::string> sortname_lang_unk;
    uint32_t sortname_lang_mask;
};

template<>
struct PgRowMapper<DbcQuestsort> {
    static DbcQuestsort map(const pqxx::row& r) {
        return DbcQuestsort{
                r["id"].as<uint32_t>(),
                get_optional_string(r, "sortname_lang_enus"),
                get_optional_string(r, "sortname_lang_engb"),
                get_optional_string(r, "sortname_lang_kokr"),
                get_optional_string(r, "sortname_lang_frfr"),
                get_optional_string(r, "sortname_lang_dede"),
                get_optional_string(r, "sortname_lang_encn"),
                get_optional_string(r, "sortname_lang_zhcn"),
                get_optional_string(r, "sortname_lang_entw"),
                get_optional_string(r, "sortname_lang_zhtw"),
                get_optional_string(r, "sortname_lang_eses"),
                get_optional_string(r, "sortname_lang_esmx"),
                get_optional_string(r, "sortname_lang_ruru"),
                get_optional_string(r, "sortname_lang_ptpt"),
                get_optional_string(r, "sortname_lang_ptbr"),
                get_optional_string(r, "sortname_lang_itit"),
                get_optional_string(r, "sortname_lang_unk"),
                r["sortname_lang_mask"].as<uint32_t>()
        };
    }
};