#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_durabilitycosts **/
struct DbcDurabilityCosts {
    uint32_t id;

    uint32_t weaponSubClassCost1;
    uint32_t weaponSubClassCost2;
    uint32_t weaponSubClassCost3;
    uint32_t weaponSubClassCost4;
    uint32_t weaponSubClassCost5;
    uint32_t weaponSubClassCost6;
    uint32_t weaponSubClassCost7;
    uint32_t weaponSubClassCost8;
    uint32_t weaponSubClassCost9;
    uint32_t weaponSubClassCost10;
    uint32_t weaponSubClassCost11;
    uint32_t weaponSubClassCost12;
    uint32_t weaponSubClassCost13;
    uint32_t weaponSubClassCost14;
    uint32_t weaponSubClassCost15;
    uint32_t weaponSubClassCost16;
    uint32_t weaponSubClassCost17;
    uint32_t weaponSubClassCost18;
    uint32_t weaponSubClassCost19;
    uint32_t weaponSubClassCost20;
    uint32_t weaponSubClassCost21;

    uint32_t armorSubClassCost1;
    uint32_t armorSubClassCost2;
    uint32_t armorSubClassCost3;
    uint32_t armorSubClassCost4;
    uint32_t armorSubClassCost5;
    uint32_t armorSubClassCost6;
    uint32_t armorSubClassCost7;
    uint32_t armorSubClassCost8;
};

template<>
struct PgRowMapper<DbcDurabilityCosts> {
    static DbcDurabilityCosts map(const pqxx::row& r) {
        DbcDurabilityCosts row;

        row.id = r["id"].as<uint32_t>();

        row.weaponSubClassCost1  = r["weaponsubclasscost_1"].as<uint32_t>();
        row.weaponSubClassCost2  = r["weaponsubclasscost_2"].as<uint32_t>();
        row.weaponSubClassCost3  = r["weaponsubclasscost_3"].as<uint32_t>();
        row.weaponSubClassCost4  = r["weaponsubclasscost_4"].as<uint32_t>();
        row.weaponSubClassCost5  = r["weaponsubclasscost_5"].as<uint32_t>();
        row.weaponSubClassCost6  = r["weaponsubclasscost_6"].as<uint32_t>();
        row.weaponSubClassCost7  = r["weaponsubclasscost_7"].as<uint32_t>();
        row.weaponSubClassCost8  = r["weaponsubclasscost_8"].as<uint32_t>();
        row.weaponSubClassCost9  = r["weaponsubclasscost_9"].as<uint32_t>();
        row.weaponSubClassCost10 = r["weaponsubclasscost_10"].as<uint32_t>();
        row.weaponSubClassCost11 = r["weaponsubclasscost_11"].as<uint32_t>();
        row.weaponSubClassCost12 = r["weaponsubclasscost_12"].as<uint32_t>();
        row.weaponSubClassCost13 = r["weaponsubclasscost_13"].as<uint32_t>();
        row.weaponSubClassCost14 = r["weaponsubclasscost_14"].as<uint32_t>();
        row.weaponSubClassCost15 = r["weaponsubclasscost_15"].as<uint32_t>();
        row.weaponSubClassCost16 = r["weaponsubclasscost_16"].as<uint32_t>();
        row.weaponSubClassCost17 = r["weaponsubclasscost_17"].as<uint32_t>();
        row.weaponSubClassCost18 = r["weaponsubclasscost_18"].as<uint32_t>();
        row.weaponSubClassCost19 = r["weaponsubclasscost_19"].as<uint32_t>();
        row.weaponSubClassCost20 = r["weaponsubclasscost_20"].as<uint32_t>();
        row.weaponSubClassCost21 = r["weaponsubclasscost_21"].as<uint32_t>();

        row.armorSubClassCost1 = r["armorsubclasscost_1"].as<uint32_t>();
        row.armorSubClassCost2 = r["armorsubclasscost_2"].as<uint32_t>();
        row.armorSubClassCost3 = r["armorsubclasscost_3"].as<uint32_t>();
        row.armorSubClassCost4 = r["armorsubclasscost_4"].as<uint32_t>();
        row.armorSubClassCost5 = r["armorsubclasscost_5"].as<uint32_t>();
        row.armorSubClassCost6 = r["armorsubclasscost_6"].as<uint32_t>();
        row.armorSubClassCost7 = r["armorsubclasscost_7"].as<uint32_t>();
        row.armorSubClassCost8 = r["armorsubclasscost_8"].as<uint32_t>();

        return row;
    }
};
