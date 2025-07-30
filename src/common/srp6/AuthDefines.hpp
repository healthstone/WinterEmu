#ifndef AUTHDEFINES_H
#define AUTHDEFINES_H

#include <array>
#include <cstdint>
#include <cstddef>

constexpr size_t SESSION_KEY_LENGTH = 40;
using SessionKey = std::array<uint8_t, SESSION_KEY_LENGTH>;

#endif