#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_gtregenhpperspt **/
struct DbcGtRegenHpPerSpt {
    uint32_t id;
    float    data;
};

template<>
struct PgRowMapper<DbcGtRegenHpPerSpt> {
    static DbcGtRegenHpPerSpt map(const pqxx::row& r) {
        DbcGtRegenHpPerSpt row{};
        row.id   = r["id"].as<uint32_t>();
        row.data = r["data"].as<float>();
        return row;
    }
};
