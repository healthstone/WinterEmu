#pragma once

#include <cstdint>

enum Classes : uint8_t
{
    CLASS_NONE          = 0, // SKIP
    CLASS_WARRIOR       = 1, // TITLE Warrior
    CLASS_PALADIN       = 2, // TITLE Paladin
    CLASS_HUNTER        = 3, // TITLE Hunter
    CLASS_ROGUE         = 4, // TITLE Rogue
    CLASS_PRIEST        = 5, // TITLE Priest
    CLASS_DEATH_KNIGHT  = 6, // TITLE Death Knight
    CLASS_SHAMAN        = 7, // TITLE Shaman
    CLASS_MAGE          = 8, // TITLE Mage
    CLASS_WARLOCK       = 9, // TITLE Warlock
    //CLASS_UNK           = 10,
    CLASS_DRUID         = 11 // TITLE Druid
};

// max+1 for player class
#define MAX_CLASSES       12

#define CLASSMASK_ALL_PLAYABLE \
    ((1<<(CLASS_WARRIOR-1))|(1<<(CLASS_PALADIN-1))|(1<<(CLASS_HUNTER-1))| \
    (1<<(CLASS_ROGUE-1))  |(1<<(CLASS_PRIEST-1)) |(1<<(CLASS_SHAMAN-1))| \
    (1<<(CLASS_MAGE-1))   |(1<<(CLASS_WARLOCK-1))|(1<<(CLASS_DRUID-1)) | \
    (1<<(CLASS_DEATH_KNIGHT-1)))
