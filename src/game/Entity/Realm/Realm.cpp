#include "Realm.hpp"
#include "asio/IpAddress.hpp"
#include "asio/IpNetwork.hpp"
#include <boost/asio/ip/tcp.hpp>

boost::asio::ip::tcp::endpoint Realm::GetAddressForClient(const boost::asio::ip::address& clientAddr) const {
    boost::asio::ip::address realmIp;

    if (clientAddr.is_loopback()) {
        if (LocalAddress && LocalAddress->is_loopback())
            realmIp = *LocalAddress;
        else if (ExternalAddress && ExternalAddress->is_loopback())
            realmIp = *ExternalAddress;
        else if (LocalAddress)
            realmIp = *LocalAddress;
        else if (ExternalAddress)
            realmIp = *ExternalAddress;
        else
            realmIp = boost::asio::ip::address_v4::loopback();
    } else {
        if (LocalAddress && LocalSubnetMask && clientAddr.is_v4()) {
            if (Net::IsInNetwork(LocalAddress->to_v4(), LocalSubnetMask->to_v4(), clientAddr.to_v4()))
                realmIp = *LocalAddress;
            else if (ExternalAddress)
                realmIp = *ExternalAddress;
        } else if (ExternalAddress) {
            realmIp = *ExternalAddress;
        } else {
            realmIp = boost::asio::ip::address_v4::any();
        }
    }

    return { realmIp, Port };
}
