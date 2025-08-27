#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"

struct CharacterEnumRow {
    uint32_t m_guid;                    // Изменено с UUID на uint32_t
    std::string m_name;
    uint8_t m_race;
    uint8_t m_class;
    uint8_t m_gender;
    uint8_t m_skin;
    uint8_t m_face;
    uint8_t m_hairStyle;
    uint8_t m_hairColor;
    uint8_t m_facialStyle;
    uint8_t m_level;
    uint32_t m_zone;
    uint32_t m_map;
    float m_position_x;
    float m_position_y;
    float m_position_z;
    float m_orientation;
    bool m_isTransfer;
    std::optional<uint32_t> guildid;  // Изменено с UUID на uint32_t
    uint32_t m_playerFlags;
    uint16_t m_at_login;
    std::optional<uint32_t> pet_entry;
    std::optional<uint32_t> pet_modelid;
    std::optional<uint32_t> pet_level;
    std::string m_equipmentCache;
    std::optional<uint32_t> banned_guid; // Изменено с UUID на uint32_t
};

template<>
struct PgRowMapper<CharacterEnumRow> {
    static CharacterEnumRow map(const pqxx::row& r) {
        CharacterEnumRow row;

        // GUID персонажа (теперь uint32_t)
        row.m_guid = r["guid"].as<uint32_t>();

        row.m_name = r["name"].as<std::string>();
        row.m_race = static_cast<uint8_t>(r["race"].as<int>());
        row.m_class = static_cast<uint8_t>(r["class"].as<int>());
        row.m_gender = static_cast<uint8_t>(r["gender"].as<int>());
        row.m_skin = static_cast<uint8_t>(r["skin"].as<int>());
        row.m_face = static_cast<uint8_t>(r["face"].as<int>());
        row.m_hairStyle = static_cast<uint8_t>(r["hairstyle"].as<int>());
        row.m_hairColor = static_cast<uint8_t>(r["haircolor"].as<int>());
        row.m_facialStyle = static_cast<uint8_t>(r["facialstyle"].as<int>());
        row.m_level = static_cast<uint8_t>(r["level"].as<int>());
        row.m_zone = static_cast<uint32_t>(r["zone"].as<int>());
        row.m_map = static_cast<uint32_t>(r["map"].as<int>());
        row.m_position_x = r["position_x"].as<float>();
        row.m_position_y = r["position_y"].as<float>();
        row.m_position_z = r["position_z"].as<float>();
        row.m_orientation = r["orientation"].as<float>();
        row.m_isTransfer = r["istransfer"].as<bool>();

        row.m_equipmentCache = r["equipmentcache"].as<std::string>();
        row.m_playerFlags = r["playerflags"].as<uint32_t>();
        row.m_at_login = static_cast<uint16_t>(r["at_login"].as<int>());

        // GUID гильдии (теперь uint32_t)
        if (!r["guild_guildid"].is_null()) {
            row.guildid = r["guild_guildid"].as<uint32_t>();
        }

        if (!r["pet_entry"].is_null()) row.pet_entry = r["pet_entry"].as<uint32_t>();
        if (!r["pet_modelid"].is_null()) row.pet_modelid = r["pet_modelid"].as<uint32_t>();
        if (!r["pet_level"].is_null()) row.pet_level = r["pet_level"].as<uint32_t>();

        // GUID бана (теперь uint32_t)
        if (!r["banned_guid"].is_null()) {
            row.banned_guid = r["banned_guid"].as<uint32_t>();
        }
        return row;
    }
};