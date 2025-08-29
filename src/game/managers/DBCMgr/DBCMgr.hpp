#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <map>
#include "src/game/enums/DBCStructure.hpp"
#include "src/game/enums/Team.hpp"

typedef std::unordered_map<uint32_t /*ID*/, AchievementDBC> AchievementDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AchievementCriteriaDBC> AchievementCriteriaDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AreaTableDBC> AreaTableDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AreaGroupDBC> AreaGroupDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AreaPOIDBC> AreaPOIDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AreaTriggerDBC> AreaTriggerDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, AuctionHouseDBC> AuctionHouseDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChrClassesDBC> ChrClassesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChrRacesDBC> ChrRacesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CharStartOutfitDBC> CharStartOutfitDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillRaceClassInfoDBC> SkillRaceClassInfoDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillLineDBC> SkillLineDBCMap;

// CharStartOutfitByTripple
typedef std::tuple<uint8_t, uint8_t, uint8_t> CharStartOutfitKey;
typedef std::map<CharStartOutfitKey, CharStartOutfitDBC const*> CharStartOutfitByTripple;

// SkillRaceClassInfoBounds
typedef std::unordered_multimap<uint32_t, SkillRaceClassInfoDBC const*> SkillRaceClassInfoMap;
typedef std::pair<SkillRaceClassInfoMap::iterator, SkillRaceClassInfoMap::iterator> SkillRaceClassInfoBounds;

class BaseServer;

class DBCMgr {
public:
    explicit DBCMgr(std::shared_ptr<BaseServer> server) : server_(std::move(server)) {}

    void initialize();

    ~DBCMgr();

    void cleanUpBeforeDelete();

    // Utilities
    Team teamForRace(uint8_t race);

    // Base functions
    AchievementDBCMap const& getAchievementDBCMap() const { return _achievementMap; }
    uint32_t getAchievementDBCSize() { return _achievementMap.size(); }
    AchievementDBC const* getAchievementDBC(uint32_t id) const
    {
        auto itr = _achievementMap.find(id);
        if (itr != _achievementMap.end())
            return &itr->second;
        return nullptr;
    }
    AchievementCriteriaDBCMap const& getAchievementCriteriaDBCMap() const { return _achievementCriteriaMap; }
    uint32_t getAchievementCriteriaDBCSize() { return _achievementCriteriaMap.size(); }
    AchievementCriteriaDBC const* getAchievementCriteriaDBC(uint32_t id) const
    {
        auto itr = _achievementCriteriaMap.find(id);
        if (itr != _achievementCriteriaMap.end())
            return &itr->second;
        return nullptr;
    }

    AreaTableDBCMap const& getAreaTableDBCMap() const { return _areaTableMap; }
    AreaTableDBC const* getAreaTableDBC(uint32_t id) const
    {
        auto itr = _areaTableMap.find(id);
        if (itr != _areaTableMap.end())
            return &itr->second;
        return nullptr;
    }

    AreaGroupDBC const* getAreaGroupDBC(uint32_t id) const
    {
        auto itr = _areaGroupMap.find(id);
        if (itr != _areaGroupMap.end())
            return &itr->second;
        return nullptr;
    }

    AreaPOIDBC const* getAreaPOIDBC(uint32_t id) const
    {
        auto itr = _areaPOIMap.find(id);
        if (itr != _areaPOIMap.end())
            return &itr->second;
        return nullptr;
    }

    AreaTriggerDBC const* getAreaTriggerDBC(uint32_t id) const
    {
        auto itr = _areaTriggerMap.find(id);
        if (itr != _areaTriggerMap.end())
            return &itr->second;
        return nullptr;
    }

    AuctionHouseDBC const* getAuctionHouseDBC(uint32_t id) const
    {
        auto itr = _auctionHouseMap.find(id);
        if (itr != _auctionHouseMap.end())
            return &itr->second;
        return nullptr;
    }

    ChrClassesDBC const* getChrClassesDBC(uint32_t id)
    {
        auto itr = _chrClassesMap.find(id);
        if (itr != _chrClassesMap.end())
            return &itr->second;
        return nullptr;
    }

