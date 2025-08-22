#include <algorithm>
#include "UTF8Utils.hpp"
#include "deps/utf8cpp/utf8.h"

namespace UTF8Utils {

    bool is_valid_utf8(const std::string &str) {
        std::wstring wname;
        return Utf8toWStr(str, wname);
    }

    // Простое lowercase для ASCII, остальные символы остаются без изменений
    std::string to_lowercase(const std::string &str) {
        std::string result;
        result.reserve(str.size());

        for (size_t i = 0; i < str.size();) {
            unsigned char c = str[i];
            if (c <= 0x7F) {
                // ASCII
                result.push_back(std::tolower(c));
                ++i;
            } else {
                // UTF-8 multibyte - просто копируем, т.к. без внешних библиотек нет удобного lower
                // Можно улучшить, если нужна поддержка Unicode case folding с ICU
                size_t length = 1;
                if ((c & 0xE0) == 0xC0) length = 2;
                else if ((c & 0xF0) == 0xE0) length = 3;
                else if ((c & 0xF8) == 0xF0) length = 4;

                result.append(str.substr(i, length));
                i += length;
            }
        }

        return result;
    }

    std::string to_uppercase(const std::string &str) {
        std::string result;
        result.reserve(str.size());

        for (size_t i = 0; i < str.size();) {
            unsigned char c = str[i];
            if (c <= 0x7F) {
                result.push_back(std::toupper(c));
                ++i;
            } else {
                size_t length = 1;
                if ((c & 0xE0) == 0xC0) length = 2;
                else if ((c & 0xF0) == 0xE0) length = 3;
                else if ((c & 0xF8) == 0xF0) length = 4;

                result.append(str.substr(i, length));
                i += length;
            }
        }

        return result;
    }

    bool normalizePlayerName(std::string& name)
    {
        if (name.empty())
            return false;

        std::wstring tmp;
        if (!Utf8toWStr(name, tmp))
            return false;

        wstrToLower(tmp);
        if (!tmp.empty())
            tmp[0] = wcharToUpper(tmp[0]);

        if (!WStrToUtf8(tmp, name))
            return false;

        return true;
    }

    bool Utf8toWStr(std::string_view utf8str, std::wstring& wstr)
    {
        wstr.clear();
        try
        {
            utf8::utf8to16(utf8str.begin(), utf8str.end(), std::back_inserter(wstr));
        }
        catch(std::exception const&)
        {
            wstr.clear();
            return false;
        }

        return true;
    }

    bool WStrToUtf8(std::wstring_view wstr, std::string& utf8str)
    {
        try
        {
            std::string utf8str2;
            utf8str2.resize(wstr.size()*4);                     // allocate for most long case

            if (!wstr.empty())
            {
                char* oend = utf8::utf16to8(wstr.begin(), wstr.end(), &utf8str2[0]);
                utf8str2.resize(oend-(&utf8str2[0]));                // remove unused tail
            }
            utf8str = utf8str2;
        }
        catch(std::exception const&)
        {
            utf8str.clear();
            return false;
        }

        return true;
    }

    void wstrToLower(std::wstring& str) { std::transform(std::begin(str), std::end(str), std::begin(str), wcharToLower); }

} // namespace UTF8Utils
