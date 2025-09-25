#pragma once

#include <string>
#include <array>
#include <unordered_map>
#include <memory>
#include <map>
#include <vector>
#include <boost/regex.hpp>
#include "src/game/enums/DBCStructure.hpp"
#include "src/game/enums/Team.hpp"
#include "src/game/enums/ResponseCodes.hpp"

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
typedef std::unordered_map<uint32_t /*ID*/, GemPropertiesDBC> GemPropertiesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GlyphPropertiesDBC> GlyphPropertiesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GlyphSlotDBC> GlyphSlotDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtBarberShopCostBaseDBC> GtBarberShopCostBaseDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtChanceToMeleeCritDBC> GtChanceToMeleeCritDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtChanceToMeleeCritBaseDBC> GtChanceToMeleeCritBaseDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtChanceToSpellCritDBC> GtChanceToSpellCritDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtChanceToSpellCritBaseDBC> GtChanceToSpellCritBaseDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtCombatRatingsDBC> GtCombatRatingsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtNPCManaCostScalerDBC> GtNPCManaCostScalerDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtOCTClassCombatRatingScalarDBC> GtOCTClassCombatRatingScalarDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtOCTRegenHPDBC> GtOCTRegenHPDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtOCTRegenMPDBC> GtOCTRegenMPDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtRegenHPPerSptDBC> GtRegenHPPerSptDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, GtRegenMPPerSptDBC> GtRegenMPPerSptDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, HolidaysDBC> HolidaysDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemDBC> ItemDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemBagFamilyDBC> ItemBagFamilyDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemDisplayInfoDBC> ItemDisplayInfoDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemExtendedCostDBC> ItemExtendedCostDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemLimitCategoryDBC> ItemLimitCategoryDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemRandomPropertiesDBC> ItemRandomPropertiesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemRandomSuffixDBC> ItemRandomSuffixDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ItemSetDBC> ItemSetDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, LFGDungeonDBC> LFGDungeonDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, LightDBC> LightDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, LiquidTypeDBC> LiquidTypeDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, LockDBC> LockDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, MailTemplateDBC> MailTemplateDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, MapDBC> MapDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, MapDifficultyDBC> MapDifficultyDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, MovieDBC> MovieDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, NamesProfanityDBC> NamesProfanityDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, NamesReservedDBC> NamesReservedDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, OverrideSpellDataDBC> OverrideSpellDataDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, PowerDisplayDBC> PowerDisplayDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, PvPDifficultyDBC> PvPDifficultyDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, QuestFactionRewardDBC> QuestFactionRewardDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, QuestSortDBC> QuestSortDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, QuestXPDBC> QuestXPDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, RandPropPointsDBC> RandPropPointsDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, ScalingStatDistributionDBC> ScalingStatDistributionDBCMap;
typedef std::unordered_map<uint32_t /*CharLevel*/, ScalingStatValuesDBC> ScalingStatValuesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillLineDBC> SkillLineDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillLineAbilityDBC> SkillLineAbilityDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillRaceClassInfoDBC> SkillRaceClassInfoDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SkillTiersDBC> SkillTiersDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SoundEntriesDBC> SoundEntriesDBCMap;
typedef std::unordered_map<uint32_t /*ID*/, SpellDBC> SpellDBCMap;

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

// LFGDungeonDBCByDouble
typedef std::tuple<int32_t, Difficulty> LFGDungeonKey;
typedef std::map<LFGDungeonKey, LFGDungeonDBC const*> LFGDungeonDBCByDouble;

// MapDifficultyByDouble
typedef std::tuple<uint32_t, Difficulty> MapDifficultyKey;
typedef std::map<MapDifficultyKey, MapDifficultyDBC const*> MapDifficultyByDouble;

