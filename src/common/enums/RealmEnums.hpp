#pragma once

#include <cstdint>

enum class RealmType : uint8_t
{
    REALM_TYPE_NORMAL       = 0,
    REALM_TYPE_PVP          = 1,
    REALM_TYPE_NORMAL2      = 4,
    REALM_TYPE_RP           = 6,
    REALM_TYPE_RPPVP        = 8,

    MAX_CLIENT_REALM_TYPE   = 14,

    REALM_TYPE_FFA_PVP      = 16                            // custom, free for all pvp mode like arena PvP in all zones except rest activated places and sanctuaries
    // replaced by REALM_PVP in realm list
};

enum class RealmFlags : uint8_t
{
    REALM_FLAG_NONE             = 0x00,
    REALM_FLAG_VERSION_MISMATCH = 0x01,
    REALM_FLAG_OFFLINE          = 0x02,
    REALM_FLAG_SPECIFYBUILD     = 0x04,
    REALM_FLAG_UNK1             = 0x08,
    REALM_FLAG_UNK2             = 0x10,
    REALM_FLAG_RECOMMENDED      = 0x20,
    REALM_FLAG_NEW              = 0x40,
    REALM_FLAG_FULL             = 0x80
};