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

struct BannedAddonRow {
    uint32_t id;
    std::string name;
    std::string version;
    uint32_t timestamp; // UNIX timestamp в секундах
};

template<>
struct PgRowMapper<BannedAddonRow> {
    static BannedAddonRow map(const pqxx::row& r) {
        BannedAddonRow row;
        row.id = r["id"].as<uint32_t>();
        row.name = r["name"].as<std::string>();
        row.version = r["version"].as<std::string>();

        // Обработка временной метки
        if (r["timestamp"].is_null()) {
            row.timestamp = 0;
        } else {
            auto tp = TimeUtils::parse_pg_timestamp(r["timestamp"].as<std::string>());
            row.timestamp = TimeUtils::to_unix_time(tp);
        }
        return row;
    }
};