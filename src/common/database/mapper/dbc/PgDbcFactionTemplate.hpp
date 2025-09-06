#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_factiontemplate **/
struct DbcFactionTemplate {
    uint32_t id;
    uint32_t faction;
    uint32_t flags;
    uint32_t factiongroup;
    uint32_t friendgroup;
    uint32_t enemygroup;
    uint32_t enemies_1;
    uint32_t enemies_2;
    uint32_t enemies_3;
    uint32_t enemies_4;
    uint32_t friend_1;
    uint32_t friend_2;
    uint32_t friend_3;
    uint32_t friend_4;
};

template<>
struct PgRowMapper<DbcFactionTemplate> {
    static DbcFactionTemplate map(const pqxx::row& r) {
        DbcFactionTemplate row{};
        row.id = r["id"].as<uint32_t>();
        row.faction = r["faction"].as<uint32_t>();
        row.flags = r["flags"].as<uint32_t>();
        row.factiongroup = r["factiongroup"].as<uint32_t>();
        row.friendgroup = r["friendgroup"].as<uint32_t>();
        row.enemygroup = r["enemygroup"].as<uint32_t>();
        row.enemies_1 = r["enemies_1"].as<uint32_t>();
        row.enemies_2 = r["enemies_2"].as<uint32_t>();
        row.enemies_3 = r["enemies_3"].as<uint32_t>();
        row.enemies_4 = r["enemies_4"].as<uint32_t>();
        row.friend_1 = r["friend_1"].as<uint32_t>();
        row.friend_2 = r["friend_2"].as<uint32_t>();
        row.friend_3 = r["friend_3"].as<uint32_t>();
        row.friend_4 = r["friend_4"].as<uint32_t>();
        return row;
    }
};
