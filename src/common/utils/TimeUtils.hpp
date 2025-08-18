#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <optional>

namespace TimeUtils {

    inline std::chrono::system_clock::time_point parse_pg_timestamp(const std::string& timestamp) {
        std::tm t = {};
        std::istringstream ss(timestamp);
        ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
        if (ss.fail()) {
            throw std::runtime_error("Failed to parse timestamp: " + timestamp);
        }
        return std::chrono::system_clock::from_time_t(std::mktime(&t));
    }

    inline std::optional<std::chrono::system_clock::time_point> parse_pg_timestamp_optional(const std::string& timestamp) {
        if (timestamp.empty()) return std::nullopt;
        try {
            return parse_pg_timestamp(timestamp);
        } catch (...) {
            return std::nullopt;
        }
    }

    inline std::string parse_time_point_to_string(std::chrono::system_clock::time_point timePoint) {
        std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
        char buffer[100];
        if (std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time))) {
            return std::string(buffer);
        } else {
            return "";
        }
    }

    inline uint32_t to_unix_time(const std::chrono::system_clock::time_point& tp) {
        return static_cast<uint32_t>(
                std::chrono::duration_cast<std::chrono::seconds>(
                        tp.time_since_epoch()).count());
    }

} // namespace TimeUtils
