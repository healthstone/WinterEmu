#pragma once

#include <pqxx/pqxx>
#include <cstdint>

#include "QueryResults.hpp"

/** dbc_bannedaddons **/
struct DbcBannedAddons {
    uint32_t ID;
    int64_t NameMD5_1;
    int64_t NameMD5_2;
    int64_t NameMD5_3;
    int64_t NameMD5_4;
    int64_t VersionMD5_1;
    int64_t VersionMD5_2;
    int64_t VersionMD5_3;
    int64_t VersionMD5_4;
    uint32_t LastModified;
    uint32_t Flags;
};

template<>
struct PgRowMapper<DbcBannedAddons> {
    static DbcBannedAddons map(const pqxx::row& r) {
        DbcBannedAddons row;

        row.ID           = r["id"].as<uint32_t>();
        row.NameMD5_1    = r["namemd5_1"].as<int64_t>();
        row.NameMD5_2    = r["namemd5_2"].as<int64_t>();
        row.NameMD5_3    = r["namemd5_3"].as<int64_t>();
        row.NameMD5_4    = r["namemd5_4"].as<int64_t>();
        row.VersionMD5_1 = r["versionmd5_1"].as<int64_t>();
        row.VersionMD5_2 = r["versionmd5_2"].as<int64_t>();
        row.VersionMD5_3 = r["versionmd5_3"].as<int64_t>();
        row.VersionMD5_4 = r["versionmd5_4"].as<int64_t>();
        row.LastModified = r["lastmodified"].as<uint32_t>();
        row.Flags        = r["flags"].as<uint32_t>();

        return row;
    }
};
