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

// Добавляем структуру для playercreateinfo
struct PlayerCreateInfoRow {
    uint8_t m_race;
    uint8_t m_class;
    uint16_t m_map;
    uint32_t m_zone;
    float position_x;
    float position_y;
    float position_z;
    float orientation;
};

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
    uint32_t zone;
    uint32_t map;
    float position_x;
    float position_y;
    float position_z;
    std::optional<uint32_t> guildid;  // Изменено с UUID на uint32_t
    uint32_t playerFlags;
    uint8_t at_login;
    std::optional<uint32_t> pet_entry;
    std::optional<uint32_t> pet_modelid;
    std::optional<uint32_t> pet_level;
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

// Добавляем маппер для PlayerCreateInfoRow
template<>
struct PgRowMapper<PlayerCreateInfoRow> {
    static PlayerCreateInfoRow map(const pqxx::row& r) {
        PlayerCreateInfoRow row;
        row.m_race = static_cast<uint8_t>(r["race"].as<int>());
        row.m_class = static_cast<uint8_t>(r["class"].as<int>());
        row.m_map = static_cast<uint16_t>(r["map"].as<int>());
        row.m_zone = r["zone"].as<uint32_t>();
        row.position_x = r["position_x"].as<float>();
        row.position_y = r["position_y"].as<float>();
        row.position_z = r["position_z"].as<float>();
        row.orientation = r["orientation"].as<float>();
        return row;
    }
};

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
        row.zone = static_cast<uint32_t>(r["zone"].as<int>());
        row.map = static_cast<uint32_t>(r["map"].as<int>());
        row.position_x = r["position_x"].as<float>();
        row.position_y = r["position_y"].as<float>();
        row.position_z = r["position_z"].as<float>();

        // GUID гильдии (теперь uint32_t)
        if (!r["guild_guildid"].is_null()) {
            row.guildid = r["guild_guildid"].as<uint32_t>();
        }

        row.playerFlags = r["playerFlags"].as<uint32_t>();
        row.at_login = static_cast<uint8_t>(r["at_login"].as<int>());

        if (!r["pet_entry"].is_null()) row.pet_entry = r["pet_entry"].as<uint32_t>();
        if (!r["pet_modelid"].is_null()) row.pet_modelid = r["pet_modelid"].as<uint32_t>();
        if (!r["pet_level"].is_null()) row.pet_level = r["pet_level"].as<uint32_t>();

        row.equipmentCache = r["equipmentCache"].as<std::string>();

        // GUID бана (теперь uint32_t)
        if (!r["banned_guid"].is_null()) {
            row.banned_guid = r["banned_guid"].as<uint32_t>();
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