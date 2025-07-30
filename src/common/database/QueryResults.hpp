#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"

// === Структуры ===

struct AccountsRow {
    uint64_t id;
    std::optional<std::string> name;
    std::optional<std::array<uint8_t, 32>> salt;
    std::optional<std::array<uint8_t, 32>> verifier;
    std::optional<std::string> email;
    std::optional<std::chrono::system_clock::time_point> created_at;
};

struct BuildInfoRow {
    std::optional<uint32_t> majorVersion;
    std::optional<uint32_t> minorVersion;
    std::optional<uint32_t> bugfixVersion;
    std::optional<std::string> hotfixVersion;
    uint32_t build; // Primary Key, NOT NULL
};

struct BuildExeHashRow {
    uint32_t build;
    std::string platform;
    std::array<uint8_t, 20> executableHash; // Например SHA1 = 20 байт
};

struct RealmRow {
    uint32_t id;
    std::string name;
    std::string address;
    std::string localAddress;
    std::string localSubnetMask;
    uint16_t port;
    uint8_t icon;
    uint8_t flag;
    uint8_t timezone;
    uint8_t securityLevel;
    float population;
    uint32_t build;
};

struct NothingRow {};

// === Шаблон PgRowMapper ===

template<typename T>
struct PgRowMapper;


template<>
struct PgRowMapper<RealmRow> {
    static RealmRow map(const pqxx::row& r) {
        RealmRow row;

        row.id = r["id"].as<uint32_t>();
        row.name = r["name"].as<std::string>();
        row.address = r["address"].as<std::string>();
        row.localAddress = r["local_address"].as<std::string>();
        row.localSubnetMask = r["local_subnet_mask"].as<std::string>();
        row.port = static_cast<uint16_t>(r["port"].as<int>());
        row.icon = static_cast<uint8_t>(r["icon"].as<int>());
        row.flag = static_cast<uint8_t>(r["flag"].as<int>());
        row.timezone = static_cast<uint8_t>(r["timezone"].as<int>());
        row.securityLevel = static_cast<uint8_t>(r["allowed_security_level"].as<int>());
        row.population = r["population"].as<float>();
        row.build = r["gamebuild"].as<uint32_t>();

        return row;
    }
};

template<>
struct PgRowMapper<BuildInfoRow> {
    static BuildInfoRow map(const pqxx::row& r) {
        BuildInfoRow row;
        row.majorVersion = r["majorversion"].is_null() ? std::nullopt : std::make_optional(r["majorversion"].as<uint32_t>());
        row.minorVersion = r["minorversion"].is_null() ? std::nullopt : std::make_optional(r["minorversion"].as<uint32_t>());
        row.bugfixVersion = r["bugfixversion"].is_null() ? std::nullopt : std::make_optional(r["bugfixversion"].as<uint32_t>());
        row.hotfixVersion = r["hotfixversion"].is_null() ? std::nullopt : std::make_optional(r["hotfixversion"].as<std::string>());
        row.build = r["build"].as<uint32_t>();
        return row;
    }
};

template<>
struct PgRowMapper<BuildExeHashRow> {
    static BuildExeHashRow map(const pqxx::row& r) {
        BuildExeHashRow row;
        row.build = r["build"].as<uint32_t>();
        row.platform = r["platform"].as<std::string>();

        pqxx::binarystring hash_bin(r["executableHash"]);
        if (hash_bin.size() != row.executableHash.size())
            throw std::runtime_error("PgRowMapper: Invalid executableHash size, expected 20 bytes");
        std::copy_n(hash_bin.begin(), hash_bin.size(), row.executableHash.begin());

        return row;
    }
};

template<>
struct PgRowMapper<AccountsRow> {
    static AccountsRow map(const pqxx::row &r) {
        AccountsRow row;

        // PostgreSQL BIGINT -> int64_t -> uint64_t ( 0 … 9223372036854775807 )
        row.id = static_cast<uint64_t>(r["id"].as<int64_t>());

        if (!r["username"].is_null())
            row.name = r["username"].as<std::string>();

        if (!r["salt"].is_null()) {
            pqxx::binarystring salt_bin(r["salt"]);
            if (salt_bin.size() != 32)
                throw std::runtime_error("PgRowMapper: Invalid salt size, expected 32 bytes");
            std::array<uint8_t, 32> salt_arr{};
            std::copy_n(salt_bin.begin(), 32, salt_arr.begin());
            row.salt = salt_arr;
        }

        if (!r["verifier"].is_null()) {
            pqxx::binarystring verifier_bin(r["verifier"]);
            if (verifier_bin.size() != 32)
                throw std::runtime_error("PgRowMapper: Invalid verifier size, expected 32 bytes");
            std::array<uint8_t, 32> verifier_arr{};
            std::copy_n(verifier_bin.begin(), 32, verifier_arr.begin());
            row.verifier = verifier_arr;
        }

        if (!r["email"].is_null())
            row.email = r["email"].as<std::string>();

        if (!r["created_at"].is_null())
            row.created_at = TimeUtils::parse_pg_timestamp_optional(r["created_at"].as<std::string>());

        return row;
    }
};

template<>
struct PgRowMapper<NothingRow> {
    static NothingRow map(const pqxx::row &) {
        return NothingRow{};
    }
};

template<>
struct PgRowMapper<uint64_t> {
    static uint64_t map(const pqxx::row &r) {
        return static_cast<uint64_t>(r[0].as<int64_t>());
    }
};
