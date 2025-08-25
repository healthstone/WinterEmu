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
        row.build = r["build"].as<uint32_t>();
        row.platform = r["platform"].as<std::string>();

        pqxx::binarystring hash_bin(r["executableHash"]);
        if (hash_bin.size() != row.executableHash.size())
            throw std::runtime_error("PgRowMapper: Invalid executableHash size, expected 20 bytes");
        std::copy_n(hash_bin.begin(), hash_bin.size(), row.executableHash.begin());

        return row;
    }
};