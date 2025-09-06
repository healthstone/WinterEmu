#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gemproperties **/
struct DbcGemProperties {
    uint32_t id;
    uint32_t enchant_id;
    uint32_t maxcount_inv;
    uint32_t maxcount_item;
    uint32_t type;
};

template<>
struct PgRowMapper<DbcGemProperties> {
    static DbcGemProperties map(const pqxx::row& r) {
        DbcGemProperties row{};
        row.id           = r["id"].as<uint32_t>();
        row.enchant_id   = r["enchant_id"].as<uint32_t>();
        row.maxcount_inv = r["maxcount_inv"].as<uint32_t>();
        row.maxcount_item= r["maxcount_item"].as<uint32_t>();
        row.type         = r["type"].as<uint32_t>();
        return row;
    }
};