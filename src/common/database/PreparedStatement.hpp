#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <optional>
#include <array>
#include <iostream>
#include <sstream>
#include <iomanip>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp> // для boost::uuids::to_string

class PreparedStatement {
public:
    explicit PreparedStatement(const std::string& name) : name_(name) {}

    const std::string& name() const { return name_; }

    const std::vector<std::optional<std::string>>& params() const { return params_; }

    // ======== SET PARAMS ========

    template<typename T>
    void set_param(size_t index, T value) {
        ensure_size(index);
        params_[index] = std::to_string(value);
    }

    void set_param(size_t index, const std::string& value) {
        ensure_size(index);
        params_[index] = value;
    }

    void set_param(size_t index, const char* value) {
        ensure_size(index);
        params_[index] = std::string(value);
    }

    void set_param(size_t index, std::string_view value) {
        ensure_size(index);
        params_[index] = std::string(value);
    }

    // --- For binary data ---
    void set_param(size_t index, const std::vector<uint8_t>& value) {
        ensure_size(index);
        params_[index] = bin_to_hex(value);
    }

    template<size_t N>
    void set_param(size_t index, const std::array<uint8_t, N>& value) {
        ensure_size(index);
        params_[index] = bin_to_hex(value);
    }

    void set_null(size_t index) {
        ensure_size(index);
        params_[index].reset();
    }

    // --- Support for boost::uuids::uuid ---
    void set_param(size_t index, const boost::uuids::uuid& value) {
        ensure_size(index);
        params_[index] = boost::uuids::to_string(value);
    }

    // ======== UTILITY ========

    void clear() {
        params_.clear();
    }

    void debug_print() const {
        std::cout << "[PreparedStatement] Name: " << name_ << "\n";
        for (size_t i = 0; i < params_.size(); ++i) {
            if (params_[i].has_value()) {
                std::cout << "  Param[" << i << "] = " << params_[i].value() << "\n";
            } else {
                std::cout << "  Param[" << i << "] = NULL\n";
            }
        }
    }

private:
    void ensure_size(size_t index) {
        if (index >= params_.size()) {
            params_.resize(index + 1);
        }
    }

    // --- Helper for hex encoding ---
    static std::string bin_to_hex(const std::vector<uint8_t>& data) {
        std::ostringstream oss;
        for (auto b : data) {
            oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(b);
        }
        return oss.str();
    }

    template<size_t N>
    static std::string bin_to_hex(const std::array<uint8_t, N>& data) {
        return bin_to_hex(std::vector<uint8_t>(data.begin(), data.end()));
    }

    std::string name_;
    std::vector<std::optional<std::string>> params_;
};
