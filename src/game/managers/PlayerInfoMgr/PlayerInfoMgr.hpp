#pragma once

#include <memory>
#include "src/game/Entity/PlayerInfo/PlayerInfo.hpp"
#include "src/game/Entity/PlayerInfo/PlayerClassInfo.hpp"
#include "src/game/enums/Races.hpp"
#include "src/game/enums/Classes.hpp"

#define CONFIG_MAX_PLAYER_LEVEL       80

class BaseServer;

class PlayerInfoMgr {
public:
    explicit PlayerInfoMgr(std::shared_ptr<BaseServer> server) : server_(std::move(server)) {}

    ~PlayerInfoMgr();

    void cleanUpBeforeDelete();

    void loadFromDB();

    PlayerInfo const *getPlayerInfo(uint8_t race, uint8_t class_) const {
        if (race >= MAX_RACES)
            return nullptr;
        if (class_ >= MAX_CLASSES)
            return nullptr;
        auto const &info = _playerInfo[race][class_];
        if (!info)
            return nullptr;
        return info.get();
    }

    PlayerClassInfo const *getPlayerClassInfo(uint8_t class_) const {
        return class_ < MAX_CLASSES ? _playerClassInfo[class_].get() : nullptr;
    }

    void getPlayerClassLevelInfo(uint8_t class_, uint8_t level, PlayerClassLevelInfo *info) const {
        if (level < 1 || class_ >= MAX_CLASSES)
            return;

        auto const &pInfo = _playerClassInfo[class_];

        if (level > CONFIG_MAX_PLAYER_LEVEL)
            level = CONFIG_MAX_PLAYER_LEVEL;

        *info = pInfo->levelInfo[level - 1];
    }

private:
    void load_playercreateinfo();
    void load_playercreateinfo_item();

    void playerCreateInfoAddItemHelper(uint8_t race_, uint8_t class_, uint32_t itemId, int16_t count);

    std::shared_ptr<BaseServer> server_;

    std::unique_ptr<PlayerInfo> _playerInfo[MAX_RACES][MAX_CLASSES];
    std::unique_ptr<PlayerClassInfo> _playerClassInfo[MAX_CLASSES];
};