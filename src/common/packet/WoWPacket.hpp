#pragma once

#include "packet/Packet.hpp"
#include "enums/WoWOpcodes.hpp"

class WoWPacket : public Packet {
private:
    WoWOpcodes opcode_ = WoWOpcodes::NULL_PACKET;

public:
    WoWPacket() = default;
    explicit WoWPacket(WoWOpcodes opcode) : opcode_(opcode) {}

    void set_opcode(WoWOpcodes opcode) { opcode_ = opcode; }
    WoWOpcodes get_opcode() const { return opcode_; }

    /// Формирует пакет для отправки клиенту:
    /// [Length (uint16 BE)][Opcode (uint16 LE)][Payload...]
    std::vector<uint8_t> build_packet() const override {
        ByteBuffer temp;

        // Полный размер: Opcode (2 байта) + Payload
        uint16_t total_length = static_cast<uint16_t>(2 + buffer_.size());
        temp.write_uint16_be(total_length);
        temp.write_uint16_le(static_cast<uint16_t>(opcode_));
        temp.write_bytes(buffer_.data());

        return temp.data();
    }

    /// Разбирает полученный от клиента пакет:
    /// [Length (uint16 BE)][Opcode (uint32 LE)][Payload...]
    void deserialize(const std::vector<uint8_t>& raw_data) override {
        if (raw_data.size() < 6) {
            throw std::runtime_error("WoWPacket::deserialize: packet too short (min 6 bytes required)");
        }

        ByteBuffer temp(raw_data);

        // Читаем длину пакета (BE)
        uint16_t total_length = temp.read_uint16_be();

        // Проверяем соответствие размера
        if (total_length < 4 || total_length + 2 != raw_data.size()) {
            std::ostringstream oss;
            oss << "Invalid packet length. Header: " << total_length
                << ", Actual: " << (raw_data.size() - 2);
            throw std::runtime_error(oss.str());
        }

        // Читаем opcode (uint32 LE) и берем только младшие 2 байта
        uint32_t full_opcode = temp.read_uint32_le();
        opcode_ = static_cast<WoWOpcodes>(full_opcode & 0xFFFF);

        // Читаем оставшиеся данные как payload
        buffer_.clear();
        if (temp.remaining() > 0) {
            auto payload = temp.read_bytes(temp.remaining());
            buffer_.write_bytes(payload);
        }
    }
};