// regex
typedef std::array<std::vector<boost::wregex>, TOTAL_LOCALES> NameValidationRegexContainer;

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
    ResponseCodes validateName(std::wstring const& name, LocaleConstant locale);

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

    GemPropertiesDBC const* getGemPropertiesDBC(uint32_t ID)
    {
        auto itr = _gemPropertiesMap.find(ID);
        if (itr != _gemPropertiesMap.end())
            return &itr->second;
        return nullptr;
    }

    GlyphPropertiesDBC const* getGlyphPropertiesDBC(uint32_t ID)
    {
        auto itr = _glyphPropertiesMap.find(ID);
        if (itr != _glyphPropertiesMap.end())
            return &itr->second;
        return nullptr;
    }

    GlyphSlotDBCMap const& getGlyphSlotDBCMap() const { return _glyphSlotMap; }
    GlyphSlotDBC const* getGlyphSlotDBC(uint32_t ID)
    {
        auto itr = _glyphSlotMap.find(ID);
        if (itr != _glyphSlotMap.end())
            return &itr->second;
        return nullptr;
    }

    GtBarberShopCostBaseDBC const* getGtBarberShopCostBaseDBC(uint32_t ID)
    {
        auto itr = _gtBarberShopCostBaseMap.find(ID);
        if (itr != _gtBarberShopCostBaseMap.end())
            return &itr->second;
        return nullptr;
    }

    GtChanceToMeleeCritDBC const* getGtChanceToMeleeCritDBC(uint32_t ID)
    {
        auto itr = _gtChanceToMeleeCritMap.find(ID);
        if (itr != _gtChanceToMeleeCritMap.end())
            return &itr->second;
        return nullptr;
    }

    GtChanceToMeleeCritBaseDBC const* getGtChanceToMeleeCritBaseDBC(uint32_t ID)
    {
        auto itr = _gtChanceToMeleeCritBaseMap.find(ID);
        if (itr != _gtChanceToMeleeCritBaseMap.end())
            return &itr->second;
        return nullptr;
    }

    GtChanceToSpellCritDBC const* getGtChanceToSpellCritDBC(uint32_t ID)
    {
        auto itr = _gtChanceToSpellCritMap.find(ID);
        if (itr != _gtChanceToSpellCritMap.end())
            return &itr->second;
        return nullptr;
    }

    GtChanceToSpellCritBaseDBC const* GetGtChanceToSpellCritBaseDBC(uint32_t ID)
    {
        auto itr = _gtChanceToSpellCritBaseMap.find(ID);
        if (itr != _gtChanceToSpellCritBaseMap.end())
            return &itr->second;
        return nullptr;
    }

    GtCombatRatingsDBC const* getGtCombatRatingsDBC(uint32_t ID)
    {
        auto itr = _gtCombatRatingsMap.find(ID);
        if (itr != _gtCombatRatingsMap.end())
            return &itr->second;
        return nullptr;
    }

    GtNPCManaCostScalerDBC const* getGtNPCManaCostScalerDBC(uint32_t ID)
    {
        auto itr = _gtNPCManaCostScalerMap.find(ID);
        if (itr != _gtNPCManaCostScalerMap.end())
            return &itr->second;
        return nullptr;
    }

    GtOCTClassCombatRatingScalarDBC const* getGtOCTClassCombatRatingScalarDBC(uint32_t ID)
    {
        auto itr = _gtOCTClassCombatRatingScalarMap.find(ID);
        if (itr != _gtOCTClassCombatRatingScalarMap.end())
            return &itr->second;
        return nullptr;
    }

    GtOCTRegenHPDBC const* getGtOCTRegenHPDBC(uint32_t ID)
    {
        auto itr = _gtOCTRegenHPMap.find(ID);
        if (itr != _gtOCTRegenHPMap.end())
            return &itr->second;
        return nullptr;
    }

    GtOCTRegenMPDBC const* getGtOCTRegenMPDBC(uint32_t ID)
    {
        auto itr = _gtOCTRegenMPMap.find(ID);
        if (itr != _gtOCTRegenMPMap.end())
            return &itr->second;
        return nullptr;
    }

    GtRegenHPPerSptDBC const* getGtRegenHPPerSptDBC(uint32_t ID)
    {
        auto itr = _gtRegenHPPerSptMap.find(ID);
        if (itr != _gtRegenHPPerSptMap.end())
            return &itr->second;
        return nullptr;
    }

    GtRegenMPPerSptDBC const* getGtRegenMPPerSptDBC(uint32_t ID)
    {
        auto itr = _gtRegenMPPerSptMap.find(ID);
        if (itr != _gtRegenMPPerSptMap.end())
            return &itr->second;
        return nullptr;
    }

    HolidaysDBC const* GetHolidaysDBC(uint32_t ID)
    {
        auto itr = _holidaysMap.find(ID);
        if (itr != _holidaysMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemDBCMap const& getItemDBCMap() const { return _itemMap; }
    ItemDBC const* getItemDBC(uint32_t ID)
    {
        auto itr = _itemMap.find(ID);
        if (itr != _itemMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemBagFamilyDBC const* getItemBagFamilyDBC(uint32_t ID)
    {
        auto itr = _itemBagFamilyMap.find(ID);
        if (itr != _itemBagFamilyMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemDisplayInfoDBC const* getItemDisplayInfoDBC(uint32_t ID)
    {
        auto itr = _itemDisplayInfoMap.find(ID);
        if (itr != _itemDisplayInfoMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemExtendedCostDBC const* getItemExtendedCostDBC(uint32_t ID)
    {
        auto itr = _itemExtendedCostMap.find(ID);
        if (itr != _itemExtendedCostMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemLimitCategoryDBC const* getItemLimitCategoryDBC(uint32_t ID)
    {
        auto itr = _itemLimitCategoryMap.find(ID);
        if (itr != _itemLimitCategoryMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemRandomPropertiesDBC const* getItemRandomPropertiesDBC(uint32_t ID)
    {
        auto itr = _itemRandomPropertiesMap.find(ID);
        if (itr != _itemRandomPropertiesMap.end())
            return &itr->second;
        return nullptr;
    }

    uint32_t getItemRandomSuffixHighestID() const { return _itemRandomSuffixHighestID; }
    ItemRandomSuffixDBC const* getItemRandomSuffixDBC(uint32_t ID)
    {
        auto itr = _itemRandomSuffixMap.find(ID);
        if (itr != _itemRandomSuffixMap.end())
            return &itr->second;
        return nullptr;
    }

    ItemSetDBCMap const& getItemSetMap() const { return _itemSetMap; }
    ItemSetDBC const* getItemSetDBC(uint32_t ID)
    {
        auto itr = _itemSetMap.find(ID);
        if (itr != _itemSetMap.end())
            return &itr->second;
        return nullptr;
    }

    LFGDungeonDBCMap const& getLFGDungeonDBCMap() const { return _lfgDungeonMap; }
    LFGDungeonDBC const* getLFGDungeonDBC(uint32_t ID)
    {
        auto itr = _lfgDungeonMap.find(ID);
        if (itr != _lfgDungeonMap.end())
            return &itr->second;
        return nullptr;
    }

    LightDBC const* getLightDBC(uint32_t ID)
    {
        auto itr = _lightMap.find(ID);
        if (itr != _lightMap.end())
            return &itr->second;
        return nullptr;
    }

    uint32_t GetDefaultMapLight(uint32_t mapId) // UNUSED
    {
        for (LightDBCMap::const_iterator itr = _lightMap.begin(); itr != _lightMap.end(); ++itr)
        {
            if (itr->second.ContinentID == mapId &&
                itr->second.GameCoords.X == 0.0f &&
                itr->second.GameCoords.Y == 0.0f &&
                itr->second.GameCoords.Z == 0.0f)
                return itr->second.ID;
        }
        return 0;
    }

    LFGDungeonDBC const* getLFGDungeon(uint32_t mapId, Difficulty difficulty)
    {
        auto i = _lfgDungeonByDouble.find(LFGDungeonKey(mapId, difficulty));
        if (i != _lfgDungeonByDouble.end())
            return i->second;
        return nullptr;
    }

    LiquidTypeDBCMap const& getLiquidTypeDBCMap() const { return _liquidTypeMap; }
    LiquidTypeDBC const* getLiquidTypeDBC(uint32_t ID)
    {
        auto itr = _liquidTypeMap.find(ID);
        if (itr != _liquidTypeMap.end())
            return &itr->second;
        return nullptr;
    }

    LockDBC const* getLockDBC(uint32_t ID)
    {
        auto itr = _lockMap.find(ID);
        if (itr != _lockMap.end())
            return &itr->second;
        return nullptr;
    }

    MailTemplateDBCMap const& getMailTemplateDBCMap() const { return _mailTemplateMap; }
    MailTemplateDBC const* getMailTemplateDBC(uint32_t ID)
    {
        auto itr = _mailTemplateMap.find(ID);
        if (itr != _mailTemplateMap.end())
            return &itr->second;
        return nullptr;
    }

    MapDBCMap const& getMapDBCMap() const { return _mapMap; }
    MapDBC const* getMapDBC(uint32_t ID)
    {
        auto itr = _mapMap.find(ID);
        if (itr != _mapMap.end())
            return &itr->second;
        return nullptr;
    }

    MapDifficultyDBCMap const& getMapDifficultyDBCMap() const { return _mapDifficultyMap; }
    MapDifficultyDBC const* getMapDifficultyData(uint32_t mapId, Difficulty difficulty)
    {
        auto i = _mapDifficultyByDouble.find(MapDifficultyKey(mapId, difficulty));
        if (i != _mapDifficultyByDouble.end())
            return i->second;
        return nullptr;
    }

    MapDifficultyDBC const* getDownscaledMapDifficultyData(uint32_t mapId, Difficulty& difficulty)
    {
        uint32_t tmpDiff = difficulty;
        MapDifficultyDBC const* mapDiff = getMapDifficultyData(mapId, Difficulty(tmpDiff));
        if (!mapDiff)
        {
            if (tmpDiff > RAID_DIFFICULTY_25MAN_NORMAL) // heroic, downscale to normal
                tmpDiff -= 2;
            else
                tmpDiff -= 1;   // any non-normal mode for raids like tbc (only one mode)

            // pull new data
            mapDiff = getMapDifficultyData(mapId, Difficulty(tmpDiff)); // we are 10 normal or 25 normal
            if (!mapDiff)
            {
                tmpDiff -= 1;
                mapDiff = getMapDifficultyData(mapId, Difficulty(tmpDiff)); // 10 normal
            }
        }

        difficulty = Difficulty(tmpDiff);
        return mapDiff;
    }

    MovieDBC const* getMovieDBC(uint32_t ID)
    {
        auto itr = _movieMap.find(ID);
        if (itr != _movieMap.end())
            return &itr->second;
        return nullptr;
    }

    OverrideSpellDataDBC const* getOverrideSpellDataDBC(uint32_t ID)
    {
        auto itr = _overrideSpellDataMap.find(ID);
        if (itr != _overrideSpellDataMap.end())
            return &itr->second;
        return nullptr;
    }

    PowerDisplayDBC const* getPowerDisplayDBC(uint32_t ID)
    {
        auto itr = _powerDisplayMap.find(ID);
        if (itr != _powerDisplayMap.end())
            return &itr->second;
        return nullptr;
    }

    PvPDifficultyDBC const* getBattlegroundBracketByLevel(uint32_t mapid, uint32_t level)
    {
        PvPDifficultyDBC const* maxEntry = nullptr;              // used for level > max listed level case
        for (PvPDifficultyDBCMap::const_iterator itr = _pvpDifficultyMap.begin(); itr != _pvpDifficultyMap.end(); ++itr)
        {
            // skip unrelated and too-high brackets
            if (itr->second.MapID != mapid || itr->second.MinLevel > level)
                continue;

            // exactly fit
            if (itr->second.MaxLevel >= level)
                return &itr->second;

            // remember for possible out-of-range case (search higher from existed)
            if (!maxEntry || maxEntry->MaxLevel < itr->second.MaxLevel)
                maxEntry = &itr->second;
        }
        return maxEntry;
    }

    PvPDifficultyDBC const* getBattlegroundBracketById(uint32_t mapid, BattlegroundBracketId id)
    {
        for (PvPDifficultyDBCMap::const_iterator itr = _pvpDifficultyMap.begin(); itr != _pvpDifficultyMap.end(); ++itr)
        {
            if (itr->second.MapID == mapid && itr->second.GetBracketId() == id)
                return &itr->second;
        }
        return nullptr;
    }

    QuestFactionRewardDBC const* getQuestFactionRewardDBC(uint32_t ID)
    {
        auto itr = _questFactionRewardMap.find(ID);
        if (itr != _questFactionRewardMap.end())
            return &itr->second;
        return nullptr;
    }

    QuestSortDBC const* getQuestSortDBC(uint32_t ID)
    {
        auto itr = _questSortMap.find(ID);
        if (itr != _questSortMap.end())
            return &itr->second;
        return nullptr;
    }

    QuestXPDBC const* getQuestXPDBC(uint32_t ID)
    {
        auto itr = _questXPMap.find(ID);
        if (itr != _questXPMap.end())
            return &itr->second;
        return nullptr;
    }

    RandPropPointsDBC const* getRandPropPointsDBC(uint32_t ID)
    {
        auto itr = _randPropPointsMap.find(ID);
        if (itr != _randPropPointsMap.end())
            return &itr->second;
        return nullptr;
    }

    ScalingStatDistributionDBC const* getScalingStatDistributionDBC(uint32_t ID)
    {
        auto itr = _scalingStatDistributionMap.find(ID);
        if (itr != _scalingStatDistributionMap.end())
            return &itr->second;
        return nullptr;
    }

    ScalingStatValuesDBC const* getScalingStatValuesDBCForLevel(uint32_t charLevel)
    {
        auto itr = _scalingStatValuesMap.find(charLevel);
        if (itr != _scalingStatValuesMap.end())
            return &itr->second;
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

    SkillLineAbilityDBCMap const& getSkillLineAbilityDBCMap() const { return _skillLineAbilityMap; }
    SkillLineAbilityDBC const* getSkillLineAbilityDBC(uint32_t ID)
    {
        auto itr = _skillLineAbilityMap.find(ID);
        if (itr != _skillLineAbilityMap.end())
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

    SkillTiersDBC const* getSkillTiersDBC(uint32_t ID)
    {
        auto itr = _skillTiersMap.find(ID);
        if (itr != _skillTiersMap.end())
            return &itr->second;
        return nullptr;
    }

    SoundEntriesDBC const* getSoundEntriesDBC(uint32_t ID)
    {
        auto itr = _soundEntriesMap.find(ID);
        if (itr != _soundEntriesMap.end())
            return &itr->second;
        return nullptr;
    }

    uint32_t getSpellDBCHighestID() const { return _spellHighestID; }
    SpellDBCMap const& getSpellDBCMap() const { return _spellMap; }
    SpellDBC const* getSpellDBC(uint32_t ID)
    {
        auto itr = _spellMap.find(ID);
        if (itr != _spellMap.end())
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
    void load_GemProperties();              // load GemProperties.dbc
    void load_GlyphProperties();            // load GlyphProperties.dbc
    void load_GlyphSlot();                  // load GlyphSlot.dbc
    void load_gtBarberShopCostBase();       // load gtBarberShopCostBase.dbc
    void load_gtChanceToMeleeCrit();        // load gtChanceToMeleeCrit.dbc
    void load_gtChanceToMeleeCritBase();    // load gtChanceToMeleeCritBase.dbc
    void load_gtChanceToSpellCrit();        // load gtChanceToSpellCrit.dbc
    void load_gtChanceToSpellCritBase();    // load gtChanceToSpellCritBase.dbc
    void load_gtCombatRatings();            // load gtCombatRatings.dbc
    void load_gtNPCManaCostScaler();        // load gtNPCManaCostScaler.dbc
    void load_gtOCTClassCombatRatingScalar();// load gtOCTClassCombatRatingScalar.dbc
    void load_gtOCTRegenHP();               // load gtOCTRegenHP.dbc
    void load_gtOCTRegenMP();               // load gtOCTRegenMP.dbc
    void load_gtRegenHPPerSpt();            // load gtRegenHPPerSpt.dbc
    void load_gtRegenMPPerSpt();            // load gtRegenMPPerSpt.dbc
    void load_Holidays();                   // load Holidays.dbc
    void load_Item();                       // load Item.dbc
    void load_ItemBagFamily();              // load ItemBagFamily.dbc
    void load_ItemDisplayInfo();            // load ItemDisplayInfo.dbc
    void load_ItemExtendedCost();           // load ItemExtendedCost.dbc
    void load_ItemLimitCategory();          // load ItemLimitCategory.dbc
    void load_ItemRandomProperties();       // load ItemRandomProperties.dbc
    void load_ItemRandomSuffix();           // load ItemRandomSuffix.dbc
    void load_ItemSet();                    // load ItemSet.dbc
    void load_LFGDungeons();                // load LFGDungeons.dbc
    void load_Light();                      // load Light.dbc
    void load_LiquidType();                 // load LiquidType.dbc
    void load_Lock();                       // load Lock.dbc
    void load_MailTemplate();               // load MailTemplate.dbc
    void load_Map();                        // load Map.dbc
    void load_MapDifficulty();              // load MapDifficulty.dbc
    void load_Movie();                      // load Movie.dbc
    void load_NamesProfanity();             // load NamesProfanity.dbc
    void load_NamesReserved();              // load NamesReserved.dbc
    void load_OverrideSpellData();          // load OverrideSpellData.dbc
    void load_PowerDisplay();               // load PowerDisplay.dbc
    void load_PvpDifficulty();              // load PvpDifficulty.dbc
    void load_QuestFactionReward();         // load QuestFactionReward.dbc
    void load_QuestSort();                  // load QuestSort.dbc
    void load_QuestXP();                    // load QuestXP.dbc
    void load_RandPropPoints();             // load RandPropPoints.dbc
    void load_ScalingStatDistribution();    // load ScalingStatDistribution.dbc
    void load_ScalingStatValues();          // load ScalingStatValues.dbc
    void load_SkillLine();                  // load SkillLine.dbc
    void load_SkillLineAbility();           // load SkillLineAbility.dbc
    void load_SkillRaceClassInfo();         // load SkillRaceClassInfo.dbc
    void load_SkillTiers();                 // load SkillTiers.dbc
    void load_SoundEntries();               // load SkillTiers.dbc
    void load_Spells();                     // load Spell.dbc

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
    GemPropertiesDBCMap _gemPropertiesMap;
    GlyphPropertiesDBCMap _glyphPropertiesMap;
    GlyphSlotDBCMap _glyphSlotMap;
    GtBarberShopCostBaseDBCMap _gtBarberShopCostBaseMap;
    GtChanceToMeleeCritDBCMap _gtChanceToMeleeCritMap;
    GtChanceToMeleeCritBaseDBCMap _gtChanceToMeleeCritBaseMap;
    GtChanceToSpellCritDBCMap _gtChanceToSpellCritMap;
    GtChanceToSpellCritBaseDBCMap _gtChanceToSpellCritBaseMap;
    GtCombatRatingsDBCMap _gtCombatRatingsMap;
    GtNPCManaCostScalerDBCMap _gtNPCManaCostScalerMap;
    GtOCTClassCombatRatingScalarDBCMap _gtOCTClassCombatRatingScalarMap;
    GtOCTRegenHPDBCMap _gtOCTRegenHPMap;
    GtOCTRegenMPDBCMap _gtOCTRegenMPMap;
    GtRegenHPPerSptDBCMap _gtRegenHPPerSptMap;
    GtRegenMPPerSptDBCMap _gtRegenMPPerSptMap;
    HolidaysDBCMap _holidaysMap;
    ItemDBCMap _itemMap;
    ItemBagFamilyDBCMap _itemBagFamilyMap;
    ItemDisplayInfoDBCMap _itemDisplayInfoMap;
    ItemExtendedCostDBCMap _itemExtendedCostMap;
    ItemLimitCategoryDBCMap _itemLimitCategoryMap;
    ItemRandomPropertiesDBCMap _itemRandomPropertiesMap;
    ItemRandomSuffixDBCMap _itemRandomSuffixMap;
    ItemSetDBCMap _itemSetMap;
    LFGDungeonDBCMap _lfgDungeonMap;
    LightDBCMap _lightMap;
    LiquidTypeDBCMap _liquidTypeMap;
    LockDBCMap _lockMap;
    MailTemplateDBCMap _mailTemplateMap;
    MapDBCMap _mapMap;
    MapDifficultyDBCMap _mapDifficultyMap;
    MovieDBCMap _movieMap;
    NamesProfanityDBCMap _namesProfanityMap;
    NamesReservedDBCMap _namesReservedMap;
    OverrideSpellDataDBCMap _overrideSpellDataMap;
    PowerDisplayDBCMap _powerDisplayMap;
    PvPDifficultyDBCMap _pvpDifficultyMap;
    QuestFactionRewardDBCMap _questFactionRewardMap;
    QuestSortDBCMap _questSortMap;
    QuestXPDBCMap _questXPMap;
    RandPropPointsDBCMap _randPropPointsMap;
    ScalingStatDistributionDBCMap _scalingStatDistributionMap;
    ScalingStatValuesDBCMap _scalingStatValuesMap;
    SkillLineDBCMap _skillLineMap;
    SkillLineAbilityDBCMap _skillLineAbilityMap;
    SkillRaceClassInfoDBCMap _skillRaceClassInfoMap;
    SkillTiersDBCMap _skillTiersMap;
    SoundEntriesDBCMap _soundEntriesMap;
    SpellDBCMap _spellMap;

    uint32_t _bannedAddonsHighestID;
    uint32_t _itemRandomSuffixHighestID;
    uint32_t _spellHighestID;

    // Handle others containers
    void initialize_Additional_Data();
    void handle_CharacterFacialHairStylesByTripple();
    void handle_CharSectionsByPenta();
    void handle_CharStartOutfitByTripple();
    void handle_EmotesTextSoundByTripple();
    void handle_LFGDungeonDBCByDouble();
    void handle_MapDifficultyByDouble();
    void handle_NamesProfanityRegex();
    void handle_NamesReservedRegex();
    void handle_SkillRaceClassInfo();

    CharacterFacialHairStylesByTripple _characterFacialHairStylesByTripple;
    CharSectionsByPenta _charSectionsByPenta;
    CharStartOutfitByTripple _charStartOutfitByTripple;
    EmotesTextSoundByTripple _emotesTextSoundByTripple;
    LFGDungeonDBCByDouble _lfgDungeonByDouble;
    MapDifficultyByDouble _mapDifficultyByDouble;
    NameValidationRegexContainer _namesProfaneValidators;
    NameValidationRegexContainer _namesReservedValidators;
    SkillRaceClassInfoMap _skillRaceClassInfoBySkill;
};