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
    /// [Length (uint16 BE)][Opcode (uint16 LE)][Payload...]
    std::vector<uint8_t> build_packet() const override {
        ByteBuffer temp;

        // Общая длина: Opcode (2 байта) + Payload
        uint16_t total_length = static_cast<uint16_t>(2 + buffer_.size());

        // Длина в big-endian (сетевой порядок)
        temp.write_uint16_be(total_length);
        // Opcode в little-endian (как ожидает клиент WoW)
        temp.write_uint16_le(static_cast<uint16_t>(opcode_));
        temp.write_bytes(buffer_.data());

        return temp.data();
    }

    /// Разбирает полученный от клиента пакет:
    /// [Length (uint16 BE)][Opcode (uint16 LE)][Payload...]
    void deserialize(const std::vector<uint8_t>& raw_data) override {
        if (raw_data.size() < 4)  // Минимум: 2 байта opcode + 2 байта данных (для пустого пакета)
            throw std::runtime_error("WoWPacket::deserialize: packet too short (min 4 bytes required)");

        ByteBuffer temp(raw_data);

        // Opcode в little-endian (первые 2 байта)
        opcode_ = static_cast<WoWOpcodes>(temp.read_uint16_le());

        buffer_.clear();
        // Читаем оставшиеся данные как payload
        if (temp.remaining() > 0) {
            auto payload = temp.read_bytes(temp.remaining());
            buffer_.write_bytes(payload);
        }
    }
};