#pragma once

#include <cstdint>

enum BAG_FAMILY_MASK
{
    BAG_FAMILY_MASK_NONE                      = 0x00000000,
    BAG_FAMILY_MASK_ARROWS                    = 0x00000001,
    BAG_FAMILY_MASK_BULLETS                   = 0x00000002,
    BAG_FAMILY_MASK_SOUL_SHARDS               = 0x00000004,
    BAG_FAMILY_MASK_LEATHERWORKING_SUPP       = 0x00000008,
    BAG_FAMILY_MASK_INSCRIPTION_SUPP          = 0x00000010,
    BAG_FAMILY_MASK_HERBS                     = 0x00000020,
    BAG_FAMILY_MASK_ENCHANTING_SUPP           = 0x00000040,
    BAG_FAMILY_MASK_ENGINEERING_SUPP          = 0x00000080,
    BAG_FAMILY_MASK_KEYS                      = 0x00000100,
    BAG_FAMILY_MASK_GEMS                      = 0x00000200,
    BAG_FAMILY_MASK_MINING_SUPP               = 0x00000400,
    BAG_FAMILY_MASK_SOULBOUND_EQUIPMENT       = 0x00000800,
    BAG_FAMILY_MASK_VANITY_PETS               = 0x00001000,
    BAG_FAMILY_MASK_CURRENCY_TOKENS           = 0x00002000,
    BAG_FAMILY_MASK_QUEST_ITEMS               = 0x00004000
};

enum ItemClass : uint8_t
{
    ITEM_CLASS_CONSUMABLE                       = 0,
    ITEM_CLASS_CONTAINER                        = 1,
    ITEM_CLASS_WEAPON                           = 2,
    ITEM_CLASS_GEM                              = 3,
    ITEM_CLASS_ARMOR                            = 4,
    ITEM_CLASS_REAGENT                          = 5,
    ITEM_CLASS_PROJECTILE                       = 6,
    ITEM_CLASS_TRADE_GOODS                      = 7,
    ITEM_CLASS_GENERIC                          = 8,
    ITEM_CLASS_RECIPE                           = 9,
    ITEM_CLASS_MONEY                            = 10,
    ITEM_CLASS_QUIVER                           = 11,
    ITEM_CLASS_QUEST                            = 12,
    ITEM_CLASS_KEY                              = 13,
    ITEM_CLASS_PERMANENT                        = 14,
    ITEM_CLASS_MISCELLANEOUS                    = 15,
    ITEM_CLASS_GLYPH                            = 16
};

#define MAX_ITEM_CLASS                            17

enum ItemSubclassConsumable
{
    ITEM_SUBCLASS_CONSUMABLE                    = 0,
    ITEM_SUBCLASS_POTION                        = 1,
    ITEM_SUBCLASS_ELIXIR                        = 2,
    ITEM_SUBCLASS_FLASK                         = 3,
    ITEM_SUBCLASS_SCROLL                        = 4,
    ITEM_SUBCLASS_FOOD_DRINK                    = 5,
    ITEM_SUBCLASS_ITEM_ENHANCEMENT              = 6,
    ITEM_SUBCLASS_BANDAGE                       = 7,
    ITEM_SUBCLASS_CONSUMABLE_OTHER              = 8
};

#define MAX_ITEM_SUBCLASS_CONSUMABLE              9

enum ItemSubclassTradeGoods
{
    ITEM_SUBCLASS_TRADE_GOODS                   = 0,
    ITEM_SUBCLASS_PARTS                         = 1,
    ITEM_SUBCLASS_EXPLOSIVES                    = 2,
    ITEM_SUBCLASS_DEVICES                       = 3,
    ITEM_SUBCLASS_JEWELCRAFTING                 = 4,
    ITEM_SUBCLASS_CLOTH                         = 5,
    ITEM_SUBCLASS_LEATHER                       = 6,
    ITEM_SUBCLASS_METAL_STONE                   = 7,
    ITEM_SUBCLASS_MEAT                          = 8,
    ITEM_SUBCLASS_HERB                          = 9,
    ITEM_SUBCLASS_ELEMENTAL                     = 10,
    ITEM_SUBCLASS_TRADE_GOODS_OTHER             = 11,
    ITEM_SUBCLASS_ENCHANTING                    = 12,
    ITEM_SUBCLASS_MATERIAL                      = 13,
    ITEM_SUBCLASS_ARMOR_ENCHANTMENT             = 14,
    ITEM_SUBCLASS_WEAPON_ENCHANTMENT            = 15
};

