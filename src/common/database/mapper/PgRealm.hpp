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

struct RealmRow {
    uint32_t id;
    std::string name;
    std::string address;
    std::string localAddress;
    std::string localSubnetMask;
    uint16_t port;
    uint8_t icon;
    uint8_t flag;
    uint8_t timezone;
    uint8_t securityLevel;
    float population;
    uint32_t build;
};

template<>
struct PgRowMapper<RealmRow> {
    static RealmRow map(const pqxx::row& r) {
        RealmRow row;

        row.id = r["id"].as<uint32_t>();
        row.name = r["name"].as<std::string>();
        row.address = r["address"].as<std::string>();
        row.localAddress = r["local_address"].as<std::string>();
        row.localSubnetMask = r["local_subnet_mask"].as<std::string>();
        row.port = static_cast<uint16_t>(r["port"].as<int>());
        row.icon = static_cast<uint8_t>(r["icon"].as<int>());
        row.flag = static_cast<uint8_t>(r["flag"].as<int>());
        row.timezone = static_cast<uint8_t>(r["timezone"].as<int>());
        row.securityLevel = static_cast<uint8_t>(r["allowed_security_level"].as<int>());
        row.population = r["population"].as<float>();
        row.build = r["gamebuild"].as<uint32_t>();

        return row;
    }
};



