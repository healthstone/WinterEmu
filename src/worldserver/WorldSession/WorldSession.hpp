#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <deque>
#include <atomic>

#include "src/worldserver/WorldServer.hpp"
#include "packet/MessageBuffer.hpp"
#include "WoWPacket.hpp"

class WorldServer; // forward declaration

class WorldSession : public std::enable_shared_from_this<WorldSession> {
public:
    WorldSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<WorldServer> server);

    void start();

    void close();

    bool isOpened() const { return !closed_; }

    void send_packet(const std::shared_ptr<const Packet>& packet);

    boost::asio::ip::tcp::socket &socket() { return socket_; }

    std::shared_ptr<WorldServer> server() const { return server_; }

    MessageBuffer &read_buffer() {
        return read_buffer_;
    }

    boost::asio::ip::address GetRemoteIpAddress() const
    {
        boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
        return remote_ep.address();
    }

    uint16_t GetRemotePort() const
    {
        boost::asio::ip::tcp::endpoint remote_ep = socket_.remote_endpoint();
        return remote_ep.port();
    }

private:
    void do_read();

    void process_read_buffer();

    void do_write();

    void do_send_packet(const Packet &packet);

    boost::asio::ip::tcp::socket socket_;
    std::shared_ptr<WorldServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::atomic<bool> closed_{false};
};
