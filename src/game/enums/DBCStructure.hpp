#pragma once

#include "DBCEnums.hpp"
#include "LocaleConstant.hpp"
#include "utils/flag96.hpp"

#include <cstdint>

// Structures used to access raw DBC data require packing for portability
#pragma pack(push, 1)

// load Achievement.dbc
struct AchievementDBC {
    uint32_t ID;                                              // 0
    int32_t Faction;                                          // 1 -1=all, 0=horde, 1=alliance
    int32_t InstanceID;                                       // 2 -1=none
    //uint32_t Supercedes;                                    // 3 its Achievement parent (can`t start while parent uncomplete, use its Criteria if don`t have own, use its progress on begin)
    std::string Title[TOTAL_LOCALES];                         // 12
    //uint32_t Title_lang_mask;                               // 20
    //char const* Description[16];                            // 21-36
    //uint32_t Description_lang_mask;                         // 37
    uint32_t Category;                                        // 38
    uint32_t Points;                                          // 39 reward points
    //uint32_t UiOrder;                                       // 40 icon (from SpellIcon.dbc)
    uint32_t Flags;                                           // 41
    //uint32_t IconID;                                        // 42
    //char const* Reward[16];                                 // 43-58
    //uint32_t Reward_lang_mask;                              // 59
    uint32_t MinimumCriteria;                                 // 60 - need this count of completed criterias (own or referenced achievement criterias)
    uint32_t SharesCriteria;                                  // 61 - referenced achievement (counting of all completed criterias)
};

// load Achievement_Criteria.dbc
struct AchievementCriteriaDBC {
    uint32_t ID;                                              // 0
    uint32_t AchievementID;                                   // 1
    uint32_t Type;                                            // 2
    union {
        uint32_t ID;
        // ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE          = 0
        // ACHIEVEMENT_CRITERIA_TYPE_KILLED_BY_CREATURE     = 20
        uint32_t CreatureID;

        // ACHIEVEMENT_CRITERIA_TYPE_WIN_BG                 = 1
        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_BATTLEGROUND  = 15
        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_AT_MAP           = 16
        // ACHIEVEMENT_CRITERIA_TYPE_WIN_ARENA              = 32
        // ACHIEVEMENT_CRITERIA_TYPE_PLAY_ARENA             = 33
        uint32_t MapID;

        // ACHIEVEMENT_CRITERIA_TYPE_REACH_SKILL_LEVEL      = 7
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LEVEL      = 40
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILLLINE_SPELLS = 75
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SKILL_LINE       = 112
        uint32_t SkillID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_ACHIEVEMENT   = 8
        uint32_t AchievementID;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUESTS_IN_ZONE = 11
        uint32_t ZoneID;

        // ACHIEVEMENT_CRITERIA_TYPE_CURRENCY = 12
        uint32_t CurrencyID;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATH_IN_DUNGEON       = 18
        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_RAID          = 19
        uint32_t GroupSize;

        // ACHIEVEMENT_CRITERIA_TYPE_DEATHS_FROM            = 26
        uint32_t DamageType;

        // ACHIEVEMENT_CRITERIA_TYPE_COMPLETE_QUEST         = 27
        uint32_t QuestID;

        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET        = 28
        // ACHIEVEMENT_CRITERIA_TYPE_BE_SPELL_TARGET2       = 69
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL             = 29
        // ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL2            = 110
        // ACHIEVEMENT_CRITERIA_TYPE_LEARN_SPELL            = 34
        uint32_t SpellID;

        // ACHIEVEMENT_CRITERIA_TYPE_BG_OBJECTIVE_CAPTURE
        uint32_t ObjectiveId;

        // ACHIEVEMENT_CRITERIA_TYPE_HONORABLE_KILL_AT_AREA = 31
        uint32_t AreaID;

        // ACHIEVEMENT_CRITERIA_TYPE_OWN_ITEM               = 36
        // ACHIEVEMENT_CRITERIA_TYPE_USE_ITEM               = 41
        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_ITEM              = 42
        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_ITEM             = 57
        uint32_t ItemID;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_TEAM_RATING    = 38
        // ACHIEVEMENT_CRITERIA_TYPE_REACH_TEAM_RATING      = 39
        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_PERSONAL_RATING = 39
        uint32_t TeamType;

        // ACHIEVEMENT_CRITERIA_TYPE_EXPLORE_AREA           = 43
        uint32_t WorldMapOverlayID;

        // ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION        = 46
        uint32_t FactionID;

        // ACHIEVEMENT_CRITERIA_TYPE_EQUIP_EPIC_ITEM        = 49
        uint32_t ItemSlot;

        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT      = 50
        // ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT     = 51
        uint32_t RollValue;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_CLASS               = 52
        uint32_t ClassID;

        // ACHIEVEMENT_CRITERIA_TYPE_HK_RACE                = 53
        uint32_t RaceID;

        // ACHIEVEMENT_CRITERIA_TYPE_DO_EMOTE               = 54
        uint32_t EmoteID;

        // ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT         = 68
        // ACHIEVEMENT_CRITERIA_TYPE_FISH_IN_GAMEOBJECT     = 72
        uint32_t GameObjectID;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_POWER          = 96
        uint32_t PowerType;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_STAT           = 97
        uint32_t StatType;

        // ACHIEVEMENT_CRITERIA_TYPE_HIGHEST_SPELLPOWER     = 98
        uint32_t SpellSchool;

        // ACHIEVEMENT_CRITERIA_TYPE_LOOT_TYPE              = 109
        uint32_t LootType;
    } Asset;                                                // 3

    uint32_t Quantity;                                        // 4

    struct {
        uint32_t Type;
        uint32_t Asset;
    } AdditionalRequirements[MAX_CRITERIA_REQUIREMENTS];    // 5-8

    std::string name[TOTAL_LOCALES];                          // 9-17
    //uint32_t Description_lang_mask;                         // 25
    uint32_t Flags;                                           // 26
    uint32_t StartEvent;                                      // 27 AchievementCriteriaTimedTypes
    uint32_t StartAsset;                                      // 28 Always appears with timed events, used internally to start the achievement, store
    uint32_t StartTimer;                                      // 29 time limit in seconds
    //uint32_t UiOrder;                                       // 30
};

// load AreaTable.dbc
struct AreaTableDBC {
    uint32_t ID;                                              // 0
    uint32_t ContinentID;                                     // 1
    uint32_t ParentAreaID;                                    // 2 if 0 then it's zone, else it's zone id of this area
    uint32_t AreaBit;                                         // 3
    uint32_t Flags;                                           // 4
    //uint32_t SoundProviderPref;                             // 5
    //uint32_t SoundProviderPrefUnderwater;                   // 6
    //uint32_t AmbienceID;                                    // 7
    //uint32_t ZoneMusic;                                     // 8
    //uint32_t IntroSound;                                    // 9
    int32_t ExplorationLevel;                                 // 10
    std::string AreaName[TOTAL_LOCALES];                      // 11-19
    //uint32_t AreaName_lang_mask;                            // 27
    uint32_t FactionGroupMask;                                // 28
    uint32_t LiquidTypeID[4];                                 // 29-32 liquid override by type
    //float MinElevation;                                     // 33
    //float AmbientMultiplier;                                // 34
    //uint32_t LightID;                                       // 35

    // helpers
    bool IsSanctuary() const {
        if (ContinentID == 609)
            return true;
        return (Flags & AREA_FLAG_SANCTUARY) != 0;
    }

    bool IsFlyable() const {
        if (Flags & AREA_FLAG_OUTLAND) {
            if (!(Flags & AREA_FLAG_NO_FLY_ZONE))
                return true;
        }

        return false;
    }
};

// load AreaGroup.dbc
#define MAX_GROUP_AREA_IDS 6

struct AreaGroupDBC {
    uint32_t ID;                                              // 0
    uint32_t AreaID[MAX_GROUP_AREA_IDS];                      // 1-6
    uint32_t NextAreaID;                                      // 7 index of next group
};

// load AreaPOI.dbc (NOT USED)
struct AreaPOIDBC {
    uint32_t ID;                                              // 0
    uint32_t Importance;                                      // 1
    uint32_t Icon[9];                                         // 2-10
    uint32_t FactionID;                                       // 11
    DBCPosition3D Pos;                                        // 12-14
    uint32_t ContinentID;                                     // 15
    //uint32_t Flags;                                         // 16
    uint32_t AreaID;                                          // 17
    //char const* Name[16];                                   // 18-33
    //uint32_t Name_lang_mask;                                // 34
    //char const* Description[16];                            // 35-50
    //uint32_t Description_lang_mask;                         // 51
    uint32_t WorldStateID;                                    // 52
    //uint32_t WorldMapLink;                                  // 53
};

// load AreaTrigger.dbc
struct AreaTriggerDBC {
    uint32_t ID;                                            // 0
    uint32_t ContinentID;                                   // 1
    DBCPosition3D Pos;                                      // 2-4
    float Radius;                                           // 5
    float BoxLength;                                        // 6
    float BoxWidth;                                         // 7
    float BoxHeight;                                        // 8
    float BoxYaw;                                           // 9
};

// load AuctionHouse.dbc
struct AuctionHouseDBC {
    uint32_t ID;                                              // 0
    uint32_t FactionID;                                       // 1 id of faction.dbc for player factions associated with city
    uint32_t DepositRate;                                     // 2 1/3 from real
    uint32_t ConsignmentRate;                                 // 3
    //char const* Name[16];                                   // 4-19
    //uint32_t Name_lang_mask;                                // 20
};

// load BankBagSlotPrices.dbc
struct BankBagSlotPricesDBC {
    uint32_t ID;                                              // 0
    uint32_t Cost;                                            // 1
};

// load BannedAddOns.dbc
struct BannedAddOnsDBC {
    uint32_t ID;                                              // 0
    //uint32_t NameMD5[4];                                    // 1-4
    //uint32_t VersionMD5[4];                                 // 5-8
    //uint32_t LastModified;                                  // 9
    //uint32_t Flags;                                         // 10
};

// load BarberShopStyle.dbc
struct BarberShopStyleDBC {
    uint32_t ID;                                              // 0
    uint32_t Type;                                            // 1 value 0 -> hair, value 2 -> facialhair
    //char const* DisplayName[16];                            // 2-17
    //uint32_t DisplayName_lang_mask;                         // 18
    //uint32_t Description[16];                               // 19-34, all empty
    //uint32_t Description_lang_mask;                         // 35
    //float CostModifier;                                     // 36
    uint8_t Race;                                             // 37
    uint8_t Sex;                                              // 38
    uint32_t Data;                                            // 39 real ID to hair/facial hair
};

