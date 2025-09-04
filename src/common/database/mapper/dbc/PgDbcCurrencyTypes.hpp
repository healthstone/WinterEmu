#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_currencytypes **/
struct DbcCurrencyTypes {
    uint32_t ID;
    uint32_t ItemID;
    uint32_t CategoryID;
    uint32_t BitIndex;
};

template<>
struct PgRowMapper<DbcCurrencyTypes> {
    static DbcCurrencyTypes map(const pqxx::row& r) {
        DbcCurrencyTypes row;

        row.ID         = r["id"].as<uint32_t>();
        row.ItemID     = r["itemid"].as<uint32_t>();
        row.CategoryID = r["categoryid"].as<uint32_t>();
        row.BitIndex   = r["bitindex"].as<uint32_t>();

        return row;
    }
};
