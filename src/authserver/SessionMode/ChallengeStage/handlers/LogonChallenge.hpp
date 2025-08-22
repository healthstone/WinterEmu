#pragma once

#include <cstdint>
#include <string>

class LogonChallenge {
public:
    uint8_t cmd;
    uint8_t error;
    uint16_t size;
    std::string gamename;
    uint8_t v1;
    uint8_t v2;
    uint8_t v3;
    uint16_t build;
    std::string platform;
    std::string os;
    std::string country;
    uint32_t timezone_bias;
    uint32_t ip;
    uint8_t I_len;
    std::string accountName;
};