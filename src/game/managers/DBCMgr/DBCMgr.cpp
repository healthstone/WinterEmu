#include "DBCMgr.hpp"
#include "Time/Timer.hpp"
#include "Logger.hpp"
#include "BaseServer.hpp"
#include "utils/utf8utils/UTF8Utils.hpp"

DBCMgr::~DBCMgr() {
    cleanUpBeforeDelete();
}

void DBCMgr::cleanUpBeforeDelete() {
    // Сначала чистим мультимапы и вторичные контейнеры
    _characterFacialHairStylesByTripple.clear();
    _charSectionsByPenta.clear();
    _charStartOutfitByTripple.clear();
    _emotesTextSoundByTripple.clear();
    _lfgDungeonByDouble.clear();
    _mapDifficultyByDouble.clear();
    _skillRaceClassInfoBySkill.clear();

    // handle additional containers
    for (uint8_t i = 0; i < TOTAL_LOCALES; i++) {
        _namesProfaneValidators[i].clear();
        _namesReservedValidators[i].clear();
    }

    // Потом уже сами основные мапы
    _achievementMap.clear();
    _achievementCriteriaMap.clear();
    _areaTableMap.clear();
    _areaGroupMap.clear();
    _areaPOIMap.clear();
    _areaTriggerMap.clear();
    _auctionHouseMap.clear();
    _bankBagSlotPricesMap.clear();
    _bannedAddonsMap.clear();
    _barberShopStyleMap.clear();
    _battlemasterListMap.clear();
    _characterFacialHairStyleMap.clear();
    _charSectionMap.clear();
    _charStartOutfitMap.clear();
    _charTitlesMap.clear();
    _chatChannelsMap.clear();
    _chrClassesMap.clear();
    _chrRacesMap.clear();
    _cinematicCameraMap.clear();
    _cinematicSequencesMap.clear();
    _creatureDisplayInfoMap.clear();
    _creatureDisplayInfoExtraMap.clear();
    _creatureFamilyMap.clear();
    _creatureModelDataMap.clear();
    _creatureSpellDataMap.clear();
    _creatureTypeMap.clear();
    _currencyCategoryMap.clear();
    _currencyTypesMap.clear();
    _destructibleModelDataMap.clear();
    _dungeonEncounterMap.clear();
    _durabilityCoastsMap.clear();
    _durabilityQualityMap.clear();
    _emotesMap.clear();
    _emotesTextMap.clear();
    _emotesTextSoundMap.clear();
    _factionMap.clear();
    _factionTemplateMap.clear();
    _gameobjectArtKitMap.clear();
    _gameobjectDisplayInfoMap.clear();
    _gemPropertiesMap.clear();
    _glyphPropertiesMap.clear();
    _glyphSlotMap.clear();
    _gtBarberShopCostBaseMap.clear();
    _gtChanceToMeleeCritMap.clear();
    _gtChanceToMeleeCritBaseMap.clear();
    _gtChanceToSpellCritMap.clear();
    _gtChanceToSpellCritBaseMap.clear();
    _gtCombatRatingsMap.clear();
    _gtNPCManaCostScalerMap.clear();
    _gtOCTClassCombatRatingScalarMap.clear();
    _gtOCTRegenHPMap.clear();
    _gtOCTRegenMPMap.clear();
    _gtRegenHPPerSptMap.clear();
    _gtRegenMPPerSptMap.clear();
    _holidaysMap.clear();
    _itemMap.clear();
    _itemBagFamilyMap.clear();
    _itemDisplayInfoMap.clear();
    _itemExtendedCostMap.clear();
    _itemLimitCategoryMap.clear();
    _itemRandomPropertiesMap.clear();
    _itemRandomSuffixMap.clear();
    _itemSetMap.clear();
    _lfgDungeonMap.clear();
    _lightMap.clear();
    _liquidTypeMap.clear();
    _lockMap.clear();
    _mailTemplateMap.clear();
    _mapMap.clear();
    _mapDifficultyMap.clear();
    _movieMap.clear();
    _namesProfanityMap.clear();
    _namesReservedMap.clear();
    _overrideSpellDataMap.clear();
    _powerDisplayMap.clear();
    _pvpDifficultyMap.clear();
    _questFactionRewardMap.clear();
    _questSortMap.clear();
    _questXPMap.clear();
    _randPropPointsMap.clear();
    _scalingStatDistributionMap.clear();
    _scalingStatValuesMap.clear();
    _skillLineMap.clear();
    _skillLineAbilityMap.clear();
    _skillRaceClassInfoMap.clear();
    _skillTiersMap.clear();
    _soundEntriesMap.clear();
    _spellMap.clear();
    _spellCastTimesMap.clear();
    _spellCategoryMap.clear();
    _spellDiffucultyMap.clear();
    _spellDurationMap.clear();
    _spellFocusObjectMap.clear();

    _bannedAddonsHighestID = 0;
    _itemRandomSuffixHighestID = 0;
    _spellHighestID = 0;
}

void DBCMgr::initialize() {
    load_Achievement();
    load_AchievementCriteria();
    load_AreaTable();
    load_AreaGroup();
    load_AreaPOI();
    load_AreaTrigger();
    load_AuctionHouse();
    load_BankBagSlotPrices();
    load_BannedAddOns();
    load_BarberShopStyle();
    load_BattlemasterList();
    load_CharacterFacialHairStyles();
    load_CharSections();
    load_CharStartOutfit();
    load_CharTitles();
    load_ChatChannels();
    load_ChrClasses();
    load_ChrRaces();
    load_CinematicCamera();
    load_CinematicSequences();
    load_CreatureDisplayInfo();
    load_CreatureDisplayInfoExtra();
    load_CreatureFamily();
    load_CreatureModelData();
    load_CreatureSpellData();
    load_CreatureType();
    load_CurrencyCategory();
    load_CurrencyTypes();
    load_DestructibleModelData();
    load_DungeonEncounter();
    load_DurabilityCosts();
    load_DurabilityQuality();
    load_Emotes();
    load_EmotesText();
    load_EmotesTextSound();
    load_Faction();
    load_FactionTemplate();
    load_GameObjectArtKit();
    load_GameObjectDisplayInfo();
    load_GemProperties();
    load_GlyphProperties();
    load_GlyphSlot();
    load_gtBarberShopCostBase();
    load_gtChanceToMeleeCrit();
    load_gtChanceToMeleeCritBase();
    load_gtChanceToSpellCrit();
    load_gtChanceToSpellCritBase();
    load_gtCombatRatings();
    load_gtNPCManaCostScaler();
    load_gtOCTClassCombatRatingScalar();
    load_gtOCTRegenHP();
    load_gtOCTRegenMP();
    load_gtRegenHPPerSpt();
    load_gtRegenMPPerSpt();
    load_Holidays();
    load_Item();
    load_ItemBagFamily();
    load_ItemDisplayInfo();
    load_ItemExtendedCost();
    load_ItemLimitCategory();
    load_ItemRandomProperties();
    load_ItemRandomSuffix();
    load_ItemSet();
    load_LFGDungeons();
    load_Light();
    load_LiquidType();
    load_Lock();
    load_MailTemplate();
    load_Map();
    load_MapDifficulty();
    load_Movie();
    load_NamesProfanity();
    load_NamesReserved();
    load_OverrideSpellData();
    load_PowerDisplay();
    load_PvpDifficulty();
    load_QuestFactionReward();
    load_QuestSort();
    load_QuestXP();
    load_RandPropPoints();
    load_ScalingStatDistribution();
    load_ScalingStatValues();
    load_SkillLine();
    load_SkillLineAbility();
    load_SkillRaceClassInfo();
    load_SkillTiers();
    load_SoundEntries();
    load_Spells();
    load_SpellCastTimes();
    load_SpellCategory();
    load_SpellDifficulty();
    load_SpellDuration();
    load_SpellFocusObject();

    initialize_Additional_Data();
}

Team DBCMgr::teamForRace(uint8_t race)
{
    auto log = Logger::get();
    if (ChrRacesDBC const* rEntry = getChrRacesDBC(race))
    {
        switch (rEntry->BaseLanguage)
        {
            case 1: return Team::HORDE;
            case 7: return Team::ALLIANCE;
        }
        log->error("DBCMgr::teamForRace: Race ({}) has wrong teamid ({}) in DBC: wrong DBC files?", race, rEntry->BaseLanguage);
    }
    else
        log->error("DBCMgr::teamForRace: Race ({}) not found in DBC: wrong DBC files?", race);

    return Team::ALLIANCE;
}

ResponseCodes DBCMgr::validateName(std::wstring const& name, LocaleConstant locale)
{
    if (locale >= TOTAL_LOCALES)
        return ResponseCodes::RESPONSE_FAILURE;

    for (boost::wregex const& regex : _namesProfaneValidators[locale])
        if (boost::regex_search(name, regex))
            return ResponseCodes::CHAR_NAME_PROFANE;

    // regexes at TOTAL_LOCALES are loaded from NamesReserved which is not locale specific
    for (boost::wregex const& regex : _namesReservedValidators[locale])
        if (boost::regex_search(name, regex))
            return ResponseCodes::CHAR_NAME_RESERVED;

    return ResponseCodes::CHAR_NAME_SUCCESS;
}

void DBCMgr::load_Achievement() {
    auto log = Logger::get();
    _achievementMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ACHIEVEMENT");
        auto rows = server_->db()->execute_sync_many<DbcAchievement>(stmt);
        for (const auto &row: rows) {
            AchievementDBC ach;
            ach.ID              = row.ID;
            ach.Faction         = row.Faction;
            ach.InstanceID      = row.InstanceID;

            // Title
            ach.Title[LOCALE_enUS] = row.Title_Lang_enUS.value_or("");
            ach.Title[LOCALE_enGB] = row.Title_Lang_enGB.value_or("");
            ach.Title[LOCALE_koKR] = row.Title_Lang_koKR.value_or("");
            ach.Title[LOCALE_frFR] = row.Title_Lang_frFR.value_or("");
            ach.Title[LOCALE_deDE] = row.Title_Lang_deDE.value_or("");
            ach.Title[LOCALE_enCN] = row.Title_Lang_enCN.value_or("");
            ach.Title[LOCALE_zhCN] = row.Title_Lang_zhCN.value_or("");
            ach.Title[LOCALE_enTW] = row.Title_Lang_enTW.value_or("");
            ach.Title[LOCALE_zhTW] = row.Title_Lang_zhTW.value_or("");
            ach.Title[LOCALE_esES] = row.Title_Lang_esES.value_or("");
            ach.Title[LOCALE_esMX] = row.Title_Lang_esMX.value_or("");
            ach.Title[LOCALE_ruRU] = row.Title_Lang_ruRU.value_or("");
            ach.Title[LOCALE_ptPT] = row.Title_Lang_ptPT.value_or("");
            ach.Title[LOCALE_ptBR] = row.Title_Lang_ptBR.value_or("");
            ach.Title[LOCALE_itIT] = row.Title_Lang_itIT.value_or("");

            ach.Category        = row.Category;
            ach.Points          = row.Points;
            ach.Flags           = row.Flags;
            ach.MinimumCriteria = row.Minimum_Criteria;
            ach.SharesCriteria  = row.Shares_Criteria;

            _achievementMap[row.ID] = ach;
        }
        log->info(">>> DBCMgr: loaded {} Achievement in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Achievement failed: {}", ex.what());
    }
}

void DBCMgr::load_AchievementCriteria() {
    auto log = Logger::get();
    _achievementCriteriaMap.clear();
    uint32_t oldMSTime = getMSTime();

    try{
        auto stmt = PreparedStatement("SELECT_DBC_ACHIEVEMENT_CRITERIA");
        auto rows = server_->db()->execute_sync_many<DbcAchievementCriteria>(stmt);
        for (const auto &row: rows) {
            AchievementCriteriaDBC achc;
            achc.ID                  = row.ID;
            achc.AchievementID       = row.AchievementId;
            achc.Type                = row.Type;

            achc.Asset.ID            = row.AssetId;
            achc.Quantity            = row.Quantity;

            achc.AdditionalRequirements[0].Type = row.StartEvent;
            achc.AdditionalRequirements[0].Asset = row.StartAsset;
            achc.AdditionalRequirements[1].Type = row.FailEvent;
            achc.AdditionalRequirements[1].Asset = row.FailAsset;

            // Локализованные описания
            achc.name[LOCALE_enUS] = row.DescriptionLang_enUS.value_or("");
            achc.name[LOCALE_enGB] = row.DescriptionLang_enGB.value_or("");
            achc.name[LOCALE_koKR] = row.DescriptionLang_koKR.value_or("");
            achc.name[LOCALE_frFR] = row.DescriptionLang_frFR.value_or("");
            achc.name[LOCALE_deDE] = row.DescriptionLang_deDE.value_or("");
            achc.name[LOCALE_enCN] = row.DescriptionLang_enCN.value_or("");
            achc.name[LOCALE_zhCN] = row.DescriptionLang_zhCN.value_or("");
            achc.name[LOCALE_enTW] = row.DescriptionLang_enTW.value_or("");
            achc.name[LOCALE_zhTW] = row.DescriptionLang_zhTW.value_or("");
            achc.name[LOCALE_esES] = row.DescriptionLang_esES.value_or("");
            achc.name[LOCALE_esMX] = row.DescriptionLang_esMX.value_or("");
            achc.name[LOCALE_ruRU] = row.DescriptionLang_ruRU.value_or("");
            achc.name[LOCALE_ptPT] = row.DescriptionLang_ptPT.value_or("");
            achc.name[LOCALE_ptBR] = row.DescriptionLang_ptBR.value_or("");
            achc.name[LOCALE_itIT] = row.DescriptionLang_itIT.value_or("");

            // Прочие поля
            achc.Flags      = row.Flags;
            achc.StartEvent = row.TimerStartEvent;
            achc.StartAsset = row.TimerAssetId;
            achc.StartTimer = row.TimerTime;

            _achievementCriteriaMap[row.ID] = achc;
        }
        log->info(">>> DBCMgr: loaded {} AchievementCriteria in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_AchievementCriteria failed: {}", ex.what());
    }
}

void DBCMgr::load_AreaTable() {
    auto log = Logger::get();
    _areaTableMap.clear();
    uint32_t oldMSTime = getMSTime();
    try {
        auto stmt = PreparedStatement("SELECT_DBC_AREATABLE");
        auto rows = server_->db()->execute_sync_many<DbcAreaTable>(stmt);
        for (const auto &row: rows) {
            AreaTableDBC at;
            at.ID               = row.ID;
            at.ContinentID      = row.ContinentID;
            at.ParentAreaID     = row.ParentAreaID;
            at.AreaBit          = row.AreaBit;
            at.Flags            = row.Flags;
            at.ExplorationLevel = row.ExplorationLevel;

            // локализованные имена
            at.AreaName[LOCALE_enUS] = row.AreaName_Lang_enUS.value_or("");
            at.AreaName[LOCALE_enGB] = row.AreaName_Lang_enGB.value_or("");
            at.AreaName[LOCALE_koKR] = row.AreaName_Lang_koKR.value_or("");
            at.AreaName[LOCALE_frFR] = row.AreaName_Lang_frFR.value_or("");
            at.AreaName[LOCALE_deDE] = row.AreaName_Lang_deDE.value_or("");
            at.AreaName[LOCALE_enCN] = row.AreaName_Lang_enCN.value_or("");
            at.AreaName[LOCALE_zhCN] = row.AreaName_Lang_zhCN.value_or("");
            at.AreaName[LOCALE_enTW] = row.AreaName_Lang_enTW.value_or("");
            at.AreaName[LOCALE_zhTW] = row.AreaName_Lang_zhTW.value_or("");
            at.AreaName[LOCALE_esES] = row.AreaName_Lang_esES.value_or("");
            at.AreaName[LOCALE_esMX] = row.AreaName_Lang_esMX.value_or("");
            at.AreaName[LOCALE_ruRU] = row.AreaName_Lang_ruRU.value_or("");
            at.AreaName[LOCALE_ptPT] = row.AreaName_Lang_ptPT.value_or("");
            at.AreaName[LOCALE_ptBR] = row.AreaName_Lang_ptBR.value_or("");
            at.AreaName[LOCALE_itIT] = row.AreaName_Lang_itIT.value_or("");

            at.FactionGroupMask = row.FactionGroupMask;
            at.LiquidTypeID[0]  = row.LiquidTypeID_1;
            at.LiquidTypeID[1]  = row.LiquidTypeID_2;
            at.LiquidTypeID[2]  = row.LiquidTypeID_3;
            at.LiquidTypeID[3]  = row.LiquidTypeID_4;

            _areaTableMap[row.ID] = at;
        }
        log->info(">>> DBCMgr: loaded {} AreaTable in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_AreaTable failed: {}", ex.what());
    }
}

