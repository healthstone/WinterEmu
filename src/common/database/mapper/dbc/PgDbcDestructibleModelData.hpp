#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

/** dbc_destructiblemodeldata **/
struct DbcDestructibleModelData {
    uint32_t id;
    uint32_t state0_impact_effect_doodad_set;
    uint32_t state0_ambient_doodad_set;
    uint32_t state1_wmo;
    uint32_t state1_destruction_doodad_set;
    uint32_t state1_impact_effect_doodad_set;
    uint32_t state1_ambient_doodad_set;
    uint32_t state2_wmo;
    uint32_t state2_destruction_doodad_set;
    uint32_t state2_impact_effect_doodad_set;
    uint32_t state2_ambient_doodad_set;
    uint32_t state3_wmo;
    uint32_t state3_init_doodad_set;
    uint32_t state3_ambient_doodad_set;
    uint32_t eject_direction;
    uint32_t repair_ground_fx;
    uint32_t do_not_highlight;
    uint32_t heal_effect;
    uint32_t heal_effect_speed;
};

template<>
struct PgRowMapper<DbcDestructibleModelData> {
    static DbcDestructibleModelData map(const pqxx::row& r) {
        DbcDestructibleModelData row;

        row.id                          = r["id"].as<uint32_t>();
        row.state0_impact_effect_doodad_set = r["state0_impact_effect_doodad_set"].as<uint32_t>();
        row.state0_ambient_doodad_set   = r["state0_ambient_doodad_set"].as<uint32_t>();
        row.state1_wmo                  = r["state1_wmo"].as<uint32_t>();
        row.state1_destruction_doodad_set  = r["state1_destruction_doodad_set"].as<uint32_t>();
        row.state1_impact_effect_doodad_set = r["state1_impact_effect_doodad_set"].as<uint32_t>();
        row.state1_ambient_doodad_set   = r["state1_ambient_doodad_set"].as<uint32_t>();
        row.state2_wmo                  = r["state2_wmo"].as<uint32_t>();
        row.state2_destruction_doodad_set  = r["state2_destruction_doodad_set"].as<uint32_t>();
        row.state2_impact_effect_doodad_set = r["state2_impact_effect_doodad_set"].as<uint32_t>();
        row.state2_ambient_doodad_set   = r["state2_ambient_doodad_set"].as<uint32_t>();
        row.state3_wmo                  = r["state3_wmo"].as<uint32_t>();
        row.state3_init_doodad_set      = r["state3_init_doodad_set"].as<uint32_t>();
        row.state3_ambient_doodad_set   = r["state3_ambient_doodad_set"].as<uint32_t>();
        row.eject_direction             = r["eject_direction"].as<uint32_t>();
        row.repair_ground_fx            = r["repair_ground_fx"].as<uint32_t>();
        row.do_not_highlight            = r["do_not_highlight"].as<uint32_t>();
        row.heal_effect                 = r["heal_effect"].as<uint32_t>();
        row.heal_effect_speed           = r["heal_effect_speed"].as<uint32_t>();

        return row;
    }
};