// load BattlemasterList.dbc
struct BattlemasterListDBC {
    uint32_t ID;                                              // 0
    int32_t MapID[8];                                         // 1-8
    uint32_t InstanceType;                                    // 9 map type (3 - BG, 4 - arena)
    //uint32_t GroupsAllowed;                                 // 10
    std::string Name[TOTAL_LOCALES];                          // 11-26
    //uint32_t Name_lang_mask;                                // 27
    uint32_t MaxGroupSize;                                    // 28 used for checking if queue as group
    uint32_t HolidayWorldState;                               // 29
    //uint32_t MinLevel;                                      // 30
    //uint32_t MaxLevel;                                      // 31
};

// load CharacterFacialHairStyles.dbc
struct CharacterFacialHairStylesDBC {
    uint32_t ID;                                             // 0
    uint8_t RaceID;                                          // 1
    uint8_t SexID;                                           // 2
    uint8_t VariationID;                                     // 3
    //uint32_t Geoset[5];                                    // 4-8
};

// load CharSections.dbc
enum CharSectionFlags {
    SECTION_FLAG_PLAYER = 0x01,
    SECTION_FLAG_DEATH_KNIGHT = 0x04
};

enum CharSectionType : uint8_t {
    SECTION_TYPE_SKIN = 0,
    SECTION_TYPE_FACE = 1,
    SECTION_TYPE_FACIAL_HAIR = 2,
    SECTION_TYPE_HAIR = 3,
    SECTION_TYPE_UNDERWEAR = 4
};

struct CharSectionsDBC {
    uint32_t ID;                                             // 0
    uint8_t RaceID;                                          // 1
    uint8_t SexID;                                           // 2
    uint8_t BaseSection;                                     // 3
    //char const* TextureName[3];                            // 4-6
    uint8_t Flags;                                           // 7
    uint8_t VariationIndex;                                  // 8
    uint8_t ColorIndex;                                      // 9

    inline bool HasFlag(CharSectionFlags flag) const { return (Flags & flag) != 0; }
};

// load CharStartOutfit.dbc
#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitDBC {
    uint32_t ID;                                              // 0
    uint8_t RaceID;                                           // 1
    uint8_t ClassID;                                          // 2
    uint8_t SexID;                                            // 3
    //uint8_t OutfitID;                                       // 4
    int32_t ItemID[MAX_OUTFIT_ITEMS];                         // 5-28
    //int32_t DisplayItemID[MAX_OUTFIT_ITEMS];                // 29-52 not required at server side
    //int32_t InventoryType[MAX_OUTFIT_ITEMS];                // 53-76 not required at server side
};

// load CharTitles.dbc
struct CharTitlesDBC {
    uint32_t ID;                                              // 0, title ids, for example in Quest::GetCharTitleId()
    //uint32_t ConditionID;                                   // 1
    std::string Name[TOTAL_LOCALES];                          // 2-17 male
    //uint32_t Name_lang_mask;                                // 18
    std::string Name1[TOTAL_LOCALES];                         // 19-34 female
    //uint32_t Name1_lang_mask;                               // 35
    uint32_t MaskID;                                          // 36 used in PLAYER_CHOSEN_TITLE and 1<<index in PLAYER__FIELD_KNOWN_TITLES
};

// load ChatChannels.dbc
struct ChatChannelsDBC {
    uint32_t ID;                                              // 0
    uint32_t Flags;                                           // 1
    //uint32_t FactionGroup;                                  // 2
    std::string Name[TOTAL_LOCALES];                          // 3-18
    //uint32_t Name_lang_mask;                                // 19
    //std::string Shortcut[TOTAL_LOCALES];                    // 20-35
    //uint32_t Shortcut_lang_mask;                            // 36
};

// load ChrClasses.dbc
struct ChrClassesDBC {
    uint32_t ID;                                              // 0
    //uint32_t DamageBonusStat;                               // 1
    uint32_t DisplayPower;                                    // 2
    //char const* PetNameToken;                               // 3
    std::string Name[TOTAL_LOCALES];                          // 4-19
    //uint32_t Name_lang_mask;                                // 20
    //char const* NameFemale[16];                             // 21-36
    //uint32_t NameFemale_lang_mask;                          // 37
    //char const* NameMale[16];                               // 38-53
    //uint32_t NameMale_lang_mask;                            // 54
    //char const* Filename;                                   // 55
    uint32_t SpellClassSet;                                   // 56
    //uint32_t Flags;                                         // 57
    uint32_t CinematicSequenceID;                             // 58 ID from CinematicSequences.dbc
    uint32_t RequiredExpansion;                               // 59
};

// load ChrRaces.dbc
enum ChrRacesAllianceType {
    CHRRACES_ALLIANCE_TYPE_ALLIANCE = 0,
    CHRRACES_ALLIANCE_TYPE_HORDE = 1,
    CHRRACES_ALLIANCE_TYPE_NOT_PLAYABLE = 2,
};

enum ChrRacesFlags {
    CHRRACES_FLAGS_NOT_PLAYABLE = 0x01,
    CHRRACES_FLAGS_BARE_FEET = 0x02,
    CHRRACES_FLAGS_CAN_MOUNT = 0x04
};

struct ChrRacesDBC {
    uint32_t ID;                                              // 0
    uint32_t Flags;                                           // 1
    uint32_t FactionID;                                       // 2
    //uint32_t ExplorationSoundID;                            // 3
    uint32_t MaleDisplayID;                                   // 4
    uint32_t FemaleDisplayID;                                 // 5
    //char const* ClientPrefix;                               // 6
    uint32_t BaseLanguage;                                    // 7 (7-Alliance 1-Horde)
    uint32_t CreatureType;                                    // 8
    uint32_t ResSicknessSpellID;                              // 9
    //uint32_t SplashSoundID;                                 // 10
    //char const* ClientFileString;                           // 11
    uint32_t CinematicSequenceID;                             // 12 ID from CinematicSequences.dbc
    uint32_t Alliance;                                        // 13
    std::string Name[TOTAL_LOCALES];                          // 14-29
    //uint32_t Name_lang_mask;                                // 30
    //char const* NameFemale[16];                             // 31-46
    //uint32_t NameFemale_lang_mask;                          // 47
    //char const* NameMale[16];                               // 48-63
    //uint32_t NameMale_lang_mask;                            // 64
    //char const* FacialHairCustomization[2];                 // 65-66
    //char const* HairCustomization;                          // 67
    uint32_t RequiredExpansion;                               // 68

    inline bool HasFlag(ChrRacesFlags flag) const { return (Flags & flag) != 0; }
};

// load CinematicCamera.dbc
struct CinematicCameraDBC {
    uint32_t ID;                                            // 0
    std::string Model;                                      // 1 Model filename (translate .mdx to .m2)
    uint32_t SoundID;                                       // 2 Sound ID (voiceover for cinematic)
    float OriginX;                                          // 3 Position X in map used for basis for M2 co-ordinates
    float OriginY;                                          // 4 Position Y in map used for basis for M2 co-ordinates
    float OriginZ;                                          // 5 Position Z in map used for basis for M2 co-ordinates
    float OriginFacing;                                     // 6 Orientation in map used for basis for M2 co-ordinates
};

// load CinematicSequences.dbc
struct CinematicSequencesDBC {
    uint32_t ID;                                              // 0
    //uint32_t SoundID;                                       // 1
    uint32_t Camera[8];                                       // 2-9 ID in CinematicCamera.dbc
};

// load CreatureDisplayInfo.dbc
struct CreatureDisplayInfoDBC {
    uint32_t ID;                                              // 0
    uint32_t ModelID;                                         // 1
    //uint32_t SoundID;                                       // 2
    uint32_t ExtendedDisplayInfoID;                           // 3
    float CreatureModelScale;                                 // 4
    //uint32_t CreatureModelAlpha;                            // 5
    //char const* TextureVariation[3];                        // 6-8
    //char const* PortraitTextureName;                        // 9
    //uint32_t SizeClass;                                     // 10
    //uint32_t BloodID;                                       // 11
    //uint32_t NPCSoundID;                                    // 12
    //uint32_t ParticleColorID;                               // 13
    //uint32_t CreatureGeosetData;                            // 14
    //uint32_t ObjectEffectPackageID;                         // 15
};

// load CreatureDisplayInfoExtra.dbc
struct CreatureDisplayInfoExtraDBC {
    uint32_t ID;                                              // 0
    uint32_t DisplayRaceID;                                   // 1
    uint8_t DisplaySexID;                                    // 2
    //uint32_t SkinID;                                        // 3
    //uint32_t FaceID;                                        // 4
    //uint32_t HairStyleID;                                   // 5
    //uint32_t HairColorID;                                   // 6
    //uint32_t FacialHairID;                                  // 7
    //uint32_t NPCItemDisplay[11];                            // 8-18
    //uint32_t Flags;                                         // 19
    //char const* BakeName;                                   // 20
};

// load CreatureFamily.dbc
struct CreatureFamilyDBC {
    uint32_t ID;                                              // 0
    float MinScale;                                           // 1
    uint32_t MinScaleLevel;                                   // 2
    float MaxScale;                                           // 3
    uint32_t MaxScaleLevel;                                   // 4
    uint32_t SkillLine[2];                                    // 5-6
    uint32_t PetFoodMask;                                     // 7
    int32_t PetTalentType;                                    // 8
    //int32_t CategoryEnumID;                                 // 9
    std::string Name[TOTAL_LOCALES];                          // 10-25
    //uint32_t Name_lang_mask;                                // 26
    //char const* IconFile;                                   // 27
};

// load CreatureModelData.dbc
enum CreatureModelDataFlags {
    CREATURE_MODEL_DATA_FLAGS_CAN_MOUNT = 0x00000080
};

