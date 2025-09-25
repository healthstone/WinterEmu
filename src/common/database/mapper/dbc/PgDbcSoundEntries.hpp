#pragma once

#include <pqxx/pqxx>
#include <cstdint>
#include <optional>
#include "QueryResults.hpp"

struct DbcSoundEntries {
    uint32_t id;
    uint32_t soundtype;
    std::optional<std::string> name;
    std::optional<std::string> file_1;
    std::optional<std::string> file_2;
    std::optional<std::string> file_3;
    std::optional<std::string> file_4;
    std::optional<std::string> file_5;
    std::optional<std::string> file_6;
    std::optional<std::string> file_7;
    std::optional<std::string> file_8;
    std::optional<std::string> file_9;
    std::optional<std::string> file_10;
    uint32_t freq_1;
    uint32_t freq_2;
    uint32_t freq_3;
    uint32_t freq_4;
    uint32_t freq_5;
    uint32_t freq_6;
    uint32_t freq_7;
    uint32_t freq_8;
    uint32_t freq_9;
    uint32_t freq_10;
    std::optional<std::string> directorybase;
    float volumefloat;
    uint32_t flags;
    float mindistance;
    float distancecutoff;
    uint32_t eaxdef;
    uint32_t soundentriesadvancedid;
};

template<>
struct PgRowMapper<DbcSoundEntries> {
    static DbcSoundEntries map(const pqxx::row& r) {
        DbcSoundEntries obj;
        obj.id = r["id"].as<uint32_t>();
        obj.soundtype = r["soundtype"].as<uint32_t>();
        obj.name = get_optional_string(r, "name");
        obj.file_1 = get_optional_string(r, "file_1");
        obj.file_2 = get_optional_string(r, "file_2");
        obj.file_3 = get_optional_string(r, "file_3");
        obj.file_4 = get_optional_string(r, "file_4");
        obj.file_5 = get_optional_string(r, "file_5");
        obj.file_6 = get_optional_string(r, "file_6");
        obj.file_7 = get_optional_string(r, "file_7");
        obj.file_8 = get_optional_string(r, "file_8");
        obj.file_9 = get_optional_string(r, "file_9");
        obj.file_10 = get_optional_string(r, "file_10");
        obj.freq_1 = r["freq_1"].as<uint32_t>();
        obj.freq_2 = r["freq_2"].as<uint32_t>();
        obj.freq_3 = r["freq_3"].as<uint32_t>();
        obj.freq_4 = r["freq_4"].as<uint32_t>();
        obj.freq_5 = r["freq_5"].as<uint32_t>();
        obj.freq_6 = r["freq_6"].as<uint32_t>();
        obj.freq_7 = r["freq_7"].as<uint32_t>();
        obj.freq_8 = r["freq_8"].as<uint32_t>();
        obj.freq_9 = r["freq_9"].as<uint32_t>();
        obj.freq_10 = r["freq_10"].as<uint32_t>();
        obj.directorybase = get_optional_string(r, "directorybase");
        obj.volumefloat = r["volumefloat"].as<float>();
        obj.flags = r["flags"].as<uint32_t>();
        obj.mindistance = r["mindistance"].as<float>();
        obj.distancecutoff = r["distancecutoff"].as<float>();
        obj.eaxdef = r["eaxdef"].as<uint32_t>();
        obj.soundentriesadvancedid = r["soundentriesadvancedid"].as<uint32_t>();
        return obj;
    }
};

