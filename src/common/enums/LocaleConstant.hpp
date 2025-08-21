#pragma once

#include <cstdint>

enum LocaleConstant : uint8_t
{
    LOCALE_enUS = 0,
    LOCALE_enGB = 1,
    LOCALE_koKR = 2,
    LOCALE_frFR = 3,
    LOCALE_deDE = 4,
    LOCALE_enCN = 5,
    LOCALE_zhCN = 6,
    LOCALE_enTW = 7,
    LOCALE_zhTW = 8,
    LOCALE_esES = 9,
    LOCALE_esMX = 10,
    LOCALE_ruRU = 11,
    LOCALE_ptPT = 12,
    LOCALE_ptBR = 13,
    LOCALE_itIT = 14,

    TOTAL_LOCALES
};

#define DEFAULT_LOCALE LOCALE_enUS

#define MAX_LOCALES 8