struct CreatureModelDataDBC {
    uint32_t ID;                                              // 0
    uint32_t Flags;                                           // 1
    std::string ModelName;                                    // 2
    //uint32_t SizeClass;                                     // 3
    float ModelScale;                                         // 4 Used in calculation of unit collision data
    //int32_t BloodID;                                        // 5
    //int32_t FootprintTextureID;                             // 6
    //uint32_t FootprintTextureLength;                        // 7
    //uint32_t FootprintTextureWidth;                         // 8
    //float FootprintParticleScale;                           // 9
    //uint32_t FoleyMaterialID;                               // 10
    //float FootstepShakeSize;                                // 11
    //uint32_t DeathThudShakeSize;                            // 12
    //uint32_t SoundID;                                       // 13
    //float CollisionWidth;                                 // 14
    float CollisionHeight;                                  // 15
    float MountHeight;                                      // 16 Used in calculation of unit collision data when mounted
    //float GeoBoxMin[3];                                   // 17-19
    //float GeoBoxMax[3];                                   // 20-22
    //float WorldEffectScale;                               // 23
    //float AttachedEffectScale;                            // 24
    //float MissileCollisionRadius;                         // 25
    //float MissileCollisionPush;                           // 26
    //float MissileCollisionRaise;                          // 27

    inline bool HasFlag(CreatureModelDataFlags flag) const { return (Flags & flag) != 0; }
};

// load CreatureSpellData.dbc
#define MAX_CREATURE_SPELL_DATA_SLOT 4
struct CreatureSpellDataDBC {
    uint32_t ID;                                              // 0
    uint32_t Spells[MAX_CREATURE_SPELL_DATA_SLOT];            // 1-4
    //uint32_t Availability[MAX_CREATURE_SPELL_DATA_SLOT];    // 4-7
};

// load CreatureType.dbc
enum CreatureTypeEntryFlags {
    CREATURE_TYPE_ENTRY_FLAGS_IGNORED_TAB_TARGETING = 0x01 // Means do not include in tab targeting.
};

struct CreatureTypeDBC {
    uint32_t ID;                                              // 0
    //char const* Name[16];                                   // 1-16
    //uint32_t Name_lang_mask;                                // 17
    //uint32_t Flags;                                         // 18
};

// load CurrencyCategory.dbc (NOT USED)
struct CurrencyCategoryDBC {
    uint32_t ID;                                              // 0
    uint32_t Flags;                                           // 1
    std::string Name[TOTAL_LOCALES];                          // 2-17
    //uint32_t Name_lang_mask;                                // 18
};

// load CurrencyTypes.dbc
struct CurrencyTypesDBC {
    uint32_t ID;                                              // 0
    uint32_t ItemID;                                          // 1
    //uint32_t CategoryID;                                    // 2
    uint32_t BitIndex;                                        // 3 bit index in PLAYER_FIELD_KNOWN_CURRENCIES (1 << (index-1))
};

// load DestructibleModelData.dbc
struct DestructibleModelDataDBC {
    uint32_t ID;                                              // 0
    //uint32_t State0ImpactEffectDoodadSet;                   // 1
    //uint32_t State0AmbientDoodadSet;                        // 2
    uint32_t State1Wmo;                                       // 3
    //uint32_t State1DestructionDoodadSet;                    // 4
    //uint32_t State1ImpactEffectDoodadSet;                   // 5
    //uint32_t State1AmbientDoodadSet;                        // 6
    uint32_t State2Wmo;                                       // 7
    //uint32_t State2DestructionDoodadSet;                    // 8
    //uint32_t State2ImpactEffectDoodadSet;                   // 9
    //uint32_t State2AmbientDoodadSet;                        // 10
    uint32_t State3Wmo;                                       // 11
    //uint32_t State3InitDoodadSet;                           // 12
    //uint32_t State3AmbientDoodadSet;                        // 13
    //uint32_t EjectDirection;                                // 14
    uint32_t RepairGroundFx;                                  // 15
    //uint32_t DoNotHighlight;                                // 16
    //uint32_t HealEffect;                                    // 17
    //uint32_t HealEffectSpeed;                               // 18
};

// load DungeonEncounter.dbc
struct DungeonEncounterDBC {
    uint32_t ID;                                              // 0
    uint32_t MapID;                                           // 1
    uint32_t Difficulty;                                      // 2
    //uint32_t OrderIndex;                                    // 3
    uint32_t Bit;                                             // 4 encounter index for creating completed mask
    std::string Name[TOTAL_LOCALES];                          // 5-20
    //uint32_t Name_lang_mask;                                // 21
    //uint32_t SpellIconID;                                   // 22
};

// load DurabilityCosts.dbc
struct DurabilityCostsDBC {
    uint32_t ID;                                              // 0
    uint32_t WeaponSubClassCost[21];                          // 1-21
    uint32_t ArmorSubClassCost[8];                            // 22-29
};

// load DurabilityQuality.dbc
struct DurabilityQualityDBC {
    uint32_t ID;                                              // 0
    float Data;                                               // 1
};

// load Emotes.dbc
struct EmotesDBC {
    uint32_t ID;                                              // 0
    //char const* EmoteSlashCommand;                          // 1 internal name
    //uint32_t AnimID;                                        // 2 ref to animationData
    uint32_t EmoteFlags;                                      // 3 bitmask, may be unit_flags
    uint32_t EmoteSpecProc;                                   // 4 Can be 0, 1 or 2 (determine how emote are shown)
    uint32_t EmoteSpecProcParam;                              // 5 unconfirmed, may be enum UnitStandStateType
    //uint32_t EventSoundID;                                  // 6
};

// load EmotesText.dbc
struct EmotesTextDBC {
    uint32_t ID;                                              // 0
    //char const* Name;                                       // 1
    uint32_t EmoteID;                                         // 2
    //uint32_t EmoteText[16];                                 // 3-18
};

// load EmotesTextSound.dbc
struct EmotesTextSoundDBC {
    uint32_t ID;                                              // 0
    uint32_t EmotesTextID;                                    // 1
    uint8_t RaceID;                                           // 2
    uint8_t SexID;                                            // 3 0 male / 1 female
    uint32_t SoundID;                                         // 4
};

// load Faction.dbc
struct FactionDBC {
    uint32_t ID;                                              // 0
    int32_t ReputationIndex;                                  // 1
    uint32_t ReputationRaceMask[4];                           // 2-5
    uint32_t ReputationClassMask[4];                          // 6-9
    int32_t ReputationBase[4];                                // 10-13
    uint32_t ReputationFlags[4];                              // 14-17
    uint32_t ParentFactionID;                                 // 18
    float ParentFactionMod[2];                                // 19-20 [0] Faction gains incoming rep * spilloverRateIn
    //       [1] Faction outputs rep * spilloverRateOut as spillover reputation
    uint32_t ParentFactionCap[2];                             // 21-22 [0] The highest rank the faction will profit from incoming spillover
    //       [1] It does not seem to be the max standing at which a faction outputs spillover ...so no idea
    std::string Name[TOTAL_LOCALES];                          // 23-38
    //uint32_t Name_lang_mask;                                // 39
    //char const* Description[16];                            // 40-55
    //uint32_t Description_lang_mask;                         // 56

    // helpers
    bool CanHaveReputation() const {
        return ReputationIndex >= 0;
    }
};

// load FactionTemplate.dbc
#define MAX_FACTION_RELATIONS 4

struct FactionTemplateDBC {
    uint32_t ID;                                              // 0
    uint32_t Faction;                                         // 1
    uint32_t Flags;                                           // 2
    uint32_t FactionGroup;                                    // 3
    uint32_t FriendGroup;                                     // 4
    uint32_t EnemyGroup;                                      // 5
    uint32_t Enemies[MAX_FACTION_RELATIONS];                  // 6-9
    uint32_t Friend[MAX_FACTION_RELATIONS];                   // 10-13

    // helpers
    bool IsFriendlyTo(FactionTemplateDBC const &entry) const {
        if (entry.Faction) {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry.Faction)
                    return false;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friend[i] == entry.Faction)
                    return true;
        }
        return (FriendGroup & entry.FactionGroup) || (FactionGroup & entry.FriendGroup);
    }

    bool IsHostileTo(FactionTemplateDBC const &entry) const {
        if (entry.Faction) {
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Enemies[i] == entry.Faction)
                    return true;
            for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
                if (Friend[i] == entry.Faction)
                    return false;
        }
        return (EnemyGroup & entry.FactionGroup) != 0;
    }

    bool IsHostileToPlayers() const { return (EnemyGroup & FACTION_MASK_PLAYER) != 0; }

    bool IsNeutralToAll() const {
        for (int i = 0; i < MAX_FACTION_RELATIONS; ++i)
            if (Enemies[i] != 0)
                return false;
        return EnemyGroup == 0 && FriendGroup == 0;
    }

    bool IsContestedGuardFaction() const { return (Flags & FACTION_TEMPLATE_FLAG_CONTESTED_GUARD) != 0; }
};

// load GameObjectArtKit.dbc
struct GameObjectArtKitDBC {
    uint32_t ID;                                            // 0
    //char const* TextureVariation[3]                       // 1-3 m_textureVariations[3]
    //char const* AttachModel[4]                            // 4-8 m_attachModels[4]
};

// load GameObjectDisplayInfo.dbc
struct GameObjectDisplayInfoDBC {
    uint32_t ID;                                            // 0
    std::string ModelName;                                  // 1
    //uint32_t Sound[10];                                   // 2-11
    DBCPosition3D GeoBoxMin;                                // 12-14
    DBCPosition3D GeoBoxMax;                                // 15-17
    //uint32_t ObjectEffectPackageID;                       // 18
};

// load GemProperties.dbc
struct GemPropertiesDBC {
    uint32_t ID;                                              // 0
    uint32_t EnchantID;                                       // 1
    //uint32_t MaxCountInv;                                   // 2
    //uint32_t MaxCountItem;                                  // 3
    uint32_t Type;                                            // 4
};

// load GlyphProperties.dbc
struct GlyphPropertiesDBC {
    uint32_t ID;                                              // 0
    uint32_t SpellID;                                         // 1
    uint32_t GlyphSlotFlags;                                  // 2
    uint32_t SpellIconID;                                     // 3
};

// load GlyphSlot.dbc
struct GlyphSlotDBC {
    uint32_t ID;                                              // 0
    uint32_t Type;                                            // 1
    uint32_t Tooltip;                                         // 2
};

// All Gt* DBC store data for 100 levels, some by 100 per class/race
constexpr uint8_t GT_MAX_LEVEL = 100;
// gtOCTClassCombatRatingScalar.dbc stores data for 32 ratings, look at MAX_COMBAT_RATING for real used amount
constexpr uint8_t GT_MAX_RATING = 32;

