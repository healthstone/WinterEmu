#pragma once

#include <string>
#include <optional>

#include "QueryResults.hpp"

/** dbc_namesreserved **/
struct DbcNamesReserved
{
    uint32_t id;
    std::optional<std::string> name;
    int32_t language;
};

template<>
struct PgRowMapper<DbcNamesReserved>
{
    static DbcNamesReserved map(const pqxx::row &r)
    {
        DbcNamesReserved row{};

        row.id       = r["id"].as<uint32_t>();
        row.name     = get_optional_string(r, "name");
        row.language = r["language"].as<int32_t>();

        return row;
    }
};