#define MAX_ITEM_SUBCLASS_TRADE_GOODS             16

enum ItemFlags : uint32_t
{
    ITEM_FLAG_NO_PICKUP                         = 0x00000001,
    ITEM_FLAG_CONJURED                          = 0x00000002, // Conjured item
    ITEM_FLAG_HAS_LOOT                          = 0x00000004, // Item can be right clicked to open for loot
    ITEM_FLAG_HEROIC_TOOLTIP                    = 0x00000008, // Makes green "Heroic" text appear on item
    ITEM_FLAG_DEPRECATED                        = 0x00000010, // Cannot equip or use
    ITEM_FLAG_NO_USER_DESTROY                   = 0x00000020, // Item can not be destroyed, except by using spell (item can be reagent for spell)
    ITEM_FLAG_PLAYERCAST                        = 0x00000040, // Item's spells are castable by players
    ITEM_FLAG_NO_EQUIP_COOLDOWN                 = 0x00000080, // No default 30 seconds cooldown when equipped
    ITEM_FLAG_MULTI_LOOT_QUEST                  = 0x00000100,
    ITEM_FLAG_IS_WRAPPER                        = 0x00000200, // Item can wrap other items
    ITEM_FLAG_USES_RESOURCES                    = 0x00000400,
    ITEM_FLAG_MULTI_DROP                        = 0x00000800, // Looting this item does not remove it from available loot
    ITEM_FLAG_ITEM_PURCHASE_RECORD              = 0x00001000, // Item can be returned to vendor for its original cost (extended cost)
    ITEM_FLAG_PETITION                          = 0x00002000, // Item is guild or arena charter
    ITEM_FLAG_HAS_TEXT                          = 0x00004000, // Only readable items have this (but not all)
    ITEM_FLAG_NO_DISENCHANT                     = 0x00008000,
    ITEM_FLAG_REAL_DURATION                     = 0x00010000,
    ITEM_FLAG_NO_CREATOR                        = 0x00020000,
    ITEM_FLAG_IS_PROSPECTABLE                   = 0x00040000, // Item can be prospected
    ITEM_FLAG_UNIQUE_EQUIPPABLE                 = 0x00080000, // You can only equip one of these
    ITEM_FLAG_IGNORE_FOR_AURAS                  = 0x00100000,
    ITEM_FLAG_IGNORE_DEFAULT_ARENA_RESTRICTIONS = 0x00200000, // Item can be used during arena match
    ITEM_FLAG_NO_DURABILITY_LOSS                = 0x00400000, // Some Thrown weapons have it (and only Thrown) but not all
    ITEM_FLAG_USE_WHEN_SHAPESHIFTED             = 0x00800000, // Item can be used in shapeshift forms
    ITEM_FLAG_HAS_QUEST_GLOW                    = 0x01000000,
    ITEM_FLAG_HIDE_UNUSABLE_RECIPE              = 0x02000000, // Profession recipes: can only be looted if you meet requirements and don't already know it
    ITEM_FLAG_NOT_USEABLE_IN_ARENA              = 0x04000000, // Item cannot be used in arena
    ITEM_FLAG_IS_BOUND_TO_ACCOUNT               = 0x08000000, // Item binds to account and can be sent only to your own characters
    ITEM_FLAG_NO_REAGENT_COST                   = 0x10000000, // Spell is cast ignoring reagents
    ITEM_FLAG_IS_MILLABLE                       = 0x20000000, // Item can be milled
    ITEM_FLAG_REPORT_TO_GUILD_CHAT              = 0x40000000,
    ITEM_FLAG_NO_PROGRESSIVE_LOOT               = 0x80000000
};