// load gtBarberShopCostBase.dbc
struct GtBarberShopCostBaseDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtCombatRatings.dbc
struct GtCombatRatingsDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtChanceToMeleeCritBase.dbc
struct GtChanceToMeleeCritBaseDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtChanceToMeleeCrit.dbc
struct GtChanceToMeleeCritDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtChanceToSpellCritBase.dbc
struct GtChanceToSpellCritBaseDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtChanceToSpellCrit.dbc
struct GtChanceToSpellCritDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtNPCManaCostScaler.dbc
struct GtNPCManaCostScalerDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtOCTClassCombatRatingScalar.dbc
struct GtOCTClassCombatRatingScalarDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtOCTRegenHP.dbc
struct GtOCTRegenHPDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtOCTRegenMP.dbc (UNUSED)
struct GtOCTRegenMPDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtRegenHPPerSpt.dbc
struct GtRegenHPPerSptDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load gtRegenMPPerSpt.dbc
struct GtRegenMPPerSptDBC {
    uint32_t ID;                                            // 0
    float Data;                                             // 1
};

// load Holidays.dbc
#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 26
#define MAX_HOLIDAY_FLAGS 10

struct HolidaysDBC {
    uint32_t ID;                                              // 0
    uint32_t Duration[MAX_HOLIDAY_DURATIONS];                 // 1-10
    uint32_t Date[MAX_HOLIDAY_DATES];                         // 11-36 dates in unix time starting at January, 1, 2000
    uint32_t Region;                                          // 37 WoW region
    uint32_t Looping;                                         // 38
    uint32_t CalendarFlags[MAX_HOLIDAY_FLAGS];                // 39-48
    //uint32_t HolidayNameID;                                 // 49 HolidayNames.dbc
    //uint32_t HolidayDescriptionID;                          // 50 HolidayDescriptions.dbc
    std::string TextureFilename;                              // 51
    uint32_t Priority;                                        // 52
    int32_t CalendarFilterType;                               // 53
    //uint32_t Flags;                                         // 54 0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1
};

// load Item.dbc
struct ItemDBC {
    uint32_t ID;                                               // 0
    uint8_t ClassID;                                          // 1
    uint8_t SubclassID;                                       // 2
    int32_t SoundOverrideSubclassID;                           // 3
    int32_t Material;                                          // 4
    uint32_t DisplayInfoID;                                    // 5
    uint32_t InventoryType;                                    // 6
    uint32_t SheatheType;                                      // 7
};

// load ItemBagFamily.dbc
struct ItemBagFamilyDBC {
    uint32_t ID;                                              // 0
    //std::string Name[16];                                   // 1-16
    //uint32_t Name_lang_mask;                                // 17
};

// load ItemDisplayInfo.dbc
struct ItemDisplayInfoDBC {
    uint32_t ID;                                              // 0
    std::string ModelName[2];                                 // 1-2
    std::string ModelTexture[2];                              // 3-4
    std::string InventoryIcon[2];                             // 5-6
    uint32_t GeosetGroup[3];                                  // 7-9
    uint32_t Flags;                                           // 10
    uint32_t SpellVisualID;                                   // 11
    uint32_t GroupSoundIndex;                                 // 12
    uint32_t HelmetGeosetVisID[2];                            // 13-14
    std::string Texture[8];                                   // 15-22
    int32_t ItemVisual;                                       // 23
    uint32_t ParticleColorID;                                 // 24
};

// load ItemExtendedCost.dbc
#define MAX_ITEM_EXTENDED_COST_REQUIREMENTS 5

struct ItemExtendedCostDBC {
    uint32_t ID;                                              // 0
    uint32_t HonorPoints;                                     // 1 required honor points
    uint32_t ArenaPoints;                                     // 2 required arena points
    uint32_t ArenaBracket;                                    // 3 arena slot restrictions (min slot value)
    uint32_t ItemID[MAX_ITEM_EXTENDED_COST_REQUIREMENTS];     // 4-8 required item id
    uint32_t ItemCount[MAX_ITEM_EXTENDED_COST_REQUIREMENTS];  // 9-13 required count of item
    uint32_t RequiredArenaRating;                             // 14 required personal arena rating
    //uint32_t ItemPurchaseGroup;                             // 15
};

// load ItemLimitCategory.dbc
struct ItemLimitCategoryDBC {
    uint32_t ID;                                              // 0
    //char const* Name[16]                                    // 1-16
    //uint32_t Name_lang_mask;                                // 17
    uint32_t Quantity;                                        // 18 max allowed equipped as item or in gem slot
    uint32_t Flags;                                           // 19 enum ItemLimitCategoryMode
};

// load ItemRandomProperties.dbc
#define MAX_ITEMRANDOM_ENCHANTMENT_EFFECTS 5

struct ItemRandomPropertiesDBC {
    uint32_t ID;                                                          // 0
    //char const* InternalName;                                           // 1
    std::array<uint32_t, MAX_ITEMRANDOM_ENCHANTMENT_EFFECTS> Enchantment;       // 2-4
    //std::array<uint32_t, 2> UnusedEnchantment;                          // 5-6
    std::string Name[TOTAL_LOCALES];                                      // 7-22
    //uint32_t Name_lang_mask;                                            // 23
};

// load ItemRandomSuffix.dbc
struct ItemRandomSuffixDBC {
    uint32_t ID;                                                          // 0
    std::string Name[TOTAL_LOCALES];                                      // 1-16
    //uint32_t Name_lang_mask;                                            // 17
    //char const* InternalName;                                           // 18
    std::array<uint32_t, MAX_ITEMRANDOM_ENCHANTMENT_EFFECTS> Enchantment;       // 19-21
    //std::array<uint32_t, 2> UnusedEnchantment;                          // 22-23
    std::array<uint32_t, MAX_ITEMRANDOM_ENCHANTMENT_EFFECTS> AllocationPct;     // 24-26
    //std::array<uint32_t, 2> UnusedAllocationPct;                        // 27-28
};

// load ItemSet.dbc
#define MAX_ITEM_SET_ITEMS 10
#define MAX_ITEM_SET_SPELLS 8

struct ItemSetDBC {
    uint32_t ID;                                              // 0
    std::string Name[TOTAL_LOCALES];                          // 1-16
    //uint32_t Name_lang_mask;                                // 17
    uint32_t ItemID[MAX_ITEM_SET_ITEMS];                      // 18-27
    //uint32_t UnusedItemID[7];                               // 28-34
    uint32_t SetSpellID[MAX_ITEM_SET_SPELLS];                 // 35-42
    uint32_t SetThreshold[MAX_ITEM_SET_SPELLS];               // 43-50
    uint32_t RequiredSkill;                                   // 51
    uint32_t RequiredSkillRank;                               // 52
};

// load LFGDungeonExpansion.dbc
struct LFGDungeonExpansionDBC
{
    uint32_t ID;                                              // 0
    uint32_t LfgID;                                           // 1
    uint32_t ExpansionLevel;                                  // 2
    //uint32_t RandomID;                                      // 3
    uint32_t HardLevelMin;                                    // 4
    uint32_t HardLevelMax;                                    // 5
    //uint32_t TargetLevelMin;                                // 6
    //uint32_t TargetLevelMax;                                // 7
};

// load LFGDungeons.dbc
struct LFGDungeonDBC {
    uint32_t ID;                                              // 0
    std::string Name[TOTAL_LOCALES];                          // 1-16
    //uint32_t Name_lang_mask;                                // 17
    uint32_t MinLevel;                                        // 18
    uint32_t MaxLevel;                                        // 19
    uint32_t TargetLevel;                                     // 20
    uint32_t TargetLevelMin;                                  // 21
    uint32_t TargetLevelMax;                                  // 22
    int32_t MapID;                                            // 23
    uint32_t Difficulty;                                      // 24
    uint32_t Flags;                                           // 25
    uint32_t TypeID;                                          // 26
    //int32_t Faction;                                        // 27
    //char const* TextureFilename;                            // 28
    uint32_t ExpansionLevel;                                  // 29
    //uint32_t OrderIndex;                                    // 30
    uint32_t GroupID;                                         // 31
    //char const* Description[16];                            // 32-47
    //uint32_t Description_lang_mask;                         // 48

    // Helpers
    uint32_t Entry() const { return ID + (TypeID << 24); }
};

// load Light.dbc
struct LightDBC {
    uint32_t ID;                                              // 0
    uint32_t ContinentID;                                     // 1
    DBCPosition3D GameCoords;                                 // 2-4
    //float GameFalloffStart;                                 // 5
    //float GameFalloffEnd;                                   // 6
    //uint32_t LightParamsID[8];                              // 7-14
};

// load LiquidType.dbc
struct LiquidTypeDBC {
    uint32_t ID;                                              // 0
    //char const* Name;                                       // 1
    //uint32_t Flags;                                         // 2
    uint32_t Type;                                            // 3
    //uint32_t SoundID;                                       // 4
    uint32_t SpellID;                                         // 5
    //float MaxDarkenDepth;                                   // 6
    //float FogDarkenIntensity;                               // 7
    //float AmbDarkenIntensity;                               // 8
    //float DirDarkenIntensity;                               // 9
    //uint32_t LightID;                                       // 10
    //float ParticleScale;                                    // 11
    //uint32_t ParticleMovement;                              // 12
    //uint32_t ParticleTexSlots;                              // 13
    //uint32_t MaterialID;                                    // 14
    //char const* Texture[6];                                 // 15-20
    //uint32_t Color[2];                                      // 21-22
    //float Unk1[18];                                         // 23-40
    //uint32_t Unk2[4];                                       // 41-44
};

// load Lock.dbc
#define MAX_LOCK_CASE 8
struct LockDBC {
    uint32_t ID;                                              // 0
    uint32_t Type[MAX_LOCK_CASE];                             // 1-8
    uint32_t Index[MAX_LOCK_CASE];                            // 9-16
    uint32_t Skill[MAX_LOCK_CASE];                            // 17-24
    //uint32_t Action[MAX_LOCK_CASE];                         // 25-32
};

// load MailTemplate.dbc
struct MailTemplateDBC {
    uint32_t ID;                                              // 0
    std::string Subject[TOTAL_LOCALES];                       // 1-16
    //uint32_t Subject_lang_mask;                             // 17
    std::string Body[TOTAL_LOCALES];                          // 18-33
    //uint32_t Body_lang_mask;                                // 34
};

