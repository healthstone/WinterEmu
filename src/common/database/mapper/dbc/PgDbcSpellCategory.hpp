#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcSpellCategory
{
    uint32_t id;
    uint32_t flags;
};

template<>
struct PgRowMapper<DbcSpellCategory>
{
    static DbcSpellCategory map(const pqxx::row& r)
    {
        DbcSpellCategory obj;
        obj.id = r["id"].as<uint32_t>();
        obj.flags = r["flags"].as<uint32_t>();
        return obj;
    }
};