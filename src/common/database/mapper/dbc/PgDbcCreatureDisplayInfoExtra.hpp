#pragma once

#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <optional>
#include "QueryResults.hpp"

/** dbc_creaturedisplayinfoextra **/
struct DbcCreatureDisplayInfoExtra {
    uint32_t ID;
    uint32_t DisplayRaceID;
    uint8_t DisplaySexID;
    uint32_t SkinID;
    uint32_t FaceID;
    uint32_t HairStyleID;
    uint32_t HairColorID;
    uint32_t FacialHairID;
    uint32_t NPCItemDisplay[11];
    uint32_t Flags;
    std::optional<std::string> BakeName;
};

template<>
struct PgRowMapper<DbcCreatureDisplayInfoExtra> {
    static DbcCreatureDisplayInfoExtra map(const pqxx::row& r) {
        DbcCreatureDisplayInfoExtra row;

        row.ID = r["id"].as<uint32_t>();
        row.DisplayRaceID = r["displayraceid"].as<uint32_t>();
        row.DisplaySexID = static_cast<uint8_t>(r["displaysexid"].as<int>());
        row.SkinID = r["skinid"].as<uint32_t>();
        row.FaceID = r["faceid"].as<uint32_t>();
        row.HairStyleID = r["hairstyleid"].as<uint32_t>();
        row.HairColorID = r["haircolorid"].as<uint32_t>();
        row.FacialHairID = r["facialhairid"].as<uint32_t>();

        row.NPCItemDisplay[0]  = r["npcitemdisplay_1"].as<uint32_t>();
        row.NPCItemDisplay[1]  = r["npcitemdisplay_2"].as<uint32_t>();
        row.NPCItemDisplay[2]  = r["npcitemdisplay_3"].as<uint32_t>();
        row.NPCItemDisplay[3]  = r["npcitemdisplay_4"].as<uint32_t>();
        row.NPCItemDisplay[4]  = r["npcitemdisplay_5"].as<uint32_t>();
        row.NPCItemDisplay[5]  = r["npcitemdisplay_6"].as<uint32_t>();
        row.NPCItemDisplay[6]  = r["npcitemdisplay_7"].as<uint32_t>();
        row.NPCItemDisplay[7]  = r["npcitemdisplay_8"].as<uint32_t>();
        row.NPCItemDisplay[8]  = r["npcitemdisplay_9"].as<uint32_t>();
        row.NPCItemDisplay[9]  = r["npcitemdisplay_10"].as<uint32_t>();
        row.NPCItemDisplay[10] = r["npcitemdisplay_11"].as<uint32_t>();

        row.Flags = r["flags"].as<uint32_t>();
        row.BakeName = get_optional_string(r, "bakename");

        return row;
    }
};
