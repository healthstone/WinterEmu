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
#include "src/game/Entity/PlayerInfo/PlayerInfo.hpp"

struct PlayerCreateInfoRow {
    uint8_t m_race;
    uint8_t m_class;
    uint16_t m_map;
    uint32_t m_zone;
    float position_x;
    float position_y;
    float position_z;
    float orientation;
};

template<>
struct PgRowMapper<PlayerCreateInfoRow> {
    static PlayerCreateInfoRow map(const pqxx::row& r) {
        PlayerCreateInfoRow row;
        row.m_race = static_cast<uint8_t>(r["race"].as<int>());
        row.m_class = static_cast<uint8_t>(r["class"].as<int>());
        row.m_map = static_cast<uint16_t>(r["map"].as<int>());
        row.m_zone = r["zone"].as<uint32_t>();
        row.position_x = r["position_x"].as<float>();
        row.position_y = r["position_y"].as<float>();
        row.position_z = r["position_z"].as<float>();
        row.orientation = r["orientation"].as<float>();
        return row;
    }
};