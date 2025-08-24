#pragma once

// Character flags
enum CharacterFlags
{
    CHARACTER_FLAG_NONE                 = 0x00000000,
    CHARACTER_FLAG_HIDE_HELM            = 0x00000001,
    CHARACTER_FLAG_HIDE_CLOAK           = 0x00000002,
    CHARACTER_FLAG_GHOST                = 0x00000004,
    CHARACTER_FLAG_RENAME               = 0x00000008,
    CHARACTER_FLAG_LOCKED_BY_BILLING    = 0x00000010,
    CHARACTER_FLAG_DECLINED             = 0x00000020,
};

// Customize flags
enum CharacterCustomizeFlags
{
    CHAR_CUSTOMIZE_FLAG_NONE            = 0x00000000,
    CHAR_CUSTOMIZE_FLAG_CUSTOMIZE       = 0x00000001,
    CHAR_CUSTOMIZE_FLAG_FACTION         = 0x00000002,
    CHAR_CUSTOMIZE_FLAG_RACE            = 0x00000004,
};

// AtLogin flags
enum AtLoginFlags
{
    AT_LOGIN_NONE           = 0x00000000,
    AT_LOGIN_RENAME         = 0x00000001,
    AT_LOGIN_RESURRECT      = 0x00000002,
    AT_LOGIN_CUSTOMIZE      = 0x00000004,
    AT_LOGIN_CHANGE_RACE    = 0x00000008,
    AT_LOGIN_CHANGE_FACTION = 0x00000010,
    AT_LOGIN_FIRST          = 0x00000020,
};

// Player flags
enum PlayerFlags
{
    PLAYER_FLAGS_GHOST          = 0x00000100,
    PLAYER_FLAGS_HIDE_HELM      = 0x00004000,
    PLAYER_FLAGS_HIDE_CLOAK     = 0x00008000,
};