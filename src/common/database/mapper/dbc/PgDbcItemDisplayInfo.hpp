#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

// === Структура ===
struct DbcItemDisplayInfo {
    uint32_t id;
    std::optional<std::string> model_name_1;
    std::optional<std::string> model_name_2;
    std::optional<std::string> model_texture_1;
    std::optional<std::string> model_texture_2;
    std::optional<std::string> inventory_icon_1;
    std::optional<std::string> inventory_icon_2;
    uint32_t geoset_group_1;
    uint32_t geoset_group_2;
    uint32_t geoset_group_3;
    uint32_t flags;
    uint32_t spell_visual_id;
    uint32_t group_sound_index;
    uint32_t helmet_geoset_vis_1;
    uint32_t helmet_geoset_vis_2;
    std::optional<std::string> texture_1;
    std::optional<std::string> texture_2;
    std::optional<std::string> texture_3;
    std::optional<std::string> texture_4;
    std::optional<std::string> texture_5;
    std::optional<std::string> texture_6;
    std::optional<std::string> texture_7;
    std::optional<std::string> texture_8;
    int32_t item_visual;
    uint32_t particle_color_id;
};

// === Маппер ===
template<>
struct PgRowMapper<DbcItemDisplayInfo> {
    static DbcItemDisplayInfo map(const pqxx::row &r) {
        DbcItemDisplayInfo row{};
        row.id = r["id"].as<uint32_t>();
        row.model_name_1 = get_optional_string(r, "model_name_1");
        row.model_name_2 = get_optional_string(r, "model_name_2");
        row.model_texture_1 = get_optional_string(r, "model_texture_1");
        row.model_texture_2 = get_optional_string(r, "model_texture_2");
        row.inventory_icon_1 = get_optional_string(r, "inventory_icon_1");
        row.inventory_icon_2 = get_optional_string(r, "inventory_icon_2");
        row.geoset_group_1 = r["geoset_group_1"].as<uint32_t>();
        row.geoset_group_2 = r["geoset_group_2"].as<uint32_t>();
        row.geoset_group_3 = r["geoset_group_3"].as<uint32_t>();
        row.flags = r["flags"].as<uint32_t>();
        row.spell_visual_id = r["spell_visual_id"].as<uint32_t>();
        row.group_sound_index = r["group_sound_index"].as<uint32_t>();
        row.helmet_geoset_vis_1 = r["helmet_geoset_vis_1"].as<uint32_t>();
        row.helmet_geoset_vis_2 = r["helmet_geoset_vis_2"].as<uint32_t>();
        row.texture_1 = get_optional_string(r, "texture_1");
        row.texture_2 = get_optional_string(r, "texture_2");
        row.texture_3 = get_optional_string(r, "texture_3");
        row.texture_4 = get_optional_string(r, "texture_4");
        row.texture_5 = get_optional_string(r, "texture_5");
        row.texture_6 = get_optional_string(r, "texture_6");
        row.texture_7 = get_optional_string(r, "texture_7");
        row.texture_8 = get_optional_string(r, "texture_8");
        row.item_visual = r["item_visual"].as<int32_t>();
        row.particle_color_id = r["particle_color_id"].as<uint32_t>();
        return row;
    }
};