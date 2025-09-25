#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSpellDifficulty {
    uint32_t id;
    int32_t difficulty_spell_id_1;
    int32_t difficulty_spell_id_2;
    int32_t difficulty_spell_id_3;
    int32_t difficulty_spell_id_4;
};

template<>
struct PgRowMapper<DbcSpellDifficulty> {
    static DbcSpellDifficulty map(const pqxx::row& r) {
        DbcSpellDifficulty obj;
        obj.id                   = r["id"].as<uint32_t>();
        obj.difficulty_spell_id_1 = r["difficulty_spell_id_1"].as<int32_t>();
        obj.difficulty_spell_id_2 = r["difficulty_spell_id_2"].as<int32_t>();
        obj.difficulty_spell_id_3 = r["difficulty_spell_id_3"].as<int32_t>();
        obj.difficulty_spell_id_4 = r["difficulty_spell_id_4"].as<int32_t>();
        return obj;
    }
};