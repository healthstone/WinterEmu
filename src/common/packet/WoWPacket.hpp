#pragma once

#include "packet/Packet.hpp"
#include "enums/WoWOpcodes.hpp"

class WoWPacket : public Packet {
private:
    WoWOpcodes opcode_ = WoWOpcodes::NULL_PACKET;

public:
    WoWPacket() = default;

    explicit WoWPacket(WoWOpcodes opcode)
            : opcode_(opcode) {}

    void set_opcode(WoWOpcodes opcode) { opcode_ = opcode; }

    WoWOpcodes get_opcode() const { return opcode_; }

    /// Собирает готовый TCP пакет:
    /// [Length(uint16 BE)][Opcode(uint16 BE)][Payload...]
    std::vector<uint8_t> build_packet() const override {
        ByteBuffer temp;

        uint16_t total_length = static_cast<uint16_t>(2 + buffer_.size());
        temp.write_uint16_be(total_length);
        temp.write_uint16_be(static_cast<uint16_t>(opcode_));
        temp.write_bytes(buffer_.data());

        return temp.data();
    }

    /// Читает готовый TCP пакет:
    /// [Length(uint16 BE)][Opcode(uint16 BE)][Payload...]
    void deserialize(const std::vector<uint8_t>& raw_data) override {
        ByteBuffer temp(raw_data);

        uint16_t length = temp.read_uint16_be();
        opcode_ = static_cast<WoWOpcodes>(temp.read_uint16_be());

        if (length != raw_data.size() - 2) {
            throw std::runtime_error("WoWPacket::deserialize: payload length mismatch");
        }

        buffer_.clear();
        auto payload = temp.read_bytes(length - 2);
        buffer_.write_bytes(payload);
    }
};
