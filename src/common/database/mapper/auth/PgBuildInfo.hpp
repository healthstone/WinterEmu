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

        // числовые поля через get_optional_number
        row.majorVersion  = get_optional_number<uint32_t>(r, "majorversion");
        row.minorVersion  = get_optional_number<uint32_t>(r, "minorversion");
        row.bugfixVersion = get_optional_number<uint32_t>(r, "bugfixversion");

        // строковое поле через get_optional_string
        row.hotfixVersion = get_optional_string(r, "hotfixversion");

        // обычное числовое поле
        row.build = r["build"].as<uint32_t>();

        return row;
    }
};
