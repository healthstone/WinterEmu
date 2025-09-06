#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_glyphslot **/
struct DbcGlyphSlot {
    uint32_t id;
    uint32_t type;
    uint32_t tooltip;
};

template<>
struct PgRowMapper<DbcGlyphSlot> {
    static DbcGlyphSlot map(const pqxx::row& r) {
        DbcGlyphSlot row{};
        row.id      = r["id"].as<uint32_t>();
        row.type    = r["type"].as<uint32_t>();
        row.tooltip = r["tooltip"].as<uint32_t>();
        return row;
    }
};
