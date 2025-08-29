#pragma once

#include <pqxx/pqxx>
#include <cstdint>

#include "QueryResults.hpp"

/** dbc_bankbagslotprices **/
struct DbcBankBagSlotPrices {
    uint32_t ID;
    uint32_t Cost;
};

template<>
struct PgRowMapper<DbcBankBagSlotPrices> {
    static DbcBankBagSlotPrices map(const pqxx::row& r) {
        DbcBankBagSlotPrices row;

        row.ID   = r["id"].as<uint32_t>();
        row.Cost = r["cost"].as<uint32_t>();

        return row;
    }
};
