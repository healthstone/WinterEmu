#pragma once
#include <pqxx/pqxx>
#include <cstdint>
#include "QueryResults.hpp"

struct DbcScalingStatValues
{
    uint32_t id;
    uint32_t charlevel;
    uint32_t shoulderbudget;
    uint32_t trinketbudget;
    uint32_t weaponbudget1h;
    uint32_t rangedbudget;
    uint32_t clothshoulderarmor;
    uint32_t leathershoulderarmor;
    uint32_t mailshoulderarmor;
    uint32_t plateshoulderarmor;
    uint32_t weapondps1h;
    uint32_t weapondps2h;
    uint32_t spellcasterdps1h;
    uint32_t spellcasterdps2h;
    uint32_t rangeddps;
    uint32_t wanddps;
    uint32_t spellpower;
    uint32_t primarybudget;
    uint32_t tertiarybudget;
    uint32_t clothcloakarmor;
    uint32_t clothchestarmor;
    uint32_t leatherchestarmor;
    uint32_t mailchestarmor;
    uint32_t platechestarmor;
};

template<>
struct PgRowMapper<DbcScalingStatValues>
{
    static DbcScalingStatValues map(const pqxx::row& r)
    {
        DbcScalingStatValues obj;
        obj.id                   = r["id"].as<uint32_t>();
        obj.charlevel            = r["charlevel"].as<uint32_t>();
        obj.shoulderbudget       = r["shoulderbudget"].as<uint32_t>();
        obj.trinketbudget        = r["trinketbudget"].as<uint32_t>();
        obj.weaponbudget1h       = r["weaponbudget1h"].as<uint32_t>();
        obj.rangedbudget         = r["rangedbudget"].as<uint32_t>();
        obj.clothshoulderarmor   = r["clothshoulderarmor"].as<uint32_t>();
        obj.leathershoulderarmor = r["leathershoulderarmor"].as<uint32_t>();
        obj.mailshoulderarmor    = r["mailshoulderarmor"].as<uint32_t>();
        obj.plateshoulderarmor   = r["plateshoulderarmor"].as<uint32_t>();
        obj.weapondps1h          = r["weapondps1h"].as<uint32_t>();
        obj.weapondps2h          = r["weapondps2h"].as<uint32_t>();
        obj.spellcasterdps1h     = r["spellcasterdps1h"].as<uint32_t>();
        obj.spellcasterdps2h     = r["spellcasterdps2h"].as<uint32_t>();
        obj.rangeddps            = r["rangeddps"].as<uint32_t>();
        obj.wanddps              = r["wanddps"].as<uint32_t>();
        obj.spellpower           = r["spellpower"].as<uint32_t>();
        obj.primarybudget        = r["primarybudget"].as<uint32_t>();
        obj.tertiarybudget       = r["tertiarybudget"].as<uint32_t>();
        obj.clothcloakarmor      = r["clothcloakarmor"].as<uint32_t>();
        obj.clothchestarmor      = r["clothchestarmor"].as<uint32_t>();
        obj.leatherchestarmor    = r["leatherchestarmor"].as<uint32_t>();
        obj.mailchestarmor       = r["mailchestarmor"].as<uint32_t>();
        obj.platechestarmor      = r["platechestarmor"].as<uint32_t>();
        return obj;
    }
};