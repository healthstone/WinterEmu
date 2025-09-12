#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_questfactionreward **/
struct DbcQuestFactionReward {
    uint32_t id;
    int32_t difficulty_1;
    int32_t difficulty_2;
    int32_t difficulty_3;
    int32_t difficulty_4;
    int32_t difficulty_5;
    int32_t difficulty_6;
    int32_t difficulty_7;
    int32_t difficulty_8;
    int32_t difficulty_9;
    int32_t difficulty_10;
};

template<>
struct PgRowMapper<DbcQuestFactionReward> {
    static DbcQuestFactionReward map(const pqxx::row& r) {
        DbcQuestFactionReward x;
        x.id           = r["id"].as<uint32_t>();
        x.difficulty_1 = r["difficulty_1"].as<int32_t>();
        x.difficulty_2 = r["difficulty_2"].as<int32_t>();
        x.difficulty_3 = r["difficulty_3"].as<int32_t>();
        x.difficulty_4 = r["difficulty_4"].as<int32_t>();
        x.difficulty_5 = r["difficulty_5"].as<int32_t>();
        x.difficulty_6 = r["difficulty_6"].as<int32_t>();
        x.difficulty_7 = r["difficulty_7"].as<int32_t>();
        x.difficulty_8 = r["difficulty_8"].as<int32_t>();
        x.difficulty_9 = r["difficulty_9"].as<int32_t>();
        x.difficulty_10 = r["difficulty_10"].as<int32_t>();
        return x;
    }
};