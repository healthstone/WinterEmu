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

/** accounts **/
struct AccountsRow {
    boost::uuids::uuid id;
    std::optional<std::string> username;
    std::optional<std::array<uint8_t, 32>> salt;
    std::optional<std::array<uint8_t, 32>> verifier;
    std::optional<std::array<uint8_t, 40>> session_key_auth;
    std::optional<std::vector<uint8_t>> session_key_bnet; // Может быть разной длины
    std::optional<std::vector<uint8_t>> totp_secret;      // Может быть разной длины
    std::optional<std::string> email;
    std::optional<std::string> reg_mail;
    std::optional<std::chrono::system_clock::time_point> joindate;
    std::optional<std::string> last_ip;
    std::optional<std::string> last_attempt_ip;
    std::optional<uint32_t> failed_logins;
    std::optional<uint8_t> locked;
    std::optional<std::string> lock_country;
    std::optional<std::chrono::system_clock::time_point> last_login;
    std::optional<uint8_t> online;
    std::optional<uint8_t> expansion;
    std::optional<int64_t> mutetime;
    std::optional<std::string> mutereason;
    std::optional<std::string> muteby;
    std::optional<uint8_t> locale;
    std::optional<std::string> os;
    std::optional<int16_t> timezone_offset;
    std::optional<uint32_t> recruiter;
    std::optional<int64_t> coins;
};

/** build_info **/
struct BuildInfoRow {
    std::optional<uint32_t> majorVersion;
    std::optional<uint32_t> minorVersion;
    std::optional<uint32_t> bugfixVersion;
    std::optional<std::string> hotfixVersion;
    uint32_t build; // Primary Key, NOT NULL
};

/** build_executable_hash **/
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

template<>
struct PgRowMapper<AccountsRow> {
    static AccountsRow map(const pqxx::row &r) {
        AccountsRow row;

        // Парсим UUID
        std::string id_str = r["id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.id = gen(id_str);
        } catch (const std::exception &e) {
            throw std::runtime_error("PgRowMapper: invalid UUID string in 'id' field: " + id_str);
        }

        // Обрабатываем строковые поля
        if (!r["username"].is_null()) row.username = r["username"].as<std::string>();
        if (!r["email"].is_null()) row.email = r["email"].as<std::string>();
        if (!r["reg_mail"].is_null()) row.reg_mail = r["reg_mail"].as<std::string>();
        if (!r["last_ip"].is_null()) row.last_ip = r["last_ip"].as<std::string>();
        if (!r["last_attempt_ip"].is_null()) row.last_attempt_ip = r["last_attempt_ip"].as<std::string>();
        if (!r["lock_country"].is_null()) row.lock_country = r["lock_country"].as<std::string>();
        if (!r["mutereason"].is_null()) row.mutereason = r["mutereason"].as<std::string>();
        if (!r["muteby"].is_null()) row.muteby = r["muteby"].as<std::string>();
        if (!r["os"].is_null()) row.os = r["os"].as<std::string>();

        // Обрабатываем бинарные поля
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
                throw std::runtime_error("PgRowMapper: Invalid session_key_auth size, expected 40 bytes");
            std::array<uint8_t, 40> session_key_arr{};
            std::copy_n(session_key_bin.begin(), 40, session_key_arr.begin());
            row.session_key_auth = session_key_arr;
        }

        if (!r["session_key_bnet"].is_null()) {
            pqxx::binarystring session_key_bnet_bin(r["session_key_bnet"]);
            std::vector<uint8_t> session_key_bnet_vec(session_key_bnet_bin.begin(), session_key_bnet_bin.end());
            row.session_key_bnet = session_key_bnet_vec;
        }

        if (!r["totp_secret"].is_null()) {
            pqxx::binarystring totp_secret_bin(r["totp_secret"]);
            std::vector<uint8_t> totp_secret_vec(totp_secret_bin.begin(), totp_secret_bin.end());
            row.totp_secret = totp_secret_vec;
        }

        // Обрабатываем временные метки
        if (!r["joindate"].is_null())
            row.joindate = TimeUtils::parse_pg_timestamp_optional(r["joindate"].as<std::string>());

        if (!r["last_login"].is_null())
            row.last_login = TimeUtils::parse_pg_timestamp_optional(r["last_login"].as<std::string>());

        // Обрабатываем числовые поля
        if (!r["failed_logins"].is_null()) row.failed_logins = r["failed_logins"].as<uint32_t>();
        if (!r["locked"].is_null()) row.locked = static_cast<uint8_t>(r["locked"].as<int>());
        if (!r["online"].is_null()) row.online = static_cast<uint8_t>(r["online"].as<int>());
        if (!r["expansion"].is_null()) row.expansion = static_cast<uint8_t>(r["expansion"].as<int>());
        if (!r["mutetime"].is_null()) row.mutetime = r["mutetime"].as<int64_t>();
        if (!r["locale"].is_null()) row.locale = static_cast<uint8_t>(r["locale"].as<int>());
        if (!r["timezone_offset"].is_null()) row.timezone_offset = r["timezone_offset"].as<int16_t>();
        if (!r["recruiter"].is_null()) row.recruiter = r["recruiter"].as<uint32_t>();
        if (!r["coins"].is_null()) row.coins = r["coins"].as<int64_t>();

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