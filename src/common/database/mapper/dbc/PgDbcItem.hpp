#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_item **/
struct DbcItem {
    uint32_t id;
    uint8_t class_id;
    uint8_t subclass_id;
    int32_t sound_override_subclassid;
    int32_t material;
    uint32_t display_info_id;
    uint32_t inventory_type;
    uint32_t sheathe_type;
};

template<>
struct PgRowMapper<DbcItem> {
    static DbcItem map(const pqxx::row& r) {
        DbcItem o {
                r["id"].as<uint32_t>(),
                static_cast<uint8_t>(r["class_id"].as<int>()),
                static_cast<uint8_t>(r["subclass_id"].as<int>()),
                r["sound_override_subclassid"].as<int32_t>(),
                r["material"].as<int32_t>(),
                r["display_info_id"].as<uint32_t>(),
                r["inventory_type"].as<uint32_t>(),
                r["sheathe_type"].as<uint32_t>()
        };
        return o;
    }
};