enum ItemFlags2 : uint32_t
{
    ITEM_FLAG2_FACTION_HORDE                            = 0x00000001,
    ITEM_FLAG2_FACTION_ALLIANCE                         = 0x00000002,
    ITEM_FLAG2_DONT_IGNORE_BUY_PRICE                    = 0x00000004, // when item uses extended cost, gold is also required
    ITEM_FLAG2_CLASSIFY_AS_CASTER                       = 0x00000008,
    ITEM_FLAG2_CLASSIFY_AS_PHYSICAL                     = 0x00000010,
    ITEM_FLAG2_EVERYONE_CAN_ROLL_NEED                   = 0x00000020,
    ITEM_FLAG2_NO_TRADE_BIND_ON_ACQUIRE                 = 0x00000040,
    ITEM_FLAG2_CAN_TRADE_BIND_ON_ACQUIRE                = 0x00000080,
    ITEM_FLAG2_CAN_ONLY_ROLL_GREED                      = 0x00000100,
    ITEM_FLAG2_CASTER_WEAPON                            = 0x00000200,
    ITEM_FLAG2_DELETE_ON_LOGIN                          = 0x00000400,
    ITEM_FLAG2_INTERNAL_ITEM                            = 0x00000800,
    ITEM_FLAG2_NO_VENDOR_VALUE                          = 0x00001000,
    ITEM_FLAG2_SHOW_BEFORE_DISCOVERED                   = 0x00002000,
    ITEM_FLAG2_OVERRIDE_GOLD_COST                       = 0x00004000,
    ITEM_FLAG2_IGNORE_DEFAULT_RATED_BG_RESTRICTIONS     = 0x00008000,
    ITEM_FLAG2_NOT_USABLE_IN_RATED_BG                   = 0x00010000,
    ITEM_FLAG2_BNET_ACCOUNT_TRADE_OK                    = 0x00020000,
    ITEM_FLAG2_CONFIRM_BEFORE_USE                       = 0x00040000,
    ITEM_FLAG2_REEVALUATE_BONDING_ON_TRANSFORM          = 0x00080000,
    ITEM_FLAG2_NO_TRANSFORM_ON_CHARGE_DEPLETION         = 0x00100000,
    ITEM_FLAG2_NO_ALTER_ITEM_VISUAL                     = 0x00200000,
    ITEM_FLAG2_NO_SOURCE_FOR_ITEM_VISUAL                = 0x00400000,
    ITEM_FLAG2_IGNORE_QUALITY_FOR_ITEM_VISUAL_SOURCE    = 0x00800000,
    ITEM_FLAG2_NO_DURABILITY                            = 0x01000000,
    ITEM_FLAG2_ROLE_TANK                                = 0x02000000,
    ITEM_FLAG2_ROLE_HEALER                              = 0x04000000,
    ITEM_FLAG2_ROLE_DAMAGE                              = 0x08000000,
    ITEM_FLAG2_CAN_DROP_IN_CHALLENGE_MODE               = 0x10000000,
    ITEM_FLAG2_NEVER_STACK_IN_LOOT_UI                   = 0x20000000,
    ITEM_FLAG2_DISENCHANT_TO_LOOT_TABLE                 = 0x40000000,
    ITEM_FLAG2_USED_IN_A_TRADESKILL                     = 0x80000000
};

enum ItemFlagsCustom
{
    ITEM_FLAGS_CU_DURATION_REAL_TIME    = 0x0001,   // Item duration will tick even if player is offline
    ITEM_FLAGS_CU_IGNORE_QUEST_STATUS   = 0x0002,   // No quest status will be checked when this item drops
    ITEM_FLAGS_CU_FOLLOW_LOOT_RULES     = 0x0004    // Item will always follow group/master/need before greed looting rules
};

