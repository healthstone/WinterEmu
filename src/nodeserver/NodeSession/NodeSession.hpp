#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <atomic>

#include "packet/Packet.hpp"
#include "packet/MessageBuffer.hpp"

using boost::asio::ip::tcp;

class NodeServer;

class NodeSession : public std::enable_shared_from_this<NodeSession> {
public:
    explicit NodeSession(tcp::socket socket, std::shared_ptr<NodeServer> server);

    void start();
    void close();

    void send_packet(const std::shared_ptr<const Packet>& packet);

    bool isOpened() const { return !closed_.load(); }
    uint8_t _nodeID = 1;

private:
    void do_read();
    void process_read_buffer();

    void do_send_packet(const Packet& packet);
    void do_write();

private:
    tcp::socket socket_;
    std::shared_ptr<NodeServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    bool writing_ = false;
    std::atomic<bool> closed_{false};
};
