#pragma once

#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_movie **/
struct DbcMovie
{
    uint32_t ID;
    std::optional<std::string> Filename;
    uint32_t Volume;
};

template<>
struct PgRowMapper<DbcMovie>
{
    static DbcMovie map(const pqxx::row &r)
    {
        DbcMovie row{};
        row.ID       = r["id"].as<uint32_t>();
        row.Filename = get_optional_string(r, "filename");
        row.Volume   = r["volume"].as<uint32_t>();
        return row;
    }
};