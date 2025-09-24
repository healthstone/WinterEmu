#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSkillTiers {
    uint32_t id;
    uint32_t cost_1;
    uint32_t cost_2;
    uint32_t cost_3;
    uint32_t cost_4;
    uint32_t cost_5;
    uint32_t cost_6;
    uint32_t cost_7;
    uint32_t cost_8;
    uint32_t cost_9;
    uint32_t cost_10;
    uint32_t cost_11;
    uint32_t cost_12;
    uint32_t cost_13;
    uint32_t cost_14;
    uint32_t cost_15;
    uint32_t cost_16;
    uint32_t value_1;
    uint32_t value_2;
    uint32_t value_3;
    uint32_t value_4;
    uint32_t value_5;
    uint32_t value_6;
    uint32_t value_7;
    uint32_t value_8;
    uint32_t value_9;
    uint32_t value_10;
    uint32_t value_11;
    uint32_t value_12;
    uint32_t value_13;
    uint32_t value_14;
    uint32_t value_15;
    uint32_t value_16;
};

template<>
struct PgRowMapper<DbcSkillTiers> {
    static DbcSkillTiers map(const pqxx::row& r) {
        DbcSkillTiers obj{};
        obj.id       = r["id"].as<uint32_t>();
        obj.cost_1   = r["cost_1"].as<uint32_t>();
        obj.cost_2   = r["cost_2"].as<uint32_t>();
        obj.cost_3   = r["cost_3"].as<uint32_t>();
        obj.cost_4   = r["cost_4"].as<uint32_t>();
        obj.cost_5   = r["cost_5"].as<uint32_t>();
        obj.cost_6   = r["cost_6"].as<uint32_t>();
        obj.cost_7   = r["cost_7"].as<uint32_t>();
        obj.cost_8   = r["cost_8"].as<uint32_t>();
        obj.cost_9   = r["cost_9"].as<uint32_t>();
        obj.cost_10  = r["cost_10"].as<uint32_t>();
        obj.cost_11  = r["cost_11"].as<uint32_t>();
        obj.cost_12  = r["cost_12"].as<uint32_t>();
        obj.cost_13  = r["cost_13"].as<uint32_t>();
        obj.cost_14  = r["cost_14"].as<uint32_t>();
        obj.cost_15  = r["cost_15"].as<uint32_t>();
        obj.cost_16  = r["cost_16"].as<uint32_t>();
        obj.value_1  = r["value_1"].as<uint32_t>();
        obj.value_2  = r["value_2"].as<uint32_t>();
        obj.value_3  = r["value_3"].as<uint32_t>();
        obj.value_4  = r["value_4"].as<uint32_t>();
        obj.value_5  = r["value_5"].as<uint32_t>();
        obj.value_6  = r["value_6"].as<uint32_t>();
        obj.value_7  = r["value_7"].as<uint32_t>();
        obj.value_8  = r["value_8"].as<uint32_t>();
        obj.value_9  = r["value_9"].as<uint32_t>();
        obj.value_10 = r["value_10"].as<uint32_t>();
        obj.value_11 = r["value_11"].as<uint32_t>();
        obj.value_12 = r["value_12"].as<uint32_t>();
        obj.value_13 = r["value_13"].as<uint32_t>();
        obj.value_14 = r["value_14"].as<uint32_t>();
        obj.value_15 = r["value_15"].as<uint32_t>();
        obj.value_16 = r["value_16"].as<uint32_t>();
        return obj;
    }
};