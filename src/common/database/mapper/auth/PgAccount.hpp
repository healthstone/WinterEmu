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
struct Account {
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
    std::optional<uint32_t> mutetime;
    std::optional<std::string> mutereason;
    std::optional<std::string> muteby;
    std::optional<uint8_t> locale;
    std::optional<std::string> os;
    std::optional<int16_t> timezone_offset;
    std::optional<uint32_t> recruiter;
    std::optional<uint32_t> coins;
};

template<>
struct PgRowMapper<Account> {
    static Account map(const pqxx::row &r) {
        Account row;

        // UUID
        std::string id_str = r["id"].as<std::string>();
        try {
            boost::uuids::string_generator gen;
            row.id = gen(id_str);
        } catch (...) {
            throw std::runtime_error("PgRowMapper: invalid UUID string in 'id' field: " + id_str);
        }

        // строковые
        row.username        = get_optional_string(r, "username");
        row.email           = get_optional_string(r, "email");
        row.reg_mail        = get_optional_string(r, "reg_mail");
        row.last_ip         = get_optional_string(r, "last_ip");
        row.last_attempt_ip = get_optional_string(r, "last_attempt_ip");
        row.lock_country    = get_optional_string(r, "lock_country");
        row.mutereason      = get_optional_string(r, "mutereason");
        row.muteby          = get_optional_string(r, "muteby");
        row.os              = get_optional_string(r, "os");

        // бинарные
        row.salt             = map_binary_fixed<32>(r, "salt");
        row.verifier         = map_binary_fixed<32>(r, "verifier");
        row.session_key_auth = map_binary_fixed<40>(r, "session_key_auth");
        row.session_key_bnet = map_binary_var(r, "session_key_bnet");
        row.totp_secret      = map_binary_var(r, "totp_secret");

        // временные
        if (!r["joindate"].is_null())
            row.joindate = TimeUtils::parse_pg_timestamp_optional(r["joindate"].as<std::string>());
        if (!r["last_login"].is_null())
            row.last_login = TimeUtils::parse_pg_timestamp_optional(r["last_login"].as<std::string>());

        // числовые
        row.failed_logins   = get_optional_number<uint32_t>(r, "failed_logins");

        //row.RaceID   = static_cast<uint8_t>(get_optional_number<int16_t>(r, "raceid").value_or(0));
        row.locked          = static_cast<uint8_t>(get_optional_number<int>(r, "locked").value_or(0));
        row.online          = static_cast<uint8_t>(get_optional_number<int>(r, "online").value_or(0));
        row.expansion       = static_cast<uint8_t>(get_optional_number<int>(r, "expansion").value_or(0));
        row.mutetime        = get_optional_number<uint32_t>(r, "mutetime");
        row.locale          = static_cast<uint8_t>(get_optional_number<int>(r, "locale").value_or(0));
        row.timezone_offset = get_optional_number<int16_t>(r, "timezone_offset");
        row.recruiter       = get_optional_number<uint32_t>(r, "recruiter");
        row.coins           = get_optional_number<uint32_t>(r, "coins");

        return row;
    }
};
