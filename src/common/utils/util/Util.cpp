#include "Util.hpp"

tm Util::TimeBreakdown(time_t time) {
    tm timeLocal;
    localtime_r(&time, &timeLocal);
    return timeLocal;
}

std::vector<std::string_view> Util::Tokenize(std::string_view str, char sep, bool keepEmpty)
{
    std::vector<std::string_view> tokens;

    size_t start = 0;
    for (size_t end = str.find(sep); end != std::string_view::npos; end = str.find(sep, start))
    {
        if (keepEmpty || (start < end))
            tokens.push_back(str.substr(start, end - start));
        start = end+1;
    }

    if (keepEmpty || (start < str.length()))
        tokens.push_back(str.substr(start));

    return tokens;
}

std::optional<uint32_t> Util::stringToUInt32(std::string_view str, int base)
{
    try
    {
        return static_cast<uint32_t>(std::stoul(std::string(str), nullptr, base));
    }
    catch (...)
    {
        return std::nullopt;
    }
}