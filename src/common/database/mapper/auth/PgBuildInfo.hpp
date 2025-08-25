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

/** build_info **/
struct BuildInfo {
    std::optional<uint32_t> majorVersion;
    std::optional<uint32_t> minorVersion;
    std::optional<uint32_t> bugfixVersion;
    std::optional<std::string> hotfixVersion;
    uint32_t build; // Primary Key, NOT NULL
};

template<>
struct PgRowMapper<BuildInfo> {
    static BuildInfo map(const pqxx::row& r) {
        BuildInfo row;
        row.majorVersion = r["majorversion"].is_null() ? std::nullopt : std::make_optional(r["majorversion"].as<uint32_t>());
        row.minorVersion = r["minorversion"].is_null() ? std::nullopt : std::make_optional(r["minorversion"].as<uint32_t>());
        row.bugfixVersion = r["bugfixversion"].is_null() ? std::nullopt : std::make_optional(r["bugfixversion"].as<uint32_t>());
        row.hotfixVersion = r["hotfixversion"].is_null() ? std::nullopt : std::make_optional(r["hotfixversion"].as<std::string>());
        row.build = r["build"].as<uint32_t>();
        return row;
    }
};