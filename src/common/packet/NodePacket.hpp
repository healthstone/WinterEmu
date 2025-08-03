#pragma once

#include <vector>
#include <cstdint>
#include <optional>
#include "Packet.hpp"
#include "NodeData.hpp"
#include "WoWPacket.hpp"
#include "enums/NodeOpcodes.hpp"

class NodePacket : public Packet {
private:
    NodeOpcodes opcode_ = NodeOpcodes::NULL_PACKET;

    std::optional<NodeData> node_data_;
    std::optional<WoWPacket> wow_packet_;

public:
    NodePacket() = default;

    NodePacket(NodeOpcodes opcode, const NodeData& node_data, const WoWPacket& wow_packet)
            : opcode_(opcode), node_data_(node_data), wow_packet_(wow_packet) {}

    NodePacket(NodeOpcodes opcode, const NodeData& node_data)
            : opcode_(opcode), node_data_(node_data) {}

    NodeOpcodes opcode() const { return opcode_; }
    void set_opcode(NodeOpcodes opcode) { opcode_ = opcode; }

    const std::optional<NodeData>& node_data() const { return node_data_; }
    void set_node_data(const NodeData& data) { node_data_ = data; }

    const std::optional<WoWPacket>& wow_packet() const { return wow_packet_; }
    void set_wow_packet(const WoWPacket& pkt) { wow_packet_ = pkt; }

    bool is_valid() const {
        return node_data_.has_value() || wow_packet_.has_value();
    }

    std::vector<uint8_t> build_packet() const override {
        ByteBuffer buf;

        // opcode (2 bytes)
        buf.write_uint16_be(static_cast<uint16_t>(opcode_));

        // node_data length + data
        if (node_data_) {
            auto nd_bytes = node_data_->build_packet();
            buf.write_uint16_be(static_cast<uint16_t>(nd_bytes.size()));
            buf.write_bytes(nd_bytes.data(), nd_bytes.size());
        } else {
            buf.write_uint16_be(0);
        }

        // wow_packet data (no length prefix)
        if (wow_packet_) {
            auto wow_bytes = wow_packet_->build_packet();
            buf.write_bytes(wow_bytes.data(), wow_bytes.size());
        }

        return buf.data();
    }

    void deserialize(const std::vector<uint8_t>& raw_data) override {
        ByteBuffer buf(raw_data);

        opcode_ = static_cast<NodeOpcodes>(buf.read_uint16_be());

        uint16_t node_data_length = buf.read_uint16_be();

        if (node_data_length > 0) {
            std::vector<uint8_t> nd_bytes = buf.read_bytes(node_data_length);
            NodeData nd;
            nd.deserialize(nd_bytes);
            node_data_ = nd;
        } else {
            node_data_ = std::nullopt;
        }

        if (buf.size() > buf.read_pos()) {
            std::vector<uint8_t> wow_bytes = buf.read_bytes(buf.size() - buf.read_pos());
            WoWPacket wp;
            wp.deserialize(wow_bytes);
            wow_packet_ = wp;
        } else {
            wow_packet_ = std::nullopt;
        }
    }
};
