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

// === Структуры ===

struct CharacterEnumRow {
    uint32_t guid;                    // Изменено с UUID на uint32_t
    std::string name;
    uint8_t race;
    uint8_t class_;
    uint8_t gender;
    uint8_t skin;
    uint8_t face;
    uint8_t hairStyle;
    uint8_t hairColor;
    uint8_t facialStyle;
    uint8_t level;
    uint16_t zone;
    uint16_t map;
    float position_x;
    float position_y;
    float position_z;
    std::optional<uint32_t> guildid;  // Изменено с UUID на uint32_t
    uint32_t playerFlags;
    uint16_t at_login;
    std::optional<uint32_t> pet_entry;
    std::optional<uint32_t> pet_modelid;
    std::optional<uint16_t> pet_level;
    std::string equipmentCache;
    std::optional<uint32_t> banned_guid; // Изменено с UUID на uint32_t
};

struct AddonRow {
    std::string name;
    uint32_t crc;
};

struct BannedAddonRow {
    uint32_t id;
    std::string name;
    std::string version;
    uint32_t timestamp; // UNIX timestamp в секундах
};

struct AccountTutorialRow {
    boost::uuids::uuid account_id;    // Оставляем UUID (не передается в пакетах)
    uint32_t tut0;
    uint32_t tut1;
    uint32_t tut2;
    uint32_t tut3;
    uint32_t tut4;
    uint32_t tut5;
    uint32_t tut6;
    uint32_t tut7;
};

struct CharacterAccountDataRow {
    uint32_t guid;                    // Изменено с UUID на uint32_t
    uint8_t type;
    uint32_t time;
    std::vector<uint8_t> data;
};

struct AccountDataRow {
    boost::uuids::uuid account_id;    // Оставляем UUID (не передается в пакетах)
    uint8_t type;
    uint32_t time;
    std::vector<uint8_t> data;
};

struct AccountsRow {
    boost::uuids::uuid id;            // Оставляем UUID (не передается в пакетах)
    std::optional<std::string> name;
    std::optional<std::array<uint8_t, 32>> salt;
    std::optional<std::array<uint8_t, 32>> verifier;
    std::optional<std::array<uint8_t, 40>> sessionkey;
    std::optional<std::string> email;
    std::optional<std::chrono::system_clock::time_point> created_at;
};

struct BuildInfoRow {
    std::optional<uint32_t> majorVersion;
    std::optional<uint32_t> minorVersion;
    std::optional<uint32_t> bugfixVersion;
    std::optional<std::string> hotfixVersion;
    uint32_t build; // Primary Key, NOT NULL
};

struct BuildExeHashRow {
    uint32_t build;
    std::string platform;
    std::array<uint8_t, 20> executableHash; // Например SHA1 = 20 байт
};

struct RealmRow {
    uint32_t id;
    std::string name;
    std::string address;
    std::string localAddress;
    std::string localSubnetMask;
    uint16_t port;
    uint8_t icon;
    uint8_t flag;
    uint8_t timezone;
    uint8_t securityLevel;
    float population;
    uint32_t build;
};

struct RealmCharactersRow {
    uint32_t realmid;
    uint16_t numchars;
};

struct NothingRow {};

// === Шаблон PgRowMapper ===

template<typename T>
struct PgRowMapper;

template<>
struct PgRowMapper<CharacterEnumRow> {
    static CharacterEnumRow map(const pqxx::row& r) {
        CharacterEnumRow row;

        // GUID персонажа (теперь uint32_t)
        row.guid = r["guid"].as<uint32_t>();

        row.name = r["name"].as<std::string>();
        row.race = static_cast<uint8_t>(r["race"].as<int>());
        row.class_ = static_cast<uint8_t>(r["class"].as<int>());
        row.gender = static_cast<uint8_t>(r["gender"].as<int>());
        row.skin = static_cast<uint8_t>(r["skin"].as<int>());
        row.face = static_cast<uint8_t>(r["face"].as<int>());
        row.hairStyle = static_cast<uint8_t>(r["hairStyle"].as<int>());
        row.hairColor = static_cast<uint8_t>(r["hairColor"].as<int>());
        row.facialStyle = static_cast<uint8_t>(r["facialStyle"].as<int>());
        row.level = static_cast<uint8_t>(r["level"].as<int>());
        row.zone = static_cast<uint16_t>(r["zone"].as<int>());
        row.map = static_cast<uint16_t>(r["map"].as<int>());
        row.position_x = r["position_x"].as<float>();
        row.position_y = r["position_y"].as<float>();
        row.position_z = r["position_z"].as<float>();

        // GUID гильдии (теперь uint32_t)
        if (!r["guildid"].is_null()) {
            row.guildid = r["guildid"].as<uint32_t>();
        }

        row.playerFlags = r["playerFlags"].as<uint32_t>();
        row.at_login = static_cast<uint16_t>(r["at_login"].as<int>());

        if (!r["entry"].is_null()) row.pet_entry = r["entry"].as<uint32_t>();
        if (!r["modelid"].is_null()) row.pet_modelid = r["modelid"].as<uint32_t>();
        if (!r["level"].is_null()) row.pet_level = static_cast<uint16_t>(r["level"].as<int>());

        row.equipmentCache = r["equipmentCache"].as<std::string>();

        // GUID бана (теперь uint32_t)
        if (!r["guid"].is_null()) {
            row.banned_guid = r["guid"].as<uint32_t>();
        }

        return row;
    }
};

