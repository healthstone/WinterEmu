#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

// === Структура ===
struct DbcItemExtendedCost {
    uint32_t id;
    uint32_t honor_points;
    uint32_t arena_points;
    uint32_t arena_bracket;
    uint32_t item_id_1;
    uint32_t item_id_2;
    uint32_t item_id_3;
    uint32_t item_id_4;
    uint32_t item_id_5;
    uint32_t item_count_1;
    uint32_t item_count_2;
    uint32_t item_count_3;
    uint32_t item_count_4;
    uint32_t item_count_5;
    uint32_t required_arena_rating;
    uint32_t item_purchase_group;
};

// === Маппер ===
template<>
struct PgRowMapper<DbcItemExtendedCost> {
    static DbcItemExtendedCost map(const pqxx::row &r) {
        DbcItemExtendedCost row{};
        row.id                   = r["id"].as<uint32_t>();
        row.honor_points         = r["honor_points"].as<uint32_t>();
        row.arena_points         = r["arena_points"].as<uint32_t>();
        row.arena_bracket        = r["arena_bracket"].as<uint32_t>();
        row.item_id_1            = r["item_id_1"].as<uint32_t>();
        row.item_id_2            = r["item_id_2"].as<uint32_t>();
        row.item_id_3            = r["item_id_3"].as<uint32_t>();
        row.item_id_4            = r["item_id_4"].as<uint32_t>();
        row.item_id_5            = r["item_id_5"].as<uint32_t>();
        row.item_count_1         = r["item_count_1"].as<uint32_t>();
        row.item_count_2         = r["item_count_2"].as<uint32_t>();
        row.item_count_3         = r["item_count_3"].as<uint32_t>();
        row.item_count_4         = r["item_count_4"].as<uint32_t>();
        row.item_count_5         = r["item_count_5"].as<uint32_t>();
        row.required_arena_rating= r["required_arena_rating"].as<uint32_t>();
        row.item_purchase_group  = r["item_purchase_group"].as<uint32_t>();
        return row;
    }
};