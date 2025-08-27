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

/** dbc_charstartoutfit **/
struct DbcCharStartOutfit {
    uint32_t ID;
    uint8_t RaceID;
    uint8_t ClassID;
    uint8_t SexID;
    int16_t OutfitID;

    int32_t ItemID[24];
    int32_t DisplayItemID[24];
    int32_t InventoryType[24];
};

template<>
struct PgRowMapper<DbcCharStartOutfit> {
    static DbcCharStartOutfit map(const pqxx::row& r) {
        DbcCharStartOutfit row;

        row.ID = static_cast<uint32_t>(r["id"].as<int32_t>());
        row.RaceID = static_cast<uint8_t>(r["raceid"].as<int16_t>());
        row.ClassID = static_cast<uint8_t>(r["classid"].as<int16_t>());
        row.SexID = static_cast<uint8_t>(r["sexid"].as<int16_t>());
        row.OutfitID = r["outfitid"].as<int16_t>();

        // itemid_1..24
        for (int i = 0; i < 24; i++) {
            std::string col = "itemid_" + std::to_string(i + 1);
            row.ItemID[i] = r[col].as<int32_t>();
        }

        // displayitemid_1..24
        for (int i = 0; i < 24; i++) {
            std::string col = "displayitemid_" + std::to_string(i + 1);
            row.DisplayItemID[i] = r[col].as<int32_t>();
        }

        // inventorytype_1..24
        for (int i = 0; i < 24; i++) {
            std::string col = "inventorytype_" + std::to_string(i + 1);
            row.InventoryType[i] = r[col].as<int32_t>();
        }

        return row;
    }
};