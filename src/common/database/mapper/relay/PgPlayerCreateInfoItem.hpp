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

struct PlayerCreateInfoItemRow {
    uint8_t race;
    uint8_t class_;
    uint32_t itemid;
    int16_t amount;
};

template<>
struct PgRowMapper<PlayerCreateInfoItemRow> {
    static PlayerCreateInfoItemRow map(const pqxx::row& r) {
        PlayerCreateInfoItemRow row;
        row.race = static_cast<uint8_t>(r["race"].as<int>());
        row.class_ = static_cast<uint8_t>(r["class"].as<int>());
        row.itemid = r["itemid"].as<uint32_t>();
        row.amount = static_cast<int16_t>(r["amount"].as<int>());
        return row;
    }
};