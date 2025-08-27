#pragma once

#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <memory>
#include <string>
#include "src/game/enums/AccountTypes.hpp"
#include "src/game/enums/RealmEnums.hpp"

// === Описание Realm ===
struct Realm {
    uint32_t Id = 0;
    uint32_t Build = 0;

    std::unique_ptr<boost::asio::ip::address> ExternalAddress;
    std::unique_ptr<boost::asio::ip::address> LocalAddress;
    std::unique_ptr<boost::asio::ip::address> LocalSubnetMask;

    uint16_t Port = 0;
    std::string Name;

    RealmType Type = RealmType::REALM_TYPE_NORMAL;
    RealmFlags Flags = RealmFlags::REALM_FLAG_NONE;
    uint8_t Timezone = 0;
    AccountTypes AllowedSecurityLevel = AccountTypes::SEC_PLAYER;

    /**
     * 0 - 0.5   - Low
     * 0.5 - 1.0 - Medium
     * 1.0 - 2.0 - High
     * >= 2.0    - Full
     */
    float PopulationLevel = 0.0f;

    [[nodiscard]]
    boost::asio::ip::tcp::endpoint GetAddressForClient(const boost::asio::ip::address &clientAddr) const;

    bool isPvPRealm() const {
        return Type == RealmType::REALM_TYPE_PVP ||
               Type == RealmType::REALM_TYPE_RPPVP ||
               Type == RealmType::REALM_TYPE_FFA_PVP;
    }

    bool isFFAPvPRealm() const {
        return Type == RealmType::REALM_TYPE_FFA_PVP;
    }
};
