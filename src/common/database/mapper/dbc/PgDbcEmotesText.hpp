#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_emotestext **/
struct DbcEmotesText {
    uint32_t ID;
    std::optional<std::string> Name;
    uint32_t EmoteID;
    uint32_t EmoteText[16];  // emotetext_1 ... emotetext_16
};

template<>
struct PgRowMapper<DbcEmotesText> {

    static DbcEmotesText map(const pqxx::row& r) {
        DbcEmotesText row;

        row.ID = r["id"].as<uint32_t>();
        row.Name = get_optional_string(r, "name");
        row.EmoteID = r["emoteid"].as<uint32_t>();

        row.EmoteText[0] = r["emotetext_1"].as<uint32_t>();
        row.EmoteText[1] = r["emotetext_2"].as<uint32_t>();
        row.EmoteText[2] = r["emotetext_3"].as<uint32_t>();
        row.EmoteText[3] = r["emotetext_4"].as<uint32_t>();
        row.EmoteText[4] = r["emotetext_5"].as<uint32_t>();
        row.EmoteText[5] = r["emotetext_6"].as<uint32_t>();
        row.EmoteText[6] = r["emotetext_7"].as<uint32_t>();
        row.EmoteText[7] = r["emotetext_8"].as<uint32_t>();
        row.EmoteText[8] = r["emotetext_9"].as<uint32_t>();
        row.EmoteText[9] = r["emotetext_10"].as<uint32_t>();
        row.EmoteText[10] = r["emotetext_11"].as<uint32_t>();
        row.EmoteText[11] = r["emotetext_12"].as<uint32_t>();
        row.EmoteText[12] = r["emotetext_13"].as<uint32_t>();
        row.EmoteText[13] = r["emotetext_14"].as<uint32_t>();
        row.EmoteText[14] = r["emotetext_15"].as<uint32_t>();
        row.EmoteText[15] = r["emotetext_16"].as<uint32_t>();

        return row;
    }
};
