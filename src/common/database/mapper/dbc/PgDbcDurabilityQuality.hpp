#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"

/** dbc_durabilityquality **/
struct DbcDurabilityQuality {
    uint32_t ID;
    float Data;
};

template<>
struct PgRowMapper<DbcDurabilityQuality> {

    static DbcDurabilityQuality map(const pqxx::row& r) {
        DbcDurabilityQuality row;

        row.ID   = r["id"].as<uint32_t>();
        row.Data = r["data"].as<float>();

        return row;
    }
};
