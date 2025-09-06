#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_glyphproperties **/
struct DbcGlyphProperties {
    uint32_t id;
    uint32_t spellid;
    uint32_t glyphslotflags;
    uint32_t spelliconid;
};

template<>
struct PgRowMapper<DbcGlyphProperties> {
    static DbcGlyphProperties map(const pqxx::row& r) {
        DbcGlyphProperties row{};
        row.id             = r["id"].as<uint32_t>();
        row.spellid        = r["spellid"].as<uint32_t>();
        row.glyphslotflags = r["glyphslotflags"].as<uint32_t>();
        row.spelliconid    = r["spelliconid"].as<uint32_t>();
        return row;
    }
};