// load Map.dbc
struct MapDBC {
    uint32_t ID;                                              // 0
    //char const* Directory;                                  // 1
    uint32_t InstanceType;                                    // 2
    uint32_t Flags;                                           // 3
    //uint32_t MapType;                                       // 4 0 or 1 for battlegrounds (not arenas)
    std::string MapName[TOTAL_LOCALES];                       // 5-20
    //uint32_t MapName_lang_mask;                             // 21
    uint32_t AreaTableID;                                     // 22 common zone for instance and continent map
    //char const* MapDescription0[16];                        // 23-38 text for PvP Zones (Horde)
    //uint32_t MapDescription0_lang_mask;                     // 39
    //char const* MapDescription1[16];                        // 40-55 text for PvP Zones (Alliance)
    //uint32_t MapDescription1_lang_mask;                     // 56
    uint32_t LoadingScreenID;                                 // 57
    //float MinimapIconScale;                                 // 58
    int32_t CorpseMapID;                                      // 59 MapID of entrance map
    DBCPosition2D Corpse;                                     // 60-61 entrance coordinate (if exist single entry)
    //uint32_t TimeOfDayOverride;                             // 62 -1, 0 and 720
    uint32_t ExpansionID;                                     // 63
    uint32_t RaidOffset;                                      // 64
    uint32_t MaxPlayers;                                      // 65 max players, fallback if not present in MapDifficulty.dbc

    // Helpers
    uint32_t Expansion() const { return ExpansionID; }

    bool IsDungeon() const { return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID; }

    bool IsNonRaidDungeon() const { return InstanceType == MAP_INSTANCE; }

    bool Instanceable() const {
        return InstanceType == MAP_INSTANCE || InstanceType == MAP_RAID || InstanceType == MAP_BATTLEGROUND ||
               InstanceType == MAP_ARENA;
    }

    bool IsRaid() const { return InstanceType == MAP_RAID; }

    bool IsBattleground() const { return InstanceType == MAP_BATTLEGROUND; }

    bool IsBattleArena() const { return InstanceType == MAP_ARENA; }

    bool IsBattlegroundOrArena() const { return InstanceType == MAP_BATTLEGROUND || InstanceType == MAP_ARENA; }

    bool IsWorldMap() const { return InstanceType == MAP_COMMON; }

    bool GetEntrancePos(int32_t &mapid, float &x, float &y) const {
        if (CorpseMapID < 0)
            return false;
        mapid = CorpseMapID;
        x = Corpse.X;
        y = Corpse.Y;
        return true;
    }

    bool IsContinent() const {
        return ID == 0 || ID == 1 || ID == 530 || ID == 571;
    }

    bool IsDynamicDifficultyMap() const { return (Flags & MAP_FLAG_DYNAMIC_DIFFICULTY) != 0; }
};

// load MapDifficulty.dbc
struct MapDifficultyDBC {
    uint32_t ID;                                              // 0
    uint32_t MapID;                                           // 1
    uint32_t Difficulty;                                      // 2 (for arenas: arena slot)
    std::string Message[TOTAL_LOCALES];                       // 3-18 text showed when transfer to map failed (missing requirements)
    //uint32_t Message_lang_mask;                             // 19
    uint32_t RaidDuration;                                    // 20
    uint32_t MaxPlayers;                                      // 21
    //char const* Difficultystring;                           // 22
};

// load Movie.dbc
struct MovieDBC {
    uint32_t ID;                                              // 0
    //char const* Filename;                                   // 1
    //uint32_t Volume;                                        // 2 always 100
};

// load NamesProfanity.dbc
struct NamesProfanityDBC {
    uint32_t ID;                                              // 0
    std::string Name;                                         // 1
    int32_t Language;                                         // 2
};

// load NamesReserved.dbc
struct NamesReservedDBC {
    uint32_t ID;                                              // 0
    std::string Name;                                         // 1
    int32_t Language;                                         // 2
};

// load OverrideSpellData.dbc
#define MAX_OVERRIDE_SPELL 10

struct OverrideSpellDataDBC {
    uint32_t ID;                                              // 0
    uint32_t Spells[MAX_OVERRIDE_SPELL];                      // 1-10
    //uint32_t Flags;                                         // 11
};

// load PowerDisplay.dbc
struct PowerDisplayDBC {
    uint32_t ID;                                              // 0
    uint32_t ActualType;                                      // 1
    //char const* GlobalStringBaseTag;                        // 2
    //uint8_t Red;                                            // 3
    //uint8_t Green;                                          // 4
    //uint8_t Blue;                                           // 5
};

// load PvpDifficulty.dbc
struct PvPDifficultyDBC {
    uint32_t ID;                                              // 0
    uint32_t MapID;                                           // 1
    uint32_t RangeIndex;                                      // 2
    uint32_t MinLevel;                                        // 3
    uint32_t MaxLevel;                                        // 4
    uint32_t Difficulty;                                      // 5

    // helpers
    BattlegroundBracketId GetBracketId() const { return BattlegroundBracketId(RangeIndex); }
};

// load QuestSort.dbc
struct QuestSortDBC {
    uint32_t ID;                                              // 0
    //char const* SortName[16];                               // 1-16
    //uint32_t SortName_lang;                                 // 17
};

// load QuestXP.dbc
struct QuestXPDBC {
    uint32_t ID;                                              // 0
    uint32_t Difficulty[10];                                  // 1-10
};

// load QuestFactionReward.dbc
struct QuestFactionRewardDBC {
    uint32_t ID;                                              // 0
    int32_t Difficulty[10];                                   // 1
};

// load RandPropPoints.dbc
struct RandPropPointsDBC {
    uint32_t ID;                                              // 0 item level
    uint32_t Epic[5];                                         // 1-5
    uint32_t Superior[5];                                     // 6-10
    uint32_t Good[5];                                         // 11-15
};

// load ScalingStatDistribution.dbc
struct ScalingStatDistributionDBC {
    uint32_t ID;                                              // 0
    int32_t StatID[10];                                       // 1-10
    uint32_t Bonus[10];                                       // 11-20
    uint32_t Maxlevel;                                        // 21
};

// load ScalingStatValues.dbc
struct ScalingStatValuesDBC {
    uint32_t ID;                                              // 0
    uint32_t Charlevel;                                       // 1
    uint32_t ShoulderBudget;                                  // 2
    uint32_t TrinketBudget;                                   // 3
    uint32_t WeaponBudget1H;                                  // 4
    uint32_t RangedBudget;                                    // 5
    uint32_t ClothShoulderArmor;                              // 6
    uint32_t LeatherShoulderArmor;                            // 7
    uint32_t MailShoulderArmor;                               // 8
    uint32_t PlateShoulderArmor;                              // 9
    uint32_t WeaponDPS1H;                                     // 10
    uint32_t WeaponDPS2H;                                     // 11
    uint32_t SpellcasterDPS1H;                                // 12
    uint32_t SpellcasterDPS2H;                                // 13
    uint32_t RangedDPS;                                       // 14
    uint32_t WandDPS;                                         // 15
    uint32_t SpellPower;                                      // 16
    uint32_t PrimaryBudget;                                   // 17
    uint32_t TertiaryBudget;                                  // 18
    uint32_t ClothCloakArmor;                                 // 19
    uint32_t ClothChestArmor;                                 // 20
    uint32_t LeatherChestArmor;                               // 21
    uint32_t MailChestArmor;                                  // 22
    uint32_t PlateChestArmor;                                 // 23

    uint32_t getssdMultiplier(uint32_t mask) const {
        if (mask & 0x4001F) {
            if (mask & 0x00000001) return ShoulderBudget;
            if (mask & 0x00000002) return TrinketBudget;
            if (mask & 0x00000004) return WeaponBudget1H;
            if (mask & 0x00000008) return PrimaryBudget;
            if (mask & 0x00000010) return RangedBudget;
            if (mask & 0x00040000) return TertiaryBudget;
        }
        return 0;
    }

    uint32_t getArmorMod(uint32_t mask) const {
        if (mask & 0x00F001E0) {
            if (mask & 0x00000020) return ClothShoulderArmor;
            if (mask & 0x00000040) return LeatherShoulderArmor;
            if (mask & 0x00000080) return MailShoulderArmor;
            if (mask & 0x00000100) return PlateShoulderArmor;

            if (mask & 0x00080000) return ClothCloakArmor;
            if (mask & 0x00100000) return ClothChestArmor;
            if (mask & 0x00200000) return LeatherChestArmor;
            if (mask & 0x00400000) return MailChestArmor;
            if (mask & 0x00800000) return PlateChestArmor;
        }
        return 0;
    }

    uint32_t getDPSMod(uint32_t mask) const {
        if (mask & 0x7E00) {
            if (mask & 0x00000200) return WeaponDPS1H;
            if (mask & 0x00000400) return WeaponDPS2H;
            if (mask & 0x00000800) return SpellcasterDPS1H;
            if (mask & 0x00001000) return SpellcasterDPS2H;
            if (mask & 0x00002000) return RangedDPS;
            if (mask & 0x00004000) return WandDPS;
        }
        return 0;
    }

    bool isTwoHand(uint32_t mask) const {
        if (mask & 0x7E00) {
            if (mask & 0x00000400) return true;
            if (mask & 0x00001000) return true;
        }
        return false;
    }

    uint32_t getSpellBonus(uint32_t mask) const {
        if (mask & 0x00008000) return SpellPower;
        return 0;
    }

    uint32_t getFeralBonus(uint32_t /*mask*/) const                 // removed in 3.2.x?
    {
        //if (mask & 0x00010000) return 0;                  // not used?
        return 0;
    }
};

// load SkillLine.dbc
struct SkillLineDBC {
    uint32_t ID;                                              // 0
    int32_t CategoryID;                                       // 1
    //uint32_t SkillCostsID;                                  // 2
    std::string DisplayName[TOTAL_LOCALES];                   // 3-18
    //uint32_t DisplayName_lang_mask;                         // 19
    //char const* Description[16];                            // 20-35
    //uint32_t Description_lang_mask;                         // 36
    uint32_t SpellIconID;                                     // 37
    //char const* AlternateVerb[16];                          // 38-53
    //uint32_t AlternateVerb_lang_mask;                       // 54
    uint32_t CanLink;                                         // 55
};

