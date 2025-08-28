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

/** playercreateinfo_skills **/
struct PlayerCreateInfoSkillRow {
    int16_t racemask;
    int16_t classmask;
    int16_t skill;
    int16_t rank;
    std::optional<std::string> comment;
};

template<>
struct PgRowMapper<PlayerCreateInfoSkillRow> {
    static PlayerCreateInfoSkillRow map(const pqxx::row& r) {
        PlayerCreateInfoSkillRow row;

        row.racemask  = r["racemask"].as<int16_t>();
        row.classmask = r["classmask"].as<int16_t>();
        row.skill     = r["skill"].as<int16_t>();
        row.rank      = r["rank"].as<int16_t>();
        row.comment   = get_optional_string(r, "comment");

        return row;
    }
};