enum InventoryType : uint8_t
{
    INVTYPE_NON_EQUIP                           = 0,
    INVTYPE_HEAD                                = 1,
    INVTYPE_NECK                                = 2,
    INVTYPE_SHOULDERS                           = 3,
    INVTYPE_BODY                                = 4,
    INVTYPE_CHEST                               = 5,
    INVTYPE_WAIST                               = 6,
    INVTYPE_LEGS                                = 7,
    INVTYPE_FEET                                = 8,
    INVTYPE_WRISTS                              = 9,
    INVTYPE_HANDS                               = 10,
    INVTYPE_FINGER                              = 11,
    INVTYPE_TRINKET                             = 12,
    INVTYPE_WEAPON                              = 13,
    INVTYPE_SHIELD                              = 14,
    INVTYPE_RANGED                              = 15,
    INVTYPE_CLOAK                               = 16,
    INVTYPE_2HWEAPON                            = 17,
    INVTYPE_BAG                                 = 18,
    INVTYPE_TABARD                              = 19,
    INVTYPE_ROBE                                = 20,
    INVTYPE_WEAPONMAINHAND                      = 21,
    INVTYPE_WEAPONOFFHAND                       = 22,
    INVTYPE_HOLDABLE                            = 23,
    INVTYPE_AMMO                                = 24,
    INVTYPE_THROWN                              = 25,
    INVTYPE_RANGEDRIGHT                         = 26,
    INVTYPE_QUIVER                              = 27,
    INVTYPE_RELIC                               = 28
};

#define MAX_INVTYPE                               29

enum ItemQualities
{
    ITEM_QUALITY_POOR                  = 0,                 //GREY
    ITEM_QUALITY_NORMAL                = 1,                 //WHITE
    ITEM_QUALITY_UNCOMMON              = 2,                 //GREEN
    ITEM_QUALITY_RARE                  = 3,                 //BLUE
    ITEM_QUALITY_EPIC                  = 4,                 //PURPLE
    ITEM_QUALITY_LEGENDARY             = 5,                 //ORANGE
    ITEM_QUALITY_ARTIFACT              = 6,                 //LIGHT YELLOW
    ITEM_QUALITY_HEIRLOOM              = 7,
    MAX_ITEM_QUALITY
};

#define ITEM_SUBCLASS_MASK_WEAPON_RANGED (\
    (1 << ITEM_SUBCLASS_WEAPON_BOW) | (1 << ITEM_SUBCLASS_WEAPON_GUN) |\
    (1 << ITEM_SUBCLASS_WEAPON_CROSSBOW) | (1 << ITEM_SUBCLASS_WEAPON_THROWN))

#define MAX_ITEM_SUBCLASS_WEAPON                  21

enum ItemSubclassArmor
{
    ITEM_SUBCLASS_ARMOR_MISCELLANEOUS           = 0,
    ITEM_SUBCLASS_ARMOR_CLOTH                   = 1,
    ITEM_SUBCLASS_ARMOR_LEATHER                 = 2,
    ITEM_SUBCLASS_ARMOR_MAIL                    = 3,
    ITEM_SUBCLASS_ARMOR_PLATE                   = 4,
    ITEM_SUBCLASS_ARMOR_BUCKLER                 = 5,
    ITEM_SUBCLASS_ARMOR_SHIELD                  = 6,
    ITEM_SUBCLASS_ARMOR_LIBRAM                  = 7,
    ITEM_SUBCLASS_ARMOR_IDOL                    = 8,
    ITEM_SUBCLASS_ARMOR_TOTEM                   = 9,
    ITEM_SUBCLASS_ARMOR_SIGIL                   = 10
};

#define MAX_ITEM_SUBCLASS_ARMOR                   11