    char const* getChrClassName(uint8_t class_, LocaleConstant locale)
    {
        auto itr = _chrClassesMap.find(class_);
        if (itr != _chrClassesMap.end())
            return itr->second.Name[locale].c_str();
        return nullptr;
    }

    ChrRacesDBC const* getChrRacesDBC(uint32_t id)
    {
        auto itr = _chrRacesMap.find(id);
        if (itr != _chrRacesMap.end())
            return &itr->second;
        return nullptr;
    }

    char const* getRaceName(uint8_t race, LocaleConstant locale)
    {
        auto itr = _chrRacesMap.find(race);
        if (itr != _chrRacesMap.end())
            return itr->second.Name[locale].c_str();
        return nullptr;
    }

    CharStartOutfitDBCMap const& getCharStartOutfitDBCMap() const { return _charStartOutfitMap; }
    CharStartOutfitDBC const* getCharStartOutfitDBC(uint8_t race, uint8_t classID, uint8_t gender)
    {
        auto i = _charStartOutfitByTripple.find(CharStartOutfitKey(race, classID, gender));
        if (i != _charStartOutfitByTripple.end())
            return i->second;
        return nullptr;
    }

    SkillRaceClassInfoDBC const* getSkillRaceClassInfo(uint32_t skill, uint8_t race, uint8_t class_)
    {
        SkillRaceClassInfoBounds bounds = _skillRaceClassInfoBySkill.equal_range(skill);
        for (auto itr = bounds.first; itr != bounds.second; ++itr)
        {
            if (itr->second->RaceMask && !(itr->second->RaceMask & (1 << (race - 1))))
                continue;
            if (itr->second->ClassMask && !(itr->second->ClassMask & (1 << (class_ - 1))))
                continue;

            return itr->second;
        }
        return nullptr;
    }

    SkillLineDBCMap const& getSkillLineDBCMap() const { return _skillLineMap; }
    SkillLineDBC const* getSkillLineDBC(uint32_t ID)
    {
        auto itr = _skillLineMap.find(ID);
        if (itr != _skillLineMap.end())
            return &itr->second;
        return nullptr;
    }

private:
    void load_Achievement();        // load Achievement.dbc
    void load_AchievementCriteria();// load Achievement_Criteria.dbc
    void load_AreaTable();          // load AreaTable.dbc
    void load_AreaGroup();          // load AreaGroup.dbc
    void load_AreaPOI();            // load AreaPOI.dbc  (NOT USED)
    void load_AreaTrigger();        // load AreaTrigger.dbc
    void load_AuctionHouse();
    void load_ChrClasses();         // load ChrClasses.dbc
    void load_ChrRaces();           // load ChrRaces.dbc
    void load_CharStartOutfit();    // load CharStartOutfit.dbc
    void load_SkillRaceClassInfo(); // load SkillRaceClassInfo.dbc
    void load_SkillLine();          // load SkillLine.dbc

    std::shared_ptr<BaseServer> server_;

    AchievementDBCMap _achievementMap;
    AchievementCriteriaDBCMap _achievementCriteriaMap;
    AreaTableDBCMap _areaTableMap;
    AreaGroupDBCMap _areaGroupMap;
    AreaPOIDBCMap _areaPOIMap;
    AreaTriggerDBCMap _areaTriggerMap;
    AuctionHouseDBCMap _auctionHouseMap;
    ChrClassesDBCMap _chrClassesMap;
    ChrRacesDBCMap _chrRacesMap;
    CharStartOutfitDBCMap _charStartOutfitMap;
    SkillRaceClassInfoDBCMap _skillRaceClassInfoMap;
    SkillLineDBCMap _skillLineMap;

    // Handle others containers
    void initialize_Additional_Data();
    void handle_CharStartOutfitByTripple();
    void handle_SkillRaceClassInfo();

    CharStartOutfitByTripple _charStartOutfitByTripple;
    SkillRaceClassInfoMap _skillRaceClassInfoBySkill;
};