void DBCMgr::load_AreaGroup() {
    auto log = Logger::get();
    _areaGroupMap.clear();
    uint32_t oldMSTime = getMSTime();
    try {
        auto stmt = PreparedStatement("SELECT_DBC_AREAGROUP");
        auto rows = server_->db()->execute_sync_many<DbcAreaGroup>(stmt);
        for (const auto &row: rows) {
            AreaGroupDBC ag;
            ag.ID = row.ID;

            // Копируем все AreaID
            ag.AreaID[0] = row.AreaID_1;
            ag.AreaID[1] = row.AreaID_2;
            ag.AreaID[2] = row.AreaID_3;
            ag.AreaID[3] = row.AreaID_4;
            ag.AreaID[4] = row.AreaID_5;
            ag.AreaID[5] = row.AreaID_6;

            ag.NextAreaID = row.NextAreaID;
            _areaGroupMap[row.ID] = ag;
        }
        log->info(">>> DBCMgr: loaded {} AreaGroup in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_AreaGroup failed: {}", ex.what());
    }
}

void DBCMgr::load_AreaPOI() {
    //TODO нужна ли эта таблица?, в TC не используется
}

void DBCMgr::load_AreaTrigger() {
    auto log = Logger::get();
    _areaTriggerMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_AREATRIGGER");
        auto rows = server_->db()->execute_sync_many<DbcAreaTrigger>(stmt);
        for (const auto &row: rows) {
            AreaTriggerDBC at;
            at.ID          = row.ID;
            at.ContinentID = row.ContinentID;
            at.Pos.X       = row.X;
            at.Pos.Y       = row.Y;
            at.Pos.Z       = row.Z;
            at.Radius      = row.Radius;
            at.BoxLength   = row.BoxLength;
            at.BoxWidth    = row.BoxWidth;
            at.BoxHeight   = row.BoxHeight;
            at.BoxYaw      = row.BoxYaw;

            _areaTriggerMap[row.ID] = at;
        }
        log->info(">>> DBCMgr: loaded {} AreaTrigger in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_AreaTrigger failed: {}", ex.what());
    }
}

void DBCMgr::load_AuctionHouse() {
    auto log = Logger::get();
    _auctionHouseMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_AUCTIONHOUSE");
        auto rows = server_->db()->execute_sync_many<DbcAuctionHouse>(stmt);
        for (const auto &row: rows) {
            AuctionHouseDBC ah;
            ah.ID              = row.ID;
            ah.FactionID       = row.FactionID;
            ah.DepositRate     = row.DepositRate;
            ah.ConsignmentRate = row.ConsignmentRate;

            _auctionHouseMap[row.ID] = ah;
        }
        log->info(">>> DBCMgr: loaded {} AuctionHouse in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_AuctionHouse failed: {}", ex.what());
    }
}

void DBCMgr::load_BankBagSlotPrices() {
    auto log = Logger::get();
    _bankBagSlotPricesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_BANKBAGSLOTPRICES");
        auto rows = server_->db()->execute_sync_many<DbcBankBagSlotPrices>(stmt);
        for (const auto &row: rows) {
            BankBagSlotPricesDBC bsp;
            bsp.ID   = row.ID;
            bsp.Cost = row.Cost;

            _bankBagSlotPricesMap[row.ID] = bsp;
        }
        log->info(">>> DBCMgr: loaded {} BankBagSlotPrices in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_BankBagSlotPrices failed: {}", ex.what());
    }
}

void DBCMgr::load_BannedAddOns() {
    auto log = Logger::get();
    _bannedAddonsMap.clear();
    _bannedAddonsHighestID = 0;
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_BANNEDADDONS");
        auto rows = server_->db()->execute_sync_many<DbcBannedAddons>(stmt);
        for (const auto &row: rows) {
            BannedAddOnsDBC ba;
            ba.ID = row.ID;

            _bannedAddonsMap[row.ID] = ba;

            if (_bannedAddonsHighestID)
            {
                if (_bannedAddonsHighestID < row.ID)
                    _bannedAddonsHighestID = row.ID;
            }
            else
                _bannedAddonsHighestID = row.ID;
        }
        log->info(">>> DBCMgr: loaded {} BannedAddOns in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_BannedAddOns failed: {}", ex.what());
    }
}

void DBCMgr::load_BarberShopStyle() {
    auto log = Logger::get();
    _barberShopStyleMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_BARBERSHOPSTYLE");
        auto rows = server_->db()->execute_sync_many<DbcBarberShopStyle>(stmt);
        for (const auto &row: rows) {
            BarberShopStyleDBC bs;
            bs.ID   = row.ID;
            bs.Type = row.Type;
            bs.Race = row.Race;
            bs.Sex  = row.Sex;
            bs.Data = row.Data;

            _barberShopStyleMap[row.ID] = bs;
        }
        log->info(">>> DBCMgr: loaded {} BarberShopStyle in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_BarberShopStyle failed: {}", ex.what());
    }
}

void DBCMgr::load_BattlemasterList() {
    auto log = Logger::get();
    _battlemasterListMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_BATTLEMASTERLIST");
        auto rows = server_->db()->execute_sync_many<DbcBattleMasterList>(stmt);
        for (const auto &row: rows) {
            BattlemasterListDBC bl;

            bl.ID           = row.id;
            bl.MapID[0]     = row.mapid_1;
            bl.MapID[1]     = row.mapid_2;
            bl.MapID[2]     = row.mapid_3;
            bl.MapID[3]     = row.mapid_4;
            bl.MapID[4]     = row.mapid_5;
            bl.MapID[5]     = row.mapid_6;
            bl.MapID[6]     = row.mapid_7;
            bl.MapID[7]     = row.mapid_8;
            bl.InstanceType = row.instance_type;
            bl.MaxGroupSize      = row.max_group_size;
            bl.HolidayWorldState = row.holiday_worldstate;

            _battlemasterListMap[bl.ID] = bl;
        }
        log->info(">>> DBCMgr: loaded {} BattlemasterList in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_BattlemasterList failed: {}", ex.what());
    }
}

void DBCMgr::load_CharacterFacialHairStyles() {
    auto log = Logger::get();
    _characterFacialHairStyleMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHARACTERFACIALHAIRSTYLES");
        auto rows = server_->db()->execute_sync_many<DbcCharacterFacialHairstyles>(stmt);
        for (const auto &row: rows) {
            CharacterFacialHairStylesDBC cfhs;
            cfhs.ID          = row.id;
            cfhs.RaceID      = row.raceid;
            cfhs.SexID       = row.sexid;
            cfhs.VariationID = row.variationid;

            _characterFacialHairStyleMap[row.id] = cfhs;
        }
        log->info(">>> DBCMgr: loaded {} CharacterFacialHairStyles in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CharacterFacialHairStyles failed: {}", ex.what());
    }
}

void DBCMgr::load_CharSections() {
    auto log = Logger::get();
    _charSectionMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHARSECTIONS");
        auto rows = server_->db()->execute_sync_many<DbcCharSections>(stmt);
        for (const auto &row: rows) {
            CharSectionsDBC cs;
            cs.ID             = row.id;
            cs.RaceID         = row.raceid;
            cs.SexID          = row.sexid;
            cs.BaseSection    = row.basesection;
            cs.Flags          = row.flags;
            cs.VariationIndex = row.variationindex;
            cs.ColorIndex     = row.colorindex;

            _charSectionMap[row.id] = cs;
        }
        log->info(">>> DBCMgr: loaded {} CharSections in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CharSections failed: {}", ex.what());
    }
}

void DBCMgr::load_CharStartOutfit() {
    auto log = Logger::get();
    _charStartOutfitMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHARSTARTOUTFIT");
        auto rows = server_->db()->execute_sync_many<DbcCharStartOutfit>(stmt);
        for (const auto &row: rows) {
            CharStartOutfitDBC cso;
            cso.ID      = row.ID;
            cso.RaceID  = row.RaceID;
            cso.ClassID = row.ClassID;
            cso.SexID   = row.SexID;

            for (uint8_t i = 0; i < MAX_OUTFIT_ITEMS; i++)
                cso.ItemID[i] = row.ItemID[i];

            _charStartOutfitMap[row.ID] = cso;
        }
        log->info(">>> DBCMgr: loaded {} CharStartOutfit in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CharStartOutfit failed: {}", ex.what());
    }
}

void DBCMgr::load_CharTitles() {
    auto log = Logger::get();
    _charTitlesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHARTITLES");
        auto rows = server_->db()->execute_sync_many<DbcCharTitles>(stmt);
        for (const auto &row: rows) {
            CharTitlesDBC ct;
            ct.ID = row.ID;

            // --- Первые локализованные названия (Name) ---
            ct.Name[LOCALE_enUS] = row.Name_Lang_enUS.value_or("");
            ct.Name[LOCALE_enGB] = row.Name_Lang_enGB.value_or("");
            ct.Name[LOCALE_koKR] = row.Name_Lang_koKR.value_or("");
            ct.Name[LOCALE_frFR] = row.Name_Lang_frFR.value_or("");
            ct.Name[LOCALE_deDE] = row.Name_Lang_deDE.value_or("");
            ct.Name[LOCALE_zhCN] = row.Name_Lang_zhCN.value_or("");
            ct.Name[LOCALE_zhTW] = row.Name_Lang_zhTW.value_or("");
            ct.Name[LOCALE_esES] = row.Name_Lang_esES.value_or("");
            ct.Name[LOCALE_esMX] = row.Name_Lang_esMX.value_or("");
            ct.Name[LOCALE_ruRU] = row.Name_Lang_ruRU.value_or("");
            ct.Name[LOCALE_ptPT] = row.Name_Lang_ptPT.value_or("");
            ct.Name[LOCALE_ptBR] = row.Name_Lang_ptBR.value_or("");
            ct.Name[LOCALE_itIT] = row.Name_Lang_itIT.value_or("");

            // --- Вторые локализованные названия (Name1) ---
            ct.Name1[LOCALE_enUS] = row.Name1_Lang_enUS.value_or("");
            ct.Name1[LOCALE_enGB] = row.Name1_Lang_enGB.value_or("");
            ct.Name1[LOCALE_koKR] = row.Name1_Lang_koKR.value_or("");
            ct.Name1[LOCALE_frFR] = row.Name1_Lang_frFR.value_or("");
            ct.Name1[LOCALE_deDE] = row.Name1_Lang_deDE.value_or("");
            ct.Name1[LOCALE_zhCN] = row.Name1_Lang_zhCN.value_or("");
            ct.Name1[LOCALE_zhTW] = row.Name1_Lang_zhTW.value_or("");
            ct.Name1[LOCALE_esES] = row.Name1_Lang_esES.value_or("");
            ct.Name1[LOCALE_esMX] = row.Name1_Lang_esMX.value_or("");
            ct.Name1[LOCALE_ruRU] = row.Name1_Lang_ruRU.value_or("");
            ct.Name1[LOCALE_ptPT] = row.Name1_Lang_ptPT.value_or("");
            ct.Name1[LOCALE_ptBR] = row.Name1_Lang_ptBR.value_or("");
            ct.Name1[LOCALE_itIT] = row.Name1_Lang_itIT.value_or("");

            // --- Маска ---
            ct.MaskID = row.Mask_ID;

            _charTitlesMap[ct.ID] = ct;
        }
        log->info(">>> DBCMgr: loaded {} CharTitles in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CharTitles failed: {}", ex.what());
    }
}

void DBCMgr::load_ChatChannels() {
    auto log = Logger::get();
    _chatChannelsMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHATCHANNELS");
        auto rows = server_->db()->execute_sync_many<DbcChatChannels>(stmt);
        for (const auto &row: rows) {
            ChatChannelsDBC cc;
            cc.ID = row.ID;
            cc.Flags = row.Flags;

            // локализованные имена
            cc.Name[LOCALE_enUS] = row.Name_Lang_enUS.value_or("");
            cc.Name[LOCALE_enGB] = row.Name_Lang_enGB.value_or("");
            cc.Name[LOCALE_koKR] = row.Name_Lang_koKR.value_or("");
            cc.Name[LOCALE_frFR] = row.Name_Lang_frFR.value_or("");
            cc.Name[LOCALE_deDE] = row.Name_Lang_deDE.value_or("");
            cc.Name[LOCALE_enCN] = row.Name_Lang_enCN.value_or("");
            cc.Name[LOCALE_zhCN] = row.Name_Lang_zhCN.value_or("");
            cc.Name[LOCALE_enTW] = row.Name_Lang_enTW.value_or("");
            cc.Name[LOCALE_zhTW] = row.Name_Lang_zhTW.value_or("");
            cc.Name[LOCALE_esES] = row.Name_Lang_esES.value_or("");
            cc.Name[LOCALE_esMX] = row.Name_Lang_esMX.value_or("");
            cc.Name[LOCALE_ruRU] = row.Name_Lang_ruRU.value_or("");
            cc.Name[LOCALE_ptPT] = row.Name_Lang_ptPT.value_or("");
            cc.Name[LOCALE_ptBR] = row.Name_Lang_ptBR.value_or("");
            cc.Name[LOCALE_itIT] = row.Name_Lang_itIT.value_or("");

            _chatChannelsMap[row.ID] = cc;
        }
        log->info(">>> DBCMgr: loaded {} ChatChannels in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ChatChannels failed: {}", ex.what());
    }
}

void DBCMgr::load_ChrClasses() {
    auto log = Logger::get();
    _chrClassesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHRCLASSES");
        auto rows = server_->db()->execute_sync_many<DbcChrClasses>(stmt);
        for (const auto &row: rows) {
            ChrClassesDBC cc;
            cc.ID           = row.ID;
            cc.DisplayPower = row.DisplayPower;

            // Заполняем локализованные названия
            cc.Name[LOCALE_enUS] = row.Name_Lang_enUS.value_or("");
            cc.Name[LOCALE_enGB] = row.Name_Lang_enGB.value_or("");
            cc.Name[LOCALE_koKR] = row.Name_Lang_koKR.value_or("");
            cc.Name[LOCALE_frFR] = row.Name_Lang_frFR.value_or("");
            cc.Name[LOCALE_deDE] = row.Name_Lang_deDE.value_or("");
            cc.Name[LOCALE_enCN] = row.Name_Lang_enCN.value_or("");
            cc.Name[LOCALE_zhCN] = row.Name_Lang_zhCN.value_or("");
            cc.Name[LOCALE_enTW] = row.Name_Lang_enTW.value_or("");
            cc.Name[LOCALE_zhTW] = row.Name_Lang_zhTW.value_or("");
            cc.Name[LOCALE_esES] = row.Name_Lang_esES.value_or("");
            cc.Name[LOCALE_esMX] = row.Name_Lang_esMX.value_or("");
            cc.Name[LOCALE_ruRU] = row.Name_Lang_ruRU.value_or("");
            cc.Name[LOCALE_ptPT] = row.Name_Lang_ptPT.value_or("");
            cc.Name[LOCALE_ptBR] = row.Name_Lang_ptBR.value_or("");
            cc.Name[LOCALE_itIT] = row.Name_Lang_itIT.value_or("");

            cc.SpellClassSet = row.SpellClassSet;
            cc.CinematicSequenceID = row.CinematicSequenceID;
            cc.RequiredExpansion = row.Required_Expansion;

            _chrClassesMap[cc.ID] = cc;
        }
        log->info(">>> DBCMgr: loaded {} ChrClasses in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));

    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ChrClasses failed: {}", ex.what());
    }
}

