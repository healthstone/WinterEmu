#include "ClientBuildInfo.hpp"
#include <algorithm>
#include <cctype>

namespace
{
    std::vector<ClientBuild::Info> Builds;
}

namespace ClientBuild
{
    std::array<char, 5> ToCharArray(uint32_t value)
    {
        auto normalize = [](uint8_t c) -> char
        {
            if (!c || std::isprint(c))
                return char(c);
            return ' ';
        };

        std::array<char, 5> chars = { char((value >> 24) & 0xFF), char((value >> 16) & 0xFF), char((value >> 8) & 0xFF), char(value & 0xFF), '\0' };

        auto firstNonZero = std::ranges::find_if(chars, [](char c) { return c != '\0'; });
        if (firstNonZero != chars.end())
        {
            // move leading zeros to end
            std::rotate(chars.begin(), firstNonZero, chars.end());

            // ensure we only have printable characters remaining
            std::ranges::transform(chars, chars.begin(), normalize);
        }

        return chars;
    }

    bool Platform::IsValid(std::string_view platform)
    {
        if (platform.length() > sizeof(uint32_t))
            return false;

        switch (ToFourCC(platform))
        {
            case Win_x86:
            case Mac_x86:
                return true;
            default:
                break;
        }

        return false;
    }

    void Clear() {
        Builds.clear();
    }

    void LoadBuildInfo(const std::shared_ptr<Database>& db)
    {
        auto log = Logger::get();
        Builds.clear();

        try {
            // === 1) Загружаем основное описание билдов ===
            PreparedStatement stmt1("SELECT_BUILD_INFO");
            auto rows = db->execute_sync_many<BuildInfoRow>(stmt1);
            for (auto const& row : rows)
            {
                Info& build = Builds.emplace_back();
                build.MajorVersion = row.majorVersion.value_or(0);
                build.MinorVersion = row.minorVersion.value_or(0);
                build.BugfixVersion = row.bugfixVersion.value_or(0);

                if (row.hotfixVersion && row.hotfixVersion->length() < build.HotfixVersion.size())
                    std::ranges::copy(*row.hotfixVersion, build.HotfixVersion.begin());
                else
                    build.HotfixVersion = {};

                build.Build = row.build;
            }
            log->info(">>> LoadBuildInfo: loaded {} build_info", rows.size());
        }
        catch (const std::exception& ex) {
            log->error("LoadBuildInfo:SELECT_BUILD_INFO {}", ex.what());
        }

        try {
            // === 2) Загружаем таблицу с хешами исполняемых файлов ===
            PreparedStatement stmt2("SELECT_BUILD_EXECUTABLE_HASH");
            auto exeHashRows = db->execute_sync_many<BuildExeHashRow>(stmt2);
            for (auto const& row : exeHashRows)
            {
                auto buildInfo = std::ranges::find(Builds, row.build, &Info::Build);
                if (buildInfo == Builds.end())
                {
                    log->error("ClientBuild::LoadBuildInfo: Unknown `build` {} in `build_executable_hash` - missing from `build_info`, skipped.", row.build);
                    continue;
                }


                auto trim = [](std::string_view sv) -> std::string_view {
                    size_t end = sv.find_last_not_of(' ');
                    if (end == std::string_view::npos) return {};
                    return sv.substr(0, end + 1);
                };

                std::string_view platform = trim(row.platform);
                if (!Platform::IsValid(platform))
                {
                    log->error("ClientBuild::LoadBuildInfo: Invalid platform {} for build {}, skipped.", row.platform, row.build);
                    continue;
                }

                ExecutableHash& buildKey = buildInfo->ExecutableHashes.emplace_back();
                buildKey.Platform = ToFourCC(row.platform);
                buildKey.Hash = row.executableHash;
            }
            log->info(">>> LoadBuildInfo: loaded {} build_executable_hash", exeHashRows.size());
        }catch (const std::exception& ex) {
            log->error("LoadBuildInfo:SELECT_BUILD_EXECUTABLE_HASH {}", ex.what());
        }
    }

    Info const* GetBuildInfo(uint32_t build)
    {
        auto buildInfo = std::ranges::find(Builds, build, &Info::Build);
        return buildInfo != Builds.end() ? &*buildInfo : nullptr;
    }

    uint32_t GetMinorMajorBugfixVersionForBuild(uint32_t build)
    {
        auto buildInfo = std::ranges::lower_bound(Builds, build, {}, &Info::Build);
        return buildInfo != Builds.end() ? (buildInfo->MajorVersion * 10000 + buildInfo->MinorVersion * 100 + buildInfo->BugfixVersion) : 0;
    }
}
