#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_liquidtype **/
struct DbcLiquidType {
    uint32_t id;
    std::optional<std::string> name;
    uint32_t flags;
    uint32_t type;
    uint32_t soundid;
    uint32_t spellid;
    float maxdarkendepth;
    float fogdarkenintensity;
    float ambdarkenintensity;
    float dirdarkenintensity;
    uint32_t lightid;
    float particlescale;
    uint32_t particlemovement;
    uint32_t particletexslots;
    uint32_t materialid;
    std::optional<std::string> texture_1;
    std::optional<std::string> texture_2;
    std::optional<std::string> texture_3;
    std::optional<std::string> texture_4;
    std::optional<std::string> texture_5;
    std::optional<std::string> texture_6;
    uint32_t color_1;
    uint32_t color_2;
    float float_1;
    float float_2;
    float float_3;
    float float_4;
    float float_5;
    float float_6;
    float float_7;
    float float_8;
    float float_9;
    float float_10;
    float float_11;
    float float_12;
    float float_13;
    float float_14;
    float float_15;
    float float_16;
    float float_17;
    float float_18;
    uint32_t int_1;
    uint32_t int_2;
    uint32_t int_3;
    uint32_t int_4;
};

template <>
struct PgRowMapper<DbcLiquidType> {
    static DbcLiquidType map(const pqxx::row &r) {
        return DbcLiquidType{
                r["id"].as<uint32_t>(),
                get_optional_string(r, "name"),
                r["flags"].as<uint32_t>(),
                r["type"].as<uint32_t>(),
                r["soundid"].as<uint32_t>(),
                r["spellid"].as<uint32_t>(),
                r["maxdarkendepth"].as<float>(),
                r["fogdarkenintensity"].as<float>(),
                r["ambdarkenintensity"].as<float>(),
                r["dirdarkenintensity"].as<float>(),
                r["lightid"].as<uint32_t>(),
                r["particlescale"].as<float>(),
                r["particlemovement"].as<uint32_t>(),
                r["particletexslots"].as<uint32_t>(),
                r["materialid"].as<uint32_t>(),
                get_optional_string(r, "texture_1"),
                get_optional_string(r, "texture_2"),
                get_optional_string(r, "texture_3"),
                get_optional_string(r, "texture_4"),
                get_optional_string(r, "texture_5"),
                get_optional_string(r, "texture_6"),
                r["color_1"].as<uint32_t>(),
                r["color_2"].as<uint32_t>(),
                r["float_1"].as<float>(),
                r["float_2"].as<float>(),
                r["float_3"].as<float>(),
                r["float_4"].as<float>(),
                r["float_5"].as<float>(),
                r["float_6"].as<float>(),
                r["float_7"].as<float>(),
                r["float_8"].as<float>(),
                r["float_9"].as<float>(),
                r["float_10"].as<float>(),
                r["float_11"].as<float>(),
                r["float_12"].as<float>(),
                r["float_13"].as<float>(),
                r["float_14"].as<float>(),
                r["float_15"].as<float>(),
                r["float_16"].as<float>(),
                r["float_17"].as<float>(),
                r["float_18"].as<float>(),
                r["int_1"].as<uint32_t>(),
                r["int_2"].as<uint32_t>(),
                r["int_3"].as<uint32_t>(),
                r["int_4"].as<uint32_t>()
        };
    }
};