void DBCMgr::load_ChrRaces() {
    auto log = Logger::get();
    _chrRacesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CHRRACES");
        auto rows = server_->db()->execute_sync_many<DbcChrRaces>(stmt);
        for (const auto &row: rows) {
            ChrRacesDBC cr;
            cr.ID                  = row.ID;
            cr.Flags               = row.Flags;
            cr.FactionID           = row.FactionID;
            cr.MaleDisplayID       = row.MaleDisplayId;
            cr.FemaleDisplayID     = row.FemaleDisplayId;
            cr.BaseLanguage        = row.BaseLanguage;
            cr.CreatureType        = row.CreatureType;
            cr.ResSicknessSpellID  = row.ResSicknessSpellID;
            cr.CinematicSequenceID = row.CinematicSequenceID;
            cr.Alliance            = row.Alliance;

            // Заполняем локализованные названия
            cr.Name[LOCALE_enUS] = row.Name_Lang_enUS.value_or("");
            cr.Name[LOCALE_enGB] = row.Name_Lang_enGB.value_or("");
            cr.Name[LOCALE_koKR] = row.Name_Lang_koKR.value_or("");
            cr.Name[LOCALE_frFR] = row.Name_Lang_frFR.value_or("");
            cr.Name[LOCALE_deDE] = row.Name_Lang_deDE.value_or("");
            cr.Name[LOCALE_enCN] = row.Name_Lang_enCN.value_or("");
            cr.Name[LOCALE_zhCN] = row.Name_Lang_zhCN.value_or("");
            cr.Name[LOCALE_enTW] = row.Name_Lang_enTW.value_or("");
            cr.Name[LOCALE_zhTW] = row.Name_Lang_zhTW.value_or("");
            cr.Name[LOCALE_esES] = row.Name_Lang_esES.value_or("");
            cr.Name[LOCALE_esMX] = row.Name_Lang_esMX.value_or("");
            cr.Name[LOCALE_ruRU] = row.Name_Lang_ruRU.value_or("");
            cr.Name[LOCALE_ptPT] = row.Name_Lang_ptPT.value_or("");
            cr.Name[LOCALE_ptBR] = row.Name_Lang_ptBR.value_or("");
            cr.Name[LOCALE_itIT] = row.Name_Lang_itIT.value_or("");

            cr.RequiredExpansion = row.Required_Expansion;

            _chrRacesMap[cr.ID] = cr;
        }
        log->info(">>> DBCMgr: loaded {} ChrRaces in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));

    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ChrRaces failed: {}", ex.what());
    }
}

void DBCMgr::load_CinematicCamera() {
    auto log = Logger::get();
    _cinematicCameraMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CINEMATICCAMERA");
        auto rows = server_->db()->execute_sync_many<DbcCinematicCamera>(stmt);
        for (const auto &row: rows) {
            CinematicCameraDBC cc;
            cc.ID           = row.id;
            cc.Model        = row.model.value_or("");
            cc.SoundID      = row.soundid;
            cc.OriginX      = row.originx;
            cc.OriginY      = row.originy;
            cc.OriginZ      = row.originz;
            cc.OriginFacing = row.originfacing;

            _cinematicCameraMap[row.id] = cc;
        }
        log->info(">>> DBCMgr: loaded {} CinematicCamera in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CinematicCamera failed: {}", ex.what());
    }
}

void DBCMgr::load_CinematicSequences() {
    auto log = Logger::get();
    _cinematicSequencesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CINEMATICSEQUENCES");
        auto rows = server_->db()->execute_sync_many<DbcCinematicSequences>(stmt);
        for (const auto &row: rows) {
            CinematicSequencesDBC cs;
            cs.ID = row.id;

            cs.Camera[0] = row.camera_1;
            cs.Camera[1] = row.camera_2;
            cs.Camera[2] = row.camera_3;
            cs.Camera[3] = row.camera_4;
            cs.Camera[4] = row.camera_5;
            cs.Camera[5] = row.camera_6;
            cs.Camera[6] = row.camera_7;
            cs.Camera[7] = row.camera_8;

            _cinematicSequencesMap[row.id] = cs;
        }
        log->info(">>> DBCMgr: loaded {} CinematicSequences in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CinematicSequences failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureDisplayInfo() {
    auto log = Logger::get();
    _creatureDisplayInfoMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATUREDISPLAYINFO");
        auto rows = server_->db()->execute_sync_many<DbcCreatureDisplayInfo>(stmt);
        for (const auto &row: rows) {
            CreatureDisplayInfoDBC cdi;
            cdi.ID = row.ID;
            cdi.ModelID               = row.ModelID;
            cdi.ExtendedDisplayInfoID = row.ExtendedDisplayInfoID;
            cdi.CreatureModelScale    = row.CreatureModelScale;

            _creatureDisplayInfoMap[row.ID] = cdi;
        }
        log->info(">>> DBCMgr: loaded {} CreatureDisplayInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureDisplayInfo failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureDisplayInfoExtra() {
    auto log = Logger::get();
    _creatureDisplayInfoExtraMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATUREDISPLAYINFOEXTRA");
        auto rows = server_->db()->execute_sync_many<DbcCreatureDisplayInfoExtra>(stmt);
        for (const auto &row: rows) {
            CreatureDisplayInfoExtraDBC cdie;
            cdie.ID = row.ID;
            cdie.DisplayRaceID = row.DisplayRaceID;
            cdie.DisplaySexID  = row.DisplaySexID;

            _creatureDisplayInfoExtraMap[row.ID] = cdie;
        }
        log->info(">>> DBCMgr: loaded {} CreatureDisplayInfoExtra in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureDisplayInfoExtra failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureFamily() {
    auto log = Logger::get();
    _creatureFamilyMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATUREFAMILY");
        auto rows = server_->db()->execute_sync_many<DbcCreatureFamily>(stmt);
        for (const auto &row: rows) {
            CreatureFamilyDBC cf;
            cf.ID            = row.ID;
            cf.MinScale      = row.MinScale;
            cf.MinScaleLevel = row.MinScaleLevel;
            cf.MaxScale      = row.MaxScale;
            cf.MaxScaleLevel = row.MaxScaleLevel;
            cf.SkillLine[0]  = row.SkillLine_1;
            cf.SkillLine[1]  = row.SkillLine_2;
            cf.PetFoodMask   = row.PetFoodMask;
            cf.PetTalentType = row.PetTalentType;

            // Локализации
            cf.Name[LOCALE_enUS] = row.Name_Lang_enUS.value_or("");
            cf.Name[LOCALE_enGB] = row.Name_Lang_enGB.value_or("");
            cf.Name[LOCALE_koKR] = row.Name_Lang_koKR.value_or("");
            cf.Name[LOCALE_frFR] = row.Name_Lang_frFR.value_or("");
            cf.Name[LOCALE_deDE] = row.Name_Lang_deDE.value_or("");
            cf.Name[LOCALE_enCN] = row.Name_Lang_enCN.value_or("");
            cf.Name[LOCALE_zhCN] = row.Name_Lang_zhCN.value_or("");
            cf.Name[LOCALE_enTW] = row.Name_Lang_enTW.value_or("");
            cf.Name[LOCALE_zhTW] = row.Name_Lang_zhTW.value_or("");
            cf.Name[LOCALE_esES] = row.Name_Lang_esES.value_or("");
            cf.Name[LOCALE_esMX] = row.Name_Lang_esMX.value_or("");
            cf.Name[LOCALE_ruRU] = row.Name_Lang_ruRU.value_or("");
            cf.Name[LOCALE_ptPT] = row.Name_Lang_ptPT.value_or("");
            cf.Name[LOCALE_ptBR] = row.Name_Lang_ptBR.value_or("");
            cf.Name[LOCALE_itIT] = row.Name_Lang_itIT.value_or("");

            _creatureFamilyMap[cf.ID] = cf;
        }
        log->info(">>> DBCMgr: loaded {} CreatureFamily in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureFamily failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureModelData() {
    auto log = Logger::get();
    _creatureModelDataMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATUREMODELDATA");
        auto rows = server_->db()->execute_sync_many<DbcCreatureModelData>(stmt);
        for (const auto &row: rows) {
            CreatureModelDataDBC cmd;
            cmd.ID = row.ID;
            cmd.Flags           = row.Flags;
            cmd.ModelName       = row.ModelName.value_or("");
            cmd.ModelScale      = row.ModelScale;
            cmd.CollisionHeight = row.CollisionHeight;
            cmd.MountHeight     = row.MountHeight;

            _creatureModelDataMap[row.ID] = cmd;
        }
        log->info(">>> DBCMgr: loaded {} CreatureModelData in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureModelData failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureSpellData() {
    auto log = Logger::get();
    _creatureSpellDataMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATURESPELDATA");
        auto rows = server_->db()->execute_sync_many<DbcCreatureSpellData>(stmt);
        for (const auto &row: rows) {
            CreatureSpellDataDBC csd;
            csd.ID = row.ID;
            csd.Spells[0] = row.Spells_1;
            csd.Spells[1] = row.Spells_2;
            csd.Spells[2] = row.Spells_3;
            csd.Spells[3] = row.Spells_4;

            _creatureSpellDataMap[row.ID] = csd;
        }
        log->info(">>> DBCMgr: loaded {} CreatureSpellData in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureSpellData failed: {}", ex.what());
    }
}

void DBCMgr::load_CreatureType() {
    auto log = Logger::get();
    _creatureTypeMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CREATURETYPE");
        auto rows = server_->db()->execute_sync_many<DbcCreatureType>(stmt);
        for (const auto &row: rows) {
            CreatureTypeDBC ct;
            ct.ID = row.ID;

            _creatureTypeMap[row.ID] = ct;
        }
        log->info(">>> DBCMgr: loaded {} CreatureType in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CreatureType failed: {}", ex.what());
    }
}

void DBCMgr::load_CurrencyCategory() {
    //TODO нужна ли эта таблица?, в TC не используется
}

void DBCMgr::load_CurrencyTypes() {
    auto log = Logger::get();
    _currencyTypesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_CURRENCYTYPES");
        auto rows = server_->db()->execute_sync_many<DbcCurrencyTypes>(stmt);
        for (const auto &row: rows) {
            CurrencyTypesDBC ct;
            ct.ID       = row.ID;
            ct.ItemID   = row.ItemID;
            ct.BitIndex = row.BitIndex;

            _currencyTypesMap[row.ItemID] = ct;
        }
        log->info(">>> DBCMgr: loaded {} CurrencyTypes in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_CurrencyTypes failed: {}", ex.what());
    }
}

void DBCMgr::load_DestructibleModelData() {
    auto log = Logger::get();
    _destructibleModelDataMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_DESTRUCTIBLEMODELDATA");
        auto rows = server_->db()->execute_sync_many<DbcDestructibleModelData>(stmt);
        for (const auto &row: rows) {
            DestructibleModelDataDBC dmd;
            dmd.ID = row.id;
            dmd.State1Wmo      = row.state1_wmo;
            dmd.State2Wmo      = row.state2_wmo;
            dmd.State3Wmo      = row.state3_wmo;
            dmd.RepairGroundFx = row.repair_ground_fx;

            _destructibleModelDataMap[row.id] = dmd;
        }
        log->info(">>> DBCMgr: loaded {} DestructibleModelData in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_DestructibleModelData failed: {}", ex.what());
    }
}

void DBCMgr::load_DungeonEncounter() {
    auto log = Logger::get();
    _dungeonEncounterMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_DUNGEONENCOUNTER");
        auto rows = server_->db()->execute_sync_many<DbcDungeonEncounter>(stmt);
        for (const auto &row: rows) {
            DungeonEncounterDBC de;

            de.ID         = row.id;
            de.MapID      = row.mapid;
            de.Difficulty = row.difficulty;
            de.Bit        = row.bit;

            // Копируем локализованные имена
            de.Name[LOCALE_enUS] = row.name_lang_enus.value_or("");
            de.Name[LOCALE_enGB] = row.name_lang_engb.value_or("");
            de.Name[LOCALE_koKR] = row.name_lang_kokr.value_or("");
            de.Name[LOCALE_frFR] = row.name_lang_frfr.value_or("");
            de.Name[LOCALE_deDE] = row.name_lang_dede.value_or("");
            de.Name[LOCALE_enCN] = row.name_lang_encn.value_or("");
            de.Name[LOCALE_zhCN] = row.name_lang_zhcn.value_or("");
            de.Name[LOCALE_enTW] = row.name_lang_entw.value_or("");
            de.Name[LOCALE_zhTW] = row.name_lang_zhtw.value_or("");
            de.Name[LOCALE_esES] = row.name_lang_eses.value_or("");
            de.Name[LOCALE_esMX] = row.name_lang_esmx.value_or("");
            de.Name[LOCALE_ruRU] = row.name_lang_ruru.value_or("");
            de.Name[LOCALE_ptPT] = row.name_lang_ptpt.value_or("");
            de.Name[LOCALE_ptBR] = row.name_lang_ptbr.value_or("");
            de.Name[LOCALE_itIT] = row.name_lang_itit.value_or("");

            _dungeonEncounterMap[de.ID] = de;
        }
        log->info(">>> DBCMgr: loaded {} DungeonEncounter in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_DungeonEncounter failed: {}", ex.what());
    }
}

void DBCMgr::load_DurabilityCosts() {
    auto log = Logger::get();
    _durabilityCoastsMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_DURABILITYCOSTS");
        auto rows = server_->db()->execute_sync_many<DbcDurabilityCosts>(stmt);
        for (const auto &row: rows) {
            DurabilityCostsDBC dc;
            dc.ID = row.id; // берем id из строки

            // Копируем WeaponSubClassCost 1-21
            dc.WeaponSubClassCost[0]  = row.weaponSubClassCost1;
            dc.WeaponSubClassCost[1]  = row.weaponSubClassCost2;
            dc.WeaponSubClassCost[2]  = row.weaponSubClassCost3;
            dc.WeaponSubClassCost[3]  = row.weaponSubClassCost4;
            dc.WeaponSubClassCost[4]  = row.weaponSubClassCost5;
            dc.WeaponSubClassCost[5]  = row.weaponSubClassCost6;
            dc.WeaponSubClassCost[6]  = row.weaponSubClassCost7;
            dc.WeaponSubClassCost[7]  = row.weaponSubClassCost8;
            dc.WeaponSubClassCost[8]  = row.weaponSubClassCost9;
            dc.WeaponSubClassCost[9]  = row.weaponSubClassCost10;
            dc.WeaponSubClassCost[10] = row.weaponSubClassCost11;
            dc.WeaponSubClassCost[11] = row.weaponSubClassCost12;
            dc.WeaponSubClassCost[12] = row.weaponSubClassCost13;
            dc.WeaponSubClassCost[13] = row.weaponSubClassCost14;
            dc.WeaponSubClassCost[14] = row.weaponSubClassCost15;
            dc.WeaponSubClassCost[15] = row.weaponSubClassCost16;
            dc.WeaponSubClassCost[16] = row.weaponSubClassCost17;
            dc.WeaponSubClassCost[17] = row.weaponSubClassCost18;
            dc.WeaponSubClassCost[18] = row.weaponSubClassCost19;
            dc.WeaponSubClassCost[19] = row.weaponSubClassCost20;
            dc.WeaponSubClassCost[20] = row.weaponSubClassCost21;

            // Копируем ArmorSubClassCost 1-8
            dc.ArmorSubClassCost[0] = row.armorSubClassCost1;
            dc.ArmorSubClassCost[1] = row.armorSubClassCost2;
            dc.ArmorSubClassCost[2] = row.armorSubClassCost3;
            dc.ArmorSubClassCost[3] = row.armorSubClassCost4;
            dc.ArmorSubClassCost[4] = row.armorSubClassCost5;
            dc.ArmorSubClassCost[5] = row.armorSubClassCost6;
            dc.ArmorSubClassCost[6] = row.armorSubClassCost7;
            dc.ArmorSubClassCost[7] = row.armorSubClassCost8;

            _durabilityCoastsMap[row.id] = dc;
        }
        log->info(">>> DBCMgr: loaded {} DurabilityCosts in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_DurabilityCosts failed: {}", ex.what());
    }
}

