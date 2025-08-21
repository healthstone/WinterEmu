#include <array>
#include <string_view>
#include <vector>
#include <memory>
#include "Database.hpp"

namespace ClientBuild
{
    inline constexpr uint32_t ToFourCC(std::string_view text)
    {
        uint32_t uintValue = 0;
        for (uint8_t c : text)
        {
            uintValue <<= 8;
            uintValue |= c;
        }
        return uintValue;
    }

    consteval uint32_t operator""_fourcc(char const* chars, std::size_t length)
    {
        if (length > sizeof(uint32_t))
            throw "ClientBuild: Text can only be max 4 characters long";

        return ToFourCC({ chars, length });
    }

    std::array<char, 5> ToCharArray(uint32_t value);

    namespace Platform
    {
        inline constexpr uint32_t Win_x86     = "Win"_fourcc;
        inline constexpr uint32_t Mac_x86     = "OSX"_fourcc;

        bool IsValid(std::string_view platform);
    }

    struct ExecutableHash
    {
        static constexpr std::size_t Size = 20;

        uint32_t Platform;
        std::array<uint8_t, Size> Hash;
    };

    struct Info
    {
        uint32_t Build;
        uint32_t MajorVersion;
        uint32_t MinorVersion;
        uint32_t BugfixVersion;
        std::array<char, 4> HotfixVersion;
        std::vector<ExecutableHash> ExecutableHashes;
    };

    void LoadBuildInfo(const std::shared_ptr<Database>& db);
    void Clear();
    Info const* GetBuildInfo(uint32_t build);
}