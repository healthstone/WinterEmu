#pragma once

#include <boost/asio.hpp>
#include <memory>
#include <vector>
#include <deque>
#include <atomic>

#include "src/relayserver/RelayServer.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/WoWPacket.hpp"

class RelayServer; // forward declaration

class GameSession : public std::enable_shared_from_this<GameSession> {
public:
    GameSession(boost::asio::ip::tcp::socket socket, std::shared_ptr<RelayServer> server);

    void start();

    void close();

    bool isOpened() const { return !closed_; }

    void send_packet(const std::shared_ptr<const WoWPacket>& packet);

    boost::asio::ip::tcp::socket &socket() { return socket_; }

    std::shared_ptr<RelayServer> server() const { return server_; }

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

    void setClientSeed(uint32_t value) { client_seed_ = value; }
    void setNeedCrypt(bool value) { isNeedCrypt_ = value; }
    bool isNeedCrypt() const { return isNeedCrypt_; }

private:
    void send_auth_challenge();

    void do_read();

    void process_read_buffer();

    void do_write();

    void do_send_packet(const WoWPacket &packet);

    boost::asio::ip::tcp::socket socket_;
    std::shared_ptr<RelayServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::atomic<bool> closed_{false};

    std::array<uint8_t, 4> authSeed_;
    uint32_t client_seed_;
    bool isNeedCrypt_ = false;
};
