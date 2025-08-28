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

/** build_executable_hash **/
struct BuildExeHash {
    uint32_t build;
    std::string platform;
    std::array<uint8_t, 20> executableHash; // Например SHA1 = 20 байт
};

template<>
struct PgRowMapper<BuildExeHash> {
    static BuildExeHash map(const pqxx::row& r) {
        BuildExeHash row;

        // обычные поля
        row.build    = r["build"].as<uint32_t>();
        row.platform = r["platform"].as<std::string>();

        // бинарное поле фиксированного размера (20 байт)
        auto hash_opt = map_binary_fixed<20>(r, "executableHash");
        if (!hash_opt)
            throw std::runtime_error("PgRowMapper: executableHash is null");
        row.executableHash = *hash_opt;

        return row;
    }
};
