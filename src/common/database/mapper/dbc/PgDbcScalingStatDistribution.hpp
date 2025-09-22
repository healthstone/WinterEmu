#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcScalingStatDistribution {
    uint32_t id;
    int32_t statid_1;
    int32_t statid_2;
    int32_t statid_3;
    int32_t statid_4;
    int32_t statid_5;
    int32_t statid_6;
    int32_t statid_7;
    int32_t statid_8;
    int32_t statid_9;
    int32_t statid_10;
    uint32_t bonus_1;
    uint32_t bonus_2;
    uint32_t bonus_3;
    uint32_t bonus_4;
    uint32_t bonus_5;
    uint32_t bonus_6;
    uint32_t bonus_7;
    uint32_t bonus_8;
    uint32_t bonus_9;
    uint32_t bonus_10;
    uint32_t maxlevel;
};

template<>
struct PgRowMapper<DbcScalingStatDistribution> {
    static DbcScalingStatDistribution map(const pqxx::row& r) {
        DbcScalingStatDistribution obj{};
        obj.id        = r["id"].as<uint32_t>();
        obj.statid_1  = r["statid_1"].as<int32_t>();
        obj.statid_2  = r["statid_2"].as<int32_t>();
        obj.statid_3  = r["statid_3"].as<int32_t>();
        obj.statid_4  = r["statid_4"].as<int32_t>();
        obj.statid_5  = r["statid_5"].as<int32_t>();
        obj.statid_6  = r["statid_6"].as<int32_t>();
        obj.statid_7  = r["statid_7"].as<int32_t>();
        obj.statid_8  = r["statid_8"].as<int32_t>();
        obj.statid_9  = r["statid_9"].as<int32_t>();
        obj.statid_10 = r["statid_10"].as<int32_t>();
        obj.bonus_1   = r["bonus_1"].as<uint32_t>();
        obj.bonus_2   = r["bonus_2"].as<uint32_t>();
        obj.bonus_3   = r["bonus_3"].as<uint32_t>();
        obj.bonus_4   = r["bonus_4"].as<uint32_t>();
        obj.bonus_5   = r["bonus_5"].as<uint32_t>();
        obj.bonus_6   = r["bonus_6"].as<uint32_t>();
        obj.bonus_7   = r["bonus_7"].as<uint32_t>();
        obj.bonus_8   = r["bonus_8"].as<uint32_t>();
        obj.bonus_9   = r["bonus_9"].as<uint32_t>();
        obj.bonus_10  = r["bonus_10"].as<uint32_t>();
        obj.maxlevel  = r["maxlevel"].as<uint32_t>();
        return obj;
    }
};
