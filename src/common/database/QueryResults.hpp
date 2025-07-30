#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>

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

struct NothingRow {};

// === Шаблон PgRowMapper ===

template<typename T>
struct PgRowMapper;

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