void DBCMgr::load_DurabilityQuality() {
    auto log = Logger::get();
    _durabilityQualityMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_DURABILITYQUALITY");
        auto rows = server_->db()->execute_sync_many<DbcDurabilityQuality>(stmt);
        for (const auto &row: rows) {
            DurabilityQualityDBC dq;
            dq.ID = row.ID;
            dq.Data = row.Data;

            _durabilityQualityMap[row.ID] = dq;
        }
        log->info(">>> DBCMgr: loaded {} DurabilityQuality in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_DurabilityQuality failed: {}", ex.what());
    }
}

void DBCMgr::load_Emotes() {
    auto log = Logger::get();
    _emotesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_EMOTES");
        auto rows = server_->db()->execute_sync_many<DbcEmotes>(stmt);
        for (const auto &row: rows) {
            EmotesDBC e;
            e.ID = row.ID;
            e.EmoteFlags         = row.EmoteFlags;
            e.EmoteSpecProc      = row.EmoteSpecProc;
            e.EmoteSpecProcParam = row.EmoteSpecProcParam;

            _emotesMap[row.ID] = e;
        }
        log->info(">>> DBCMgr: loaded {} Emotes in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Emotes failed: {}", ex.what());
    }
}

void DBCMgr::load_EmotesText() {
    auto log = Logger::get();
    _emotesTextMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_EMOTESTEXT");
        auto rows = server_->db()->execute_sync_many<DbcEmotesText>(stmt);
        for (const auto &row: rows) {
            EmotesTextDBC et;
            et.ID = row.ID;
            et.EmoteID = row.EmoteID;

            _emotesTextMap[row.ID] = et;
        }
        log->info(">>> DBCMgr: loaded {} EmotesText in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_EmotesText failed: {}", ex.what());
    }
}

void DBCMgr::load_EmotesTextSound() {
    auto log = Logger::get();
    _emotesTextSoundMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_EMOTESTEXTSOUND");
        auto rows = server_->db()->execute_sync_many<DbcEmotesTextSound>(stmt);
        for (const auto &row: rows) {
            EmotesTextSoundDBC ets;
            ets.ID = row.ID;
            ets.EmotesTextID = row.EmotesTextID;
            ets.RaceID       = row.RaceID;
            ets.SexID        = row.SexID;
            ets.SoundID      = row.SoundID;

            _emotesTextSoundMap[row.ID] = ets;
        }
        log->info(">>> DBCMgr: loaded {} EmotesTextSound in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_EmotesTextSound failed: {}", ex.what());
    }
}

void DBCMgr::load_Faction() {
    auto log = Logger::get();
    _factionMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_FACTION");
        auto rows = server_->db()->execute_sync_many<DbcFaction>(stmt);
        for (const auto &row: rows) {
            FactionDBC f;
            f.ID = row.id;
            f.ReputationIndex = row.reputationindex;

            f.ReputationRaceMask[0] = row.reputationracemask_1;
            f.ReputationRaceMask[1] = row.reputationracemask_2;
            f.ReputationRaceMask[2] = row.reputationracemask_3;
            f.ReputationRaceMask[3] = row.reputationracemask_4;

            f.ReputationClassMask[0] = row.reputationclassmask_1;
            f.ReputationClassMask[1] = row.reputationclassmask_2;
            f.ReputationClassMask[2] = row.reputationclassmask_3;
            f.ReputationClassMask[3] = row.reputationclassmask_4;

            f.ReputationBase[0] = row.reputationbase_1;
            f.ReputationBase[1] = row.reputationbase_2;
            f.ReputationBase[2] = row.reputationbase_3;
            f.ReputationBase[3] = row.reputationbase_4;

            f.ReputationFlags[0] = row.reputationflags_1;
            f.ReputationFlags[1] = row.reputationflags_2;
            f.ReputationFlags[2] = row.reputationflags_3;
            f.ReputationFlags[3] = row.reputationflags_4;

            f.ParentFactionID = row.parentfactionid;

            f.ParentFactionMod[0] = row.parentfactionmod_1;
            f.ParentFactionMod[1] = row.parentfactionmod_2;
            f.ParentFactionCap[0] = row.parentfactioncap_1;
            f.ParentFactionCap[1] = row.parentfactioncap_2;

            // name localization
            f.Name[0]  = row.name_lang_enus.value_or("");
            f.Name[1]  = row.name_lang_engb.value_or("");
            f.Name[2]  = row.name_lang_kokr.value_or("");
            f.Name[3]  = row.name_lang_frfr.value_or("");
            f.Name[4]  = row.name_lang_dede.value_or("");
            f.Name[5]  = row.name_lang_encn.value_or("");
            f.Name[6]  = row.name_lang_zhcn.value_or("");
            f.Name[7]  = row.name_lang_entw.value_or("");
            f.Name[8]  = row.name_lang_zhtw.value_or("");
            f.Name[9]  = row.name_lang_eses.value_or("");
            f.Name[10] = row.name_lang_esmx.value_or("");
            f.Name[11] = row.name_lang_ruru.value_or("");
            f.Name[12] = row.name_lang_ptpt.value_or("");
            f.Name[13] = row.name_lang_ptbr.value_or("");
            f.Name[14] = row.name_lang_itit.value_or("");

            _factionMap[f.ID] = f;
        }
        log->info(">>> DBCMgr: loaded {} Faction in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Faction failed: {}", ex.what());
    }
}

void DBCMgr::load_FactionTemplate() {
    auto log = Logger::get();
    _factionTemplateMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_FACTIONTEMPLATE");
        auto rows = server_->db()->execute_sync_many<DbcFactionTemplate>(stmt);
        for (const auto &row: rows) {
            FactionTemplateDBC ft;
            ft.ID = row.id;
            ft.Faction      = row.faction;
            ft.Flags        = row.flags;
            ft.FactionGroup = row.factiongroup;
            ft.FriendGroup  = row.friendgroup;
            ft.EnemyGroup   = row.enemygroup;

            ft.Enemies[0] = row.enemies_1;
            ft.Enemies[1] = row.enemies_2;
            ft.Enemies[2] = row.enemies_3;
            ft.Enemies[3] = row.enemies_4;

            ft.Friend[0] = row.friend_1;
            ft.Friend[1] = row.friend_2;
            ft.Friend[2] = row.friend_3;
            ft.Friend[3] = row.friend_4;

            _factionTemplateMap[row.id] = ft;
        }
        log->info(">>> DBCMgr: loaded {} FactionTemplate in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_FactionTemplate failed: {}", ex.what());
    }
}

void DBCMgr::load_GameObjectArtKit() {
    auto log = Logger::get();
    _gameobjectArtKitMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GAMEOBJECTARTKIT");
        auto rows = server_->db()->execute_sync_many<DbcGameObjectArtKit>(stmt);
        for (const auto &row: rows) {
            GameObjectArtKitDBC gak;
            gak.ID = row.id;

            _gameobjectArtKitMap[row.id] = gak;
        }
        log->info(">>> DBCMgr: loaded {} GameObjectArtKit in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_GameObjectArtKit failed: {}", ex.what());
    }
}

void DBCMgr::load_GameObjectDisplayInfo() {
    auto log = Logger::get();
    _gameobjectDisplayInfoMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GAMEOBJECTDISPLAYINFO");
        auto rows = server_->db()->execute_sync_many<DbcGameObjectDisplayInfo>(stmt);
        for (const auto &row: rows) {
            GameObjectDisplayInfoDBC gdi;
            gdi.ID = row.id;
            gdi.ModelName   = row.modelname.value_or("");
            gdi.GeoBoxMin.X = row.geoboxminx;
            gdi.GeoBoxMin.Y = row.geoboxminy;
            gdi.GeoBoxMin.Z = row.geoboxminz;
            gdi.GeoBoxMax.X = row.geoboxmaxx;
            gdi.GeoBoxMax.Y = row.geoboxmaxy;
            gdi.GeoBoxMax.Z = row.geoboxmaxz;

            _gameobjectDisplayInfoMap[row.id] = gdi;
        }
        log->info(">>> DBCMgr: loaded {} GameObjectDisplayInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_GameObjectDisplayInfo failed: {}", ex.what());
    }
}

void DBCMgr::load_GemProperties() {
    auto log = Logger::get();
    _gemPropertiesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GEMPROPERTIES");
        auto rows = server_->db()->execute_sync_many<DbcGemProperties>(stmt);
        for (const auto &row: rows) {
            GemPropertiesDBC gp;
            gp.ID = row.id;
            gp.EnchantID = row.enchant_id;
            gp.Type      = row.type;

            _gemPropertiesMap[row.id] = gp;
        }
        log->info(">>> DBCMgr: loaded {} GemProperties in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_GemProperties failed: {}", ex.what());
    }
}

void DBCMgr::load_GlyphProperties() {
    auto log = Logger::get();
    _glyphPropertiesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GLYPHPROPERTIES");
        auto rows = server_->db()->execute_sync_many<DbcGlyphProperties>(stmt);
        for (const auto &row: rows) {
            GlyphPropertiesDBC gp;
            gp.ID = row.id;
            gp.SpellID        = row.spellid;
            gp.GlyphSlotFlags = row.glyphslotflags;
            gp.SpellIconID    = row.spelliconid;

            _glyphPropertiesMap[row.id] = gp;
        }
        log->info(">>> DBCMgr: loaded {} GlyphProperties in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_GlyphProperties failed: {}", ex.what());
    }
}

void DBCMgr::load_GlyphSlot() {
    auto log = Logger::get();
    _glyphSlotMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GLYPHSLOT");
        auto rows = server_->db()->execute_sync_many<DbcGlyphSlot>(stmt);
        for (const auto &row: rows) {
            GlyphSlotDBC gs;
            gs.ID = row.id;
            gs.Type    = row.type;
            gs.Tooltip = row.tooltip;

            _glyphSlotMap[row.id] = gs;
        }
        log->info(">>> DBCMgr: loaded {} GlyphSlot in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_GlyphSlot failed: {}", ex.what());
    }
}

void DBCMgr::load_gtBarberShopCostBase() {
    auto log = Logger::get();
    _gtBarberShopCostBaseMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTBARBERSHOPCOSTBASE");
        auto rows = server_->db()->execute_sync_many<DbcGtBarberShopCostBase>(stmt);
        for (const auto &row: rows) {
            GtBarberShopCostBaseDBC gtbsc;
            gtbsc.ID = row.id;
            gtbsc.Data = row.data;

            _gtBarberShopCostBaseMap[row.id] = gtbsc;
        }
        log->info(">>> DBCMgr: loaded {} gtBarberShopCostBase in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtBarberShopCostBase failed: {}", ex.what());
    }
}

void DBCMgr::load_gtChanceToMeleeCrit() {
    auto log = Logger::get();
    _gtChanceToMeleeCritMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTCHANGETOMELEECRIT");
        auto rows = server_->db()->execute_sync_many<DbcGtChanceToMeleeCrit>(stmt);
        for (const auto &row: rows) {
            GtChanceToMeleeCritDBC gtcmc;
            gtcmc.ID = row.id;
            gtcmc.Data = row.data;

            _gtChanceToMeleeCritMap[row.id] = gtcmc;
        }
        log->info(">>> DBCMgr: loaded {} gtChanceToMeleeCrit in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtChanceToMeleeCrit failed: {}", ex.what());
    }
}

void DBCMgr::load_gtChanceToMeleeCritBase() {
    auto log = Logger::get();
    _gtChanceToMeleeCritBaseMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTCHANGETOMELEECRITBASE");
        auto rows = server_->db()->execute_sync_many<DbcGtChanceToMeleeCritBase>(stmt);
        for (const auto &row: rows) {
            GtChanceToMeleeCritBaseDBC gtcmcb;
            gtcmcb.ID = row.id;
            gtcmcb.Data = row.data;

            _gtChanceToMeleeCritBaseMap[row.id] = gtcmcb;
        }
        log->info(">>> DBCMgr: loaded {} gtChanceToMeleeCritBase in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtChanceToMeleeCritBase failed: {}", ex.what());
    }
}

void DBCMgr::load_gtChanceToSpellCrit() {
    auto log = Logger::get();
    _gtChanceToSpellCritMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTCHANGETOSPELLCRIT");
        auto rows = server_->db()->execute_sync_many<DbcGtChanceToSpellCrit>(stmt);
        for (const auto &row: rows) {
            GtChanceToSpellCritDBC gtcsc;
            gtcsc.ID = row.id;
            gtcsc.Data = row.data;

            _gtChanceToSpellCritMap[row.id] = gtcsc;
        }
        log->info(">>> DBCMgr: loaded {} gtChanceToSpellCrit in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtChanceToSpellCrit failed: {}", ex.what());
    }
}

void DBCMgr::load_gtChanceToSpellCritBase() {
    auto log = Logger::get();
    _gtChanceToSpellCritBaseMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTCHANGETOSPELLCRITBASE");
        auto rows = server_->db()->execute_sync_many<DbcGtChanceToSpellCritBase>(stmt);
        for (const auto &row: rows) {
            GtChanceToSpellCritBaseDBC gtcscb;
            gtcscb.ID = row.id;
            gtcscb.Data = row.data;

            _gtChanceToSpellCritBaseMap[row.id] = gtcscb;
        }
        log->info(">>> DBCMgr: loaded {} gtChanceToSpellCritBase in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtChanceToSpellCritBase failed: {}", ex.what());
    }
}

void DBCMgr::load_gtCombatRatings() {
    auto log = Logger::get();
    _gtCombatRatingsMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTCOMBATRATINGS");
        auto rows = server_->db()->execute_sync_many<DbcGtCombatRatings>(stmt);
        for (const auto &row: rows) {
            GtCombatRatingsDBC gtcr;
            gtcr.ID = row.id;
            gtcr.Data = row.data;

            _gtCombatRatingsMap[row.id] = gtcr;
        }
        log->info(">>> DBCMgr: loaded {} gtCombatRatings in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtCombatRatings failed: {}", ex.what());
    }
}

void DBCMgr::load_gtNPCManaCostScaler() {
    auto log = Logger::get();
    _gtNPCManaCostScalerMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTNPCMANACOSTSCALER");
        auto rows = server_->db()->execute_sync_many<DbcGtnpcManaCostScaler>(stmt);
        for (const auto &row: rows) {
            GtNPCManaCostScalerDBC gtnpcMCS;
            gtnpcMCS.ID = row.id;
            gtnpcMCS.Data = row.data;

            _gtNPCManaCostScalerMap[row.id] = gtnpcMCS;
        }
        log->info(">>> DBCMgr: loaded {} gtNPCManaCostScaler in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtNPCManaCostScaler failed: {}", ex.what());
    }
}

void DBCMgr::load_gtOCTClassCombatRatingScalar() {
    auto log = Logger::get();
    _gtOCTClassCombatRatingScalarMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTOCTCLASSCOMBATRATINGSALAR");
        auto rows = server_->db()->execute_sync_many<DbcGtoctClassCombatRatingScalar>(stmt);
        for (const auto &row: rows) {
            GtOCTClassCombatRatingScalarDBC gtOCTccrs;
            gtOCTccrs.ID = row.id;
            gtOCTccrs.Data = row.data;

            _gtOCTClassCombatRatingScalarMap[row.id] = gtOCTccrs;
        }
        log->info(">>> DBCMgr: loaded {} gtOCTClassCombatRatingScalar in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtOCTClassCombatRatingScalar failed: {}", ex.what());
    }
}

void DBCMgr::load_gtOCTRegenHP() {
    auto log = Logger::get();
    _gtOCTRegenHPMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTOCTREGENHP");
        auto rows = server_->db()->execute_sync_many<DbcGtoctRegenHP>(stmt);
        for (const auto &row: rows) {
            GtOCTRegenHPDBC gtOCTrhp;
            gtOCTrhp.ID = row.id;
            gtOCTrhp.Data = row.data;

            _gtOCTRegenHPMap[row.id] = gtOCTrhp;
        }
        log->info(">>> DBCMgr: loaded {} gtOCTRegenHP in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtOCTRegenHP failed: {}", ex.what());
    }
}

void DBCMgr::load_gtOCTRegenMP() {
    auto log = Logger::get();
    _gtOCTRegenMPMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTOCTREGENMP");
        auto rows = server_->db()->execute_sync_many<DbcGtoctRegenMP>(stmt);
        for (const auto &row: rows) {
            GtOCTRegenMPDBC gtOCTrmp;
            gtOCTrmp.ID = row.id;
            gtOCTrmp.Data = row.data;

            _gtOCTRegenMPMap[row.id] = gtOCTrmp;
        }
        log->info(">>> DBCMgr: loaded {} gtOCTRegenMP in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtOCTRegenMP failed: {}", ex.what());
    }
}

