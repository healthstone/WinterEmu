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

/** realmcharacters **/
struct RealmCharacters {
    uint32_t realmid;
    uint16_t numchars;
};

template<>
struct PgRowMapper<RealmCharacters> {
    static RealmCharacters map(const pqxx::row& r) {
        RealmCharacters row;
        row.realmid = r["realmid"].as<uint32_t>();
        row.numchars = static_cast<uint16_t>(r["numchars"].as<int>());
        return row;
    }
};