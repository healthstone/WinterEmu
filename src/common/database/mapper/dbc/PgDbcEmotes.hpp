#pragma once

#include <pqxx/pqxx>
#include <string>
#include <optional>
#include "QueryResults.hpp"

/** dbc_emotes **/
struct DbcEmotes {
    uint32_t ID;
    std::optional<std::string> EmoteSlashCommand;
    uint32_t AnimID;
    uint32_t EmoteFlags;
    uint32_t EmoteSpecProc;
    uint32_t EmoteSpecProcParam;
    int32_t EventSoundID;
};

template<>
struct PgRowMapper<DbcEmotes> {

    static DbcEmotes map(const pqxx::row& r) {
        DbcEmotes row;

        row.ID = r["id"].as<uint32_t>();
        row.EmoteSlashCommand = get_optional_string(r, "emoteslashcommand");
        row.AnimID = r["animid"].as<uint32_t>();
        row.EmoteFlags = r["emoteflags"].as<uint32_t>();
        row.EmoteSpecProc = r["emotespecproc"].as<uint32_t>();
        row.EmoteSpecProcParam = r["emotespecprocparam"].as<uint32_t>();
        row.EventSoundID = r["eventsoundid"].as<int32_t>();

        return row;
    }
};
