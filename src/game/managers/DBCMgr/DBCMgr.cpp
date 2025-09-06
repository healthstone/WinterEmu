#include "DBCMgr.hpp"
#include "Time/Timer.hpp"
#include "Logger.hpp"
#include "BaseServer.hpp"

DBCMgr::~DBCMgr() {
    cleanUpBeforeDelete();
}

void DBCMgr::cleanUpBeforeDelete() {
    // Сначала чистим мультимапы и вторичные контейнеры
    _characterFacialHairStylesByTripple.clear();
    _charSectionsByPenta.clear();
    _charStartOutfitByTripple.clear();
    _emotesTextSoundByTripple.clear();
    _skillRaceClassInfoBySkill.clear();

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
    _skillRaceClassInfoMap.clear();
    _skillLineMap.clear();

    _bannedAddonsHighestID = 0;
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
    load_SkillRaceClassInfo();
    load_SkillLine();

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

void DBCMgr::initialize_Additional_Data() {
    handle_CharacterFacialHairStylesByTripple();
    handle_CharSectionsByPenta();
    handle_CharStartOutfitByTripple();
    handle_EmotesTextSoundByTripple();
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

void DBCMgr::handle_SkillRaceClassInfo() {
    for (const auto& srciID : _skillRaceClassInfoMap)
    {
        if (SkillRaceClassInfoDBC const* entry = &srciID.second)
            if (getSkillLineDBC(entry->SkillID))
                _skillRaceClassInfoBySkill.emplace(entry->SkillID, entry);
    }
}