#include "ItemTemplate.hpp"
#include "src/game/enums/SkillType.hpp"

bool ItemTemplate::HasSignature() const
{
    return GetMaxStackSize() == 1 &&
           Class != ITEM_CLASS_CONSUMABLE &&
           Class != ITEM_CLASS_QUEST &&
           !HasFlag(ITEM_FLAG_NO_CREATOR) &&
           ItemId != 6948; /*Hearthstone*/
}

bool ItemTemplate::CanChangeEquipStateInCombat() const
{
    switch (InventoryType)
    {
        case INVTYPE_RELIC:
        case INVTYPE_SHIELD:
        case INVTYPE_HOLDABLE:
            return true;
    }

    switch (Class)
    {
        case ITEM_CLASS_WEAPON:
        case ITEM_CLASS_PROJECTILE:
            return true;
    }

    return false;
}

float ItemTemplate::getDPS() const
{
    if (!Delay)
        return 0.f;

    float temp = 0.f;
    for (uint8_t i = 0; i < MAX_ITEM_PROTO_DAMAGES; ++i)
        temp += Damage[i].DamageMin + Damage[i].DamageMax;

    return temp * 500.f / Delay;
}

int32_t ItemTemplate::getFeralBonus(int32_t extraDPS /*= 0*/) const
{
    constexpr uint32_t feralApEnabledInventoryTypeMaks = 1 << INVTYPE_WEAPON | 1 << INVTYPE_2HWEAPON | 1 << INVTYPE_WEAPONMAINHAND | 1 << INVTYPE_WEAPONOFFHAND;

    // 0x02A5F3 - is mask for Melee weapon from ItemSubClassMask.dbc
    if (Class == ITEM_CLASS_WEAPON && (1 << InventoryType) & feralApEnabledInventoryTypeMaks)
    {
        int32_t bonus = int32_t((extraDPS + getDPS()) * 14.0f) - 767;
        if (bonus < 0)
            return 0;
        return bonus;
    }

    return 0;
}

float ItemTemplate::GetItemLevelIncludingQuality() const
{
    float itemLevel(ItemLevel);
    switch (Quality)
    {
        case ITEM_QUALITY_POOR:
        case ITEM_QUALITY_NORMAL:
        case ITEM_QUALITY_UNCOMMON:
        case ITEM_QUALITY_ARTIFACT:
        case ITEM_QUALITY_HEIRLOOM:
            itemLevel -= 13.f; // leaving this as a separate statement since we do not know the real behavior in this case
            break;
        case ITEM_QUALITY_RARE:
            itemLevel -= 13.f;
            break;
        case ITEM_QUALITY_EPIC:
        case ITEM_QUALITY_LEGENDARY:
        default:
            break;
    }

    return std::max<float>(0.f, itemLevel);
}

uint32_t ItemTemplate::GetSkill() const
{
    static uint32_t const itemWeaponSkills[MAX_ITEM_SUBCLASS_WEAPON] =
            {
                    SKILL_AXES,     SKILL_2H_AXES,  SKILL_BOWS,          SKILL_GUNS,         SKILL_MACES,
                    SKILL_2H_MACES, SKILL_POLEARMS, SKILL_SWORDS,        SKILL_2H_SWORDS,    0,
                    SKILL_STAVES,   0,              0,                   SKILL_FIST_WEAPONS, 0,
                    SKILL_DAGGERS,  SKILL_THROWN,   SKILL_ASSASSINATION, SKILL_CROSSBOWS,    SKILL_WANDS,
                    SKILL_FISHING
            };

    static uint32_t const itemArmorSkills[MAX_ITEM_SUBCLASS_ARMOR] =
            {
                    0, SKILL_CLOTH, SKILL_LEATHER, SKILL_MAIL, SKILL_PLATE_MAIL, 0, SKILL_SHIELD, 0, 0, 0, 0
            };

    switch (Class)
    {
        case ITEM_CLASS_WEAPON:
            if (SubClass >= MAX_ITEM_SUBCLASS_WEAPON)
                return 0;
            else
                return itemWeaponSkills[SubClass];

        case ITEM_CLASS_ARMOR:
            if (SubClass >= MAX_ITEM_SUBCLASS_ARMOR)
                return 0;
            else
                return itemArmorSkills[SubClass];

        default:
            return 0;
    }
}

void ItemTemplate::_LoadTotalAP()
{
    int32_t totalAP = 0;
    for (uint32_t i = 0; i < StatsCount; ++i)
        if (ItemStat[i].ItemStatType == ITEM_MOD_ATTACK_POWER)
            totalAP += ItemStat[i].ItemStatValue;

    // TODO
//    // some items can have equip spells with +AP
//    for (uint32_t i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
//        if (Spells[i].SpellId > 0 && Spells[i].SpellTrigger == ITEM_SPELLTRIGGER_ON_EQUIP)
//            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(Spells[i].SpellId))
//                for (SpellEffectInfo const& effect : spellInfo->GetEffects())
//                    if (effect.IsAura(SPELL_AURA_MOD_ATTACK_POWER))
//                        totalAP += effect.CalcValue();

    _totalAP = totalAP;
}