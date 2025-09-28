#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcTeamContributionPoints {
    uint32_t id;
    float data;
};

template<>
struct PgRowMapper<DbcTeamContributionPoints> {
    static DbcTeamContributionPoints map(const pqxx::row& r) {
        DbcTeamContributionPoints x{};
        x.id   = r["id"].as<uint32_t>();
        x.data = r["data"].as<float>();
        return x;
    }
};