#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <algorithm> // для std::reverse
#include <endian.h>
#include "Logger.hpp"

class ByteBuffer {
public:
    ByteBuffer() = default;
    explicit ByteBuffer(const std::vector<uint8_t>& data) : buffer_(data), read_pos_(0) {}

    // ==================== WRITE METHODS ====================

    void write_uint8(uint8_t value) { buffer_.push_back(value); }
    void write_int8(int8_t value) { buffer_.push_back(static_cast<uint8_t>(value)); }

    // ---------- Big-Endian ----------

    void write_uint16_be(uint16_t value) {
        uint16_t be = htobe16(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&be), sizeof(be));
    }
    void write_int16_be(int16_t value) { write_uint16_be(static_cast<uint16_t>(value)); }

    void write_uint32_be(uint32_t value) {
        uint32_t be = htobe32(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&be), sizeof(be));
    }
    void write_int32_be(int32_t value) { write_uint32_be(static_cast<uint32_t>(value)); }

    void write_uint64_be(uint64_t value) {
        uint64_t be = htobe64(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&be), sizeof(be));
    }
    void write_int64_be(int64_t value) { write_uint64_be(static_cast<uint64_t>(value)); }

    void write_float_be(float value) {
        static_assert(sizeof(float) == sizeof(uint32_t), "Float size mismatch");
        uint32_t int_val;
        std::memcpy(&int_val, &value, sizeof(int_val));
        write_uint32_be(int_val);
    }

    void write_double_be(double value) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Double size mismatch");
        uint64_t int_val;
        std::memcpy(&int_val, &value, sizeof(int_val));
        write_uint64_be(int_val);
    }

    // ---------- Little-Endian ----------

    void write_uint16_le(uint16_t value) {
        uint16_t le = htole16(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&le), sizeof(le));
    }
    void write_int16_le(int16_t value) { write_uint16_le(static_cast<uint16_t>(value)); }

    void write_uint32_le(uint32_t value) {
        uint32_t le = htole32(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&le), sizeof(le));
    }
    void write_int32_le(int32_t value) { write_uint32_le(static_cast<uint32_t>(value)); }

    void write_uint64_le(uint64_t value) {
        uint64_t le = htole64(value);
        write_bytes(reinterpret_cast<const uint8_t*>(&le), sizeof(le));
    }
    void write_int64_le(int64_t value) { write_uint64_le(static_cast<uint64_t>(value)); }

    void write_float_le(float value) {
        static_assert(sizeof(float) == sizeof(uint32_t), "Float size mismatch");
        uint32_t int_val;
        std::memcpy(&int_val, &value, sizeof(int_val));
        write_uint32_le(int_val);
    }

    void write_double_le(double value) {
        static_assert(sizeof(double) == sizeof(uint64_t), "Double size mismatch");
        uint64_t int_val;
        std::memcpy(&int_val, &value, sizeof(int_val));
        write_uint64_le(int_val);
    }

    // ---------- Boolean ----------
    void write_bool(bool value) { buffer_.push_back(value ? 1 : 0); }

    // ---------- Strings ----------

    // Raw string write/read BE (as is)
    void write_string_raw_be(const std::string& str) {
        write_bytes(reinterpret_cast<const uint8_t*>(str.data()), str.size());
    }

    std::string read_string_raw_be(size_t length) {
        check_read(length, ReadSource::READ_STRING_RAW);
        std::string str(reinterpret_cast<const char*>(&buffer_[read_pos_]), length);
        read_pos_ += length;
        return str;
    }

    // Raw string write/read LE (reversed)
    void write_string_raw_le(const std::string& str) {
        std::string reversed_str(str.rbegin(), str.rend());
        write_bytes(reinterpret_cast<const uint8_t*>(reversed_str.data()), reversed_str.size());
    }

    std::string read_string_raw_le(size_t length) {
        check_read(length, ReadSource::READ_STRING_RAW);

        std::string str(reinterpret_cast<const char*>(&buffer_[read_pos_]), length);
        read_pos_ += length;

        // Если последний символ - нулевой байт, то разворачиваем все, кроме последнего символа
        if (!str.empty() && str.back() == '\0') {
            std::reverse(str.begin(), str.end() - 1);
        } else {
            // Иначе разворачиваем всю строку целиком
            std::reverse(str.begin(), str.end());
        }

        // Обрезаем всё после первого '\0' если он есть (на всякий случай)
        auto pos = str.find('\0');
        if (pos != std::string::npos) {
            str.resize(pos);
        }

        return str;
    }

    // Null-terminated string write/read BE (write as is + '\0', read until '\0')
    void write_string_nt_be(const std::string& str) {
        write_string_raw_be(str);
        write_uint8(0x00); // null terminator
    }

    std::string read_string_nt_be() {
        std::string str;
        while (read_pos_ < buffer_.size()) {
            char c = static_cast<char>(buffer_[read_pos_++]);
            if (c == '\0') break;
            str += c;
        }
        return str;
    }

    // Null-terminated string write/read LE (reverse string, write + '\0', read reversed and flip)
    void write_string_nt_le(const std::string& str) {
        write_string_raw_le(str);
        write_uint8(0x00); // null terminator
    }

    std::string read_string_nt_le() {
        std::string str;
        while (read_pos_ < buffer_.size()) {
            char c = static_cast<char>(buffer_[read_pos_++]);
            if (c == '\0') break;
            str += c;
        }
        std::reverse(str.begin(), str.end());
        return str;
    }

    // ---------- Bytes ----------
    void write_bytes(const uint8_t* data, size_t length) {
        size_t old_size = buffer_.size();
        buffer_.resize(old_size + length);
        std::memcpy(buffer_.data() + old_size, data, length);
    }

    void write_bytes(const std::vector<uint8_t>& data) {
        write_bytes(data.data(), data.size());
    }

    // ==================== READ METHODS ====================

    uint8_t read_uint8() {
        check_read(sizeof(uint8_t), ReadSource::READ_UINT8);
        return buffer_[read_pos_++];
    }
    int8_t read_int8() { return static_cast<int8_t>(read_uint8()); }

    uint16_t read_uint16_be() {
        check_read(sizeof(uint16_t), ReadSource::READ_UINT16_BE);
        uint16_t be;
        std::memcpy(&be, &buffer_[read_pos_], sizeof(be));
        read_pos_ += sizeof(be);
        return be16toh(be);
    }
    int16_t read_int16_be() { return static_cast<int16_t>(read_uint16_be()); }

    uint32_t read_uint32_be() {
        check_read(sizeof(uint32_t), ReadSource::READ_UINT32_BE);
        uint32_t be;
        std::memcpy(&be, &buffer_[read_pos_], sizeof(be));
        read_pos_ += sizeof(be);
        return be32toh(be);
    }
    int32_t read_int32_be() { return static_cast<int32_t>(read_uint32_be()); }

    uint64_t read_uint64_be() {
        check_read(sizeof(uint64_t), ReadSource::READ_UINT64_BE);
        uint64_t be;
        std::memcpy(&be, &buffer_[read_pos_], sizeof(be));
        read_pos_ += sizeof(be);
        return be64toh(be);
    }
    int64_t read_int64_be() { return static_cast<int64_t>(read_uint64_be()); }

    float read_float_be() {
        uint32_t int_val = read_uint32_be();
        float value;
        std::memcpy(&value, &int_val, sizeof(value));
        return value;
    }

    double read_double_be() {
        uint64_t int_val = read_uint64_be();
        double value;
        std::memcpy(&value, &int_val, sizeof(value));
        return value;
    }

    uint16_t read_uint16_le() {
        check_read(sizeof(uint16_t), ReadSource::READ_UINT16_LE);
        uint16_t le;
        std::memcpy(&le, &buffer_[read_pos_], sizeof(le));
        read_pos_ += sizeof(le);
        return le16toh(le);
    }
    int16_t read_int16_le() { return static_cast<int16_t>(read_uint16_le()); }

    uint32_t read_uint32_le() {
        check_read(sizeof(uint32_t), ReadSource::READ_UINT32_LE);
        uint32_t le;
        std::memcpy(&le, &buffer_[read_pos_], sizeof(le));
        read_pos_ += sizeof(le);
        return le32toh(le);
    }
    int32_t read_int32_le() { return static_cast<int32_t>(read_uint32_le()); }

    uint64_t read_uint64_le() {
        check_read(sizeof(uint64_t), ReadSource::READ_UINT64_LE);
        uint64_t le;
        std::memcpy(&le, &buffer_[read_pos_], sizeof(le));
        read_pos_ += sizeof(le);
        return le64toh(le);
    }
    int64_t read_int64_le() { return static_cast<int64_t>(read_uint64_le()); }

    float read_float_le() {
        uint32_t int_val = read_uint32_le();
        float value;
        std::memcpy(&value, &int_val, sizeof(value));
        return value;
    }

    double read_double_le() {
        uint64_t int_val = read_uint64_le();
        double value;
        std::memcpy(&value, &int_val, sizeof(value));
        return value;
    }

    bool read_bool() {
        check_read(sizeof(uint8_t), ReadSource::READ_BOOL);
        return read_uint8() != 0;
    }

    std::vector<uint8_t> read_bytes(size_t length) {
        check_read(length, ReadSource::READ_BYTES);
        std::vector<uint8_t> bytes(buffer_.begin() + read_pos_, buffer_.begin() + read_pos_ + length);
        read_pos_ += length;
        return bytes;
    }

    // ==================== UTILITY METHODS ====================

    void skip(size_t bytes) {
        check_read(bytes, ReadSource::SKIP);
        read_pos_ += bytes;
    }

    void clear() {
        buffer_.clear();
        read_pos_ = 0;
    }

    void reset_read() {
        read_pos_ = 0;
    }

    const std::vector<uint8_t>& data() const { return buffer_; }
    size_t size() const { return buffer_.size(); }
    size_t read_pos() const { return read_pos_; }

