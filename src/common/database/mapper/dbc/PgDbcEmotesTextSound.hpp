#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

/** dbc_emotestextsound **/
struct DbcEmotesTextSound {
    uint32_t ID;
    uint32_t EmotesTextID;
    uint8_t RaceID;
    uint8_t SexID;
    uint32_t SoundID;
};

template<>
struct PgRowMapper<DbcEmotesTextSound> {

    static DbcEmotesTextSound map(const pqxx::row& r) {
        DbcEmotesTextSound row;

        row.ID = r["id"].as<uint32_t>();
        row.EmotesTextID = r["emotestextid"].as<uint32_t>();
        row.RaceID = static_cast<uint8_t>(r["raceid"].as<int>());
        row.SexID = static_cast<uint8_t>(r["sexid"].as<int>());
        row.SoundID = r["soundid"].as<uint32_t>();

        return row;
    }
};
