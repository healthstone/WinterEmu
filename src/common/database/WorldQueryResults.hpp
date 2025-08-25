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
        item.ItemId = static_cast<uint32_t>(row["entry"].as<int32_t>());
        item.Class = static_cast<uint8_t>(row["class"].as<int16_t>());
        item.SubClass = static_cast<uint8_t>(row["subclass"].as<int16_t>());
        item.SoundOverrideSubclass = row["soundoverridesubclass"].as<int32_t>();
        item.Name1 = row["name"].as<std::string>();
        item.DisplayInfoID = static_cast<uint32_t>(row["displayid"].as<int32_t>());
        item.Quality = static_cast<uint8_t>(row["quality"].as<int16_t>());
        item.Flags = static_cast<uint32_t>(row["flags"].as<int32_t>());
        item.Flags2 = static_cast<uint32_t>(row["flagsextra"].as<int32_t>());
        item.BuyCount = static_cast<uint32_t>(row["buycount"].as<int16_t>());
        item.BuyPrice = row["buyprice"].as<int32_t>();
        item.SellPrice = static_cast<uint32_t>(row["sellprice"].as<int32_t>());
        item.InventoryType = static_cast<uint8_t>(row["inventorytype"].as<int16_t>());
        item.AllowableClass = static_cast<uint32_t>(row["allowableclass"].as<int32_t>());
        item.AllowableRace = static_cast<uint32_t>(row["allowablerace"].as<int32_t>());
        item.ItemLevel = static_cast<uint32_t>(row["itemlevel"].as<int16_t>());
        item.RequiredLevel = static_cast<uint32_t>(row["requiredlevel"].as<int16_t>());
        item.RequiredSkill = static_cast<uint32_t>(row["requiredskill"].as<int16_t>());
        item.RequiredSkillRank = static_cast<uint32_t>(row["requiredskillrank"].as<int16_t>());
        item.RequiredSpell = static_cast<uint32_t>(row["requiredspell"].as<int32_t>());
        item.RequiredHonorRank = static_cast<uint32_t>(row["requiredhonorrank"].as<int32_t>());
        item.RequiredCityRank = static_cast<uint32_t>(row["requiredcityrank"].as<int32_t>());
        item.RequiredReputationFaction = static_cast<uint32_t>(row["requiredreputationfaction"].as<int16_t>());
        item.RequiredReputationRank = static_cast<uint32_t>(row["requiredreputationrank"].as<int16_t>());
        item.MaxCount = row["maxcount"].as<int32_t>();
        item.Stackable = row["stackable"].as<int32_t>();
        item.ContainerSlots = static_cast<uint32_t>(row["containerslots"].as<int16_t>());
        item.StatsCount = static_cast<uint32_t>(row["statscount"].as<int16_t>());

        // Статистические характеристики
        for (int i = 0; i < MAX_ITEM_PROTO_STATS; i++) {
            std::string statTypeCol = "stat_type" + std::to_string(i + 1);
            std::string statValueCol = "stat_value" + std::to_string(i + 1);
            item.ItemStat[i].ItemStatType = static_cast<uint32_t>(row[statTypeCol].as<int16_t>());
            item.ItemStat[i].ItemStatValue = row[statValueCol].as<int32_t>();
        }

        item.ScalingStatDistribution = static_cast<uint32_t>(row["scalingstatdistribution"].as<int16_t>());
        item.ScalingStatValue = static_cast<uint32_t>(row["scalingstatvalue"].as<int32_t>());

        // Урон
        for (int i = 0; i < MAX_ITEM_PROTO_DAMAGES; i++) {
            std::string dmgMinCol = "dmg_min" + std::to_string(i + 1);
            std::string dmgMaxCol = "dmg_max" + std::to_string(i + 1);
            std::string dmgTypeCol = "dmg_type" + std::to_string(i + 1);
            item.Damage[i].DamageMin = row[dmgMinCol].as<float>();
            item.Damage[i].DamageMax = row[dmgMaxCol].as<float>();
            item.Damage[i].DamageType = static_cast<uint32_t>(row[dmgTypeCol].as<int16_t>());
        }

        item.Armor = static_cast<uint32_t>(row["armor"].as<int16_t>());
        item.HolyRes = static_cast<uint32_t>(row["holy_res"].as<int16_t>());
        item.FireRes = static_cast<uint32_t>(row["fire_res"].as<int16_t>());
        item.NatureRes = static_cast<uint32_t>(row["nature_res"].as<int16_t>());
        item.FrostRes = static_cast<uint32_t>(row["frost_res"].as<int16_t>());
        item.ShadowRes = static_cast<uint32_t>(row["shadow_res"].as<int16_t>());
        item.ArcaneRes = static_cast<uint32_t>(row["arcane_res"].as<int16_t>());
        item.Delay = static_cast<uint32_t>(row["delay"].as<int16_t>());
        item.AmmoType = static_cast<uint32_t>(row["ammo_type"].as<int16_t>());
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
            item.Spells[i].SpellTrigger = static_cast<uint32_t>(row[spellTriggerCol].as<int16_t>());
            item.Spells[i].SpellCharges = row[spellChargesCol].as<int32_t>();
            item.Spells[i].SpellPPMRate = row[spellPpmRateCol].as<float>();
            item.Spells[i].SpellCooldown = row[spellCooldownCol].as<int32_t>();
            item.Spells[i].SpellCategory = static_cast<uint32_t>(row[spellCategoryCol].as<int16_t>());
            item.Spells[i].SpellCategoryCooldown = row[spellCategoryCooldownCol].as<int32_t>();
        }

        item.Bonding = static_cast<uint32_t>(row["bonding"].as<int16_t>());
        item.Description = row["description"].as<std::string>();
        item.PageText = static_cast<uint32_t>(row["pagetext"].as<int32_t>());
        item.LanguageID = static_cast<uint32_t>(row["languageid"].as<int16_t>());
        item.PageMaterial = static_cast<uint32_t>(row["pagematerial"].as<int16_t>());
        item.StartQuest = static_cast<uint32_t>(row["startquest"].as<int32_t>());
        item.LockID = static_cast<uint32_t>(row["lockid"].as<int32_t>());
        item.Material = row["material"].as<int32_t>();
        item.Sheath = static_cast<uint32_t>(row["sheath"].as<int16_t>());
        item.RandomProperty = row["randomproperty"].as<int32_t>();
        item.RandomSuffix = row["randomsuffix"].as<int32_t>();
        item.Block = static_cast<uint32_t>(row["block"].as<int32_t>());
        item.ItemSet = static_cast<uint32_t>(row["itemset"].as<int32_t>());
        item.MaxDurability = static_cast<uint32_t>(row["maxdurability"].as<int16_t>());
        item.Area = static_cast<uint32_t>(row["area"].as<int32_t>());
        item.Map = static_cast<uint32_t>(row["map"].as<int16_t>());
        item.BagFamily = static_cast<uint32_t>(row["bagfamily"].as<int32_t>());
        item.TotemCategory = static_cast<uint32_t>(row["totemcategory"].as<int32_t>());

        // Сокеты
        for (int i = 0; i < MAX_ITEM_PROTO_SOCKETS; i++) {
            std::string socketColorCol = "socketcolor_" + std::to_string(i + 1);
            std::string socketContentCol = "socketcontent_" + std::to_string(i + 1);
            item.Socket[i].Color = static_cast<uint32_t>(row[socketColorCol].as<int16_t>());
            item.Socket[i].Content = static_cast<uint32_t>(row[socketContentCol].as<int32_t>());
        }

        item.socketBonus = static_cast<uint32_t>(row["socketbonus"].as<int32_t>());
        item.GemProperties = static_cast<uint32_t>(row["gemproperties"].as<int32_t>());
        item.RequiredDisenchantSkill = static_cast<uint32_t>(row["requireddisenchantskill"].as<int16_t>());
        item.ArmorDamageModifier = row["armordamagemodifier"].as<float>();
        item.Duration = static_cast<uint32_t>(row["duration"].as<int32_t>());
        item.ItemLimitCategory = static_cast<uint32_t>(row["itemlimitcategory"].as<int16_t>());
        item.HolidayId = static_cast<uint32_t>(row["holidayid"].as<int32_t>());
        item.ScriptName  = row["scriptname"].as<std::string>();
        item.DisenchantID = static_cast<uint32_t>(row["disenchantid"].as<int32_t>());
        item.FoodType = static_cast<uint32_t>(row["foodtype"].as<int16_t>());
        item.MinMoneyLoot = static_cast<uint32_t>(row["minmoneyloot"].as<int32_t>());
        item.MaxMoneyLoot = static_cast<uint32_t>(row["maxmoneyloot"].as<int32_t>());
        item.FlagsCu = static_cast<uint32_t>(row["flagscustom"].as<int32_t>());

        if (!row["verifiedbuild"].is_null()) {
            item.VerifiedBuild = static_cast<uint32_t>(row["verifiedbuild"].as<int32_t>());
        }

        return item;
    }
};