private:
    enum class ReadSource {
        READ_UINT8,
        READ_INT8,

        READ_UINT16_BE,
        READ_UINT32_BE,
        READ_UINT64_BE,
        READ_INT16_BE,
        READ_INT32_BE,
        READ_INT64_BE,
        READ_FLOAT_BE,
        READ_DOUBLE_BE,

        READ_UINT16_LE,
        READ_UINT32_LE,
        READ_UINT64_LE,
        READ_INT16_LE,
        READ_INT32_LE,
        READ_INT64_LE,
        READ_FLOAT_LE,
        READ_DOUBLE_LE,

        READ_BOOL,
        READ_STRING_RAW,
        READ_BYTES,
        SKIP,
        READ_FIELD,
        UNKNOWN
    };

    static const char* read_source_to_string(ReadSource source) {
        switch (source) {
            case ReadSource::READ_UINT8: return "read_uint8";
            case ReadSource::READ_INT8: return "read_int8";
            case ReadSource::READ_UINT16_BE: return "read_uint16_be";
            case ReadSource::READ_UINT32_BE: return "read_uint32_be";
            case ReadSource::READ_UINT64_BE: return "read_uint64_be";
            case ReadSource::READ_INT16_BE: return "read_int16_be";
            case ReadSource::READ_INT32_BE: return "read_int32_be";
            case ReadSource::READ_INT64_BE: return "read_int64_be";
            case ReadSource::READ_FLOAT_BE: return "read_float_be";
            case ReadSource::READ_DOUBLE_BE: return "read_double_be";
            case ReadSource::READ_UINT16_LE: return "read_uint16_le";
            case ReadSource::READ_UINT32_LE: return "read_uint32_le";
            case ReadSource::READ_UINT64_LE: return "read_uint64_le";
            case ReadSource::READ_INT16_LE: return "read_int16_le";
            case ReadSource::READ_INT32_LE: return "read_int32_le";
            case ReadSource::READ_INT64_LE: return "read_int64_le";
            case ReadSource::READ_FLOAT_LE: return "read_float_le";
            case ReadSource::READ_DOUBLE_LE: return "read_double_le";
            case ReadSource::READ_BOOL: return "read_bool";
            case ReadSource::READ_STRING_RAW: return "read_string_raw";
            case ReadSource::READ_BYTES: return "read_bytes";
            case ReadSource::SKIP: return "skip";
            case ReadSource::READ_FIELD: return "ReadField";
            default: return "unknown";
        }
    }

    void check_read(size_t size, ReadSource source) const {
        if (read_pos_ + size > buffer_.size()) {
            std::ostringstream oss;
            oss << "[ByteBuffer] Not enough data! Source: " << read_source_to_string(source)
                << " | Wanted: " << size << " bytes | ReadPos: " << read_pos_
                << " | BufferSize: " << buffer_.size();
            Logger::get()->error("{}", oss.str());
            throw std::out_of_range("ByteBuffer: not enough data to read");
        }
    }

    std::vector<uint8_t> buffer_;
    size_t read_pos_ = 0;
};