// load SkillLineAbility.dbc
struct SkillLineAbilityDBC {
    uint32_t ID;                                              // 0
    uint32_t SkillLine;                                       // 1
    uint32_t Spell;                                           // 2
    uint32_t RaceMask;                                        // 3
    uint32_t ClassMask;                                       // 4
    //uint32_t ExcludeRace;                                   // 5
    //uint32_t ExcludeClass;                                  // 6
    uint32_t MinSkillLineRank;                                // 7
    uint32_t SupercededBySpell;                               // 8
    uint32_t AcquireMethod;                                   // 9
    uint32_t TrivialSkillLineRankHigh;                        // 10
    uint32_t TrivialSkillLineRankLow;                         // 11
    //uint32_t CharacterPoints[2];                            // 12-13
};

// load SkillRaceClassInfo.dbc
struct SkillRaceClassInfoDBC {
    uint32_t ID;                                              // 0
    uint32_t SkillID;                                         // 1
    int32_t RaceMask;                                         // 2
    int32_t ClassMask;                                        // 3
    uint32_t Flags;                                           // 4
    //uint32_t MinLevel;                                      // 5
    uint32_t SkillTierID;                                     // 6
    //uint32_t SkillCostIndex;                                // 7
};

// load SkillTiers.dbc
#define MAX_SKILL_STEP 16

struct SkillTiersDBC {
    uint32_t ID;                                              // 0
    //uint32_t Cost[MAX_SKILL_STEP];                          // 1-16
    uint32_t Value[MAX_SKILL_STEP];                           // 17-32
};

// load SoundEntries.dbc
struct SoundEntriesDBC {
    uint32_t ID;                                              // 0
    //uint32_t SoundType;                                     // 1
    //char const* Name;                                       // 2
    //char const* File[10];                                   // 3-12
    //uint32_t Freq[10];                                      // 13-22
    //char const* DirectoryBase;                              // 23
    //float VolumeFloat;                                      // 24
    //uint32_t Flags;                                         // 25
    //float MinDistance;                                      // 26
    //float DistanceCutoff;                                   // 27
    //uint32_t EAXDef;                                        // 28
    //uint32_t SoundEntriesAdvancedID;                        // 29
};

// load Spell.dbc
struct SpellDBC {
    uint32_t ID;                                                          // 0
    uint32_t Category;                                                    // 1
    uint32_t DispelType;                                                  // 2
    uint32_t Mechanic;                                                    // 3
    uint32_t Attributes;                                                  // 4
    uint32_t AttributesEx;                                                // 5
    uint32_t AttributesExB;                                               // 6
    uint32_t AttributesExC;                                               // 7
    uint32_t AttributesExD;                                               // 8
    uint32_t AttributesExE;                                               // 9
    uint32_t AttributesExF;                                               // 10
    uint32_t AttributesExG;                                               // 11
    std::array<uint32_t, 2> ShapeshiftMask;                               // 12-13
    std::array<uint32_t, 2> ShapeshiftExclude;                            // 14-15
    uint32_t Targets;                                                     // 16
    uint32_t TargetCreatureType;                                          // 17
    uint32_t RequiresSpellFocus;                                          // 18
    uint32_t FacingCasterFlags;                                           // 19
    uint32_t CasterAuraState;                                             // 20
    uint32_t TargetAuraState;                                             // 21
    uint32_t ExcludeCasterAuraState;                                      // 22
    uint32_t ExcludeTargetAuraState;                                      // 23
    uint32_t CasterAuraSpell;                                             // 24
    uint32_t TargetAuraSpell;                                             // 25
    uint32_t ExcludeCasterAuraSpell;                                      // 26
    uint32_t ExcludeTargetAuraSpell;                                      // 27
    uint32_t CastingTimeIndex;                                            // 28
    uint32_t RecoveryTime;                                                // 29
    uint32_t CategoryRecoveryTime;                                        // 30
    uint32_t InterruptFlags;                                              // 31
    uint32_t AuraInterruptFlags;                                          // 32
    uint32_t ChannelInterruptFlags;                                       // 33
    uint32_t ProcTypeMask;                                                // 34
    uint32_t ProcChance;                                                  // 35
    uint32_t ProcCharges;                                                 // 36
    uint32_t MaxLevel;                                                    // 37
    uint32_t BaseLevel;                                                   // 38
    uint32_t SpellLevel;                                                  // 39
    uint32_t DurationIndex;                                               // 40
    uint32_t PowerType;                                                   // 41
    uint32_t ManaCost;                                                    // 42
    uint32_t ManaCostPerLevel;                                            // 43
    uint32_t ManaPerSecond;                                               // 44
    uint32_t ManaPerSecondPerLevel;                                       // 45
    uint32_t RangeIndex;                                                  // 46
    float Speed;                                                          // 47
    //uint32_t ModalNextSpell;                                            // 48
    uint32_t CumulativeAura;                                              // 49
    std::array<uint32_t, 2> Totem;                                        // 50-51
    std::array<int32_t, MAX_SPELL_REAGENTS> Reagent;                      // 52-59
    std::array<int32_t, MAX_SPELL_REAGENTS> ReagentCount;                 // 60-67
    int32_t EquippedItemClass;                                            // 68
    int32_t EquippedItemSubclass;                                         // 69
    int32_t EquippedItemInvTypes;                                         // 70
    std::array<uint32_t, MAX_SPELL_EFFECTS> Effect;                       // 71-73
    std::array<int32_t, MAX_SPELL_EFFECTS> EffectDieSides;                // 74-76
    std::array<float, MAX_SPELL_EFFECTS> EffectRealPointsPerLevel;        // 77-79
    std::array<int32_t, MAX_SPELL_EFFECTS> EffectBasePoints;              // 80-82
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectMechanic;               // 83-85
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectImplicitTargetA;        // 86-88
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectImplicitTargetB;        // 89-91
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectRadiusIndex;            // 92-94
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectAura;                   // 95-97
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectAuraPeriod;             // 98-100
    std::array<float, MAX_SPELL_EFFECTS> EffectAmplitude;                 // 101-103
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectChainTargets;           // 104-106
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectItemType;               // 107-109
    std::array<int32_t, MAX_SPELL_EFFECTS> EffectMiscValue;               // 110-112
    std::array<int32_t, MAX_SPELL_EFFECTS> EffectMiscValueB;              // 113-115
    std::array<uint32_t, MAX_SPELL_EFFECTS> EffectTriggerSpell;           // 116-118
    std::array<float, MAX_SPELL_EFFECTS> EffectPointsPerCombo;            // 119-121
    std::array<flag96, MAX_SPELL_EFFECTS> EffectSpellClassMask;           // 122-130
    std::array<uint32_t, 2> SpellVisualID;                                // 131-132
    uint32_t SpellIconID;                                                 // 133
    uint32_t ActiveIconID;                                                // 134
    uint32_t SpellPriority;                                               // 135
    std::string Name[TOTAL_LOCALES];                                      // 136-151
    //uint32_t Name_lang_mask;                                            // 152
    std::string NameSubtext[TOTAL_LOCALES];                               // 153-168
    //uint32_t NameSubtext_lang_mask;                                     // 169
    //std::array<char const*, 16> Description;                            // 170-185
    //uint32_t Description_lang_mask;                                     // 186
    //std::array<char const*, 16> AuraDescription;                        // 187-202
    //uint32_t AuraDescription_lang_mask;                                 // 203
    uint32_t ManaCostPct;                                                 // 204
    uint32_t StartRecoveryCategory;                                       // 205
    uint32_t StartRecoveryTime;                                           // 206
    uint32_t MaxTargetLevel;                                              // 207
    uint32_t SpellClassSet;                                               // 208
    flag96 SpellClassMask;                                                // 209-211
    uint32_t MaxTargets;                                                  // 212
    uint32_t DefenseType;                                                 // 213
    uint32_t PreventionType;                                              // 214
    //uint32_t StanceBarOrder;                                            // 215
    std::array<float, MAX_SPELL_EFFECTS> EffectChainAmplitude;            // 216-218
    //uint32_t MinFactionID;                                              // 219
    //uint32_t MinReputation;                                             // 220
    //uint32_t RequiredAuraVision;                                        // 221
    std::array<uint32_t, 2> RequiredTotemCategoryID;                      // 222-223
    int32_t RequiredAreasID;                                              // 224
    uint32_t SchoolMask;                                                  // 225
    uint32_t RuneCostID;                                                  // 226
    //uint32_t SpellMissileID;                                            // 227
    //int32_t PowerDisplayID;                                             // 228
    std::array<float, MAX_SPELL_EFFECTS> EffectBonusCoefficient;          // 229-231
    //uint32_t DescriptionVariablesID;                                    // 232
    //uint32_t Difficulty;                                                // 233
};

// load SpellCastTimes.dbc
struct SpellCastTimesDBC {
    uint32_t ID;                                              // 0
    int32_t Base;                                             // 1
    //int32_t PerLevel;                                       // 2
    //int32_t Minimum;                                        // 3
};

// load SpellCategory.dbc
struct SpellCategoryDBC {
    uint32_t ID;                                              // 0
    uint32_t Flags;                                           // 1
};

#define MAX_ITEM_ENCHANTMENT_EFFECTS 3

// load SpellItemEnchantment.dbc
struct SpellItemEnchantmentDBC {
    uint32_t ID;                                              // 0
    //uint32_t Charges;                                       // 1
    uint32_t Effect[MAX_ITEM_ENCHANTMENT_EFFECTS];            // 2-4
    int32_t EffectPointsMin[MAX_ITEM_ENCHANTMENT_EFFECTS];    // 5-7
    //int32_t EffectPointsMax[MAX_ITEM_ENCHANTMENT_EFFECTS]   // 8-10
    uint32_t EffectArg[MAX_ITEM_ENCHANTMENT_EFFECTS];         // 11-13
    std::string Name[TOTAL_LOCALES];                          // 14-29
    //uint32_t Name_lang_mask;                                // 30
    int32_t ItemVisual;                                       // 31
    uint32_t Flags;                                           // 32
    uint32_t SrcItemID;                                       // 33
    uint32_t ConditionID;                                     // 34
    uint32_t RequiredSkillID;                                 // 35
    uint32_t RequiredSkillRank;                               // 36
    uint32_t MinLevel;                                        // 37
};

// load SpellDifficulty.dbc
struct SpellDifficultyDBC {
    uint32_t ID;                                              // 0
    int32_t DifficultySpellID[MAX_DIFFICULTY];                // 1-4 instance modes: 10N, 25N, 10H, 25H or Normal/Heroic if only 1-2 is set, if 3-4 is 0 then Mode-2
};

