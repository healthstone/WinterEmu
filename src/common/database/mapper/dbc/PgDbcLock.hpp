#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_lock **/
struct DbcLock {
    uint32_t id;
    uint32_t type_1;
    uint32_t type_2;
    uint32_t type_3;
    uint32_t type_4;
    uint32_t type_5;
    uint32_t type_6;
    uint32_t type_7;
    uint32_t type_8;
    uint32_t index_1;
    uint32_t index_2;
    uint32_t index_3;
    uint32_t index_4;
    uint32_t index_5;
    uint32_t index_6;
    uint32_t index_7;
    uint32_t index_8;
    uint32_t skill_1;
    uint32_t skill_2;
    uint32_t skill_3;
    uint32_t skill_4;
    uint32_t skill_5;
    uint32_t skill_6;
    uint32_t skill_7;
    uint32_t skill_8;
    uint32_t action_1;
    uint32_t action_2;
    uint32_t action_3;
    uint32_t action_4;
    uint32_t action_5;
    uint32_t action_6;
    uint32_t action_7;
    uint32_t action_8;
};

template<>
struct PgRowMapper<DbcLock> {
    static DbcLock map(const pqxx::row& r) {
        DbcLock obj;
        obj.id       = r["id"].as<uint32_t>();
        obj.type_1   = r["type_1"].as<uint32_t>();
        obj.type_2   = r["type_2"].as<uint32_t>();
        obj.type_3   = r["type_3"].as<uint32_t>();
        obj.type_4   = r["type_4"].as<uint32_t>();
        obj.type_5   = r["type_5"].as<uint32_t>();
        obj.type_6   = r["type_6"].as<uint32_t>();
        obj.type_7   = r["type_7"].as<uint32_t>();
        obj.type_8   = r["type_8"].as<uint32_t>();
        obj.index_1  = r["index_1"].as<uint32_t>();
        obj.index_2  = r["index_2"].as<uint32_t>();
        obj.index_3  = r["index_3"].as<uint32_t>();
        obj.index_4  = r["index_4"].as<uint32_t>();
        obj.index_5  = r["index_5"].as<uint32_t>();
        obj.index_6  = r["index_6"].as<uint32_t>();
        obj.index_7  = r["index_7"].as<uint32_t>();
        obj.index_8  = r["index_8"].as<uint32_t>();
        obj.skill_1  = r["skill_1"].as<uint32_t>();
        obj.skill_2  = r["skill_2"].as<uint32_t>();
        obj.skill_3  = r["skill_3"].as<uint32_t>();
        obj.skill_4  = r["skill_4"].as<uint32_t>();
        obj.skill_5  = r["skill_5"].as<uint32_t>();
        obj.skill_6  = r["skill_6"].as<uint32_t>();
        obj.skill_7  = r["skill_7"].as<uint32_t>();
        obj.skill_8  = r["skill_8"].as<uint32_t>();
        obj.action_1 = r["action_1"].as<uint32_t>();
        obj.action_2 = r["action_2"].as<uint32_t>();
        obj.action_3 = r["action_3"].as<uint32_t>();
        obj.action_4 = r["action_4"].as<uint32_t>();
        obj.action_5 = r["action_5"].as<uint32_t>();
        obj.action_6 = r["action_6"].as<uint32_t>();
        obj.action_7 = r["action_7"].as<uint32_t>();
        obj.action_8 = r["action_8"].as<uint32_t>();
        return obj;
    }
};