template<>
struct PgRowMapper<AddonRow> {
    static AddonRow map(const pqxx::row& r) {
        AddonRow row;
        row.name = r["name"].as<std::string>();
        row.crc = r["crc"].as<uint32_t>();
        return row;
    }
};

template<>
struct PgRowMapper<BannedAddonRow> {
    static BannedAddonRow map(const pqxx::row& r) {
        BannedAddonRow row;
        row.id = r["id"].as<uint32_t>();
        row.name = r["name"].as<std::string>();
        row.version = r["version"].as<std::string>();

        // Обработка временной метки
        if (r["timestamp"].is_null()) {
            row.timestamp = 0;
        } else {
            auto tp = TimeUtils::parse_pg_timestamp(r["timestamp"].as<std::string>());
            row.timestamp = TimeUtils::to_unix_time(tp);
        }
        return row;
    }
};

template<>
struct PgRowMapper<AccountTutorialRow> {
    static AccountTutorialRow map(const pqxx::row& r) {
        AccountTutorialRow row;

        // Парсинг UUID account_id (оставляем UUID)
        std::string account_id_str = r["account_id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.account_id = gen(account_id_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("PgRowMapper: invalid UUID in account_id: " + account_id_str);
        }

        // Чтение tutorial flags
        row.tut0 = r["tut0"].as<uint32_t>();
        row.tut1 = r["tut1"].as<uint32_t>();
        row.tut2 = r["tut2"].as<uint32_t>();
        row.tut3 = r["tut3"].as<uint32_t>();
        row.tut4 = r["tut4"].as<uint32_t>();
        row.tut5 = r["tut5"].as<uint32_t>();
        row.tut6 = r["tut6"].as<uint32_t>();
        row.tut7 = r["tut7"].as<uint32_t>();

        return row;
    }
};

template<>
struct PgRowMapper<CharacterAccountDataRow> {
    static CharacterAccountDataRow map(const pqxx::row& r) {
        CharacterAccountDataRow row;

        // GUID персонажа (теперь uint32_t)
        row.guid = r["guid"].as<uint32_t>();

        // Тип данных (0-7)
        row.type = static_cast<uint8_t>(r["type"].as<int>());

        // Временная метка
        row.time = r["time"].as<uint32_t>();

        // Бинарные данные
        if (!r["data"].is_null()) {
            pqxx::binarystring data_bin(r["data"]);
            row.data = std::vector<uint8_t>(data_bin.data(), data_bin.data() + data_bin.size());
        }

        return row;
    }
};

template<>
struct PgRowMapper<AccountDataRow> {
    static AccountDataRow map(const pqxx::row& r) {
        AccountDataRow row;

        // Парсинг UUID account_id (оставляем UUID)
        std::string account_id_str = r["account_id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.account_id = gen(account_id_str);
        } catch (const std::exception& e) {
            throw std::runtime_error("PgRowMapper: invalid UUID string in 'account_id' field: " + account_id_str);
        }

        // Тип данных (0-7)
        row.type = static_cast<uint8_t>(r["type"].as<int>());

        // Временная метка
        row.time = r["time"].as<uint32_t>();

        // Бинарные данные
        if (!r["data"].is_null()) {
            pqxx::binarystring data_bin(r["data"]);
            row.data = std::vector<uint8_t>(data_bin.data(), data_bin.data() + data_bin.size());
        }

        return row;
    }
};

template<>
struct PgRowMapper<RealmCharactersRow> {
    static RealmCharactersRow map(const pqxx::row& r) {
        RealmCharactersRow row;
        row.realmid = r["realmid"].as<uint32_t>();
        row.numchars = static_cast<uint16_t>(r["numchars"].as<int>());
        return row;
    }
};

