#pragma once

#include <cstdint>

enum class NodeOpcodes : uint16_t {
    NULL_PACKET                                     = 0x000,

    REL_TO_NODE_PING                                      = 0x001,
    NODE_TO_REL_PONG                                      = 0x002,

    REL_TO_NODE_WOWPACKET                           = 0x003,
    NODE_TO_REL_WOWPACKET                           = 0x004,
};