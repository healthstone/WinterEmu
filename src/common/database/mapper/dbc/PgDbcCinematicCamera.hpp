#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_cinematiccamera **/
struct DbcCinematicCamera {
    uint32_t id;
    std::optional<std::string> model;
    uint32_t soundid;
    float originx;
    float originy;
    float originz;
    float originfacing;
};

template<>
struct PgRowMapper<DbcCinematicCamera> {
    static DbcCinematicCamera map(const pqxx::row& r) {
        DbcCinematicCamera row;

        row.id           = r["id"].as<uint32_t>();
        row.soundid      = r["soundid"].as<uint32_t>();
        row.originx      = r["originx"].as<float>();
        row.originy      = r["originy"].as<float>();
        row.originz      = r["originz"].as<float>();
        row.originfacing = r["originfacing"].as<float>();

        // Optional string
        row.model = get_optional_string(r, "model");

        return row;
    }
};