template<>
struct PgRowMapper<RealmRow> {
    static RealmRow map(const pqxx::row& r) {
        RealmRow row;

        row.id = r["id"].as<uint32_t>();
        row.name = r["name"].as<std::string>();
        row.address = r["address"].as<std::string>();
        row.localAddress = r["local_address"].as<std::string>();
        row.localSubnetMask = r["local_subnet_mask"].as<std::string>();
        row.port = static_cast<uint16_t>(r["port"].as<int>());
        row.icon = static_cast<uint8_t>(r["icon"].as<int>());
        row.flag = static_cast<uint8_t>(r["flag"].as<int>());
        row.timezone = static_cast<uint8_t>(r["timezone"].as<int>());
        row.securityLevel = static_cast<uint8_t>(r["allowed_security_level"].as<int>());
        row.population = r["population"].as<float>();
        row.build = r["gamebuild"].as<uint32_t>();

        return row;
    }
};

template<>
struct PgRowMapper<BuildInfoRow> {
    static BuildInfoRow map(const pqxx::row& r) {
        BuildInfoRow row;
        row.majorVersion = r["majorversion"].is_null() ? std::nullopt : std::make_optional(r["majorversion"].as<uint32_t>());
        row.minorVersion = r["minorversion"].is_null() ? std::nullopt : std::make_optional(r["minorversion"].as<uint32_t>());
        row.bugfixVersion = r["bugfixversion"].is_null() ? std::nullopt : std::make_optional(r["bugfixversion"].as<uint32_t>());
        row.hotfixVersion = r["hotfixversion"].is_null() ? std::nullopt : std::make_optional(r["hotfixversion"].as<std::string>());
        row.build = r["build"].as<uint32_t>();
        return row;
    }
};

template<>
struct PgRowMapper<BuildExeHashRow> {
    static BuildExeHashRow map(const pqxx::row& r) {
        BuildExeHashRow row;
        row.build = r["build"].as<uint32_t>();
        row.platform = r["platform"].as<std::string>();

        pqxx::binarystring hash_bin(r["executableHash"]);
        if (hash_bin.size() != row.executableHash.size())
            throw std::runtime_error("PgRowMapper: Invalid executableHash size, expected 20 bytes");
        std::copy_n(hash_bin.begin(), hash_bin.size(), row.executableHash.begin());

        return row;
    }
};

template<>
struct PgRowMapper<AccountsRow> {
    static AccountsRow map(const pqxx::row &r) {
        AccountsRow row;

        // Парсим UUID строку (оставляем UUID)
        std::string id_str = r["id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.id = gen(id_str);
        } catch (const std::exception &e) {
            throw std::runtime_error("PgRowMapper: invalid UUID string in 'id' field: " + id_str);
        }

        if (!r["username"].is_null())
            row.name = r["username"].as<std::string>();

        if (!r["salt"].is_null()) {
            pqxx::binarystring salt_bin(r["salt"]);
            if (salt_bin.size() != 32)
                throw std::runtime_error("PgRowMapper: Invalid salt size, expected 32 bytes");
            std::array<uint8_t, 32> salt_arr{};
            std::copy_n(salt_bin.begin(), 32, salt_arr.begin());
            row.salt = salt_arr;
        }

        if (!r["verifier"].is_null()) {
            pqxx::binarystring verifier_bin(r["verifier"]);
            if (verifier_bin.size() != 32)
                throw std::runtime_error("PgRowMapper: Invalid verifier size, expected 32 bytes");
            std::array<uint8_t, 32> verifier_arr{};
            std::copy_n(verifier_bin.begin(), 32, verifier_arr.begin());
            row.verifier = verifier_arr;
        }

        if (!r["session_key_auth"].is_null()) {
            pqxx::binarystring session_key_bin(r["session_key_auth"]);
            if (session_key_bin.size() != 40)
                throw std::runtime_error("PgRowMapper: Invalid sessionkey size, expected 40 bytes");
            std::array<uint8_t, 40> session_key_bin_arr{};
            std::copy_n(session_key_bin.begin(), 40, session_key_bin_arr.begin());
            row.sessionkey = session_key_bin_arr;
        }

        if (!r["email"].is_null())
            row.email = r["email"].as<std::string>();

        if (!r["created_at"].is_null())
            row.created_at = TimeUtils::parse_pg_timestamp_optional(r["created_at"].as<std::string>());

        return row;
    }
};

template<>
struct PgRowMapper<NothingRow> {
    static NothingRow map(const pqxx::row &) {
        return NothingRow{};
    }
};

template<>
struct PgRowMapper<uint64_t> {
    static uint64_t map(const pqxx::row &r) {
        return static_cast<uint64_t>(r[0].as<int64_t>());
    }
};