enum ItemModType
{
    ITEM_MOD_MANA                     = 0,
    ITEM_MOD_HEALTH                   = 1,
    ITEM_MOD_AGILITY                  = 3,
    ITEM_MOD_STRENGTH                 = 4,
    ITEM_MOD_INTELLECT                = 5,
    ITEM_MOD_SPIRIT                   = 6,
    ITEM_MOD_STAMINA                  = 7,
    ITEM_MOD_DEFENSE_SKILL_RATING     = 12,
    ITEM_MOD_DODGE_RATING             = 13,
    ITEM_MOD_PARRY_RATING             = 14,
    ITEM_MOD_BLOCK_RATING             = 15,
    ITEM_MOD_HIT_MELEE_RATING         = 16,
    ITEM_MOD_HIT_RANGED_RATING        = 17,
    ITEM_MOD_HIT_SPELL_RATING         = 18,
    ITEM_MOD_CRIT_MELEE_RATING        = 19,
    ITEM_MOD_CRIT_RANGED_RATING       = 20,
    ITEM_MOD_CRIT_SPELL_RATING        = 21,
    ITEM_MOD_HIT_TAKEN_MELEE_RATING   = 22,
    ITEM_MOD_HIT_TAKEN_RANGED_RATING  = 23,
    ITEM_MOD_HIT_TAKEN_SPELL_RATING   = 24,
    ITEM_MOD_CRIT_TAKEN_MELEE_RATING  = 25,
    ITEM_MOD_CRIT_TAKEN_RANGED_RATING = 26,
    ITEM_MOD_CRIT_TAKEN_SPELL_RATING  = 27,
    ITEM_MOD_HASTE_MELEE_RATING       = 28,
    ITEM_MOD_HASTE_RANGED_RATING      = 29,
    ITEM_MOD_HASTE_SPELL_RATING       = 30,
    ITEM_MOD_HIT_RATING               = 31,
    ITEM_MOD_CRIT_RATING              = 32,
    ITEM_MOD_HIT_TAKEN_RATING         = 33,
    ITEM_MOD_CRIT_TAKEN_RATING        = 34,
    ITEM_MOD_RESILIENCE_RATING        = 35,
    ITEM_MOD_HASTE_RATING             = 36,
    ITEM_MOD_EXPERTISE_RATING         = 37,
    ITEM_MOD_ATTACK_POWER             = 38,
    ITEM_MOD_RANGED_ATTACK_POWER      = 39,
    //ITEM_MOD_FERAL_ATTACK_POWER       = 40, not in 3.3
    ITEM_MOD_SPELL_HEALING_DONE       = 41,                 // deprecated
    ITEM_MOD_SPELL_DAMAGE_DONE        = 42,                 // deprecated
    ITEM_MOD_MANA_REGENERATION        = 43,
    ITEM_MOD_ARMOR_PENETRATION_RATING = 44,
    ITEM_MOD_SPELL_POWER              = 45,
    ITEM_MOD_HEALTH_REGEN             = 46,
    ITEM_MOD_SPELL_PENETRATION        = 47,
    ITEM_MOD_BLOCK_VALUE              = 48
};

#define MAX_ITEM_MOD                    49

enum ItemSpelltriggerType
{
    ITEM_SPELLTRIGGER_ON_USE          = 0,                  // use after equip cooldown
    ITEM_SPELLTRIGGER_ON_EQUIP        = 1,
    ITEM_SPELLTRIGGER_CHANCE_ON_HIT   = 2,
    ITEM_SPELLTRIGGER_SOULSTONE       = 4,
    /*
     * ItemSpelltriggerType 5 might have changed on 2.4.3/3.0.3: Such auras
     * will be applied on item pickup and removed on item loss - maybe on the
     * other hand the item is destroyed if the aura is removed ("removed on
     * death" of spell 57348 makes me think so)
     */
    ITEM_SPELLTRIGGER_ON_NO_DELAY_USE = 5,                  // no equip cooldown
    ITEM_SPELLTRIGGER_LEARN_SPELL_ID  = 6                   // used in item_template.spell_2 with spell_id with SPELL_GENERIC_LEARN in spell_1
};

#define MAX_ITEM_SPELLTRIGGER           7