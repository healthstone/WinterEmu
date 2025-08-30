#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_charsections **/
struct DbcCharSections {
    uint32_t id;
    uint8_t raceid;
    uint8_t sexid;
    uint8_t basesection;

    std::optional<std::string> texturename_1;
    std::optional<std::string> texturename_2;
    std::optional<std::string> texturename_3;

    uint8_t flags;
    uint8_t variationindex;
    uint8_t colorindex;
};

template<>
struct PgRowMapper<DbcCharSections> {
    static DbcCharSections map(const pqxx::row& r) {
        DbcCharSections row;

        row.id             = r["id"].as<uint32_t>();
        row.raceid         = static_cast<uint8_t>(r["raceid"].as<int>());
        row.sexid          = static_cast<uint8_t>(r["sexid"].as<int>());
        row.basesection    = static_cast<uint8_t>(r["basesection"].as<int>());

        row.texturename_1  = get_optional_string(r, "texturename_1");
        row.texturename_2  = get_optional_string(r, "texturename_2");
        row.texturename_3  = get_optional_string(r, "texturename_3");

        row.flags          = static_cast<uint8_t>(r["flags"].as<int>());
        row.variationindex = static_cast<uint8_t>(r["variationindex"].as<int>());
        row.colorindex     = static_cast<uint8_t>(r["colorindex"].as<int>());

        return row;
    }
};
