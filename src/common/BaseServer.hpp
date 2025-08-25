#pragma once
#include <memory>
#include "Database.hpp"
#include "src/game/managers/DBCMgr/DBCMgr.hpp"
#include "src/game/managers/PlayerInfoMgr/PlayerInfoMgr.hpp"

class BaseServer {
public:
    virtual ~BaseServer() = default;
    virtual std::shared_ptr<Database> db() const = 0;

    virtual DBCMgr* getDBCMgr() const = 0;
    virtual PlayerInfoMgr* getPlayerInfoMgr() const = 0;
};