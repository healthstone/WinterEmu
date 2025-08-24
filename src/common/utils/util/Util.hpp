#include <sstream>
#include <vector>
#include <optional>

namespace Util {
    tm TimeBreakdown(time_t t);

    std::vector<std::string_view> Tokenize(std::string_view str, char sep, bool keepEmpty);

    /** Аргумент int base = 10 задаёт систему счисления, в которой будет интерпретироваться строка при конвертации в число.
    10 → десятичная (обычная) система, например "42" → 42.
    16 → шестнадцатеричная, например "0x2A" → 42.
    2 → бинарная, например "101010" → 42. **/
    std::optional<uint32_t> stringToUInt32(std::string_view str, int base = 10);
}