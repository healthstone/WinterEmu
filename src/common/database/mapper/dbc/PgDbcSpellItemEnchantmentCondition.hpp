#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSpellItemEnchantmentCondition {
    uint32_t id;

    uint8_t lt_operandtype_1;
    uint8_t lt_operandtype_2;
    uint8_t lt_operandtype_3;
    uint8_t lt_operandtype_4;
    uint8_t lt_operandtype_5;

    uint32_t lt_operand_1;
    uint32_t lt_operand_2;
    uint32_t lt_operand_3;
    uint32_t lt_operand_4;
    uint32_t lt_operand_5;

    uint8_t operator_1;
    uint8_t operator_2;
    uint8_t operator_3;
    uint8_t operator_4;
    uint8_t operator_5;

    uint8_t rt_operandtype_1;
    uint8_t rt_operandtype_2;
    uint8_t rt_operandtype_3;
    uint8_t rt_operandtype_4;
    uint8_t rt_operandtype_5;

    uint32_t rt_operand_1;
    uint32_t rt_operand_2;
    uint32_t rt_operand_3;
    uint32_t rt_operand_4;
    uint32_t rt_operand_5;

    uint8_t logic_1;
    uint8_t logic_2;
    uint8_t logic_3;
    uint8_t logic_4;
    uint8_t logic_5;
};

template<>
struct PgRowMapper<DbcSpellItemEnchantmentCondition> {
    static DbcSpellItemEnchantmentCondition map(const pqxx::row& r) {
        DbcSpellItemEnchantmentCondition row{};
        row.id               = r["id"].as<uint32_t>();

        row.lt_operandtype_1 = static_cast<uint8_t>(r["lt_operandtype_1"].as<int>());
        row.lt_operandtype_2 = static_cast<uint8_t>(r["lt_operandtype_2"].as<int>());
        row.lt_operandtype_3 = static_cast<uint8_t>(r["lt_operandtype_3"].as<int>());
        row.lt_operandtype_4 = static_cast<uint8_t>(r["lt_operandtype_4"].as<int>());
        row.lt_operandtype_5 = static_cast<uint8_t>(r["lt_operandtype_5"].as<int>());

        row.lt_operand_1     = r["lt_operand_1"].as<uint32_t>();
        row.lt_operand_2     = r["lt_operand_2"].as<uint32_t>();
        row.lt_operand_3     = r["lt_operand_3"].as<uint32_t>();
        row.lt_operand_4     = r["lt_operand_4"].as<uint32_t>();
        row.lt_operand_5     = r["lt_operand_5"].as<uint32_t>();

        row.operator_1       = static_cast<uint8_t>(r["operator_1"].as<int>());
        row.operator_2       = static_cast<uint8_t>(r["operator_2"].as<int>());
        row.operator_3       = static_cast<uint8_t>(r["operator_3"].as<int>());
        row.operator_4       = static_cast<uint8_t>(r["operator_4"].as<int>());
        row.operator_5       = static_cast<uint8_t>(r["operator_5"].as<int>());

        row.rt_operandtype_1 = static_cast<uint8_t>(r["rt_operandtype_1"].as<int>());
        row.rt_operandtype_2 = static_cast<uint8_t>(r["rt_operandtype_2"].as<int>());
        row.rt_operandtype_3 = static_cast<uint8_t>(r["rt_operandtype_3"].as<int>());
        row.rt_operandtype_4 = static_cast<uint8_t>(r["rt_operandtype_4"].as<int>());
        row.rt_operandtype_5 = static_cast<uint8_t>(r["rt_operandtype_5"].as<int>());

        row.rt_operand_1     = r["rt_operand_1"].as<uint32_t>();
        row.rt_operand_2     = r["rt_operand_2"].as<uint32_t>();
        row.rt_operand_3     = r["rt_operand_3"].as<uint32_t>();
        row.rt_operand_4     = r["rt_operand_4"].as<uint32_t>();
        row.rt_operand_5     = r["rt_operand_5"].as<uint32_t>();

        row.logic_1          = static_cast<uint8_t>(r["logic_1"].as<int>());
        row.logic_2          = static_cast<uint8_t>(r["logic_2"].as<int>());
        row.logic_3          = static_cast<uint8_t>(r["logic_3"].as<int>());
        row.logic_4          = static_cast<uint8_t>(r["logic_4"].as<int>());
        row.logic_5          = static_cast<uint8_t>(r["logic_5"].as<int>());

        return row;
    }
};