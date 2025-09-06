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
typedef std::unordered_map<uint32_t /*ID*/, CinematicSequencesDBC> CinematicSequencesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureDisplayInfoDBC> CreatureDisplayInfoDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureDisplayInfoExtraDBC> CreatureDisplayInfoExtraDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureFamilyDBC> CreatureFamilyDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureModelDataDBC> CreatureModelDataDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureSpellDataDBC> CreatureSpellDataDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CreatureTypeDBC> CreatureTypeDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, CurrencyCategoryDBC> CurrencyCategoryDBCMap;
typedef std::unordered_map<uint32_t /*ItemID*/, CurrencyTypesDBC> CurrencyTypesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, DestructibleModelDataDBC> DestructibleModelDataDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, DungeonEncounterDBC> DungeonEncounterDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, DurabilityCostsDBC> DurabilityCostsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, DurabilityQualityDBC> DurabilityQualityDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, EmotesDBC> EmotesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, EmotesTextDBC> EmotesTextDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, EmotesTextSoundDBC> EmotesTextSoundDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, FactionDBC> FactionDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, FactionTemplateDBC> FactionTemplateDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GameObjectArtKitDBC> GameObjectArtKitDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GameObjectDisplayInfoDBC> GameObjectDisplayInfoDBCMap;
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

