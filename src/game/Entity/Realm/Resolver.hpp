#pragma once

#include <boost/asio.hpp>
#include <optional>
#include <string>

class Resolver {
public:
    explicit Resolver(boost::asio::io_context& io_context)
            : resolver_(io_context)
    {}

    std::optional<boost::asio::ip::tcp::endpoint> Resolve(
            const boost::asio::ip::tcp& protocol,
            const std::string& host,
            const std::string& service)
    {
        boost::system::error_code ec;
        boost::asio::ip::tcp::resolver::results_type results =
                resolver_.resolve(protocol, host, service, ec);

        if (ec || results.empty())
            return std::nullopt;

        return *results.begin();
    }

private:
    boost::asio::ip::tcp::resolver resolver_;
};
