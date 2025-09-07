#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include <string>
#include "QueryResults.hpp"

/** dbc_holidays **/
struct DbcHolidays {
    uint32_t id;
    uint32_t duration_1;
    uint32_t duration_2;
    uint32_t duration_3;
    uint32_t duration_4;
    uint32_t duration_5;
    uint32_t duration_6;
    uint32_t duration_7;
    uint32_t duration_8;
    uint32_t duration_9;
    uint32_t duration_10;
    uint32_t date_1;
    uint32_t date_2;
    uint32_t date_3;
    uint32_t date_4;
    uint32_t date_5;
    uint32_t date_6;
    uint32_t date_7;
    uint32_t date_8;
    uint32_t date_9;
    uint32_t date_10;
    uint32_t date_11;
    uint32_t date_12;
    uint32_t date_13;
    uint32_t date_14;
    uint32_t date_15;
    uint32_t date_16;
    uint32_t date_17;
    uint32_t date_18;
    uint32_t date_19;
    uint32_t date_20;
    uint32_t date_21;
    uint32_t date_22;
    uint32_t date_23;
    uint32_t date_24;
    uint32_t date_25;
    uint32_t date_26;
    uint32_t region;
    uint32_t looping;
    uint32_t calendarflags_1;
    uint32_t calendarflags_2;
    uint32_t calendarflags_3;
    uint32_t calendarflags_4;
    uint32_t calendarflags_5;
    uint32_t calendarflags_6;
    uint32_t calendarflags_7;
    uint32_t calendarflags_8;
    uint32_t calendarflags_9;
    uint32_t calendarflags_10;
    uint32_t holidaynameid;
    uint32_t holidaydescriptionid;
    std::optional<std::string> texturefilename; // nullable
    uint32_t priority;
    int32_t calendarfiltertype;
    uint32_t flags;
};

template<>
struct PgRowMapper<DbcHolidays> {
    static DbcHolidays map(const pqxx::row& r) {
        DbcHolidays o {
                r["id"].as<uint32_t>(),
                r["duration_1"].as<uint32_t>(),
                r["duration_2"].as<uint32_t>(),
                r["duration_3"].as<uint32_t>(),
                r["duration_4"].as<uint32_t>(),
                r["duration_5"].as<uint32_t>(),
                r["duration_6"].as<uint32_t>(),
                r["duration_7"].as<uint32_t>(),
                r["duration_8"].as<uint32_t>(),
                r["duration_9"].as<uint32_t>(),
                r["duration_10"].as<uint32_t>(),
                r["date_1"].as<uint32_t>(),
                r["date_2"].as<uint32_t>(),
                r["date_3"].as<uint32_t>(),
                r["date_4"].as<uint32_t>(),
                r["date_5"].as<uint32_t>(),
                r["date_6"].as<uint32_t>(),
                r["date_7"].as<uint32_t>(),
                r["date_8"].as<uint32_t>(),
                r["date_9"].as<uint32_t>(),
                r["date_10"].as<uint32_t>(),
                r["date_11"].as<uint32_t>(),
                r["date_12"].as<uint32_t>(),
                r["date_13"].as<uint32_t>(),
                r["date_14"].as<uint32_t>(),
                r["date_15"].as<uint32_t>(),
                r["date_16"].as<uint32_t>(),
                r["date_17"].as<uint32_t>(),
                r["date_18"].as<uint32_t>(),
                r["date_19"].as<uint32_t>(),
                r["date_20"].as<uint32_t>(),
                r["date_21"].as<uint32_t>(),
                r["date_22"].as<uint32_t>(),
                r["date_23"].as<uint32_t>(),
                r["date_24"].as<uint32_t>(),
                r["date_25"].as<uint32_t>(),
                r["date_26"].as<uint32_t>(),
                r["region"].as<uint32_t>(),
                r["looping"].as<uint32_t>(),
                r["calendarflags_1"].as<uint32_t>(),
                r["calendarflags_2"].as<uint32_t>(),
                r["calendarflags_3"].as<uint32_t>(),
                r["calendarflags_4"].as<uint32_t>(),
                r["calendarflags_5"].as<uint32_t>(),
                r["calendarflags_6"].as<uint32_t>(),
                r["calendarflags_7"].as<uint32_t>(),
                r["calendarflags_8"].as<uint32_t>(),
                r["calendarflags_9"].as<uint32_t>(),
                r["calendarflags_10"].as<uint32_t>(),
                r["holidaynameid"].as<uint32_t>(),
                r["holidaydescriptionid"].as<uint32_t>(),
                get_optional_string(r, "texturefilename"),
                r["priority"].as<uint32_t>(),
                r["calendarfiltertype"].as<int32_t>(),
                r["flags"].as<uint32_t>()
        };
        return o;
    }
};
