#pragma once

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>
#include "src/authserver/enums/AccountTypes.hpp"

// === Флаги реалмов ===
enum RealmFlags : uint8_t {
    REALM_FLAG_NONE             = 0x00,
    REALM_FLAG_VERSION_MISMATCH = 0x01,
    REALM_FLAG_OFFLINE          = 0x02,
    REALM_FLAG_SPECIFYBUILD     = 0x04,
    REALM_FLAG_UNK1             = 0x08,
    REALM_FLAG_UNK2             = 0x10,
    REALM_FLAG_RECOMMENDED      = 0x20,
    REALM_FLAG_NEW              = 0x40,
    REALM_FLAG_FULL             = 0x80
};

// === Типы реалмов ===
enum RealmType : uint8_t {
    REALM_TYPE_NORMAL  = 0,
    REALM_TYPE_PVP     = 1,
    REALM_TYPE_NORMAL2 = 4,
    REALM_TYPE_RP      = 6,
    REALM_TYPE_RPPVP   = 8,
    REALM_TYPE_FFA_PVP = 16 // кастомный PvP
};

// === Описание Realm ===
struct Realm {
    uint32_t Id = 0;
    uint32_t Build = 0;

    std::unique_ptr<boost::asio::ip::address> ExternalAddress;
    std::unique_ptr<boost::asio::ip::address> LocalAddress;
    std::unique_ptr<boost::asio::ip::address> LocalSubnetMask;

    uint16_t Port = 0;
    std::string Name;

    uint8_t Type = REALM_TYPE_NORMAL;
    RealmFlags Flags = REALM_FLAG_NONE;
    uint8_t Timezone = 0;
    AccountTypes AllowedSecurityLevel = AccountTypes::SEC_PLAYER;
    float PopulationLevel = 0.0f;

    [[nodiscard]]
    boost::asio::ip::tcp::endpoint GetAddressForClient(const boost::asio::ip::address& clientAddr) const;
};
