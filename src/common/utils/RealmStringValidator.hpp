#pragma once

#include "src/game/Entity/Realm/Realm.hpp"
#include "src/game/enums/LanguageType.hpp"

namespace RealmStringValidator {
    static LanguageType GetRealmLanguageType(bool create, RealmZone Zone)
    {
        switch (Zone)
        {
            case REALM_ZONE_UNKNOWN:                            // any language
            case REALM_ZONE_DEVELOPMENT:
            case REALM_ZONE_TEST_SERVER:
            case REALM_ZONE_QA_SERVER:
                return LT_ANY;
            case REALM_ZONE_UNITED_STATES:                      // extended-Latin
            case REALM_ZONE_OCEANIC:
            case REALM_ZONE_LATIN_AMERICA:
            case REALM_ZONE_ENGLISH:
            case REALM_ZONE_GERMAN:
            case REALM_ZONE_FRENCH:
            case REALM_ZONE_SPANISH:
                return LT_EXTENDEN_LATIN;
            case REALM_ZONE_KOREA:                              // East-Asian
            case REALM_ZONE_TAIWAN:
            case REALM_ZONE_CHINA:
                return LT_EAST_ASIA;
            case REALM_ZONE_RUSSIAN:                            // Cyrillic
                return LT_CYRILLIC;
            default:
                return create ? LT_BASIC_LATIN : LT_ANY;        // basic-Latin at create, any at login
        }
    }

    // String validators
    inline bool isNumeric(wchar_t wchar)
    {
        return (wchar >= L'0' && wchar <=L'9');
    }

    inline bool isNumericOrSpace(wchar_t wchar)
    {
        return isNumeric(wchar) || wchar == L' ';
    }

    inline bool isBasicLatinCharacter(wchar_t wchar)
    {
        if (wchar >= L'a' && wchar <= L'z')                      // LATIN SMALL LETTER A - LATIN SMALL LETTER Z
            return true;
        if (wchar >= L'A' && wchar <= L'Z')                      // LATIN CAPITAL LETTER A - LATIN CAPITAL LETTER Z
            return true;
        return false;
    }

    inline bool isExtendedLatinCharacter(wchar_t wchar)
    {
        if (isBasicLatinCharacter(wchar))
            return true;
        if (wchar >= 0x00C0 && wchar <= 0x00D6)                  // LATIN CAPITAL LETTER A WITH GRAVE - LATIN CAPITAL LETTER O WITH DIAERESIS
            return true;
        if (wchar >= 0x00D8 && wchar <= 0x00DE)                  // LATIN CAPITAL LETTER O WITH STROKE - LATIN CAPITAL LETTER THORN
            return true;
        if (wchar == 0x00DF)                                     // LATIN SMALL LETTER SHARP S
            return true;
        if (wchar >= 0x00E0 && wchar <= 0x00F6)                  // LATIN SMALL LETTER A WITH GRAVE - LATIN SMALL LETTER O WITH DIAERESIS
            return true;
        if (wchar >= 0x00F8 && wchar <= 0x00FE)                  // LATIN SMALL LETTER O WITH STROKE - LATIN SMALL LETTER THORN
            return true;
        if (wchar >= 0x0100 && wchar <= 0x012F)                  // LATIN CAPITAL LETTER A WITH MACRON - LATIN SMALL LETTER I WITH OGONEK
            return true;
        if (wchar == 0x1E9E)                                     // LATIN CAPITAL LETTER SHARP S
            return true;
        return false;
    }

    inline bool isCyrillicCharacter(wchar_t wchar)
    {
        if (wchar >= 0x0410 && wchar <= 0x044F)                  // CYRILLIC CAPITAL LETTER A - CYRILLIC SMALL LETTER YA
            return true;
        if (wchar == 0x0401 || wchar == 0x0451)                  // CYRILLIC CAPITAL LETTER IO, CYRILLIC SMALL LETTER IO
            return true;
        return false;
    }

    inline bool isEastAsianCharacter(wchar_t wchar)
    {
        if (wchar >= 0x1100 && wchar <= 0x11F9)                  // Hangul Jamo
            return true;
        if (wchar >= 0x3041 && wchar <= 0x30FF)                  // Hiragana + Katakana
            return true;
        if (wchar >= 0x3131 && wchar <= 0x318E)                  // Hangul Compatibility Jamo
            return true;
        if (wchar >= 0x31F0 && wchar <= 0x31FF)                  // Katakana Phonetic Ext.
            return true;
        if (wchar >= 0x3400 && wchar <= 0x4DB5)                  // CJK Ideographs Ext. A
            return true;
        if (wchar >= 0x4E00 && wchar <= 0x9FC3)                  // Unified CJK Ideographs
            return true;
        if (wchar >= 0xAC00 && wchar <= 0xD7A3)                  // Hangul Syllables
            return true;
        if (wchar >= 0xFF01 && wchar <= 0xFFEE)                  // Halfwidth forms
            return true;
        return false;
    }

    inline bool isBasicLatinString(std::wstring_view wstr, bool numericOrSpace)
    {
        for (wchar_t c : wstr)
            if (!isBasicLatinCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
                return false;
        return true;
    }

    inline bool isExtendedLatinString(std::wstring_view wstr, bool numericOrSpace)
    {
        for (wchar_t c : wstr)
            if (!isExtendedLatinCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
                return false;
        return true;
    }

    inline bool isCyrillicString(std::wstring_view wstr, bool numericOrSpace)
    {
        for (wchar_t c : wstr)
            if (!isCyrillicCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
                return false;
        return true;
    }

    inline bool isEastAsianString(std::wstring_view wstr, bool numericOrSpace)
    {
        for (wchar_t c : wstr)
            if (!isEastAsianCharacter(c) && (!numericOrSpace || !isNumericOrSpace(c)))
                return false;
        return true;
    }

    bool isValidString(RealmZone Zone, const std::wstring& wstr, uint32_t strictMask, bool numericOrSpace,  bool create = false)
    {
        if (strictMask == 0)                                       // any language, ignore realm
        {
            if (isExtendedLatinString(wstr, numericOrSpace))
                return true;
            if (isCyrillicString(wstr, numericOrSpace))
                return true;
            if (isEastAsianString(wstr, numericOrSpace))
                return true;
            return false;
        }

        if (strictMask & 0x2)                                    // realm zone specific
        {
            LanguageType lt = GetRealmLanguageType(create, Zone);
            if (lt & LT_EXTENDEN_LATIN)
                if (isExtendedLatinString(wstr, numericOrSpace))
                    return true;
            if (lt & LT_CYRILLIC)
                if (isCyrillicString(wstr, numericOrSpace))
                    return true;
            if (lt & LT_EAST_ASIA)
                if (isEastAsianString(wstr, numericOrSpace))
                    return true;
        }

        if (strictMask & 0x1)                                    // basic Latin
        {
            if (isBasicLatinString(wstr, numericOrSpace))
                return true;
        }

        return false;
    }



}