void DBCMgr::load_gtRegenHPPerSpt() {
    auto log = Logger::get();
    _gtRegenHPPerSptMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTREGENHPPERSPT");
        auto rows = server_->db()->execute_sync_many<DbcGtRegenHpPerSpt>(stmt);
        for (const auto &row: rows) {
            GtRegenHPPerSptDBC gtrhpps;
            gtrhpps.ID = row.id;
            gtrhpps.Data = row.data;

            _gtRegenHPPerSptMap[row.id] = gtrhpps;
        }
        log->info(">>> DBCMgr: loaded {} gtRegenHPPerSpt in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtRegenHPPerSpt failed: {}", ex.what());
    }
}

void DBCMgr::load_gtRegenMPPerSpt() {
    auto log = Logger::get();
    _gtRegenMPPerSptMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_GTREGENMPPERSPT");
        auto rows = server_->db()->execute_sync_many<DbcGtRegenMpPerSpt>(stmt);
        for (const auto &row: rows) {
            GtRegenMPPerSptDBC gtrmpps;
            gtrmpps.ID = row.id;
            gtrmpps.Data = row.data;

            _gtRegenMPPerSptMap[row.id] = gtrmpps;
        }
        log->info(">>> DBCMgr: loaded {} gtRegenMPPerSpt in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_gtRegenMPPerSpt failed: {}", ex.what());
    }
}

void DBCMgr::load_Holidays() {
    auto log = Logger::get();
    _holidaysMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_HOLIDAYS");
        auto rows = server_->db()->execute_sync_many<DbcHolidays>(stmt);
        for (const auto &row: rows) {
            HolidaysDBC h;
            h.ID = row.id;

            h.Duration[0] = row.duration_1;
            h.Duration[1] = row.duration_2;
            h.Duration[2] = row.duration_3;
            h.Duration[3] = row.duration_4;
            h.Duration[4] = row.duration_5;
            h.Duration[5] = row.duration_6;
            h.Duration[6] = row.duration_7;
            h.Duration[7] = row.duration_8;
            h.Duration[8] = row.duration_9;
            h.Duration[9] = row.duration_10;

            // Date
            h.Date[0] = row.date_1;
            h.Date[1] = row.date_2;
            h.Date[2] = row.date_3;
            h.Date[3] = row.date_4;
            h.Date[4] = row.date_5;
            h.Date[5] = row.date_6;
            h.Date[6] = row.date_7;
            h.Date[7] = row.date_8;
            h.Date[8] = row.date_9;
            h.Date[9] = row.date_10;
            h.Date[10] = row.date_11;
            h.Date[11] = row.date_12;
            h.Date[12] = row.date_13;
            h.Date[13] = row.date_14;
            h.Date[14] = row.date_15;
            h.Date[15] = row.date_16;
            h.Date[16] = row.date_17;
            h.Date[17] = row.date_18;
            h.Date[18] = row.date_19;
            h.Date[19] = row.date_20;
            h.Date[20] = row.date_21;
            h.Date[21] = row.date_22;
            h.Date[22] = row.date_23;
            h.Date[23] = row.date_24;
            h.Date[24] = row.date_25;
            h.Date[25] = row.date_26;

            h.Region  = row.region;
            h.Looping = row.looping;

            // CalendarFlags
            h.CalendarFlags[0] = row.calendarflags_1;
            h.CalendarFlags[1] = row.calendarflags_2;
            h.CalendarFlags[2] = row.calendarflags_3;
            h.CalendarFlags[3] = row.calendarflags_4;
            h.CalendarFlags[4] = row.calendarflags_5;
            h.CalendarFlags[5] = row.calendarflags_6;
            h.CalendarFlags[6] = row.calendarflags_7;
            h.CalendarFlags[7] = row.calendarflags_8;
            h.CalendarFlags[8] = row.calendarflags_9;
            h.CalendarFlags[9] = row.calendarflags_10;

            h.TextureFilename = row.texturefilename.value_or("");

            h.Priority           = row.priority;
            h.CalendarFilterType = row.calendarfiltertype;

            _holidaysMap[row.id] = h;
        }
        log->info(">>> DBCMgr: loaded {} Holidays in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Holidays failed: {}", ex.what());
    }
}

void DBCMgr::load_Item() {
    auto log = Logger::get();
    _itemMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEM");
        auto rows = server_->db()->execute_sync_many<DbcItem>(stmt);
        for (const auto &row: rows) {
            ItemDBC i;
            i.ID = row.id;
            i.ClassID                 = row.class_id;
            i.SubclassID              = row.subclass_id;
            i.SoundOverrideSubclassID = row.sound_override_subclassid;
            i.Material                = row.material;
            i.DisplayInfoID           = row.display_info_id;
            i.InventoryType           = row.inventory_type;
            i.SheatheType             = row.sheathe_type;

            _itemMap[row.id] = i;
        }
        log->info(">>> DBCMgr: loaded {} Item in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Item failed: {}", ex.what());
    }
}

void DBCMgr::load_ItemBagFamily() {
    auto log = Logger::get();
    _itemBagFamilyMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMBAGFAMILY");
        auto rows = server_->db()->execute_sync_many<DbcItemBagFamily>(stmt);
        for (const auto &row: rows) {
            ItemBagFamilyDBC ibf;
            ibf.ID = row.id;

            _itemBagFamilyMap[row.id] = ibf;
        }
        log->info(">>> DBCMgr: loaded {} ItemBagFamily in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemBagFamily: {}", ex.what());
    }
}

void DBCMgr::load_ItemDisplayInfo() {
    auto log = Logger::get();
    _itemDisplayInfoMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMDISPLAYINFO");
        auto rows = server_->db()->execute_sync_many<DbcItemDisplayInfo>(stmt);
        for (const auto &row: rows) {
            ItemDisplayInfoDBC idi;
            idi.ID = row.id;

            // Массивы строк
            idi.ModelName[0]     = row.model_name_1.value_or("");
            idi.ModelName[1]     = row.model_name_2.value_or("");
            idi.ModelTexture[0]  = row.model_texture_1.value_or("");
            idi.ModelTexture[1]  = row.model_texture_2.value_or("");
            idi.InventoryIcon[0] = row.inventory_icon_1.value_or("");
            idi.InventoryIcon[1] = row.inventory_icon_2.value_or("");

            // Числовые массивы
            idi.GeosetGroup[0] = row.geoset_group_1;
            idi.GeosetGroup[1] = row.geoset_group_2;
            idi.GeosetGroup[2] = row.geoset_group_3;

            idi.Flags           = row.flags;
            idi.SpellVisualID   = row.spell_visual_id;
            idi.GroupSoundIndex = row.group_sound_index;

            idi.HelmetGeosetVisID[0] = row.helmet_geoset_vis_1;
            idi.HelmetGeosetVisID[1] = row.helmet_geoset_vis_2;

            idi.Texture[0] = row.texture_1.value_or("");
            idi.Texture[1] = row.texture_2.value_or("");
            idi.Texture[2] = row.texture_3.value_or("");
            idi.Texture[3] = row.texture_4.value_or("");
            idi.Texture[4] = row.texture_5.value_or("");
            idi.Texture[5] = row.texture_6.value_or("");
            idi.Texture[6] = row.texture_7.value_or("");
            idi.Texture[7] = row.texture_8.value_or("");

            idi.ItemVisual      = row.item_visual;
            idi.ParticleColorID = row.particle_color_id;

            _itemDisplayInfoMap[row.id] = idi;
        }
        log->info(">>> DBCMgr: loaded {} ItemDisplayInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemDisplayInfo: {}", ex.what());
    }
}

void DBCMgr::load_ItemExtendedCost() {
    auto log = Logger::get();
    _itemExtendedCostMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMEXTENDEDCOST");
        auto rows = server_->db()->execute_sync_many<DbcItemExtendedCost>(stmt);
        for (const auto &row: rows) {
            ItemExtendedCostDBC iec;
            iec.ID = row.id;
            iec.HonorPoints = row.honor_points;
            iec.ArenaPoints = row.arena_points;
            iec.ArenaBracket = row.arena_bracket;

            iec.ItemID[0] = row.item_id_1;
            iec.ItemID[1] = row.item_id_2;
            iec.ItemID[2] = row.item_id_3;
            iec.ItemID[3] = row.item_id_4;
            iec.ItemID[4] = row.item_id_5;

            iec.ItemCount[0] = row.item_count_1;
            iec.ItemCount[1] = row.item_count_2;
            iec.ItemCount[2] = row.item_count_3;
            iec.ItemCount[3] = row.item_count_4;
            iec.ItemCount[4] = row.item_count_5;

            iec.RequiredArenaRating = row.required_arena_rating;

            _itemExtendedCostMap[row.id] = iec;
        }
        log->info(">>> DBCMgr: loaded {} ItemExtendedCost in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemExtendedCost: {}", ex.what());
    }
}

void DBCMgr::load_ItemLimitCategory() {
    auto log = Logger::get();
    _itemLimitCategoryMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMLIMITCATEGORY");
        auto rows = server_->db()->execute_sync_many<DbcItemLimitCategory>(stmt);
        for (const auto &row: rows) {
            ItemLimitCategoryDBC ilc;
            ilc.ID = row.id;
            ilc.Quantity = row.quantity;
            ilc.Flags    = row.flags;

            _itemLimitCategoryMap[row.id] = ilc;
        }
        log->info(">>> DBCMgr: loaded {} ItemLimitCategory in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemLimitCategory: {}", ex.what());
    }
}

void DBCMgr::load_ItemRandomProperties() {
    auto log = Logger::get();
    _itemRandomPropertiesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMRANDOMPROPERTIES");
        auto rows = server_->db()->execute_sync_many<DbcItemRandomProperties>(stmt);
        for (const auto &row: rows) {
            ItemRandomPropertiesDBC irp;
            irp.ID = row.id;

            irp.Enchantment[0] = row.enchantment_1;
            irp.Enchantment[1] = row.enchantment_2;
            irp.Enchantment[2] = row.enchantment_3;
            irp.Enchantment[3] = row.enchantment_4;
            irp.Enchantment[4] = row.enchantment_5;

            irp.Name[LOCALE_enUS] = row.name_lang_enus.value_or("");
            irp.Name[LOCALE_enGB] = row.name_lang_engb.value_or("");
            irp.Name[LOCALE_koKR] = row.name_lang_kokr.value_or("");
            irp.Name[LOCALE_frFR] = row.name_lang_frfr.value_or("");
            irp.Name[LOCALE_deDE] = row.name_lang_dede.value_or("");
            irp.Name[LOCALE_enCN] = row.name_lang_encn.value_or("");
            irp.Name[LOCALE_zhCN] = row.name_lang_zhcn.value_or("");
            irp.Name[LOCALE_enTW] = row.name_lang_entw.value_or("");
            irp.Name[LOCALE_zhTW] = row.name_lang_zhtw.value_or("");
            irp.Name[LOCALE_esES] = row.name_lang_eses.value_or("");
            irp.Name[LOCALE_esMX] = row.name_lang_esmx.value_or("");
            irp.Name[LOCALE_ruRU] = row.name_lang_ruru.value_or("");
            irp.Name[LOCALE_ptPT] = row.name_lang_ptpt.value_or("");
            irp.Name[LOCALE_ptBR] = row.name_lang_ptbr.value_or("");
            irp.Name[LOCALE_itIT] = row.name_lang_itit.value_or("");

            _itemRandomPropertiesMap[row.id] = irp;
        }
        log->info(">>> DBCMgr: loaded {} ItemRandomProperties in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemRandomProperties: {}", ex.what());
    }
}

void DBCMgr::load_ItemRandomSuffix() {
    auto log = Logger::get();
    _itemRandomSuffixMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMRANDOMSUFFIX");
        auto rows = server_->db()->execute_sync_many<DbcItemRandomSuffix>(stmt);
        for (const auto &row: rows) {
            ItemRandomSuffixDBC irs;
            irs.ID = row.id;

            irs.Name[LOCALE_enUS] = row.name_lang_enus.value_or("");
            irs.Name[LOCALE_enGB] = row.name_lang_engb.value_or("");
            irs.Name[LOCALE_koKR] = row.name_lang_kokr.value_or("");
            irs.Name[LOCALE_frFR] = row.name_lang_frfr.value_or("");
            irs.Name[LOCALE_deDE] = row.name_lang_dede.value_or("");
            irs.Name[LOCALE_enCN] = row.name_lang_encn.value_or("");
            irs.Name[LOCALE_zhCN] = row.name_lang_zhcn.value_or("");
            irs.Name[LOCALE_enTW] = row.name_lang_entw.value_or("");
            irs.Name[LOCALE_zhTW] = row.name_lang_zhtw.value_or("");
            irs.Name[LOCALE_esES] = row.name_lang_eses.value_or("");
            irs.Name[LOCALE_esMX] = row.name_lang_esmx.value_or("");
            irs.Name[LOCALE_ruRU] = row.name_lang_ruru.value_or("");
            irs.Name[LOCALE_ptPT] = row.name_lang_ptpt.value_or("");
            irs.Name[LOCALE_ptBR] = row.name_lang_ptbr.value_or("");
            irs.Name[LOCALE_itIT] = row.name_lang_itit.value_or("");

            irs.Enchantment[0] = row.enchantment_1;
            irs.Enchantment[1] = row.enchantment_2;
            irs.Enchantment[2] = row.enchantment_3;
            irs.Enchantment[3] = row.enchantment_4;
            irs.Enchantment[4] = row.enchantment_5;

            irs.AllocationPct[0] = row.allocationpct_1;
            irs.AllocationPct[1] = row.allocationpct_2;
            irs.AllocationPct[2] = row.allocationpct_3;
            irs.AllocationPct[3] = row.allocationpct_4;
            irs.AllocationPct[4] = row.allocationpct_5;

            _itemRandomSuffixMap[row.id] = irs;

            if (_itemRandomSuffixHighestID)
            {
                if (_itemRandomSuffixHighestID < row.id)
                    _itemRandomSuffixHighestID = row.id;
            }
            else
                _itemRandomSuffixHighestID = row.id;
        }
        log->info(">>> DBCMgr: loaded {} ItemRandomSuffix in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemRandomSuffix: {}", ex.what());
    }
}

void DBCMgr::load_ItemSet() {
    auto log = Logger::get();
    _itemSetMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_ITEMSET");
        auto rows = server_->db()->execute_sync_many<DbcItemSet>(stmt);
        for (const auto &row: rows) {
            ItemSetDBC is;
            is.ID = row.id;

            is.Name[LOCALE_enUS] = row.name_lang_enus.value_or("");
            is.Name[LOCALE_enGB] = row.name_lang_engb.value_or("");
            is.Name[LOCALE_koKR] = row.name_lang_kokr.value_or("");
            is.Name[LOCALE_frFR] = row.name_lang_frfr.value_or("");
            is.Name[LOCALE_deDE] = row.name_lang_dede.value_or("");
            is.Name[LOCALE_enCN] = row.name_lang_encn.value_or("");
            is.Name[LOCALE_zhCN] = row.name_lang_zhcn.value_or("");
            is.Name[LOCALE_enTW] = row.name_lang_entw.value_or("");
            is.Name[LOCALE_zhTW] = row.name_lang_zhtw.value_or("");
            is.Name[LOCALE_esES] = row.name_lang_eses.value_or("");
            is.Name[LOCALE_esMX] = row.name_lang_esmx.value_or("");
            is.Name[LOCALE_ruRU] = row.name_lang_ruru.value_or("");
            is.Name[LOCALE_ptPT] = row.name_lang_ptpt.value_or("");
            is.Name[LOCALE_ptBR] = row.name_lang_ptbr.value_or("");
            is.Name[LOCALE_itIT] = row.name_lang_itit.value_or("");

            is.ItemID[0] = row.itemid_1;
            is.ItemID[1] = row.itemid_2;
            is.ItemID[2] = row.itemid_3;
            is.ItemID[3] = row.itemid_4;
            is.ItemID[4] = row.itemid_5;
            is.ItemID[5] = row.itemid_6;
            is.ItemID[6] = row.itemid_7;
            is.ItemID[7] = row.itemid_8;
            is.ItemID[8] = row.itemid_9;
            is.ItemID[9] = row.itemid_10;

            is.SetSpellID[0] = row.setspellid_1;
            is.SetSpellID[1] = row.setspellid_2;
            is.SetSpellID[2] = row.setspellid_3;
            is.SetSpellID[3] = row.setspellid_4;
            is.SetSpellID[4] = row.setspellid_5;
            is.SetSpellID[5] = row.setspellid_6;
            is.SetSpellID[6] = row.setspellid_7;
            is.SetSpellID[7] = row.setspellid_8;

            is.SetThreshold[0] = row.setthreshold_1;
            is.SetThreshold[1] = row.setthreshold_2;
            is.SetThreshold[2] = row.setthreshold_3;
            is.SetThreshold[3] = row.setthreshold_4;
            is.SetThreshold[4] = row.setthreshold_5;
            is.SetThreshold[5] = row.setthreshold_6;
            is.SetThreshold[6] = row.setthreshold_7;
            is.SetThreshold[7] = row.setthreshold_8;

            is.RequiredSkill     = row.requiredskill;
            is.RequiredSkillRank = row.requiredskillrank;

            _itemSetMap[row.id] = is;
        }
        log->info(">>> DBCMgr: loaded {} ItemSet in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ItemSet: {}", ex.what());
    }
}

