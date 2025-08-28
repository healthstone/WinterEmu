#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <chrono>
#include <optional>
#include <algorithm>
#include "utils/TimeUtils.hpp"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/string_generator.hpp>

#include "QueryResults.hpp"
#include "src/game/Entity/Item/ItemTemplate/ItemTemplate.hpp"

// === Специализация PgRowMapper для ItemTemplate ===
template<>
struct PgRowMapper<ItemTemplate> {
    static ItemTemplate map(const pqxx::row &row) {
        ItemTemplate item;

        // Базовые поля
        item.ItemId = row["entry"].as<uint32_t>();
        item.Class = static_cast<uint8_t>(row["class"].as<int>());
        item.SubClass = static_cast<uint8_t>(row["subclass"].as<int>());
        item.SoundOverrideSubclass = row["soundoverridesubclass"].as<int32_t>();
        item.Name1 = row["name"].as<std::string>();
        item.DisplayInfoID = row["displayid"].as<uint32_t>();
        item.Quality = static_cast<uint8_t>(row["quality"].as<int>());
        item.Flags = row["flags"].as<uint32_t>();
        item.Flags2 = row["flagsextra"].as<uint32_t>();
        item.BuyCount = row["buycount"].as<uint32_t>();
        item.BuyPrice = row["buyprice"].as<int32_t>();
        item.SellPrice = row["sellprice"].as<uint32_t>();
        item.InventoryType = static_cast<uint8_t>(row["inventorytype"].as<int>());
        item.AllowableClass = row["allowableclass"].as<uint32_t>();
        item.AllowableRace = row["allowablerace"].as<uint32_t>();
        item.ItemLevel = row["itemlevel"].as<uint32_t>();
        item.RequiredLevel = row["requiredlevel"].as<uint32_t>();
        item.RequiredSkill = row["requiredskill"].as<uint32_t>();
        item.RequiredSkillRank = row["requiredskillrank"].as<uint32_t>();
        item.RequiredSpell = row["requiredspell"].as<uint32_t>();
        item.RequiredHonorRank = row["requiredhonorrank"].as<uint32_t>();
        item.RequiredCityRank = row["requiredcityrank"].as<uint32_t>();
        item.RequiredReputationFaction = row["requiredreputationfaction"].as<uint32_t>();
        item.RequiredReputationRank = row["requiredreputationrank"].as<uint32_t>();
        item.MaxCount = row["maxcount"].as<int32_t>();
        item.Stackable = row["stackable"].as<int32_t>();
        item.ContainerSlots = row["containerslots"].as<uint32_t>();
        item.StatsCount = row["statscount"].as<uint32_t>();

        // Статистические характеристики
        for (int i = 0; i < MAX_ITEM_PROTO_STATS; i++) {
            std::string statTypeCol = "stat_type" + std::to_string(i + 1);
            std::string statValueCol = "stat_value" + std::to_string(i + 1);
            item.ItemStat[i].ItemStatType = row[statTypeCol].as<uint32_t>();
            item.ItemStat[i].ItemStatValue = row[statValueCol].as<int32_t>();
        }

        item.ScalingStatDistribution = row["scalingstatdistribution"].as<uint32_t>();
        item.ScalingStatValue = row["scalingstatvalue"].as<uint32_t>();

        // Урон
        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; i++) {
            std::string dmgMinCol = "dmg_min" + std::to_string(i + 1);
            std::string dmgMaxCol = "dmg_max" + std::to_string(i + 1);
            std::string dmgTypeCol = "dmg_type" + std::to_string(i + 1);
            item.Damage[i].DamageMin = row[dmgMinCol].as<float>();
            item.Damage[i].DamageMax = row[dmgMaxCol].as<float>();
            item.Damage[i].DamageType = row[dmgTypeCol].as<uint32_t>();
        }

        item.Armor = row["armor"].as<uint32_t>();
        item.HolyRes = row["holy_res"].as<uint32_t>();
        item.FireRes = row["fire_res"].as<uint32_t>();
        item.NatureRes = row["nature_res"].as<uint32_t>();
        item.FrostRes = row["frost_res"].as<uint32_t>();
        item.ShadowRes = row["shadow_res"].as<uint32_t>();
        item.ArcaneRes = row["arcane_res"].as<uint32_t>();
        item.Delay = row["delay"].as<uint32_t>();
        item.AmmoType = row["ammo_type"].as<uint32_t>();
        item.RangedModRange = row["rangedmodrange"].as<float>();

