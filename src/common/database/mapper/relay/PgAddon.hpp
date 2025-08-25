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

struct Addon {
    std::string name;
    uint32_t crc;
};

template<>
struct PgRowMapper<Addon> {
    static Addon map(const pqxx::row& r) {
        Addon row;
        row.name = r["name"].as<std::string>();
        row.crc = r["crc"].as<uint32_t>();
        return row;
    }
};