#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtregenmpperspt **/
struct DbcGtRegenMpPerSpt {
    uint32_t id;
    float data;
};

template<>
struct PgRowMapper<DbcGtRegenMpPerSpt> {
    static DbcGtRegenMpPerSpt map(const pqxx::row& r) {
        DbcGtRegenMpPerSpt row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};