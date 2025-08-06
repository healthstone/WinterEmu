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

    /// Формирует пакет для отправки клиенту:
    /// [Length (uint16 LE)][Opcode (uint16 LE)][Payload...]
    std::vector<uint8_t> build_packet() const override {
        ByteBuffer temp;

        // Общая длина: Opcode (2 байта) + Payload
        uint16_t total_length = static_cast<uint16_t>(2 + buffer_.size());

        temp.write_uint16_le(total_length);
        temp.write_uint16_le(static_cast<uint16_t>(opcode_));
        temp.write_bytes(buffer_.data());

        return temp.data();
    }

    /// Разбирает полученный от клиента пакет:
    /// [Length (uint16 LE)][Opcode (uint16 LE)][Payload...]
    void deserialize(const std::vector<uint8_t>& raw_data) override {
        if (raw_data.size() < 4)
            throw std::runtime_error("WoWPacket::deserialize: packet too short");

        ByteBuffer temp(raw_data);

        uint16_t length = temp.read_uint16_le(); // length = opcode + payload
        opcode_ = static_cast<WoWOpcodes>(temp.read_uint16_le());

        if (length != raw_data.size() - 2)
            throw std::runtime_error("WoWPacket::deserialize: payload length mismatch");

        buffer_.clear();
        auto payload = temp.read_bytes(length - 2); // minus opcode
        buffer_.write_bytes(payload);
    }
};
