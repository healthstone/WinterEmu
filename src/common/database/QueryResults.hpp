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

// === Структуры ===
struct NothingRow {};

// === Шаблон PgRowMapper ===
template<typename T>
struct PgRowMapper;

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