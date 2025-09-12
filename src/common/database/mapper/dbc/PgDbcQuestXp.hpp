#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_questxp **/
struct DbcQuestXp {
    uint32_t id;
    uint32_t difficulty_1;
    uint32_t difficulty_2;
    uint32_t difficulty_3;
    uint32_t difficulty_4;
    uint32_t difficulty_5;
    uint32_t difficulty_6;
    uint32_t difficulty_7;
    uint32_t difficulty_8;
    uint32_t difficulty_9;
    uint32_t difficulty_10;
};

template<>
struct PgRowMapper<DbcQuestXp> {
    static DbcQuestXp map(const pqxx::row& r) {
        DbcQuestXp obj{};
        obj.id           = r["id"].as<uint32_t>();
        obj.difficulty_1 = r["difficulty_1"].as<uint32_t>();
        obj.difficulty_2 = r["difficulty_2"].as<uint32_t>();
        obj.difficulty_3 = r["difficulty_3"].as<uint32_t>();
        obj.difficulty_4 = r["difficulty_4"].as<uint32_t>();
        obj.difficulty_5 = r["difficulty_5"].as<uint32_t>();
        obj.difficulty_6 = r["difficulty_6"].as<uint32_t>();
        obj.difficulty_7 = r["difficulty_7"].as<uint32_t>();
        obj.difficulty_8 = r["difficulty_8"].as<uint32_t>();
        obj.difficulty_9 = r["difficulty_9"].as<uint32_t>();
        obj.difficulty_10 = r["difficulty_10"].as<uint32_t>();
        return obj;
    }
};