#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_randproppoints **/
struct DbcRandPropPoints {
    uint32_t id;
    uint32_t epic_1;
    uint32_t epic_2;
    uint32_t epic_3;
    uint32_t epic_4;
    uint32_t epic_5;
    uint32_t superior_1;
    uint32_t superior_2;
    uint32_t superior_3;
    uint32_t superior_4;
    uint32_t superior_5;
    uint32_t good_1;
    uint32_t good_2;
    uint32_t good_3;
    uint32_t good_4;
    uint32_t good_5;
};

template<>
struct PgRowMapper<DbcRandPropPoints> {
    static DbcRandPropPoints map(const pqxx::row &r) {
        DbcRandPropPoints row{};
        row.id          = r["id"].as<uint32_t>();
        row.epic_1      = r["epic_1"].as<uint32_t>();
        row.epic_2      = r["epic_2"].as<uint32_t>();
        row.epic_3      = r["epic_3"].as<uint32_t>();
        row.epic_4      = r["epic_4"].as<uint32_t>();
        row.epic_5      = r["epic_5"].as<uint32_t>();
        row.superior_1  = r["superior_1"].as<uint32_t>();
        row.superior_2  = r["superior_2"].as<uint32_t>();
        row.superior_3  = r["superior_3"].as<uint32_t>();
        row.superior_4  = r["superior_4"].as<uint32_t>();
        row.superior_5  = r["superior_5"].as<uint32_t>();
        row.good_1      = r["good_1"].as<uint32_t>();
        row.good_2      = r["good_2"].as<uint32_t>();
        row.good_3      = r["good_3"].as<uint32_t>();
        row.good_4      = r["good_4"].as<uint32_t>();
        row.good_5      = r["good_5"].as<uint32_t>();
        return row;
    }
};