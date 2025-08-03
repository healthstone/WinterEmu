#pragma once

#include <vector>
#include "Packet.hpp"

class NodeData : public Packet {
public:
    NodeData() = default;

    explicit NodeData(const std::vector<uint8_t>& payload) {
        buffer_ = ByteBuffer(payload); // Инициализируем ByteBuffer с данными
    }

    const std::vector<uint8_t>& get_payload() const {
        return buffer_.data();
    }

    void set_payload(const std::vector<uint8_t>& payload) {
        buffer_ = ByteBuffer(payload);
    }

    std::vector<uint8_t> build_packet() const override {
        // Просто возвращаем содержимое ByteBuffer
        return buffer_.data();
    }

    void deserialize(const std::vector<uint8_t>& raw_data) override {
        buffer_ = ByteBuffer(raw_data);
    }
};