        // Заклинания
        for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; i++) {
            std::string spellIdCol = "spellid_" + std::to_string(i + 1);
            std::string spellTriggerCol = "spelltrigger_" + std::to_string(i + 1);
            std::string spellChargesCol = "spellcharges_" + std::to_string(i + 1);
            std::string spellPpmRateCol = "spellppmrate_" + std::to_string(i + 1);
            std::string spellCooldownCol = "spellcooldown_" + std::to_string(i + 1);
            std::string spellCategoryCol = "spellcategory_" + std::to_string(i + 1);
            std::string spellCategoryCooldownCol = "spellcategorycooldown_" + std::to_string(i + 1);

            item.Spells[i].SpellId = row[spellIdCol].as<int32_t>();
            item.Spells[i].SpellTrigger = row[spellTriggerCol].as<uint32_t>();
            item.Spells[i].SpellCharges = row[spellChargesCol].as<int32_t>();
            item.Spells[i].SpellPPMRate = row[spellPpmRateCol].as<float>();
            item.Spells[i].SpellCooldown = row[spellCooldownCol].as<int32_t>();
            item.Spells[i].SpellCategory = row[spellCategoryCol].as<uint32_t>();
            item.Spells[i].SpellCategoryCooldown = row[spellCategoryCooldownCol].as<int32_t>();
        }

        item.Bonding = row["bonding"].as<uint32_t>();
        item.Description = get_optional_string(row, "description").value_or("");
        item.PageText = row["pagetext"].as<uint32_t>();
        item.LanguageID = row["languageid"].as<uint32_t>();
        item.PageMaterial = row["pagematerial"].as<uint32_t>();
        item.StartQuest = row["startquest"].as<uint32_t>();
        item.LockID = row["lockid"].as<uint32_t>();
        item.Material = row["material"].as<int32_t>();
        item.Sheath = row["sheath"].as<uint32_t>();
        item.RandomProperty = row["randomproperty"].as<int32_t>();
        item.RandomSuffix = row["randomsuffix"].as<int32_t>();
        item.Block = row["block"].as<uint32_t>();
        item.ItemSet = row["itemset"].as<uint32_t>();
        item.MaxDurability = row["maxdurability"].as<uint32_t>();
        item.Area = row["area"].as<uint32_t>();
        item.Map = row["map"].as<uint32_t>();
        item.BagFamily = row["bagfamily"].as<uint32_t>();
        item.TotemCategory = row["totemcategory"].as<uint32_t>();

        // Сокеты
        for (int i = 0; i < MAX_ITEM_PROTO_SOCKETS; i++) {
            std::string socketColorCol = "socketcolor_" + std::to_string(i + 1);
            std::string socketContentCol = "socketcontent_" + std::to_string(i + 1);
            item.Socket[i].Color = row[socketColorCol].as<uint32_t>();
            item.Socket[i].Content = row[socketContentCol].as<uint32_t>();
        }

        item.socketBonus = row["socketbonus"].as<uint32_t>();
        item.GemProperties = row["gemproperties"].as<uint32_t>();
        item.RequiredDisenchantSkill = row["requireddisenchantskill"].as<uint32_t>();
        item.ArmorDamageModifier = row["armordamagemodifier"].as<float>();
        item.Duration = row["duration"].as<uint32_t>();
        item.ItemLimitCategory = row["itemlimitcategory"].as<uint32_t>();
        item.HolidayId = row["holidayid"].as<uint32_t>();
        item.ScriptName  = get_optional_string(row, "scriptname").value_or("");
        item.DisenchantID = row["disenchantid"].as<uint32_t>();
        item.FoodType = row["foodtype"].as<uint32_t>();
        item.MinMoneyLoot = row["minmoneyloot"].as<uint32_t>();
        item.MaxMoneyLoot = row["maxmoneyloot"].as<uint32_t>();
        item.FlagsCu = row["flagscustom"].as<uint32_t>();
        item.VerifiedBuild = get_optional_number<uint32_t>(row, "verifiedbuild").value_or(0);

        return item;
    }
};
