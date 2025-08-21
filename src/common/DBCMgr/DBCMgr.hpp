#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include "enums/DBCStructure.h"

typedef std::unordered_map<uint32_t /*ID*/, ChrClassesDBC> ChrClassesDBCMap;

class RelayServer;

class DBCMgr {
public:
    explicit DBCMgr(std::shared_ptr<RelayServer> server) : server_(std::move(server)) {}

    void initialize_for_relay();

    ~DBCMgr();

    void cleanUpBeforeDelete();

    ChrClassesDBC const* GetChrClassesDBC(uint32_t id)
    {
        auto itr = _chrClassesMap.find(id);
        if (itr != _chrClassesMap.end())
            return &itr->second;
        return nullptr;
    }

    char const* GetChrClassName(uint8_t class_, uint8_t locale)
    {
        auto itr = _chrClassesMap.find(class_);
        if (itr != _chrClassesMap.end())
            return itr->second.Name[locale].c_str();
        return nullptr;
    }

private:
    void load_ChrClasses();

    std::shared_ptr<RelayServer> server_;

    ChrClassesDBCMap _chrClassesMap;
};