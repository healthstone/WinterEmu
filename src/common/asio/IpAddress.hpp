#pragma once

#include <boost/asio/ip/address.hpp>

namespace Net {
    inline uint32_t address_to_uint(boost::asio::ip::address_v4 const &address) {
        return address.to_uint();
    }
}
