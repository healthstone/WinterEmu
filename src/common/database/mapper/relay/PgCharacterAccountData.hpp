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

struct CharacterAccountData {
    uint32_t guid;                    // Изменено с UUID на uint32_t
    uint8_t type;
    uint32_t time;
    std::vector<uint8_t> data;
};

template<>
struct PgRowMapper<CharacterAccountData> {
    static CharacterAccountData map(const pqxx::row& r) {
        CharacterAccountData row;

        // GUID персонажа
        row.guid = r["guid"].as<uint32_t>();

        // Тип данных
        row.type = static_cast<uint8_t>(r["type"].as<int>());

        // Временная метка
        row.time = r["time"].as<uint32_t>();

        // Бинарные данные через утилиту
        auto data_opt = map_binary_var(r, "data");
        row.data = data_opt.value_or(std::vector<uint8_t>{});

        return row;
    }
};