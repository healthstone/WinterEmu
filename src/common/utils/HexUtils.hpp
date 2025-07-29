#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <iomanip>

namespace HexUtils {

    inline uint8_t hex_char_to_int(char c) {
        if ('0' <= c && c <= '9') return c - '0';
        if ('a' <= c && c <= 'f') return c - 'a' + 10;
        if ('A' <= c && c <= 'F') return c - 'A' + 10;
        throw std::invalid_argument("Invalid hex character");
    }

    inline std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
        if (hex.size() % 2 != 0) {
            throw std::invalid_argument("Hex string must have even length");
        }
        std::vector<uint8_t> bytes;
        bytes.reserve(hex.size() / 2);
        for (size_t i = 0; i < hex.size(); i += 2) {
            uint8_t hi = hex_char_to_int(hex[i]);
            uint8_t lo = hex_char_to_int(hex[i + 1]);
            bytes.push_back((hi << 4) | lo);
        }
        return bytes;
    }

    inline std::string bytes_to_hex(const uint8_t* bytes, size_t len) {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');
        for (size_t i = 0; i < len; ++i) {
            oss << std::setw(2) << static_cast<int>(bytes[i]);
        }
        return oss.str();
    }

    inline std::string bytes_to_hex(const std::vector<uint8_t>& bytes) {
        return bytes_to_hex(bytes.data(), bytes.size());
    }

} // namespace HexUtils
