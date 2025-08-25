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

struct AccountTutorial {
    boost::uuids::uuid account_id;    // Оставляем UUID (не передается в пакетах)
    uint32_t tut0;
    uint32_t tut1;
    uint32_t tut2;
    uint32_t tut3;
    uint32_t tut4;
    uint32_t tut5;
    uint32_t tut6;
    uint32_t tut7;
};

template<>
struct PgRowMapper<AccountTutorial> {
    static AccountTutorial map(const pqxx::row& r) {
        AccountTutorial row;

        // Парсинг UUID account_id (оставляем UUID)
        std::string account_id_str = r["account_id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.account_id = gen(account_id_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("PgRowMapper: invalid UUID in account_id: " + account_id_str);
        }

        // Чтение tutorial flags
        row.tut0 = r["tut0"].as<uint32_t>();
        row.tut1 = r["tut1"].as<uint32_t>();
        row.tut2 = r["tut2"].as<uint32_t>();
        row.tut3 = r["tut3"].as<uint32_t>();
        row.tut4 = r["tut4"].as<uint32_t>();
        row.tut5 = r["tut5"].as<uint32_t>();
        row.tut6 = r["tut6"].as<uint32_t>();
        row.tut7 = r["tut7"].as<uint32_t>();

        return row;
    }
};