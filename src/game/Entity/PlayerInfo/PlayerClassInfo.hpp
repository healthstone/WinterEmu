#pragma once

#include <cstdint>

struct PlayerClassLevelInfo
{
    uint16_t basehealth = 0;
    uint16_t basemana = 0;
};

struct PlayerClassInfo
{
    //[level-1] 0..MaxPlayerLevel-1
    std::unique_ptr<PlayerClassLevelInfo[]> levelInfo;
};