void DBCMgr::load_LFGDungeons() {
    auto log = Logger::get();
    _lfgDungeonMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_LFGDUNGEONS");
        auto rows = server_->db()->execute_sync_many<PgDbcLfgDungeons>(stmt);
        for (const auto &row: rows) {
            LFGDungeonDBC ld;
            ld.ID = row.id;

            ld.Name[LOCALE_enUS] = row.name_lang_enus.value_or("");
            ld.Name[LOCALE_enGB] = row.name_lang_engb.value_or("");
            ld.Name[LOCALE_koKR] = row.name_lang_kokr.value_or("");
            ld.Name[LOCALE_frFR] = row.name_lang_frfr.value_or("");
            ld.Name[LOCALE_deDE] = row.name_lang_dede.value_or("");
            ld.Name[LOCALE_enCN] = row.name_lang_encn.value_or("");
            ld.Name[LOCALE_zhCN] = row.name_lang_zhcn.value_or("");
            ld.Name[LOCALE_enTW] = row.name_lang_entw.value_or("");
            ld.Name[LOCALE_zhTW] = row.name_lang_zhtw.value_or("");
            ld.Name[LOCALE_esES] = row.name_lang_eses.value_or("");
            ld.Name[LOCALE_esMX] = row.name_lang_esmx.value_or("");
            ld.Name[LOCALE_ruRU] = row.name_lang_ruru.value_or("");
            ld.Name[LOCALE_ptPT] = row.name_lang_ptpt.value_or("");
            ld.Name[LOCALE_ptBR] = row.name_lang_ptbr.value_or("");
            ld.Name[LOCALE_itIT] = row.name_lang_itit.value_or("");

            ld.MinLevel       = row.min_level;
            ld.MaxLevel       = row.max_level;
            ld.TargetLevel    = row.target_level;
            ld.TargetLevelMin = row.target_level_min;
            ld.TargetLevelMax = row.target_level_max;
            ld.MapID          = row.mapid;
            ld.Difficulty     = row.difficulty;
            ld.Flags          = row.flags;
            ld.TypeID         = row.typeId;
            ld.ExpansionLevel = row.expansionlevel;
            ld.GroupID        = row.group_id;

            _lfgDungeonMap[row.id] = ld;
        }
        log->info(">>> DBCMgr: loaded {} LFGDungeons in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_LFGDungeons failed: {}", ex.what());
    }
}

void DBCMgr::load_Light() {
    auto log = Logger::get();
    _lightMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_LIGHT");
        auto rows = server_->db()->execute_sync_many<DbcLight>(stmt);
        for (const auto &row: rows) {
            LightDBC l;
            l.ID = row.id;
            l.ContinentID  = row.continent_id;
            l.GameCoords.X = row.x;
            l.GameCoords.Y = row.y;
            l.GameCoords.Z = row.z;

            _lightMap[row.id] = l;
        }
        log->info(">>> DBCMgr: loaded {} Light in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_LFGDungeons failed: {}", ex.what());
    }
}

void DBCMgr::load_LiquidType() {
    auto log = Logger::get();
    _liquidTypeMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_LIQUIDTYPE");
        auto rows = server_->db()->execute_sync_many<DbcLiquidType>(stmt);
        for (const auto &row: rows) {
            LiquidTypeDBC lt;
            lt.ID = row.id;
            lt.Type = row.type;
            lt.SpellID = row.spellid;

            _liquidTypeMap[row.id] = lt;
        }
        log->info(">>> DBCMgr: loaded {} LiquidType in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_LiquidType failed: {}", ex.what());
    }
}

void DBCMgr::load_Lock() {
    auto log = Logger::get();
    _lockMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_LOCK");
        auto rows = server_->db()->execute_sync_many<DbcLock>(stmt);
        for (const auto &row: rows) {
            LockDBC l;
            l.ID = row.id;

            l.Type[0] = row.type_1;
            l.Type[1] = row.type_2;
            l.Type[2] = row.type_3;
            l.Type[3] = row.type_4;
            l.Type[4] = row.type_5;
            l.Type[5] = row.type_6;
            l.Type[6] = row.type_7;
            l.Type[7] = row.type_8;

            l.Index[0] = row.index_1;
            l.Index[1] = row.index_2;
            l.Index[2] = row.index_3;
            l.Index[3] = row.index_4;
            l.Index[4] = row.index_5;
            l.Index[5] = row.index_6;
            l.Index[6] = row.index_7;
            l.Index[7] = row.index_8;

            l.Skill[0] = row.skill_1;
            l.Skill[1] = row.skill_2;
            l.Skill[2] = row.skill_3;
            l.Skill[3] = row.skill_4;
            l.Skill[4] = row.skill_5;
            l.Skill[5] = row.skill_6;
            l.Skill[6] = row.skill_7;
            l.Skill[7] = row.skill_8;

            _lockMap[row.id] = l;
        }
        log->info(">>> DBCMgr: loaded {} Lock in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Lock failed: {}", ex.what());
    }
}

void DBCMgr::load_MailTemplate() {
    auto log = Logger::get();
    _mailTemplateMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_MAILTEMPLATE");
        auto rows = server_->db()->execute_sync_many<PgDbcMailTemplate>(stmt);
        for (const auto &row: rows) {
            MailTemplateDBC mt;
            mt.ID = row.id;

            mt.Subject[LOCALE_enUS] = row.subject_lang_en_us.value_or("");
            mt.Subject[LOCALE_enGB] = row.subject_lang_en_gb.value_or("");
            mt.Subject[LOCALE_koKR] = row.subject_lang_ko_kr.value_or("");
            mt.Subject[LOCALE_frFR] = row.subject_lang_fr_fr.value_or("");
            mt.Subject[LOCALE_deDE] = row.subject_lang_de_de.value_or("");
            mt.Subject[LOCALE_enCN] = row.subject_lang_en_cn.value_or("");
            mt.Subject[LOCALE_zhCN] = row.subject_lang_zh_cn.value_or("");
            mt.Subject[LOCALE_enTW] = row.subject_lang_en_tw.value_or("");
            mt.Subject[LOCALE_zhTW] = row.subject_lang_zh_tw.value_or("");
            mt.Subject[LOCALE_esES] = row.subject_lang_es_es.value_or("");
            mt.Subject[LOCALE_esMX] = row.subject_lang_es_mx.value_or("");
            mt.Subject[LOCALE_ruRU] = row.subject_lang_ru_ru.value_or("");
            mt.Subject[LOCALE_ptPT] = row.subject_lang_pt_pt.value_or("");
            mt.Subject[LOCALE_ptBR] = row.subject_lang_pt_br.value_or("");
            mt.Subject[LOCALE_itIT] = row.subject_lang_it_it.value_or("");

            mt.Body[LOCALE_enUS] = row.body_lang_en_us.value_or("");
            mt.Body[LOCALE_enGB] = row.body_lang_en_gb.value_or("");
            mt.Body[LOCALE_koKR] = row.body_lang_ko_kr.value_or("");
            mt.Body[LOCALE_frFR] = row.body_lang_fr_fr.value_or("");
            mt.Body[LOCALE_deDE] = row.body_lang_de_de.value_or("");
            mt.Body[LOCALE_enCN] = row.body_lang_en_cn.value_or("");
            mt.Body[LOCALE_zhCN] = row.body_lang_zh_cn.value_or("");
            mt.Body[LOCALE_enTW] = row.body_lang_en_tw.value_or("");
            mt.Body[LOCALE_zhTW] = row.body_lang_zh_tw.value_or("");
            mt.Body[LOCALE_esES] = row.body_lang_es_es.value_or("");
            mt.Body[LOCALE_esMX] = row.body_lang_es_mx.value_or("");
            mt.Body[LOCALE_ruRU] = row.body_lang_ru_ru.value_or("");
            mt.Body[LOCALE_ptPT] = row.body_lang_pt_pt.value_or("");
            mt.Body[LOCALE_ptBR] = row.body_lang_pt_br.value_or("");
            mt.Body[LOCALE_itIT] = row.body_lang_it_it.value_or("");

            _mailTemplateMap[row.id] = mt;
        }
        log->info(">>> DBCMgr: loaded {} MailTemplate in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_MailTemplate failed: {}", ex.what());
    }
}

void DBCMgr::load_Map() {
    auto log = Logger::get();
    _mapMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_MAP");
        auto rows = server_->db()->execute_sync_many<DbcMap>(stmt);
        for (const auto &row: rows) {
            MapDBC m;
            m.ID = row.id;
            m.InstanceType = row.instance_type;
            m.Flags        = row.flags;

            m.MapName[LOCALE_enUS] = row.mapname_lang_en_us.value_or("");
            m.MapName[LOCALE_enGB] = row.mapname_lang_en_gb.value_or("");
            m.MapName[LOCALE_koKR] = row.mapname_lang_ko_kr.value_or("");
            m.MapName[LOCALE_frFR] = row.mapname_lang_fr_fr.value_or("");
            m.MapName[LOCALE_deDE] = row.mapname_lang_de_de.value_or("");
            m.MapName[LOCALE_enCN] = row.mapname_lang_en_cn.value_or("");
            m.MapName[LOCALE_zhCN] = row.mapname_lang_zh_cn.value_or("");
            m.MapName[LOCALE_enTW] = row.mapname_lang_en_tw.value_or("");
            m.MapName[LOCALE_zhTW] = row.mapname_lang_zh_tw.value_or("");
            m.MapName[LOCALE_esES] = row.mapname_lang_es_es.value_or("");
            m.MapName[LOCALE_esMX] = row.mapname_lang_es_mx.value_or("");
            m.MapName[LOCALE_ruRU] = row.mapname_lang_ru_ru.value_or("");
            m.MapName[LOCALE_ptPT] = row.mapname_lang_pt_pt.value_or("");
            m.MapName[LOCALE_ptBR] = row.mapname_lang_pt_br.value_or("");
            m.MapName[LOCALE_itIT] = row.mapname_lang_it_it.value_or("");

            m.AreaTableID     = row.area_table_id;
            m.LoadingScreenID = row.loading_screen_id;
            m.CorpseMapID     = row.corpse_map_id;
            m.Corpse.X        = row.corpse_x;
            m.Corpse.Y        = row.corpse_y;
            m.ExpansionID     = row.expansion_id;
            m.RaidOffset      = row.raid_offset;
            m.MaxPlayers      = row.max_players;

            _mapMap[row.id] = m;
        }
        log->info(">>> DBCMgr: loaded {} Map in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Map failed: {}", ex.what());
    }
}

void DBCMgr::load_MapDifficulty() {
    auto log = Logger::get();
    _mapDifficultyMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_MAPDIFFICULTY");
        auto rows = server_->db()->execute_sync_many<DbcMapDifficulty>(stmt);
        for (const auto &row: rows) {
            MapDifficultyDBC md;
            md.ID = row.id;
            md.MapID      = row.map_id;
            md.Difficulty = row.difficulty;

            md.Message[LOCALE_enUS] = row.message_lang_en_us.value_or("");
            md.Message[LOCALE_enGB] = row.message_lang_en_gb.value_or("");
            md.Message[LOCALE_koKR] = row.message_lang_ko_kr.value_or("");
            md.Message[LOCALE_frFR] = row.message_lang_fr_fr.value_or("");
            md.Message[LOCALE_deDE] = row.message_lang_de_de.value_or("");
            md.Message[LOCALE_enCN] = row.message_lang_en_cn.value_or("");
            md.Message[LOCALE_zhCN] = row.message_lang_zh_cn.value_or("");
            md.Message[LOCALE_enTW] = row.message_lang_en_tw.value_or("");
            md.Message[LOCALE_zhTW] = row.message_lang_zh_tw.value_or("");
            md.Message[LOCALE_esES] = row.message_lang_es_es.value_or("");
            md.Message[LOCALE_esMX] = row.message_lang_es_mx.value_or("");
            md.Message[LOCALE_ruRU] = row.message_lang_ru_ru.value_or("");
            md.Message[LOCALE_ptPT] = row.message_lang_pt_pt.value_or("");
            md.Message[LOCALE_ptBR] = row.message_lang_pt_br.value_or("");
            md.Message[LOCALE_itIT] = row.message_lang_it_it.value_or("");

            md.RaidDuration = row.raid_duration;
            md.MaxPlayers   = row.max_players;

            _mapDifficultyMap[row.id] = md;
        }
        log->info(">>> DBCMgr: loaded {} MapDifficulty in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_MapDifficulty failed: {}", ex.what());
    }
}

void DBCMgr::load_Movie() {
    auto log = Logger::get();
    _movieMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_MOVIE");
        auto rows = server_->db()->execute_sync_many<DbcMovie>(stmt);
        for (const auto &row: rows) {
            MovieDBC m;
            m.ID = row.ID;

            _movieMap[row.ID] = m;
        }
        log->info(">>> DBCMgr: loaded {} Movie in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Movie failed: {}", ex.what());
    }
}

void DBCMgr::load_NamesProfanity() {
    auto log = Logger::get();
    _namesProfanityMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_NAMESPROFANITY");
        auto rows = server_->db()->execute_sync_many<DbcNamesProfanity>(stmt);
        for (const auto &row: rows) {
            NamesProfanityDBC np;
            np.ID = row.id;
            np.Name     = row.name.value_or("");
            np.Language = row.language;

            _namesProfanityMap[row.id] = np;
        }
        log->info(">>> DBCMgr: loaded {} NamesProfanity in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_NamesProfanity failed: {}", ex.what());
    }
}

void DBCMgr::load_NamesReserved() {
    auto log = Logger::get();
    _namesReservedMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_NAMESRESERVED");
        auto rows = server_->db()->execute_sync_many<DbcNamesReserved>(stmt);
        for (const auto &row: rows) {
            NamesReservedDBC nr;
            nr.ID = row.id;
            nr.Name = row.name.value_or("");
            nr.Language = row.language;

            _namesReservedMap[row.id] = nr;
        }
        log->info(">>> DBCMgr: loaded {} NamesReserved in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_NamesReserved failed: {}", ex.what());
    }
}

void DBCMgr::load_OverrideSpellData() {
    auto log = Logger::get();
    _overrideSpellDataMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_OVERRIDESPELLDATA");
        auto rows = server_->db()->execute_sync_many<DbcOverridespellData>(stmt);
        for (const auto &row: rows) {
            OverrideSpellDataDBC osd;
            osd.ID = row.id;

            osd.Spells[0] = row.spells_1;
            osd.Spells[1] = row.spells_2;
            osd.Spells[2] = row.spells_3;
            osd.Spells[3] = row.spells_4;
            osd.Spells[4] = row.spells_5;
            osd.Spells[5] = row.spells_6;
            osd.Spells[6] = row.spells_7;
            osd.Spells[7] = row.spells_8;
            osd.Spells[8] = row.spells_9;
            osd.Spells[9] = row.spells_10;

            _overrideSpellDataMap[row.id] = osd;
        }
        log->info(">>> DBCMgr: loaded {} OverrideSpellData in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_OverrideSpellData failed: {}", ex.what());
    }
}

