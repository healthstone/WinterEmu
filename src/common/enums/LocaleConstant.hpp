#pragma once

#include <cstdint>
#include <string>
#include <algorithm>
#include <cctype>
#include <unordered_map>

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

// Вспомогательная функция для преобразования строки в нижний регистр
static std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

static LocaleConstant getLocaleConstantFromString(const std::string &source) {
    static const std::unordered_map<std::string, LocaleConstant> localeMap = {
            {"enus", LOCALE_enUS}, {"engb", LOCALE_enGB}, {"kokr", LOCALE_koKR},
            {"frfr", LOCALE_frFR}, {"dede", LOCALE_deDE}, {"encn", LOCALE_enCN},
            {"zhcn", LOCALE_zhCN}, {"entw", LOCALE_enTW}, {"zhtw", LOCALE_zhTW},
            {"eses", LOCALE_esES}, {"esmx", LOCALE_esMX}, {"ruru", LOCALE_ruRU},
            {"ptpt", LOCALE_ptPT}, {"ptbr", LOCALE_ptBR}, {"itit", LOCALE_itIT}
    };

    std::string lowerSource = toLower(source);
    auto it = localeMap.find(lowerSource);

    if (it != localeMap.end()) {
        return it->second;
    }

    return DEFAULT_LOCALE;
}