#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <vector>
#include <cstdint>
#include "packet/NodePacket.hpp"
#include "Logger.hpp"

using boost::asio::ip::tcp;

class NodeConnector : public std::enable_shared_from_this<NodeConnector> {
public:
    NodeConnector(boost::asio::io_context& io, const std::string& host, int port, uint8_t node_id);

    void start();
    void stop();

    void send_packet(const NodePacket& packet);
    void cancel_pong_timeout();

private:
    void connect();
    void schedule_reconnect();

    void start_receive_loop();
    void read_header();
    void read_payload(std::size_t length, const std::vector<uint8_t>& header);

    void do_send_packet(const NodePacket& packet);
    void do_write();
    void flush_queue();

    void start_heartbeat();
    void send_ping();
    void schedule_pong_timeout();

private:
    boost::asio::io_context& io_;
    tcp::socket socket_;
    std::string host_;
    int port_;

    bool connected_;
    bool writing_;
    bool awaiting_pong_;

    boost::asio::steady_timer reconnect_timer_;
    boost::asio::steady_timer heartbeat_timer_;
    boost::asio::steady_timer pong_timeout_timer_;

    uint8_t node_id_;

    std::deque<std::vector<uint8_t>> write_queue_;
};
