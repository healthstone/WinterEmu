#pragma once

#include <string>
#include <optional>

#include "QueryResults.hpp"

/** dbc_namesprofanity **/
struct DbcNamesProfanity
{
    uint32_t id;
    std::optional<std::string> name;
    int32_t language;
};

template<>
struct PgRowMapper<DbcNamesProfanity>
{
    static DbcNamesProfanity map(const pqxx::row &r)
    {
        DbcNamesProfanity row{};

        row.id       = r["id"].as<uint32_t>();
        row.name     = get_optional_string(r, "name");
        row.language = r["language"].as<int32_t>();

        return row;
    }
};
