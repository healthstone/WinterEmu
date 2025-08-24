#pragma once

#include <memory>
#include "PlayerInfoData.hpp"
#include "enums/Races.hpp"
#include "enums/Classes.hpp"

class RelayServer;

class PlayerInfoMgr {
public:
    explicit PlayerInfoMgr(std::shared_ptr<RelayServer> server) : server_(std::move(server)) {}

    ~PlayerInfoMgr();

    void cleanUpBeforeDelete();

    void loadFromDB();

    PlayerInfo const* getPlayerInfo(uint8_t race, uint8_t class_) const
    {
        if (race >= MAX_RACES)
            return nullptr;
        if (class_ >= MAX_CLASSES)
            return nullptr;
        auto const& info = _playerInfo[race][class_];
        if (!info)
            return nullptr;
        return info.get();
    }

private:
    std::shared_ptr<RelayServer> server_;
    std::unique_ptr<PlayerInfo> _playerInfo[MAX_RACES][MAX_CLASSES];
};