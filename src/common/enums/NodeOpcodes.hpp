#pragma once

#include <cstdint>

enum class NodeOpcodes : uint16_t {
    NULL_PACKET                                     = 0x000,

    REL_TO_NODE_PING                                = 0x001,
    NODE_TO_REL_PONG                                = 0x002,

    REL_TO_NODE_ADD_PLAYER                          = 0x003,    // Оповещаем ноды, чтобы загрузили игрока по ObjectGuid
    REL_TO_NODE_DEL_PLAYER                          = 0x004,    // Оповещаем ноды, чтобы выгрузили игрока

    REL_TO_NODE_WOWPACKET                           = 0x005,
    NODE_TO_REL_WOWPACKET                           = 0x006,
};