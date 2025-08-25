#pragma once

// === Безопасность аккаунта ===
enum class AccountTypes : uint8_t
{
    SEC_PLAYER         = 0,
    SEC_MODERATOR      = 1,
    SEC_GAME_MASTER    = 2,
    SEC_ADMINISTRATOR  = 3
};