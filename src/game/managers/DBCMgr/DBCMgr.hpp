#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include "src/game/enums/DBCStructure.hpp"
#include "src/game/enums/Team.hpp"

typedef std::unordered_map<uint32_t /*ID*/, ChrClassesDBC> ChrClassesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChrRacesDBC> ChrRacesDBCMap;

class BaseServer;

class DBCMgr {
public:
    explicit DBCMgr(std::shared_ptr<BaseServer> server) : server_(std::move(server)) {}

    void initialize_for_relay();
    void initialize_for_node();

    ~DBCMgr();

    void cleanUpBeforeDelete();

    // Utilities
    Team teamForRace(uint8_t race);

    // Base functions

    ChrClassesDBC const* getChrClassesDBC(uint32_t id)
    {
        auto itr = _chrClassesMap.find(id);
        if (itr != _chrClassesMap.end())
            return &itr->second;
        return nullptr;
    }

    char const* getChrClassName(uint8_t class_, uint8_t locale)
    {
        auto itr = _chrClassesMap.find(class_);
        if (itr != _chrClassesMap.end())
            return itr->second.Name[locale].c_str();
        return nullptr;
    }

    ChrRacesDBC const* getChrRacesDBC(uint32_t id)
    {
        auto itr = _chrRacesMap.find(id);
        if (itr != _chrRacesMap.end())
            return &itr->second;
        return nullptr;
    }

    char const* getRaceName(uint8_t race, uint8_t locale)
    {
        auto itr = _chrRacesMap.find(race);
        if (itr != _chrRacesMap.end())
            return itr->second.Name[locale].c_str();
        return nullptr;
    }

private:
    void load_ChrClasses();     // load ChrClasses.dbc
    void load_ChrRaces();       // load ChrRaces.dbc

    std::shared_ptr<BaseServer> server_;

    ChrClassesDBCMap _chrClassesMap;
    ChrRacesDBCMap _chrRacesMap;
};