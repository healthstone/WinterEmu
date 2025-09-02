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
typedef std::unordered_map<uint32_t /*ID*/, BankBagSlotPricesDBC> BankBagSlotPricesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, BannedAddOnsDBC> BannedAddOnsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, BarberShopStyleDBC> BarberShopStyleDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, BattlemasterListDBC> BattlemasterListDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CharacterFacialHairStylesDBC> CharacterFacialHairStylesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CharSectionsDBC> CharSectionsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CharStartOutfitDBC> CharStartOutfitDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CharTitlesDBC> CharTitlesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChatChannelsDBC> ChatChannelsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChrClassesDBC> ChrClassesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ChrRacesDBC> ChrRacesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CinematicCameraDBC> CinematicCameraDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillRaceClassInfoDBC> SkillRaceClassInfoDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillLineDBC> SkillLineDBCMap;

// tuples for the Fastest search by more indexes
// CharacterFacialHairStylesByTripple
typedef std::tuple<uint8_t, uint8_t, uint8_t> CharacterFacialHairStylesKey;
typedef std::map<CharacterFacialHairStylesKey, CharacterFacialHairStylesDBC const*> CharacterFacialHairStylesByTripple;

// CharSectionsByPenta
typedef std::tuple<uint8_t, CharSectionType, uint8_t, uint8_t, uint8_t> CharSectionsKey;
typedef std::map<CharSectionsKey, CharSectionsDBC const*> CharSectionsByPenta;

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

    BankBagSlotPricesDBC const* getBankBagSlotPricesDBC(uint32_t id) const
    {
        auto itr = _bankBagSlotPricesMap.find(id);
        if (itr != _bankBagSlotPricesMap.end())
            return &itr->second;
        return nullptr;
    }

    uint32_t getBannedAddOnsHighestID() const { return _bannedAddonsHighestID; }
    BannedAddOnsDBC const* getBannedAddOnsDBC(uint32_t id) const
    {
        auto itr = _bannedAddonsMap.find(id);
        if (itr != _bannedAddonsMap.end())
            return &itr->second;
        return nullptr;
    }

    BarberShopStyleDBC const* getBarberShopStyleDBC(uint32_t id) const
    {
        auto itr = _barberShopStyleMap.find(id);
        if (itr != _barberShopStyleMap.end())
            return &itr->second;
        return nullptr;
    }

    BattlemasterListDBCMap const& getBattlemasterListDBCMap() const { return _battlemasterListMap; }
    BattlemasterListDBC const* getBattlemasterListDBC(uint32_t id) const
    {
        auto itr = _battlemasterListMap.find(id);
        if (itr != _battlemasterListMap.end())
            return &itr->second;
        return nullptr;
    }

    CharacterFacialHairStylesDBC const* getCharFacialHairDBC(uint8_t race, uint8_t gender, uint8_t facialHairID)
    {
        auto i = _characterFacialHairStylesByTripple.find(CharacterFacialHairStylesKey(race, gender, facialHairID));
        if (i != _characterFacialHairStylesByTripple.end())
            return i->second;
        return nullptr;
    }

    CharSectionsDBC const* getCharSectionsDBC(uint8_t race, CharSectionType genType, uint8_t gender, uint8_t type, uint8_t color)
    {
        auto i = _charSectionsByPenta.find(CharSectionsKey(race, genType, gender, type, color));
        if (i != _charSectionsByPenta.end())
            return i->second;
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

    CharTitlesDBCMap const& getCharTitlesDBCMap() const { return _charTitlesMap; }
    CharTitlesDBC const* getCharTitlesDBC(uint32_t id)
    {
        auto itr = _charTitlesMap.find(id);
        if (itr != _charTitlesMap.end())
            return &itr->second;
        return nullptr;
    }

    ChatChannelsDBCMap const& getChatChannelsDBCMap() const { return _chatChannelsMap; }
    ChatChannelsDBC const* getChatChannelsDBC(uint32_t id)
    {
        ChatChannelsDBCMap::const_iterator itr = _chatChannelsMap.find(id);
        if (itr != _chatChannelsMap.end())
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

    CinematicCameraDBCMap const& getCinematicCameraDBCMap() const { return _cinematicCameraMap; }

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
    void load_Achievement();                // load Achievement.dbc
    void load_AchievementCriteria();        // load Achievement_Criteria.dbc
    void load_AreaTable();                  // load AreaTable.dbc
    void load_AreaGroup();                  // load AreaGroup.dbc
    void load_AreaPOI();                    // load AreaPOI.dbc  (NOT USED)
    void load_AreaTrigger();                // load AreaTrigger.dbc
    void load_AuctionHouse();               // load AuctionHouse.dbc
    void load_BankBagSlotPrices();          // load BankBagSlotPrices.dbc
    void load_BannedAddOns();               // load BannedAddOns.dbc
    void load_BarberShopStyle();            // load BarberShopStyle.dbc
    void load_BattlemasterList();           // load BattlemasterList.dbc
    void load_CharacterFacialHairStyles();  // load CharacterFacialHairStyles.dbc
    void load_CharSections();               // load CharSections.dbc
    void load_CharStartOutfit();            // load CharStartOutfit.dbc
    void load_CharTitles();                 // load CharTitles.dbc
    void load_ChatChannels();               // load ChatChannels.dbc
    void load_ChrClasses();                 // load ChrClasses.dbc
    void load_ChrRaces();                   // load ChrRaces.dbc
    void load_CinematicCamera();            // load CinematicCamera.dbc
    void load_SkillRaceClassInfo();         // load SkillRaceClassInfo.dbc
    void load_SkillLine();                  // load SkillLine.dbc

    std::shared_ptr<BaseServer> server_;

    AchievementDBCMap _achievementMap;
    AchievementCriteriaDBCMap _achievementCriteriaMap;
    AreaTableDBCMap _areaTableMap;
    AreaGroupDBCMap _areaGroupMap;
    AreaPOIDBCMap _areaPOIMap;
    AreaTriggerDBCMap _areaTriggerMap;
    AuctionHouseDBCMap _auctionHouseMap;
    BankBagSlotPricesDBCMap _bankBagSlotPricesMap;
    BannedAddOnsDBCMap _bannedAddonsMap;
    BarberShopStyleDBCMap _barberShopStyleMap;
    BattlemasterListDBCMap _battlemasterListMap;
    CharacterFacialHairStylesDBCMap _characterFacialHairStyleMap;
    CharSectionsDBCMap _charSectionMap;
    CharStartOutfitDBCMap _charStartOutfitMap;
    CharTitlesDBCMap _charTitlesMap;
    ChatChannelsDBCMap _chatChannelsMap;
    ChrClassesDBCMap _chrClassesMap;
    ChrRacesDBCMap _chrRacesMap;
    CinematicCameraDBCMap _cinematicCameraMap;
    SkillRaceClassInfoDBCMap _skillRaceClassInfoMap;
    SkillLineDBCMap _skillLineMap;

    uint32_t _bannedAddonsHighestID;

    // Handle others containers
    void initialize_Additional_Data();
    void handle_CharacterFacialHairStylesByTripple();
    void handle_CharSectionsByPenta();
    void handle_CharStartOutfitByTripple();
    void handle_SkillRaceClassInfo();

    CharacterFacialHairStylesByTripple _characterFacialHairStylesByTripple;
    CharSectionsByPenta _charSectionsByPenta;
    CharStartOutfitByTripple _charStartOutfitByTripple;
    SkillRaceClassInfoMap _skillRaceClassInfoBySkill;
};