void DBCMgr::load_PowerDisplay() {
    auto log = Logger::get();
    _powerDisplayMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_POWERDISPLAY");
        auto rows = server_->db()->execute_sync_many<DbcPowerDisplay>(stmt);
        for (const auto &row: rows) {
            PowerDisplayDBC pd;
            pd.ID = row.id;
            pd.ActualType = row.actualtype;

            _powerDisplayMap[row.id] = pd;
        }
        log->info(">>> DBCMgr: loaded {} PowerDisplay in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_PowerDisplay failed: {}", ex.what());
    }
}

void DBCMgr::load_PvpDifficulty() {
    auto log = Logger::get();
    _pvpDifficultyMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_PVPDifficulty");
        auto rows = server_->db()->execute_sync_many<DbcPvpdifficulty>(stmt);
        for (const auto &row: rows) {
            PvPDifficultyDBC pd;
            pd.ID = row.id;
            pd.MapID      = row.map_id;
            pd.RangeIndex = row.range_index;
            pd.MinLevel   = row.min_level;
            pd.MaxLevel   = row.max_level;
            pd.Difficulty = row.difficulty;

            if (pd.RangeIndex < MAX_BATTLEGROUND_BRACKETS) {
                _pvpDifficultyMap[row.id] = pd;
            }
            else {
                log->error("PvpDifficulty bracket ({}) exceeded max allowed value ({})", pd.RangeIndex, MAX_BATTLEGROUND_BRACKETS);
            }
        }
        log->info(">>> DBCMgr: loaded {} PvpDifficulty in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_PvpDifficulty failed: {}", ex.what());
    }
}

void DBCMgr::load_QuestFactionReward() {
    auto log = Logger::get();
    _questFactionRewardMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_QUESTFACTIONREWARD");
        auto rows = server_->db()->execute_sync_many<DbcQuestFactionReward>(stmt);
        for (const auto &row: rows) {
            QuestFactionRewardDBC qfr;
            qfr.ID = row.id;

            qfr.Difficulty[0] = row.difficulty_1;
            qfr.Difficulty[1] = row.difficulty_2;
            qfr.Difficulty[2] = row.difficulty_3;
            qfr.Difficulty[3] = row.difficulty_4;
            qfr.Difficulty[4] = row.difficulty_5;
            qfr.Difficulty[5] = row.difficulty_6;
            qfr.Difficulty[6] = row.difficulty_7;
            qfr.Difficulty[7] = row.difficulty_8;
            qfr.Difficulty[8] = row.difficulty_9;
            qfr.Difficulty[9] = row.difficulty_10;

            _questFactionRewardMap[row.id] = qfr;
        }
        log->info(">>> DBCMgr: loaded {} QuestFactionReward in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_QuestFactionReward failed: {}", ex.what());
    }
}

void DBCMgr::load_QuestSort() {
    auto log = Logger::get();
    _questSortMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_QUESTSORT");
        auto rows = server_->db()->execute_sync_many<DbcQuestsort>(stmt);
        for (const auto &row: rows) {
            QuestSortDBC qs;
            qs.ID = row.id;

            _questSortMap[row.id] = qs;
        }
        log->info(">>> DBCMgr: loaded {} QuestSort in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_QuestSort failed: {}", ex.what());
    }
}

void DBCMgr::load_QuestXP() {
    auto log = Logger::get();
    _questXPMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_QUESTXP");
        auto rows = server_->db()->execute_sync_many<DbcQuestXp>(stmt);
        for (const auto &row: rows) {
            QuestXPDBC qxp;
            qxp.ID = row.id;

            qxp.Difficulty[0] = row.difficulty_1;
            qxp.Difficulty[1] = row.difficulty_2;
            qxp.Difficulty[2] = row.difficulty_3;
            qxp.Difficulty[3] = row.difficulty_4;
            qxp.Difficulty[4] = row.difficulty_5;
            qxp.Difficulty[5] = row.difficulty_6;
            qxp.Difficulty[6] = row.difficulty_7;
            qxp.Difficulty[7] = row.difficulty_8;
            qxp.Difficulty[8] = row.difficulty_9;
            qxp.Difficulty[9] = row.difficulty_10;

            _questXPMap[row.id] = qxp;
        }
        log->info(">>> DBCMgr: loaded {} QuestXP in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_QuestXP failed: {}", ex.what());
    }
}

void DBCMgr::load_RandPropPoints() {
    auto log = Logger::get();
    _randPropPointsMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_RANDPROPPOINTS");
        auto rows = server_->db()->execute_sync_many<DbcRandPropPoints>(stmt);
        for (const auto &row: rows) {
            RandPropPointsDBC rpp;
            rpp.ID = row.id;
            rpp.Epic[0] = row.epic_1;
            rpp.Epic[1] = row.epic_2;
            rpp.Epic[2] = row.epic_3;
            rpp.Epic[3] = row.epic_4;
            rpp.Epic[4] = row.epic_5;

            rpp.Superior[0] = row.superior_1;
            rpp.Superior[1] = row.superior_2;
            rpp.Superior[2] = row.superior_3;
            rpp.Superior[3] = row.superior_4;
            rpp.Superior[4] = row.superior_5;

            rpp.Good[0] = row.good_1;
            rpp.Good[1] = row.good_2;
            rpp.Good[2] = row.good_3;
            rpp.Good[3] = row.good_4;
            rpp.Good[4] = row.good_5;

            _randPropPointsMap[row.id] = rpp;
        }
        log->info(">>> DBCMgr: loaded {} RandPropPoints in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_RandPropPoints failed: {}", ex.what());
    }
}

void DBCMgr::load_ScalingStatDistribution() {
    auto log = Logger::get();
    _scalingStatDistributionMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SCALINGSTATDISTRIBUTION");
        auto rows = server_->db()->execute_sync_many<DbcScalingStatDistribution>(stmt);
        for (const auto &row: rows) {
            ScalingStatDistributionDBC ssd;
            ssd.ID = row.id;

            ssd.StatID[0] = row.statid_1;
            ssd.StatID[1] = row.statid_2;
            ssd.StatID[2] = row.statid_3;
            ssd.StatID[3] = row.statid_4;
            ssd.StatID[4] = row.statid_5;
            ssd.StatID[5] = row.statid_6;
            ssd.StatID[6] = row.statid_7;
            ssd.StatID[7] = row.statid_8;
            ssd.StatID[8] = row.statid_9;
            ssd.StatID[9] = row.statid_10;

            ssd.Bonus[0] = row.bonus_1;
            ssd.Bonus[1] = row.bonus_2;
            ssd.Bonus[2] = row.bonus_3;
            ssd.Bonus[3] = row.bonus_4;
            ssd.Bonus[4] = row.bonus_5;
            ssd.Bonus[5] = row.bonus_6;
            ssd.Bonus[6] = row.bonus_7;
            ssd.Bonus[7] = row.bonus_8;
            ssd.Bonus[8] = row.bonus_9;
            ssd.Bonus[9] = row.bonus_10;

            ssd.Maxlevel = row.maxlevel;

            _scalingStatDistributionMap[row.id] = ssd;
        }
        log->info(">>> DBCMgr: loaded {} ScalingStatDistribution in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ScalingStatDistribution failed: {}", ex.what());
    }
}

void DBCMgr::load_ScalingStatValues() {
    auto log = Logger::get();
    _scalingStatValuesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SCALINGSTATVALUES");
        auto rows = server_->db()->execute_sync_many<DbcScalingStatValues>(stmt);
        for (const auto &row: rows) {
            ScalingStatValuesDBC ssv;
            ssv.ID        = row.id;
            ssv.Charlevel = row.charlevel;
            ssv.ShoulderBudget       = row.shoulderbudget;
            ssv.TrinketBudget        = row.trinketbudget;
            ssv.WeaponBudget1H       = row.weaponbudget1h;
            ssv.RangedBudget         = row.rangedbudget;
            ssv.ClothShoulderArmor   = row.clothshoulderarmor;
            ssv.LeatherShoulderArmor = row.leathershoulderarmor;
            ssv.MailShoulderArmor    = row.mailshoulderarmor;
            ssv.PlateShoulderArmor   = row.plateshoulderarmor;
            ssv.WeaponDPS1H          = row.weapondps1h;
            ssv.WeaponDPS2H          = row.weapondps2h;
            ssv.SpellcasterDPS1H     = row.spellcasterdps1h;
            ssv.SpellcasterDPS2H     = row.spellcasterdps2h;
            ssv.RangedDPS            = row.rangeddps;
            ssv.WandDPS              = row.wanddps;
            ssv.SpellPower           = row.spellpower;
            ssv.PrimaryBudget        = row.primarybudget;
            ssv.TertiaryBudget       = row.tertiarybudget;
            ssv.ClothCloakArmor      = row.clothcloakarmor;
            ssv.ClothChestArmor      = row.clothchestarmor;
            ssv.LeatherChestArmor    = row.leatherchestarmor;
            ssv.MailChestArmor       = row.mailchestarmor;
            ssv.PlateChestArmor      = row.platechestarmor;

            _scalingStatValuesMap[row.charlevel] = ssv;
        }
        log->info(">>> DBCMgr: loaded {} ScalingStatValues in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_ScalingStatValues failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillLine() {
    auto log = Logger::get();
    _skillLineMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLLINE");
        auto rows = server_->db()->execute_sync_many<DbcSkillLine>(stmt);
        for (const auto &row: rows) {
            SkillLineDBC sl;
            sl.ID = row.ID;
            sl.CategoryID = row.CategoryID;

            // DisplayName
            sl.DisplayName[LOCALE_enUS] = row.DisplayName_Lang_enUS.value_or("");
            sl.DisplayName[LOCALE_enGB] = row.DisplayName_Lang_enGB.value_or("");
            sl.DisplayName[LOCALE_koKR] = row.DisplayName_Lang_koKR.value_or("");
            sl.DisplayName[LOCALE_frFR] = row.DisplayName_Lang_frFR.value_or("");
            sl.DisplayName[LOCALE_deDE] = row.DisplayName_Lang_deDE.value_or("");
            sl.DisplayName[LOCALE_enCN] = row.DisplayName_Lang_enCN.value_or("");
            sl.DisplayName[LOCALE_zhCN] = row.DisplayName_Lang_zhCN.value_or("");
            sl.DisplayName[LOCALE_enTW] = row.DisplayName_Lang_enTW.value_or("");
            sl.DisplayName[LOCALE_zhTW] = row.DisplayName_Lang_zhTW.value_or("");
            sl.DisplayName[LOCALE_esES] = row.DisplayName_Lang_esES.value_or("");
            sl.DisplayName[LOCALE_esMX] = row.DisplayName_Lang_esMX.value_or("");
            sl.DisplayName[LOCALE_ruRU] = row.DisplayName_Lang_ruRU.value_or("");
            sl.DisplayName[LOCALE_ptPT] = row.DisplayName_Lang_ptPT.value_or("");
            sl.DisplayName[LOCALE_ptBR] = row.DisplayName_Lang_ptBR.value_or("");
            sl.DisplayName[LOCALE_itIT] = row.DisplayName_Lang_itIT.value_or("");

            sl.SpellIconID = row.SpellIconID;
            sl.CanLink     = row.CanLink;

            _skillLineMap[row.ID] = sl;
        }
        log->info(">>> DBCMgr: loaded {} SkillLine in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillLine failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillLineAbility() {
    auto log = Logger::get();
    _skillLineAbilityMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLLINEABILITY");
        auto rows = server_->db()->execute_sync_many<DbcSkillLineAbility>(stmt);
        for (const auto &row: rows) {
            SkillLineAbilityDBC sla;
            sla.ID = row.id;
            sla.SkillLine                = row.skillline;
            sla.Spell                    = row.spell;
            sla.RaceMask                 = row.racemask;
            sla.ClassMask                = row.classmask;
            sla.MinSkillLineRank         = row.minskilllinerank;
            sla.SupercededBySpell        = row.supercededbyspell;
            sla.AcquireMethod            = row.acquiremethod;
            sla.TrivialSkillLineRankHigh = row.trivialskilllinerankhigh;
            sla.TrivialSkillLineRankLow  = row.trivialskilllineranklow;

            _skillLineAbilityMap[row.id] = sla;
        }
        log->info(">>> DBCMgr: loaded {} SkillLineAbility in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillLineAbility failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillRaceClassInfo() {
    auto log = Logger::get();
    _skillRaceClassInfoMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLRACECLASSINFO");
        auto rows = server_->db()->execute_sync_many<DbcSkillRaceClassInfo>(stmt);
        for (const auto &row: rows) {
            SkillRaceClassInfoDBC srci;
            srci.ID = row.ID;
            srci.SkillID     = row.SkillID;
            srci.RaceMask    = row.RaceMask;
            srci.ClassMask   = row.ClassMask;
            srci.Flags       = row.Flags;
            srci.SkillTierID = row.SkillTierID;

            _skillRaceClassInfoMap[row.ID] = srci;
        }
        log->info(">>> DBCMgr: loaded {} SkillRaceClassInfo in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillRaceClassInfo failed: {}", ex.what());
    }
}

void DBCMgr::load_SkillTiers() {
    auto log = Logger::get();
    _skillTiersMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SKILLTIERS");
        auto rows = server_->db()->execute_sync_many<DbcSkillTiers>(stmt);
        for (const auto &row: rows) {
            SkillTiersDBC st;
            st.ID = row.id;

            st.Value[0] = row.value_1;
            st.Value[1] = row.value_2;
            st.Value[2] = row.value_3;
            st.Value[3] = row.value_4;
            st.Value[4] = row.value_5;
            st.Value[5] = row.value_6;
            st.Value[6] = row.value_7;
            st.Value[7] = row.value_8;
            st.Value[8] = row.value_9;
            st.Value[9] = row.value_10;
            st.Value[10] = row.value_11;
            st.Value[11] = row.value_12;
            st.Value[12] = row.value_13;
            st.Value[13] = row.value_14;
            st.Value[14] = row.value_15;
            st.Value[15] = row.value_16;

            _skillTiersMap[row.id] = st;
        }
        log->info(">>> DBCMgr: loaded {} SkillTiers in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SkillTiers failed: {}", ex.what());
    }
}

void DBCMgr::load_SoundEntries() {
    auto log = Logger::get();
    _soundEntriesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SOUNDENTRIES");
        auto rows = server_->db()->execute_sync_many<DbcSoundEntries>(stmt);
        for (const auto &row: rows) {
            SoundEntriesDBC se;
            se.ID = row.id;

            _soundEntriesMap[row.id] = se;
        }
        log->info(">>> DBCMgr: loaded {} SoundEntries in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SoundEntries failed: {}", ex.what());
    }
}