// EmotesTextSoundByTripple
typedef std::tuple<uint32_t, uint8_t, uint8_t> EmotesTextSoundKey;
typedef std::map<EmotesTextSoundKey, EmotesTextSoundDBC const*> EmotesTextSoundByTripple;

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
        auto itr = _chatChannelsMap.find(id);
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

    CinematicSequencesDBC const* getCinematicSequencesDBC(uint32_t id)
    {
        auto itr = _cinematicSequencesMap.find(id);
        if (itr != _cinematicSequencesMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureDisplayInfoDBC const* getCreatureDisplayInfoDBC(uint32_t id)
    {
        auto itr = _creatureDisplayInfoMap.find(id);
        if (itr != _creatureDisplayInfoMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureDisplayInfoExtraDBC const* getCreatureDisplayInfoExtraDBC(uint32_t id)
    {
        auto itr = _creatureDisplayInfoExtraMap.find(id);
        if (itr != _creatureDisplayInfoExtraMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureFamilyDBCMap const& getCreatureFamilyDBCMap() const { return _creatureFamilyMap; }
    CreatureFamilyDBC const* getCreatureFamilyDBC(uint32_t id)
    {
        auto itr = _creatureFamilyMap.find(id);
        if (itr != _creatureFamilyMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureModelDataDBC const* getCreatureModelDataDBC(uint32_t id)
    {
        auto itr = _creatureModelDataMap.find(id);
        if (itr != _creatureModelDataMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureSpellDataDBC const* getCreatureSpellDataDBC(uint32_t id)
    {
        auto itr = _creatureSpellDataMap.find(id);
        if (itr != _creatureSpellDataMap.end())
            return &itr->second;
        return nullptr;
    }

    CreatureTypeDBC const* getCreatureTypeDBC(uint32_t id)
    {
        auto itr = _creatureTypeMap.find(id);
        if (itr != _creatureTypeMap.end())
            return &itr->second;
        return nullptr;
    }

    CurrencyCategoryDBC const* getCurrencyCategoryDBC(uint32_t id)
    {
        auto itr = _currencyCategoryMap.find(id);
        if (itr != _currencyCategoryMap.end())
            return &itr->second;
        return nullptr;
    }

    CurrencyTypesDBC const* getCurrencyTypesDBCByItemID(uint32_t ItemID)
    {
        auto itr = _currencyTypesMap.find(ItemID);
        if (itr != _currencyTypesMap.end())
            return &itr->second;
        return nullptr;
    }

    DestructibleModelDataDBC const* getDestructibleModelDataDBC(uint32_t ID)
    {
        auto itr = _destructibleModelDataMap.find(ID);
        if (itr != _destructibleModelDataMap.end())
            return &itr->second;
        return nullptr;
    }

    DungeonEncounterDBC const* getDungeonEncounterDBC(uint32_t ID)
    {
        auto itr = _dungeonEncounterMap.find(ID);
        if (itr != _dungeonEncounterMap.end())
            return &itr->second;
        return nullptr;
    }

    DurabilityCostsDBC const* getDurabilityCostsDBC(uint32_t ID)
    {
        auto itr = _durabilityCoastsMap.find(ID);
        if (itr != _durabilityCoastsMap.end())
            return &itr->second;
        return nullptr;
    }

    DurabilityQualityDBC const* getDurabilityQualityDBC(uint32_t ID)
    {
        auto itr = _durabilityQualityMap.find(ID);
        if (itr != _durabilityQualityMap.end())
            return &itr->second;
        return nullptr;
    }

    EmotesDBC const* getEmotesDBC(uint32_t ID)
    {
        auto itr = _emotesMap.find(ID);
        if (itr != _emotesMap.end())
            return &itr->second;
        return nullptr;
    }

    EmotesTextDBC const* getEmotesTextDBC(uint32_t ID)
    {
        auto itr = _emotesTextMap.find(ID);
        if (itr != _emotesTextMap.end())
            return &itr->second;
        return nullptr;
    }

    EmotesTextSoundDBC const* getEmotesTextSoundDBCWithParam(uint32_t emote, uint8_t race, uint8_t gender)
    {
        auto i = _emotesTextSoundByTripple.find(EmotesTextSoundKey(emote, race, gender));
        if (i != _emotesTextSoundByTripple.end())
            return i->second;
        return nullptr;
    }

    FactionDBCMap const& getFactionDBCMap() const { return _factionMap; }
    FactionDBC const* getFactionDBC(uint32_t ID)
    {
        auto itr = _factionMap.find(ID);
        if (itr != _factionMap.end())
            return &itr->second;
        return nullptr;
    }

    FactionTemplateDBC const* getFactionTemplateDBC(uint32_t ID)
    {
        auto itr = _factionTemplateMap.find(ID);
        if (itr != _factionTemplateMap.end())
            return &itr->second;
        return nullptr;
    }

    GameObjectArtKitDBC const* getGameObjectArtKitDBC(uint32_t ID)
    {
        auto itr = _gameobjectArtKitMap.find(ID);
        if (itr != _gameobjectArtKitMap.end())
            return &itr->second;
        return nullptr;
    }

    GameObjectDisplayInfoDBC const* getGameObjectDisplayInfoDBC(uint32_t ID)
    {
        auto itr = _gameobjectDisplayInfoMap.find(ID);
        if (itr != _gameobjectDisplayInfoMap.end())
            return &itr->second;
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
    void load_CinematicSequences();         // load CinematicSequences.dbc
    void load_CreatureDisplayInfo();        // load CreatureDisplayInfo.dbc
    void load_CreatureDisplayInfoExtra();   // load CreatureDisplayInfoExtra.dbc
    void load_CreatureFamily();             // load CreatureFamily.dbc
    void load_CreatureModelData();          // load CreatureModelData.dbc
    void load_CreatureSpellData();          // load CreatureSpellData.dbc
    void load_CreatureType();               // load CreatureSpellData.dbc
    void load_CurrencyCategory();           // load CurrencyCategory.dbc (NOT USED)
    void load_CurrencyTypes();              // load CurrencyTypes.dbc
    void load_DestructibleModelData();      // load DestructibleModelData.dbc
    void load_DungeonEncounter();           // load DungeonEncounter.dbc
    void load_DurabilityCosts();            // load DurabilityCosts.dbc
    void load_DurabilityQuality();          // load DurabilityQuality.dbc
    void load_Emotes();                     // load Emotes.dbc
    void load_EmotesText();                 // load EmotesText.dbc
    void load_EmotesTextSound();            // load EmotesTextSound.dbc
    void load_Faction();                    // load Faction.dbc
    void load_FactionTemplate();            // load FactionTemplate.dbc
    void load_GameObjectArtKit();           // load GameObjectArtKit.dbc
    void load_GameObjectDisplayInfo();      // load GameObjectDisplayInfo.dbc
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
    CinematicSequencesDBCMap _cinematicSequencesMap;
    CreatureDisplayInfoDBCMap _creatureDisplayInfoMap;
    CreatureDisplayInfoExtraDBCMap _creatureDisplayInfoExtraMap;
    CreatureFamilyDBCMap _creatureFamilyMap;
    CreatureModelDataDBCMap _creatureModelDataMap;
    CreatureSpellDataDBCMap _creatureSpellDataMap;
    CreatureTypeDBCMap _creatureTypeMap;
    CurrencyCategoryDBCMap _currencyCategoryMap;
    CurrencyTypesDBCMap _currencyTypesMap;
    DestructibleModelDataDBCMap _destructibleModelDataMap;
    DungeonEncounterDBCMap _dungeonEncounterMap;
    DurabilityCostsDBCMap _durabilityCoastsMap;
    DurabilityQualityDBCMap _durabilityQualityMap;
    EmotesDBCMap _emotesMap;
    EmotesTextDBCMap _emotesTextMap;
    EmotesTextSoundDBCMap _emotesTextSoundMap;
    FactionDBCMap _factionMap;
    FactionTemplateDBCMap _factionTemplateMap;
    GameObjectArtKitDBCMap _gameobjectArtKitMap;
    GameObjectDisplayInfoDBCMap _gameobjectDisplayInfoMap;
    SkillRaceClassInfoDBCMap _skillRaceClassInfoMap;
    SkillLineDBCMap _skillLineMap;

    uint32_t _bannedAddonsHighestID;

    // Handle others containers
    void initialize_Additional_Data();
    void handle_CharacterFacialHairStylesByTripple();
    void handle_CharSectionsByPenta();
    void handle_CharStartOutfitByTripple();
    void handle_EmotesTextSoundByTripple();
    void handle_SkillRaceClassInfo();

    CharacterFacialHairStylesByTripple _characterFacialHairStylesByTripple;
    CharSectionsByPenta _charSectionsByPenta;
    CharStartOutfitByTripple _charStartOutfitByTripple;
    EmotesTextSoundByTripple _emotesTextSoundByTripple;
    SkillRaceClassInfoMap _skillRaceClassInfoBySkill;
};