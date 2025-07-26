#pragma once

#include <cstdint>

enum class AuthOpcodes : uint8_t {
    EMPTY_STAGE = 0x00, // Служебный. Можно использовать для очистки стадии.

    CMSG_PING = 0x01,   ///< Клиент → Сервер: Ping
    SMSG_PONG = 0x02,   ///< Сервер → Клиент: Pong

    CMSG_AUTH_LOGON_CHALLENGE = 0x03,   ///< Клиент → Сервер: Запрос логина (начало SRP)
    SMSG_AUTH_LOGON_CHALLENGE = 0x04,   ///< Сервер → Клиент: Параметры SRP (salt, B и т.д.)

    CMSG_AUTH_LOGON_PROOF = 0x05,       ///< Клиент → Сервер: Присылает M1
    SMSG_AUTH_LOGON_PROOF = 0x06,       ///< Сервер → Клиент: Присылает M2 (проверка)

    SMSG_AUTH_RESPONSE = 0x07           ///< Сервер → Клиент: Финальный результат (OK / FAIL)
};

enum class AuthStatusCode: uint8_t {
    AUTH_SUCCESS = 0,
    AUTH_FAILED = 1
};

enum class AuthErrorCode: uint8_t {
    INTERNAL_ERROR = 0,
    WRONG_USERNAME = 1,
    WRONG_PASSWORD = 2,
    DATABASE_BUSY  = 3,
    ALREADY_LOGGED_IN  = 4
};