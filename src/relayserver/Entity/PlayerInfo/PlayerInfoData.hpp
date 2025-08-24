#pragma once

#include <cstdint>
#include <vector>

struct PlayerCreateInfoItem {
    PlayerCreateInfoItem(uint32_t id, uint32_t amount) : item_id(id), item_amount(amount) {}

    uint32_t item_id = 0;
    uint32_t item_amount = 0;
};

struct PlayerCreateInfoAction {
    PlayerCreateInfoAction(uint8_t _button, uint32_t _action, uint8_t _type) : button(_button), type(_type),
                                                                               action(_action) {}

    uint8_t button = 0;
    uint8_t type = 0;
    uint32_t action = 0;
};

struct PlayerCreateInfoSkill {
    uint16_t SkillId;
    uint16_t Rank;
};

typedef std::vector<PlayerCreateInfoItem> PlayerCreateInfoItems;
typedef std::vector<uint32_t> PlayerCreateInfoSpells;
typedef std::vector<PlayerCreateInfoAction> PlayerCreateInfoActions;
typedef std::vector<PlayerCreateInfoSkill> PlayerCreateInfoSkills;

struct PlayerInfo {
    uint32_t mapId = 0;
    uint32_t areaId = 0;
    float positionX = 0.0f;
    float positionY = 0.0f;
    float positionZ = 0.0f;
    float orientation = 0.0f;
    uint16_t displayId_m = 0;
    uint16_t displayId_f = 0;
    PlayerCreateInfoItems item;
    PlayerCreateInfoSpells customSpells;
    PlayerCreateInfoSpells castSpells;
    PlayerCreateInfoActions action;
    PlayerCreateInfoSkills skills;
};