// load SpellDuration.dbc
struct SpellDurationDBC {
    uint32_t ID;                                              // 0
    int32_t Duration;                                         // 1
    int32_t DurationPerLevel;                                 // 2
    int32_t MaxDuration;                                      // 3
};

// load SpellFocusObject.dbc
struct SpellFocusObjectDBC {
    uint32_t ID;                                              // 0
    //char const* Name[16];                                   // 1-15
    //uint32_t Name_lang_mask;                                // 16
};

// load SpellItemEnchantmentCondition.dbc
struct SpellItemEnchantmentConditionDBC {
    uint32_t ID;                                              // 0
    uint8_t LtOperandType[5];                                 // 1-5
    //uint32_t LtOperand[5];                                  // 6-10
    uint8_t Operator[5];                                      // 11-15
    uint8_t RtOperandType[5];                                 // 15-20
    uint32_t RtOperand[5];                                    // 21-25
    //uint8_t Logic[5];                                       // 25-30
};

// load SpellRadius.dbc
struct SpellRadiusDBC {
    uint32_t ID;                                            // 0
    float Radius;                                           // 1
    float RadiusPerLevel;                                   // 2
    float RadiusMax;                                        // 3
};

// load SpellRange.dbc
struct SpellRangeDBC {
    uint32_t ID;                                              // 0
    float RangeMin[2];                                        // 1-2 [0] Hostile [1] Friendly
    float RangeMax[2];                                        // 3-4 [0] Hostile [1] Friendly
    uint32_t Flags;                                           // 5
    //char const* DisplayName[16];                            // 6-21
    //uint32_t DisplayName_lang_mask;                         // 22
    //char const* DisplayNameShort[16];                       // 23-38
    //uint32_t DisplayNameShort_lang_mask;                    // 39
};

// load SpellRuneCost.dbc
struct SpellRuneCostDBC {
    uint32_t ID;                                              // 0
    uint32_t RuneCost[3];                                     // 1-3 [0] Blood [1] Unholy [2] Frost
    uint32_t RunicPower;                                      // 4

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }

    bool NoRunicPowerGain() const { return RunicPower == 0; }
};

//load SpellShapeshiftForm.dbc
#define MAX_SHAPESHIFT_SPELLS 8

struct SpellShapeshiftFormDBC {
    uint32_t ID;                                              // 0
    //uint32_t BonusActionBar;                                // 1
    //char const* Name[16];                                   // 2-17
    //uint32_t Name_lang_mask;                                // 18
    uint32_t Flags;                                           // 19
    int32_t CreatureType;                                     // 20 <= 0 humanoid, other normal creature types
    //uint32_t AttackIconID;                                  // 21
    uint32_t CombatRoundTime;                                 // 22
    uint32_t CreatureDisplayID[4];                            // 23-26 [0] Alliance [1] Horde [2] Unk [3] Unk
    uint32_t PresetSpellID[MAX_SHAPESHIFT_SPELLS];            // 27-34
};

// load SpellVisual.dbc
struct SpellVisualDBC {
    uint32_t ID;                                              // 0
    //uint32_t PrecastKit;                                    // 1
    //uint32_t CastKit;                                       // 2
    //uint32_t ImpactKit;                                     // 3
    //uint32_t StateKit;                                      // 4
    //uint32_t StateDoneKit;                                  // 5
    //uint32_t ChannelKit;                                    // 6
    uint32_t HasMissile;                                      // 7
    int32_t MissileModel;                                     // 8
    //uint32_t MissilePathType;                               // 9
    //uint32_t MissileDestinationAttachment;                  // 10
    //uint32_t MissileSound;                                  // 11
    //uint32_t AnimEventSoundID;                              // 12
    //uint32_t Flags;                                         // 13
    //uint32_t CasterImpactKit;                               // 14
    //uint32_t TargetImpactKit;                               // 15
    //int32_t MissileAttachment;                              // 16
    //uint32_t MissileFollowGroundHeight;                     // 17
    //uint32_t MissileFollowGroundDropSpeed;                  // 18
    //uint32_t MissileFollowGroundApproach;                   // 19
    //uint32_t MissileFollowGroundFlags;                      // 20
    //uint32_t MissileMotion;                                 // 21
    //uint32_t MissileTargetingKit;                           // 22
    //uint32_t InstantAreaKit;                                // 23
    //uint32_t ImpactAreaKit;                                 // 24
    //uint32_t PersistentAreaKit;                             // 25
    //DBCPosition3D MissileCastOffset;                        // 26-28
    //DBCPosition3D MissileImpactOffset;                      // 29-31
};

// load StableSlotPrices.dbc
struct StableSlotPricesDBC {
    uint32_t ID;                                              // 0
    uint32_t Cost;                                            // 1
};

// load SummonProperties.dbc
struct SummonPropertiesDBC {
    uint32_t ID;                                              // 0
    uint32_t Control;                                         // 1
    uint32_t Faction;                                         // 2
    uint32_t Title;                                           // 3
    uint32_t Slot;                                            // 4
    uint32_t Flags;                                           // 5
};

// load Talent.dbc
struct TalentDBC {
    uint32_t ID;                                              // 0
    uint32_t TabID;                                           // 1 index in TalentTab.dbc (TalentTabEntry)
    uint32_t TierID;                                          // 2
    uint32_t ColumnIndex;                                     // 3
    std::array<uint32_t, MAX_TALENT_RANK> SpellRank;          // 4-8
    //std::array<uint32_t, 4> SpellRankUnused;                // 9-12
    uint32_t PrereqTalent;                                    // 13 index in Talent.dbc (TalentEntry)
    //std::array<uint32_t, 2> PrereqTalentUnused;             // 14-15
    uint32_t PrereqRank;                                      // 16
    //std::array<uint32_t, 2> PrereqRankUnused;               // 17-18
    //uint32_t Flags;                                         // 19
    //uint32_t RequiredSpellID;                               // 20 all 0
    //std::array<uint32_t, 2> CategoryMask;                   // 21 its a 64 bit mask for pet 1<<CategoryEnumID in CreatureFamily.dbc
};

//     structure helper
struct TalentSpellPos {
    TalentSpellPos() : talent_id(0), rank(0) {}

    TalentSpellPos(uint16_t _talent_id, uint8_t _rank) : talent_id(_talent_id), rank(_rank) {}

    uint16_t talent_id;
    uint8_t rank;
};

// load TalentTab.dbc
struct TalentTabDBC {
    uint32_t ID;                                              // 0
    //char const* Name[16];                                   // 1-16
    //uint32_t Name_lang_mask;                                // 17
    //unit32 SpellIconID;                                     // 18
    //uint32_t RaceMask;                                      // 19
    uint32_t ClassMask;                                       // 20
    uint32_t PetTalentMask;                                   // 21
    uint32_t OrderIndex;                                      // 22
    //char const* BackgroundFile;                             // 23
};

// load TaxiNodes.dbc
struct TaxiNodesDBC {
    uint32_t ID;                                              // 0
    uint32_t ContinentID;                                     // 1
    DBCPosition3D Pos;                                        // 2-4
    std::string Name[TOTAL_LOCALES];                          // 5-21
    //uint32_t Name_lang_mask;                                // 22
    uint32_t MountCreatureID[2];                              // 23-24
};

// load TaxiPath.dbc
struct TaxiPathDBC {
    uint32_t ID;                                              // 0
    int32_t FromTaxiNode;                                    // 1
    int32_t ToTaxiNode;                                      // 2
    uint32_t Cost;                                            // 3
};

//     structure helper
struct TaxiPathBySourceAndDestination {
    TaxiPathBySourceAndDestination() : ID(0), price(0) {}

    TaxiPathBySourceAndDestination(uint32_t _id, uint32_t _price) : ID(_id), price(_price) {}

    uint32_t ID;
    uint32_t price;
};

// load TaxiPathNode.dbc
struct TaxiPathNodeDBC {
    uint32_t ID;                                              // 0
    uint32_t PathID;                                          // 1
    uint32_t NodeIndex;                                       // 2
    uint32_t ContinentID;                                     // 3
    DBCPosition3D Loc;                                        // 4-6
    uint32_t Flags;                                           // 7
    uint32_t Delay;                                           // 8
    uint32_t ArrivalEventID;                                  // 9
    uint32_t DepartureEventID;                                // 10
};

// load TeamContributionPoints.dbc
struct TeamContributionPointsDBC {
    uint32_t ID;                                              // 0
    float Data;                                               // 1
};

