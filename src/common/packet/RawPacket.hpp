#pragma once
#include "Packet.hpp"

class RawPacket : public Packet {
public:
    RawPacket() = default;
    explicit RawPacket(const ByteBuffer& buffer) {
        buffer_ = buffer;
    }

    // Реализуем build_packet, просто возвращаем содержимое буфера
    std::vector<uint8_t> build_packet() const override {
        return buffer_.data();
    }

    // Для RawPacket десериализация не нужна, можно бросить исключение
    void deserialize(const std::vector<uint8_t>&) override {
        throw std::runtime_error("RawPacket::deserialize not implemented");
    }
};
