#pragma once

#include <boost/asio.hpp>
#include <deque>
#include <memory>
#include <atomic>

#include "packet/Packet.hpp"
#include "packet/MessageBuffer.hpp"
#include "packet/NodePacket.hpp"

using boost::asio::ip::tcp;

class NodeServer;

class NodeSession : public std::enable_shared_from_this<NodeSession> {
public:
    explicit NodeSession(tcp::socket socket, std::shared_ptr<NodeServer> server);

    void start();
    void close();

    void send_packet(const std::shared_ptr<const NodePacket>& packet);
    bool isOpened() const { return !closed_.load(); }

    uint8_t _nodeID = 1;

private:
    void do_read();
    void process_read_buffer();
    void do_send_packet(const NodePacket &packet);
    void do_write();

    void schedule_processing();
    void process_packets();

private:
    tcp::socket socket_;
    std::shared_ptr<NodeServer> server_;

    MessageBuffer read_buffer_;

    std::deque<std::vector<uint8_t>> write_queue_;
    std::deque<std::shared_ptr<NodePacket>> recv_queue_;

    bool processing_queue_ = false;
    bool writing_ = false;
    std::atomic<bool> closed_{false};
};
