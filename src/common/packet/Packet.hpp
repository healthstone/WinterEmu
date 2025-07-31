#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include "ByteBuffer.hpp"
#include "Logger.hpp"

class Packet {
protected:
    ByteBuffer buffer_;

public:
    Packet() = default;
    virtual ~Packet() = default;

    ByteBuffer& raw() { return buffer_; }
    const std::vector<uint8_t>& serialize() const { return buffer_.data(); }

    virtual std::vector<uint8_t> build_packet() const = 0;
    virtual void deserialize(const std::vector<uint8_t>& raw_data) = 0;

    static void log_raw_payload(
            const std::string& opcode,
            const std::vector<uint8_t>& payload,
            const std::string& prefix = "[Packet] DUMP")
    {
        std::string hex_dump;
        hex_dump.reserve(payload.size() * 3);
        for (uint8_t b : payload) {
            fmt::format_to(std::back_inserter(hex_dump), "{:02X} ", b);
        }

        Logger::get()->debug("{} {} ({} bytes) RAW: {}",
                             prefix,
                             opcode,
                             payload.size(),
                             hex_dump);
    }

    // ==================== WRITE METHODS ====================
    void write_uint8(uint8_t value) { buffer_.write_uint8(value); }
    void write_int8(int8_t value) { buffer_.write_int8(value); }

    // ---------- Big-Endian ----------
    void write_uint16_be(uint16_t value) { buffer_.write_uint16_be(value); }
    void write_int16_be(int16_t value) { buffer_.write_int16_be(value); }
    void write_uint32_be(uint32_t value) { buffer_.write_uint32_be(value); }
    void write_int32_be(int32_t value) { buffer_.write_int32_be(value); }
    void write_uint64_be(uint64_t value) { buffer_.write_uint64_be(value); }
    void write_int64_be(int64_t value) { buffer_.write_int64_be(value); }
    void write_float_be(float value) { buffer_.write_float_be(value); }
    void write_double_be(double value) { buffer_.write_double_be(value); }

    // ---------- Little-Endian ----------
    void write_uint16_le(uint16_t value) { buffer_.write_uint16_le(value); }
    void write_int16_le(int16_t value) { buffer_.write_int16_le(value); }
    void write_uint32_le(uint32_t value) { buffer_.write_uint32_le(value); }
    void write_int32_le(int32_t value) { buffer_.write_int32_le(value); }
    void write_uint64_le(uint64_t value) { buffer_.write_uint64_le(value); }
    void write_int64_le(int64_t value) { buffer_.write_int64_le(value); }
    void write_float_le(float value) { buffer_.write_float_le(value); }
    void write_double_le(double value) { buffer_.write_double_le(value); }

    // ---------- Boolean ----------
    void write_bool(bool value) { buffer_.write_bool(value); }

    // ---------- Strings ----------

    // BE raw string (без нуль-терминатора)
    void write_string_raw_be(const std::string& str) { buffer_.write_string_raw_be(str); }
    // LE raw string (реверс)
    void write_string_raw_le(const std::string& str) { buffer_.write_string_raw_le(str); }

    // BE null-terminated string
    void write_string_nt_be(const std::string& str) { buffer_.write_string_nt_be(str); }
    // LE null-terminated string (реверс + '\0')
    void write_string_nt_le(const std::string& str) { buffer_.write_string_nt_le(str); }

    // ---------- Bytes ----------
    std::vector<uint8_t> read_bytes(size_t length) { return buffer_.read_bytes(length); }
    void write_bytes(const uint8_t* data, size_t length) { buffer_.write_bytes(data, length); }
    void write_bytes(const std::vector<uint8_t>& data) { buffer_.write_bytes(data); }

    // ==================== READ METHODS ====================
    uint8_t read_uint8() { return buffer_.read_uint8(); }
    int8_t read_int8() { return buffer_.read_int8(); }

    // ---------- Big-Endian ----------
    uint16_t read_uint16_be() { return buffer_.read_uint16_be(); }
    int16_t read_int16_be() { return buffer_.read_int16_be(); }
    uint32_t read_uint32_be() { return buffer_.read_uint32_be(); }
    int32_t read_int32_be() { return buffer_.read_int32_be(); }
    uint64_t read_uint64_be() { return buffer_.read_uint64_be(); }
    int64_t read_int64_be() { return buffer_.read_int64_be(); }
    float read_float_be() { return buffer_.read_float_be(); }
    double read_double_be() { return buffer_.read_double_be(); }

    // ---------- Little-Endian ----------
    uint16_t read_uint16_le() { return buffer_.read_uint16_le(); }
    int16_t read_int16_le() { return buffer_.read_int16_le(); }
    uint32_t read_uint32_le() { return buffer_.read_uint32_le(); }
    int32_t read_int32_le() { return buffer_.read_int32_le(); }
    uint64_t read_uint64_le() { return buffer_.read_uint64_le(); }
    int64_t read_int64_le() { return buffer_.read_int64_le(); }
    float read_float_le() { return buffer_.read_float_le(); }
    double read_double_le() { return buffer_.read_double_le(); }

    // ---------- Boolean ----------
    bool read_bool() { return buffer_.read_bool(); }

    // ---------- Strings ----------

    // BE raw string (без нуль-терминатора)
    std::string read_string_raw_be(size_t length) { return buffer_.read_string_raw_be(length); }
    // LE raw string (реверс)
    std::string read_string_raw_le(size_t length) { return buffer_.read_string_raw_le(length); }

    // BE null-terminated string
    std::string read_string_nt_be() { return buffer_.read_string_nt_be(); }
    // LE null-terminated string (реверс)
    std::string read_string_nt_le() { return buffer_.read_string_nt_le(); }

    // ==================== UTILITY METHODS ====================
    void skip(size_t bytes) { buffer_.skip(bytes); }
    size_t read_pos() const { return buffer_.read_pos(); }
    size_t size() const { return buffer_.size(); }
};