void DBCMgr::load_Spells() {
    auto log = Logger::get();
    _spellMap.clear();
    uint32_t oldMSTime = getMSTime();
    log->info("loading spell.dbc...");

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELL");
        auto rows = server_->db()->execute_sync_many<DbcSpell>(stmt);
        for (const auto &row: rows) {
            SpellDBC spell;
            spell.ID = row.id;
            spell.Category = row.category;
            spell.DispelType = row.dispelType;
            spell.Mechanic = row.mechanic;
            spell.Attributes = row.attributes;
            spell.AttributesEx = row.attributesEx;
            spell.AttributesExB = row.attributesExB;
            spell.AttributesExC = row.attributesExC;
            spell.AttributesExD = row.attributesExD;
            spell.AttributesExE = row.attributesExE;
            spell.AttributesExF = row.attributesExF;
            spell.AttributesExG = row.attributesExG;
            spell.ShapeshiftMask = {row.shapeshiftMask1, row.shapeshiftMask2};
            spell.ShapeshiftExclude = {row.shapeshiftExclude1, row.shapeshiftExclude2};
            spell.Targets = row.targets;
            spell.TargetCreatureType = row.targetCreatureType;
            spell.RequiresSpellFocus = row.requiresSpellFocus;
            spell.FacingCasterFlags = row.facingCasterFlags;
            spell.CasterAuraState = row.casterAuraState;
            spell.TargetAuraState = row.targetAuraState;
            spell.ExcludeCasterAuraState = row.excludeCasterAuraState;
            spell.ExcludeTargetAuraState = row.excludeTargetAuraState;
            spell.CasterAuraSpell = row.casterAuraSpell;
            spell.TargetAuraSpell = row.targetAuraSpell;
            spell.ExcludeCasterAuraSpell = row.excludeCasterAuraSpell;
            spell.ExcludeTargetAuraSpell = row.excludeTargetAuraSpell;
            spell.CastingTimeIndex = row.castingTimeIndex;
            spell.RecoveryTime = row.recoveryTime;
            spell.CategoryRecoveryTime = row.categoryRecoveryTime;
            spell.InterruptFlags = row.interruptFlags;
            spell.AuraInterruptFlags = row.auraInterruptFlags;
            spell.ChannelInterruptFlags = row.channelInterruptFlags;
            spell.ProcTypeMask = row.procTypeMask;
            spell.ProcChance = row.procChance;
            spell.ProcCharges = row.procCharges;
            spell.MaxLevel = row.maxLevel;
            spell.BaseLevel = row.baseLevel;
            spell.SpellLevel = row.spellLevel;
            spell.DurationIndex = row.durationIndex;
            spell.PowerType = row.powerType;
            spell.ManaCost = row.manaCost;
            spell.ManaCostPerLevel = row.manaCostPerLevel;
            spell.ManaPerSecond = row.manaPerSecond;
            spell.ManaPerSecondPerLevel = row.manaPerSecondPerLevel;
            spell.RangeIndex = row.rangeIndex;
            spell.Speed = row.speed;
            spell.CumulativeAura = row.cumulativeAura;
            spell.Totem = {row.totem1, row.totem2};
            spell.Reagent = {row.reagent1, row.reagent2, row.reagent3, row.reagent4, row.reagent5, row.reagent6, row.reagent7, row.reagent8};
            spell.ReagentCount = {row.reagentCount1, row.reagentCount2, row.reagentCount3, row.reagentCount4, row.reagentCount5, row.reagentCount6, row.reagentCount7, row.reagentCount8};
            spell.EquippedItemClass = row.equippedItemClass;
            spell.EquippedItemSubclass = row.equippedItemSubclass;
            spell.EquippedItemInvTypes = row.equippedItemInvTypes;
            spell.Effect = {row.effect1, row.effect2, row.effect3};
            spell.EffectDieSides = {row.effectDieSides1, row.effectDieSides2, row.effectDieSides3};
            spell.EffectRealPointsPerLevel = {row.effectRealPointsPerLevel1, row.effectRealPointsPerLevel2, row.effectRealPointsPerLevel3};
            spell.EffectBasePoints = {row.effectBasePoints1, row.effectBasePoints2, row.effectBasePoints3};
            spell.EffectMechanic = {row.effectMechanic1, row.effectMechanic2, row.effectMechanic3};
            spell.EffectImplicitTargetA = {row.implicitTargetA1, row.implicitTargetA2, row.implicitTargetA3};
            spell.EffectImplicitTargetB = {row.implicitTargetB1, row.implicitTargetB2, row.implicitTargetB3};
            spell.EffectRadiusIndex = {row.effectRadiusIndex1, row.effectRadiusIndex2, row.effectRadiusIndex3};
            spell.EffectAura = {row.effectAura1, row.effectAura2, row.effectAura3};
            spell.EffectAuraPeriod = {row.effectAuraPeriod1, row.effectAuraPeriod2, row.effectAuraPeriod3};
            spell.EffectAmplitude = {row.effectAmplitude1, row.effectAmplitude2, row.effectAmplitude3};
            spell.EffectChainTargets = {row.effectChainTargets1, row.effectChainTargets2, row.effectChainTargets3};
            spell.EffectItemType = {row.effectItemType1, row.effectItemType2, row.effectItemType3};
            spell.EffectMiscValue = {row.effectMiscValue1, row.effectMiscValue2, row.effectMiscValue3};
            spell.EffectMiscValueB = {row.effectMiscValueB1, row.effectMiscValueB2, row.effectMiscValueB3};
            spell.EffectTriggerSpell = {row.effectTriggerSpell1, row.effectTriggerSpell2, row.effectTriggerSpell3};
            spell.EffectPointsPerCombo = {row.effectPointsPerCombo1, row.effectPointsPerCombo2, row.effectPointsPerCombo3};
            spell.EffectSpellClassMask = {
                    flag96(row.effectSpellClassMaskA1, row.effectSpellClassMaskB1, row.effectSpellClassMaskC1),
                    flag96(row.effectSpellClassMaskA2, row.effectSpellClassMaskB2, row.effectSpellClassMaskC2),
                    flag96(row.effectSpellClassMaskA3, row.effectSpellClassMaskB3, row.effectSpellClassMaskC3)
            };
            spell.SpellVisualID = {row.spellVisualID1, row.spellVisualID2};
            spell.SpellIconID = row.spellIconID;
            spell.ActiveIconID = row.activeIconID;
            spell.SpellPriority = row.spellPriority;
            // Заполнение локализованных имен
            if (row.nameLangEnUS) spell.Name[LOCALE_enUS] = *row.nameLangEnUS;
            if (row.nameLangEnGB) spell.Name[LOCALE_enGB] = *row.nameLangEnGB;
            if (row.nameLangKoKR) spell.Name[LOCALE_koKR] = *row.nameLangKoKR;
            if (row.nameLangFrFR) spell.Name[LOCALE_frFR] = *row.nameLangFrFR;
            if (row.nameLangDeDE) spell.Name[LOCALE_deDE] = *row.nameLangDeDE;
            if (row.nameLangEnCN) spell.Name[LOCALE_enCN] = *row.nameLangEnCN;
            if (row.nameLangZhCN) spell.Name[LOCALE_zhCN] = *row.nameLangZhCN;
            if (row.nameLangEnTW) spell.Name[LOCALE_enTW] = *row.nameLangEnTW;
            if (row.nameLangZhTW) spell.Name[LOCALE_zhTW] = *row.nameLangZhTW;
            if (row.nameLangEsES) spell.Name[LOCALE_esES] = *row.nameLangEsES;
            if (row.nameLangEsMX) spell.Name[LOCALE_esMX] = *row.nameLangEsMX;
            if (row.nameLangRuRU) spell.Name[LOCALE_ruRU] = *row.nameLangRuRU;
            if (row.nameLangPtPT) spell.Name[LOCALE_ptPT] = *row.nameLangPtPT;
            if (row.nameLangPtBR) spell.Name[LOCALE_ptBR] = *row.nameLangPtBR;
            if (row.nameLangItIT) spell.Name[LOCALE_itIT] = *row.nameLangItIT;

            spell.ManaCostPct = row.manaCostPct;
            spell.StartRecoveryCategory = row.startRecoveryCategory;
            spell.StartRecoveryTime = row.startRecoveryTime;
            spell.MaxTargetLevel = row.maxTargetLevel;
            spell.SpellClassSet = row.spellClassSet;
            spell.SpellClassMask = flag96(row.spellClassMask1, row.spellClassMask2, row.spellClassMask3);
            spell.MaxTargets = row.maxTargets;
            spell.DefenseType = row.defenseType;
            spell.PreventionType = row.preventionType;
            spell.EffectChainAmplitude = {row.effectChainAmplitude1, row.effectChainAmplitude2, row.effectChainAmplitude3};
            spell.RequiredTotemCategoryID = {row.requiredTotemCategoryId1, row.requiredTotemCategoryId2};
            spell.RequiredAreasID = row.requiredAreasId;
            spell.SchoolMask = row.schoolMask;
            spell.RuneCostID = row.runeCostId;
            spell.EffectBonusCoefficient = {row.effectBonusCoefficient1, row.effectBonusCoefficient2, row.effectBonusCoefficient3};

            _spellMap[row.id] = spell;

            if (!_spellHighestID || _spellHighestID < row.id)
                _spellHighestID = row.id;
        }
        log->info(">>> DBCMgr: loaded {} Spells in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_Spells failed: {}", ex.what());
    }
}

void DBCMgr::load_SpellCastTimes() {
    auto log = Logger::get();
    _spellCastTimesMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELLCASTTIMES");
        auto rows = server_->db()->execute_sync_many<DbcSpellCastTimes>(stmt);
        for (const auto &row: rows) {
            SpellCastTimesDBC sct;
            sct.ID = row.id;
            sct.Base = row.base;

            _spellCastTimesMap[row.id] = sct;
        }
        log->info(">>> DBCMgr: loaded {} SpellCastTimes in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SpellCastTimes failed: {}", ex.what());
    }
}

void DBCMgr::load_SpellCategory() {
    auto log = Logger::get();
    _spellCategoryMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELLCATEGORY");
        auto rows = server_->db()->execute_sync_many<DbcSpellCategory>(stmt);
        for (const auto &row: rows) {
            SpellCategoryDBC sc;
            sc.ID = row.id;
            sc.Flags = row.flags;

            _spellCategoryMap[row.id] = sc;
        }
        log->info(">>> DBCMgr: loaded {} SpellCategory in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SpellCategory failed: {}", ex.what());
    }
}

void DBCMgr::load_SpellDifficulty() {
    auto log = Logger::get();
    _spellDiffucultyMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELLDIFFICULTY");
        auto rows = server_->db()->execute_sync_many<DbcSpellDifficulty>(stmt);
        for (const auto &row: rows) {
            SpellDifficultyDBC sd;
            sd.ID = row.id;
            sd.DifficultySpellID[0] = row.difficulty_spell_id_1;
            sd.DifficultySpellID[1] = row.difficulty_spell_id_2;
            sd.DifficultySpellID[2] = row.difficulty_spell_id_3;
            sd.DifficultySpellID[3] = row.difficulty_spell_id_4;

            _spellDiffucultyMap[row.id] = sd;
        }
        log->info(">>> DBCMgr: loaded {} SpellDifficulty in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SpellDifficulty failed: {}", ex.what());
    }
}

void DBCMgr::load_SpellDuration() {
    auto log = Logger::get();
    _spellDurationMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELLDURATION");
        auto rows = server_->db()->execute_sync_many<DbcSpellDuration>(stmt);
        for (const auto &row: rows) {
            SpellDurationDBC sd;
            sd.ID = row.id;
            sd.Duration         = row.duration;
            sd.DurationPerLevel = row.duration_per_level;
            sd.MaxDuration      = row.max_duration;

            _spellDurationMap[row.id] = sd;
        }
        log->info(">>> DBCMgr: loaded {} SpellDuration in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SpellDuration failed: {}", ex.what());
    }
}

void DBCMgr::load_SpellFocusObject() {
    auto log = Logger::get();
    _spellFocusObjectMap.clear();
    uint32_t oldMSTime = getMSTime();

    try {
        auto stmt = PreparedStatement("SELECT_DBC_SPELLFOCUSOBJECT");
        auto rows = server_->db()->execute_sync_many<DbcSpellFocusObject>(stmt);
        for (const auto &row: rows) {
            SpellFocusObjectDBC sfo;
            sfo.ID = row.id;

            _spellFocusObjectMap[row.id] = sfo;
        }
        log->info(">>> DBCMgr: loaded {} SpellFocusObject in {} ms",
                  rows.size(), GetMSTimeDiffToNow(oldMSTime));
    } catch (const std::exception &ex) {
        log->error("DBCMgr::load_SpellFocusObject failed: {}", ex.what());
    }
}

void DBCMgr::initialize_Additional_Data() {
    handle_CharacterFacialHairStylesByTripple();
    handle_CharSectionsByPenta();
    handle_CharStartOutfitByTripple();
    handle_EmotesTextSoundByTripple();
    handle_LFGDungeonDBCByDouble();
    handle_MapDifficultyByDouble();
    handle_NamesProfanityRegex();
    handle_NamesReservedRegex();
    handle_SkillRaceClassInfo();
}

void DBCMgr::handle_CharacterFacialHairStylesByTripple() {
    for (const auto& cfhsID : _characterFacialHairStyleMap)
    {
        if (CharacterFacialHairStylesDBC const* entry = &cfhsID.second)
            _characterFacialHairStylesByTripple[CharacterFacialHairStylesKey(entry->RaceID, entry->SexID, entry->VariationID)] = entry;
    }
}

void DBCMgr::handle_CharSectionsByPenta() {
    for (const auto& csID : _charSectionMap)
    {
        if (CharSectionsDBC const* entry = &csID.second)
            _charSectionsByPenta[CharSectionsKey(entry->RaceID, CharSectionType(entry->BaseSection), entry->SexID, entry->VariationIndex, entry->ColorIndex)] = entry;
    }
}

void DBCMgr::handle_CharStartOutfitByTripple() {
    for (const auto& csoID : _charStartOutfitMap)
    {
        if (CharStartOutfitDBC const* entry = &csoID.second)
            _charStartOutfitByTripple[CharStartOutfitKey(entry->RaceID, entry->ClassID, entry->SexID)] = entry;
    }
}

void DBCMgr::handle_EmotesTextSoundByTripple() {
    for (const auto& etsID : _emotesTextSoundMap)
    {
        if (EmotesTextSoundDBC const* entry = &etsID.second)
            _emotesTextSoundByTripple[EmotesTextSoundKey(entry->EmotesTextID, entry->RaceID, entry->SexID)] = entry;
    }
}

void DBCMgr::handle_LFGDungeonDBCByDouble() {
    for (const auto& lfgdID : _lfgDungeonMap)
    {
        if (LFGDungeonDBC const* entry = &lfgdID.second)
            _lfgDungeonByDouble[LFGDungeonKey(entry->MapID, Difficulty(entry->Difficulty))] = entry;
    }
}
void DBCMgr::handle_MapDifficultyByDouble() {
    for (const auto& mpID : _mapDifficultyMap)
    {
        if (MapDifficultyDBC const* entry = &mpID.second)
            _mapDifficultyByDouble[MapDifficultyKey(entry->MapID, Difficulty(entry->Difficulty))] = entry;
    }
}

void DBCMgr::handle_NamesProfanityRegex() {
    // Separate namesprofanity for languages
    for (NamesProfanityDBCMap::const_iterator itr = _namesProfanityMap.begin(); itr != _namesProfanityMap.end(); ++itr)
    {
        std::wstring wname;
        bool conversionResult = UTF8Utils::Utf8toWStr(itr->second.Name, wname);
        if (!conversionResult) {
            Logger::get()->error("DBCMgr::handle_NamesProfanityRegex failed for ID: {}", itr->second.ID);
            continue;
        }

        if (itr->second.Language != -1)
            _namesProfaneValidators[itr->second.Language].emplace_back(wname, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
        else
            for (uint32_t i = 0; i < TOTAL_LOCALES; ++i)
                _namesProfaneValidators[i].emplace_back(wname, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
    }

    // clear this DBC container (UNUSED in life-cycle server)
    _namesProfanityMap.clear();
}

void DBCMgr::handle_NamesReservedRegex() {
    // Separate namesreserved for languages
    for (NamesReservedDBCMap::const_iterator itr = _namesReservedMap.begin(); itr != _namesReservedMap.end(); ++itr)
    {
        std::wstring wname;
        bool conversionResult = UTF8Utils::Utf8toWStr(itr->second.Name, wname);
        if (!conversionResult) {
            Logger::get()->error("DBCMgr::handle_NamesReservedRegex failed for ID: {}", itr->second.ID);
            continue;
        }

        if (itr->second.Language != -1)
            _namesReservedValidators[itr->second.Language].emplace_back(wname, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
        else
            for (uint32_t i = 0; i < TOTAL_LOCALES; ++i)
                _namesReservedValidators[i].emplace_back(wname, boost::regex::perl | boost::regex::icase | boost::regex::optimize);
    }

    // clear this DBC container (UNUSED in life-cycle server)
    _namesReservedMap.clear();
}

void DBCMgr::handle_SkillRaceClassInfo() {
    for (const auto& srciID : _skillRaceClassInfoMap)
    {
        if (SkillRaceClassInfoDBC const* entry = &srciID.second)
            if (getSkillLineDBC(entry->SkillID))
                _skillRaceClassInfoBySkill.emplace(entry->SkillID, entry);
    }
}