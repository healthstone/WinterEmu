#pragma once

#include <cstdint>

enum TutorialsFlag : uint8_t
{
    TUTORIALS_FLAG_NONE                           = 0x00,
    TUTORIALS_FLAG_CHANGED                        = 0x01,
    TUTORIALS_FLAG_LOADED_FROM_DB                 = 0x02
};

#define MAX_ACCOUNT_TUTORIAL_VALUES 8