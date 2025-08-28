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
        row.m_zone = r["zone"].as<uint32_t>();
        row.m_map = r["map"].as<uint32_t>();
        row.m_position_x = r["position_x"].as<float>();
        row.m_position_y = r["position_y"].as<float>();
        row.m_position_z = r["position_z"].as<float>();
        row.m_orientation = r["orientation"].as<float>();
        row.m_isTransfer = r["istransfer"].as<bool>();

        row.guildid = get_optional_number<uint32_t>(r, "guild_guildid");

        row.m_playerFlags = r["playerflags"].as<uint32_t>();
        row.m_at_login = r["at_login"].as<uint16_t>();

        row.pet_entry = get_optional_number<uint32_t>(r, "pet_entry");
        row.pet_modelid = get_optional_number<uint32_t>(r, "pet_modelid");
        row.pet_level = get_optional_number<uint32_t>(r, "pet_level");

        row.m_equipmentCache = r["equipmentcache"].as<std::string>();

        row.banned_guid = get_optional_number<uint32_t>(r, "banned_guid");

        return row;
    }
};