// load TotemCategory.dbc
struct TotemCategoryDBC {
    uint32_t ID;                                              // 0
    //char const* Name[16];                                   // 1-16
    //uint32_t Name_lang_mask;                                // 17
    uint32_t TotemCategoryType;                               // 18 (one for specialization)
    uint32_t TotemCategoryMask;                               // 19 (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

// load TransportAnimation.dbc
struct TransportAnimationDBC {
    uint32_t ID;                                              // 0
    uint32_t TransportID;                                     // 1
    uint32_t TimeIndex;                                       // 2
    DBCPosition3D Pos;                                        // 3
    //uint32_t SequenceID;                                    // 4
};

// load TransportRotation.dbc
struct TransportRotationDBC {
    uint32_t ID;                                              // 0
    uint32_t GameObjectsID;                                   // 1
    uint32_t TimeIndex;                                       // 2
    float X;                                                  // 3
    float Y;                                                  // 4
    float Z;                                                  // 5
    float W;                                                  // 6
};

// load Vehicle.dbc
#define MAX_VEHICLE_SEATS 8

struct VehicleDBC {
    uint32_t ID;                                            // 0
    int32_t Flags;                                          // 1
    float TurnSpeed;                                        // 2
    float PitchSpeed;                                       // 3
    float PitchMin;                                         // 4
    float PitchMax;                                         // 5
    uint32_t SeatID[MAX_VEHICLE_SEATS];                     // 6-13
    float MouseLookOffsetPitch;                             // 14
    float CameraFadeDistScalarMin;                          // 15
    float CameraFadeDistScalarMax;                          // 16
    float CameraPitchOffset;                                // 17
    float FacingLimitRight;                                 // 18
    float FacingLimitLeft;                                  // 19
    float MsslTrgtTurnLingering;                            // 20
    float MsslTrgtPitchLingering;                           // 21
    float MsslTrgtMouseLingering;                           // 22
    float MsslTrgtEndOpacity;                               // 23
    float MsslTrgtArcSpeed;                                 // 24
    float MsslTrgtArcRepeat;                                // 25
    float MsslTrgtArcWidth;                                 // 26
    float MsslTrgtImpactRadius[2];                          // 27-28
    std::string MsslTrgtArcTexture;                         // 29
    std::string MsslTrgtImpactTexture;                      // 30
    std::string MsslTrgtImpactModel[2];                     // 31-32
    float CameraYawOffset;                                  // 33
    uint32_t UiLocomotionType;                              // 34
    float MsslTrgtImpactTexRadius;                          // 35
    uint32_t VehicleUIIndicatorID;                          // 36
    int32_t PowerDisplayID;                                 // 37
    //int32_t PowerDisplayIDUnused[2];                      // 38-39
};

// load VehicleSeat.dbc
struct VehicleSeatDBC {
    uint32_t ID;                                            // 0
    int32_t Flags;                                          // 1
    int32_t AttachmentID;                                   // 2
    DBCPosition3D AttachmentOffset;                         // 3-5
    float EnterPreDelay;                                    // 6
    float EnterSpeed;                                       // 7
    float EnterGravity;                                     // 8
    float EnterMinDuration;                                 // 9
    float EnterMaxDuration;                                 // 10
    float EnterMinArcHeight;                                // 11
    float EnterMaxArcHeight;                                // 12
    int32_t EnterAnimStart;                                 // 13
    int32_t EnterAnimLoop;                                  // 14
    int32_t RideAnimStart;                                  // 15
    int32_t RideAnimLoop;                                   // 16
    int32_t RideUpperAnimStart;                             // 17
    int32_t RideUpperAnimLoop;                              // 18
    float ExitPreDelay;                                     // 19
    float ExitSpeed;                                        // 20
    float ExitGravity;                                      // 21
    float ExitMinDuration;                                  // 22
    float ExitMaxDuration;                                  // 23
    float ExitMinArcHeight;                                 // 24
    float ExitMaxArcHeight;                                 // 25
    int32_t ExitAnimStart;                                  // 26
    int32_t ExitAnimLoop;                                   // 27
    int32_t ExitAnimEnd;                                    // 28
    float PassengerYaw;                                     // 29
    float PassengerPitch;                                   // 30
    float PassengerRoll;                                    // 31
    int32_t PassengerAttachmentID;                          // 32
    int32_t VehicleEnterAnim;                               // 33
    int32_t VehicleExitAnim;                                // 34
    int32_t VehicleRideAnimLoop;                            // 35
    int32_t VehicleEnterAnimBone;                           // 36
    int32_t VehicleExitAnimBone;                            // 37
    int32_t VehicleRideAnimLoopBone;                        // 38
    float VehicleEnterAnimDelay;                            // 39
    float VehicleExitAnimDelay;                             // 40
    uint32_t VehicleAbilityDisplay;                         // 41
    uint32_t EnterUISoundID;                                // 42
    uint32_t ExitUISoundID;                                 // 43
    int32_t UiSkin;                                         // 44
    uint32_t FlagsB;                                        // 45
    //float CameraEnteringDelay;                            // 46
    //float CameraEnteringDuration;                         // 47
    //float CameraExitingDelay;                             // 48
    //float CameraExitingDuration;                          // 49
    //DBCPosition3D CameraOffset;                           // 50-52
    //float CameraPosChaseRate;                             // 53
    //float CameraFacingChaseRate;                          // 54
    //float CameraEnteringZoom;                             // 55
    //float CameraSeatZoomMin;                              // 56
    //float CameraSeatZoomMax;                              // 57

    inline bool HasFlag(VehicleSeatFlags flag) const { return (Flags & flag) != 0; }

    inline bool HasFlag(VehicleSeatFlagsB flag) const { return (FlagsB & flag) != 0; }

    inline bool CanEnterOrExit() const {
        return HasFlag(VehicleSeatFlags(VEHICLE_SEAT_FLAG_CAN_ENTER_OR_EXIT | VEHICLE_SEAT_FLAG_CAN_CONTROL |
                                        VEHICLE_SEAT_FLAG_SHOULD_USE_VEH_SEAT_EXIT_ANIM_ON_VOLUNTARY_EXIT));
    }

    inline bool CanSwitchFromSeat() const { return HasFlag(VEHICLE_SEAT_FLAG_CAN_SWITCH); }

    inline bool IsUsableByOverride() const {
        return HasFlag(VehicleSeatFlags(VEHICLE_SEAT_FLAG_UNCONTROLLED | VEHICLE_SEAT_FLAG_UNK18))
               || HasFlag(VehicleSeatFlagsB(VEHICLE_SEAT_FLAG_B_USABLE_FORCED | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_2 |
                                            VEHICLE_SEAT_FLAG_B_USABLE_FORCED_3 | VEHICLE_SEAT_FLAG_B_USABLE_FORCED_4));
    }

    inline bool IsEjectable() const { return HasFlag(VEHICLE_SEAT_FLAG_B_EJECTABLE); }
};

// load WMOAreaTable.dbc
struct WMOAreaTableDBC {
    uint32_t ID;                                              // 0 index
    int32_t WMOID;                                            // 1 used in root WMO
    int32_t NameSetID;                                        // 2 used in adt file
    int32_t WMOGroupID;                                       // 3 used in group WMO
    //uint32_t SoundProviderPref;                             // 4
    //uint32_t SoundProviderPrefUnderwater;                   // 5
    //uint32_t AmbienceID;                                    // 6
    //uint32_t ZoneMusic;                                     // 7
    //uint32_t IntroSound;                                    // 8
    uint32_t Flags;                                           // 9 used for indoor/outdoor determination
    uint32_t AreaTableID;                                     // 10 link to AreaTableEntry.ID
    //char const* AreaName[16];                               // 11
    //uint32_t AreaName_lang_mask;                            // 12
};

// load WorldMapArea.dbc
struct WorldMapAreaDBC {
    uint32_t ID;                                              // 0
    uint32_t MapID;                                           // 1
    uint32_t AreaID;                                          // 2
    //char const* AreaName                                    // 3
    float LocLeft;                                            // 4
    float LocRight;                                           // 5
    float LocTop;                                             // 6
    float LocBottom;                                          // 7
    int32_t DisplayMapID;                                     // 8 -1 (map_id have correct map) other: virtual map where zone show (map_id - where zone in fact internally)
    //int32_t DefaultDungeonFloor;                            // 9 pointer to DungeonMap.dbc (override x1, x2, y1, y2 coordinates)
    //uint32_t ParentWorldMapID;                              // 10
};

// load WorldMapOverlay.dbc
#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct WorldMapOverlayDBC {
    uint32_t ID;                                              // 0
    //uint32_t MapAreaID;                                     // 1 ID in WorldMapArea.dbc
    uint32_t AreaID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];          // 2-5
    //uint32_t MapPointX;                                     // 6
    //uint32_t MapPointY;                                     // 7
    //char const* TextureName                                 // 8
    //uint32_t TextureWidth;                                  // 9
    //uint32_t TextureHeight;                                 // 10
    //uint32_t OffsetX;                                       // 11
    //uint32_t OffsetY;                                       // 12
    //uint32_t HitRectTop;                                    // 13
    //uint32_t HitRectLeft;                                   // 14
    //uint32_t HitRectBottom;                                 // 15
    //uint32_t HitRectRight;                                  // 16
};

// load WorldSafeLocs.dbc
struct WorldSafeLocsDBC {
    uint32_t ID;                                              // 0
    uint32_t Continent;                                       // 1
    DBCPosition3D Loc;                                        // 2-4
    //char const* AreaName[16]                                // 5-20
    //uint32_t AreaName_lang_mask;                            // 21
};

// UNUSED structures
/*
struct HolidayDescriptionsEntry
{
    uint32_t ID;                                              // 0
    char const* Description[16];                              // 1-16
    uint32_t Description_lang_mask;                           // 17
};
*/

/*
struct HolidayNamesEntry
{
    uint32_t ID;                                              // 0
    char const* Name[16];                                     // 1-16
    uint32_t Name_lang_mask;                                  // 17
};
*/

/*
struct ItemCondExtCostsEntry
{
    uint32_t ID;                                              // 0
    uint32_t CondExtendedCost;                                // 1 ItemTemplate::CondExtendedCost
    uint32_t ItemExtendedCostEntry;                           // 2 ItemTemplate::ExtendedCost
    uint32_t ArenaSeason;                                     // 3 arena season number (1-4)
};
*/

//#define MAX_ITEM_ENCHANTMENT_EFFECTS 3

/*
struct SkillLineCategoryEntry
{
    uint32_t ID;                                              // 0
    char const* Name[16];                                     // 1-17
    uint32_t Name_lang_mask;                                  // 18
    uint32_t SortIndex;                                       // 19
};
*/

/*
struct WorldStateUI
{
    uint32_t ID;                                              // 0
    uint32_t MapID;                                           // 1 Can be -1 to show up everywhere.
    uint32_t AreaID;                                          // 2 Can be zero for "everywhere".
    uint32_t PhaseShift;                                      // 3 Phase this WorldState is avaliable in
    char const* Icon;                                         // 4 The icon that is used in the interface.
    char const* String;                                       // 5-20
    uint32_t String_lang_mask;                                // 21
    char const* Tooltip;                                      // 22-37
    uint32_t Tooltip_lang_mask;                               // 38
    uint32_t StateVariable;                                   // 39
    uint32_t Type;                                            // 40 0 = unknown, 1 = unknown, 2 = not shown in ui, 3 = wintergrasp
    char const* DynamicIcon;                                  // 41
    char const* DynamicTooltip[16];                           // 42-57
    uint32_t DynamicTooltip_lang_mask;                        // 58
    char const* ExtendedUI;                                   // 59
    uint32_t ExtendedUIStateVariable[3];                      // 60-62
};
*/

/*
struct WorldStateZoneSounds
{
    uint32_t WorldStateID;                                    // 0
    uint32_t WorldStateValue;                                 // 1
    uint32_t AreaID;                                          // 2
    uint32_t WMOAreaID;                                       // 3
    uint32_t ZoneIntroMusicID;                                // 4
    uint32_t ZoneMusicID;                                     // 5
    uint32_t SoundAmbienceID;                                 // 6
    uint32_t SoundProviderPreferencesID;                      // 7
};
*/
#pragma pack(pop)