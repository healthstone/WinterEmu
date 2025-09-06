#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_gameobjectartkit **/
struct DbcGameObjectArtKit {
    uint32_t id;

    std::optional<std::string> texturevariation_1;
    std::optional<std::string> texturevariation_2;
    std::optional<std::string> texturevariation_3;

    std::optional<std::string> attachmodel_1;
    std::optional<std::string> attachmodel_2;
    std::optional<std::string> attachmodel_3;
    std::optional<std::string> attachmodel_4;
};

template<>
struct PgRowMapper<DbcGameObjectArtKit> {
    static DbcGameObjectArtKit map(const pqxx::row& r) {
        DbcGameObjectArtKit row{};

        row.id = r["id"].as<uint32_t>();

        row.texturevariation_1 = get_optional_string(r, "texturevariation_1");
        row.texturevariation_2 = get_optional_string(r, "texturevariation_2");
        row.texturevariation_3 = get_optional_string(r, "texturevariation_3");

        row.attachmodel_1 = get_optional_string(r, "attachmodel_1");
        row.attachmodel_2 = get_optional_string(r, "attachmodel_2");
        row.attachmodel_3 = get_optional_string(r, "attachmodel_3");
        row.attachmodel_4 = get_optional_string(r, "attachmodel_4");

        return row;
    }
};
