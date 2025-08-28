#pragma once
#include <string>
#include <array>
#include "src/game/enums/ItemTemplateEnums.hpp"

struct _ItemStat
{
    uint32_t ItemStatType = 0;
    int32_t ItemStatValue = 0;
};
#define MAX_ITEM_PROTO_STATS  10

struct _Damage
{
    float DamageMin = 0.0f;
    float DamageMax = 0.0f;
    uint32_t DamageType = 0;                                 // id from Resistances.dbc
};
#define MAX_ITEM_PROTO_DAMAGES 2                            // changed in 3.1.0

struct _Spell
{
    int32_t SpellId = 0;                                      // id from Spell.dbc
    uint32_t SpellTrigger = 0;
    int32_t SpellCharges = 0;
    float SpellPPMRate = 0.0f;
    int32_t SpellCooldown = -1;
    uint32_t SpellCategory = 0;                               // id from SpellCategory.dbc
    int32_t SpellCategoryCooldown = -1;
};
#define MAX_ITEM_PROTO_SPELLS  5

struct _Socket
{
    uint32_t Color = 0;
    uint32_t Content = 0;
};
#define MAX_ITEM_PROTO_SOCKETS 3

struct ItemTemplate {
    uint32_t ItemId;
    uint8_t Class;                                           // id from ItemClass.dbc
    uint8_t SubClass;                                        // id from ItemSubClass.dbc
    int32_t SoundOverrideSubclass;                           // < 0: id from ItemSubClass.dbc, used to override weapon sound from actual SubClass
    std::string Name1;
    uint32_t DisplayInfoID;                                   // id from ItemDisplayInfo.dbc
    uint8_t Quality;
    int32_t Flags;
    uint32_t Flags2;
    uint32_t BuyCount;
    int32_t BuyPrice;
    uint32_t SellPrice;
    uint8_t InventoryType;
    int32_t AllowableClass;
    int32_t AllowableRace;
    uint32_t ItemLevel;
    uint32_t RequiredLevel;
    uint32_t RequiredSkill;                                   // id from SkillLine.dbc
    uint32_t RequiredSkillRank;
    uint32_t RequiredSpell;                                   // id from Spell.dbc
    uint32_t RequiredHonorRank;
    uint32_t RequiredCityRank;
    uint32_t RequiredReputationFaction;                       // id from Faction.dbc
    uint32_t RequiredReputationRank;
    int32_t MaxCount;                                        // <= 0: no limit
    int32_t Stackable;                                       // 0: not allowed, -1: put in player coin info tab and don't limit stacking (so 1 slot)
    uint32_t ContainerSlots;
    uint32_t StatsCount;
    std::array <_ItemStat, MAX_ITEM_PROTO_STATS> ItemStat;
    uint32_t ScalingStatDistribution;                         // id from ScalingStatDistribution.dbc
    uint32_t ScalingStatValue;                                // mask for selecting column in ScalingStatValues.dbc
    std::array <_Damage, MAX_ITEM_PROTO_DAMAGES> Damage;
    uint32_t Armor;
    uint32_t HolyRes;
    uint32_t FireRes;
    uint32_t NatureRes;
    uint32_t FrostRes;
    uint32_t ShadowRes;
    uint32_t ArcaneRes;
    uint32_t Delay;
    uint32_t AmmoType;
    float RangedModRange;
    std::array <_Spell, MAX_ITEM_PROTO_SPELLS> Spells;
    uint32_t Bonding;
    std::string Description;
    uint32_t PageText;
    uint32_t LanguageID;
    uint32_t PageMaterial;
    uint32_t StartQuest;                                      // id from QuestCache.wdb
    uint32_t LockID;
    int32_t Material;                                        // id from Material.dbc
    uint32_t Sheath;
    int32_t RandomProperty;                                  // id from ItemRandomProperties.dbc
    int32_t RandomSuffix;                                    // id from ItemRandomSuffix.dbc
    uint32_t Block;
    uint32_t ItemSet;                                         // id from ItemSet.dbc
    uint32_t MaxDurability;
    uint32_t Area;                                            // id from AreaTable.dbc
    uint32_t Map;                                             // id from Map.dbc
    uint32_t BagFamily;                                       // bit mask (1 << id from ItemBagFamily.dbc)
    uint32_t TotemCategory;                                   // id from TotemCategory.dbc
    std::array <_Socket, MAX_ITEM_PROTO_SOCKETS> Socket;
    uint32_t socketBonus;                                     // id from SpellItemEnchantment.dbc
    uint32_t GemProperties;                                   // id from GemProperties.dbc
    int32_t RequiredDisenchantSkill;
    float ArmorDamageModifier;
    uint32_t Duration;
    uint32_t ItemLimitCategory;                               // id from ItemLimitCategory.dbc
    uint32_t HolidayId;                                       // id from Holidays.dbc
    std::string ScriptName;
    uint32_t DisenchantID;
    uint32_t FoodType;
    uint32_t MinMoneyLoot;
    uint32_t MaxMoneyLoot;
    uint32_t FlagsCu;
    int32_t VerifiedBuild;

    // helpers
    bool CanChangeEquipStateInCombat() const;

    bool IsCurrencyToken() const { return (BagFamily & BAG_FAMILY_MASK_CURRENCY_TOKENS) != 0; }

    uint32_t GetMaxStackSize() const {
        return (Stackable == 2147483647 || Stackable <= 0) ? uint32_t(0x7FFFFFFF - 1) : uint32_t(Stackable);
    }

    float getDPS() const;

    int32_t getFeralBonus(int32_t extraDPS = 0) const;

    int32_t GetTotalAPBonus() const { return _totalAP; }

    float GetItemLevelIncludingQuality() const;

    uint32_t GetSkill() const;

    bool IsPotion() const { return Class == ITEM_CLASS_CONSUMABLE && SubClass == ITEM_SUBCLASS_POTION; }

    bool IsWeaponVellum() const {
        return Class == ITEM_CLASS_TRADE_GOODS && SubClass == ITEM_SUBCLASS_WEAPON_ENCHANTMENT;
    }

    bool IsArmorVellum() const {
        return Class == ITEM_CLASS_TRADE_GOODS && SubClass == ITEM_SUBCLASS_ARMOR_ENCHANTMENT;
    }

    bool IsConjuredConsumable() const { return Class == ITEM_CLASS_CONSUMABLE && HasFlag(ITEM_FLAG_CONJURED); }

    bool HasSignature() const;

    inline bool HasFlag(ItemFlags flag) const { return (Flags & flag) != 0; }

    inline bool HasFlag(ItemFlags2 flag) const { return (Flags2 & flag) != 0; }

    inline bool HasFlag(ItemFlagsCustom customFlag) const { return (FlagsCu & customFlag) != 0; }

private:
    // Cached info
    int32_t _totalAP;

    // Loading Helpers
    void _LoadTotalAP();
};