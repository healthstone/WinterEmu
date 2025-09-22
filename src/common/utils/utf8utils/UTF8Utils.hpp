#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include <cctype>

namespace UTF8Utils {

    // Проверка, что строка валидный UTF-8 (базовая реализация)
    bool is_valid_utf8(const std::string& str);

    // Преобразование строки UTF-8 в lowercase (работает корректно для ASCII)
    std::string to_lowercase(const std::string& str);

    // Преобразование строки UTF-8 в uppercase (работает корректно для ASCII)
    std::string to_uppercase(const std::string &str);

    struct WcharToLower
    {
        wchar_t operator()(wchar_t wchar) const
        {
            if (wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
                return wchar_t(uint16_t(wchar)+0x0020);
            if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
                return wchar_t(uint16_t(wchar)+0x0020);
            if (wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
                return wchar_t(uint16_t(wchar)+0x0020);
            if (wchar >= 0x0100 && wchar <= 0x012E)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN CAPITAL LETTER I WITH OGONEK (only %2=0)
            {
                if (wchar % 2 == 0)
                    return wchar_t(uint16_t(wchar)+0x0001);
            }
            if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
                return wchar_t(0x00DF);
            if (wchar == 0x0401)                                     // CYRILLIC CAPITAL LETTER IO
                return wchar_t(0x0451);
            if (wchar >= 0x0410 && wchar <= 0x042F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC CAPITAL LETTER YA
                return wchar_t(uint16_t(wchar)+0x0020);

            return wchar;
        }
    } inline constexpr wcharToLower;

    struct WcharToUpper
    {
        wchar_t operator()(wchar_t wchar) const
        {
            if (wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
                return wchar_t(uint16_t(wchar) - 0x0020);
            if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
                return wchar_t(0x1E9E);
            if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
                return wchar_t(uint16_t(wchar) - 0x0020);
            if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
                return wchar_t(uint16_t(wchar) - 0x0020);
            if (wchar >= 0x0101 && wchar <= 0x012F)                  // LATIN SMALL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK (only %2=1)
            {
                if (wchar % 2 == 1)
                    return wchar_t(uint16_t(wchar) - 0x0001);
            }
            if (wchar >= 0x0430 && wchar <= 0x044F)                  // CYRILLIC SMALL LETTER A - CYRILLIC SMALL LETTER YA
                return wchar_t(uint16_t(wchar) - 0x0020);
            if (wchar == 0x0451)                                     // CYRILLIC SMALL LETTER IO
                return wchar_t(0x0401);

            return wchar;
        }
    } inline constexpr wcharToUpper;

    bool normalizePlayerName(std::string& name);

    bool Utf8toWStr(std::string_view utf8str, std::wstring& wstr);

    bool WStrToUtf8(std::wstring_view wstr, std::string& utf8str);

    void wstrToLower(std::wstring& str);

} // namespace UTF8Utils
