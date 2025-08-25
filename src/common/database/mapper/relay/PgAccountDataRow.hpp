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

struct AccountDataRow {
    boost::uuids::uuid account_id;    // Оставляем UUID (не передается в пакетах)
    uint8_t type;
    uint32_t time;
    std::vector<uint8_t> data;
};

template<>
struct PgRowMapper<AccountDataRow> {
    static AccountDataRow map(const pqxx::row& r) {
        AccountDataRow row;

        // Парсинг UUID account_id (оставляем UUID)
        std::string account_id_str = r["account_id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.account_id = gen(account_id_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("PgRowMapper: invalid UUID string in 'account_id' field: " + account_id_str);
        }

        // Тип данных (0-7)
        row.type = static_cast<uint8_t>(r["type"].as<int>());

        // Временная метка
        row.time = r["time"].as<uint32_t>();

        // Бинарные данные
        if (!r["data"].is_null()) {
            pqxx::binarystring data_bin(r["data"]);
            row.data = std::vector<uint8_t>(data_bin.data(), data_bin.data() + data_bin.size());
        }

        return row;
    }
};