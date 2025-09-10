#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_powerdisplay **/
struct DbcPowerDisplay
{
    uint32_t id;
    uint32_t actualtype;
    std::optional<std::string> globalstring_basetag;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
};

template<>
struct PgRowMapper<DbcPowerDisplay>
{
    static DbcPowerDisplay map(const pqxx::row& r)
    {
        DbcPowerDisplay obj;
        obj.id                   = r["id"].as<uint32_t>();
        obj.actualtype           = r["actualtype"].as<uint32_t>();
        if (r["globalstring_basetag"].is_null())
            obj.globalstring_basetag = std::nullopt;
        else
            obj.globalstring_basetag = r["globalstring_basetag"].as<std::string>();
        obj.red                  = r["red"].as<uint16_t>();
        obj.green                = r["green"].as<uint16_t>();
        obj.blue                 = r["blue"].as<uint16_t>();
        return obj;
    }
};