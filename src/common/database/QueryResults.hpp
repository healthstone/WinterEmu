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

// универсальная функция для извлечения optional<T>
template <typename T>
static std::optional<T> get_optional(const pqxx::row& r, const std::string& col) {
    return r[col].is_null() ? std::optional<T>{} : std::optional<T>{r[col].as<T>()};
}

// строковые
inline std::optional<std::string> get_optional_string(const pqxx::row& r, const std::string& col) {
    return r[col].is_null() ? std::nullopt : std::make_optional(r[col].as<std::string>());
}

// числовые
template <typename T>
inline std::optional<T> get_optional_number(const pqxx::row& r, const std::string& col) {
    return r[col].is_null() ? std::nullopt : std::make_optional(r[col].as<T>());
}

// === Бинарные поля ===

// фиксированный размер, например std::array<uint8_t, 32>
template <std::size_t N>
inline std::optional<std::array<uint8_t, N>> map_binary_fixed(const pqxx::row& r, const std::string& col) {
    if (r[col].is_null())
        return std::nullopt;

    pqxx::binarystring bin(r[col]);
    if (bin.size() != N)
        throw std::runtime_error("map_binary_fixed: invalid size for column '" + col +
                                 "', expected " + std::to_string(N) +
                                 ", got " + std::to_string(bin.size()));

    std::array<uint8_t, N> arr{};
    std::copy_n(bin.begin(), N, arr.begin());
    return arr;
}

// переменный размер, возвращаем std::vector<uint8_t>
inline std::optional<std::vector<uint8_t>> map_binary_var(const pqxx::row& r, const std::string& col) {
    if (r[col].is_null())
        return std::nullopt;

    pqxx::binarystring bin(r[col]);
    return std::vector<uint8_t>(bin.begin(), bin.end());
}

template<>
struct PgRowMapper<uint64_t> {
    static uint64_t map(const pqxx::row &r) {
        return static_cast<uint64_t>(r[0].as<int64_t>());
    }
};