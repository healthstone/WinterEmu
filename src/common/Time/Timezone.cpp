#include "Timezone.hpp"
#include "utils/Hash.hpp"
#include "utils/locales/Locales.hpp"
#include "Containers/Utilities/MapUtils.hpp"
#include "utils/util/Util.hpp"
#include <boost/locale/date_time_facet.hpp>
#include <chrono>
#include <memory>
#include <unordered_map>

namespace {
    std::unordered_map<uint32_t, Minutes, std::identity> InitTimezoneHashDb() {
        // Pre-generated list of timezone offsets and their hashes for compilers (and their stl implementations) that dont support timezone api yet
        std::unordered_map<uint32_t, Minutes, std::identity> hashToOffset =
                {
                        {0xAADC2D37u, -720min},
                        {0x362F107Bu, -690min},
                        {0x2C44C70Cu, -660min},
                        {0xB84A209Eu, -640min},
                        {0xBA3D57D1u, -630min},
                        {0x4040695Au, -600min},
                        {0xB65A75D0u, -570min},
                        {0xC8614DEBu, -540min},
                        {0x3A68BD26u, -510min},
                        {0x51E8096Cu, -480min},
                        {0x4DD8F896u, -420min},
                        {0x674B7C0Fu, -360min},
                        {0x633C6B39u, -300min},
                        {0x0BAD340Au, -240min},
                        {0x74B25683u, -225min},
                        {0x09B9FCD7u, -210min},
                        {0x150C169Bu, -180min},
                        {0x191B2771u, -120min},
                        {0xD7D3B14Eu, -60min},
                        {0x47CE5170u, -44min},
                        {0x350CA8AFu, 0min},
                        {0x15E8E23Bu, 60min},
                        {0x733864AEu, 120min},
                        {0xF71F9C94u, 180min},
                        {0xBDE50F54u, 210min},
                        {0x2BDD6DB9u, 240min},
                        {0xB1E07F42u, 270min},
                        {0x454FF132u, 300min},
                        {0x3F4DA929u, 330min},
                        {0xD1554AC4u, 360min},
                        {0xBB667143u, 390min},
                        {0x9E2B78C9u, 420min},
                        {0x1C377816u, 450min},
                        {0x1A4440E3u, 480min},
                        {0xB49DF789u, 525min},
                        {0xC3A28C54u, 540min},
                        {0x35A9FB8Fu, 570min},
                        {0x889BD751u, 600min},
                        {0x8CAAE827u, 660min},
                        {0x7285EE60u, 690min},
                        {0x1CC2DEF4u, 720min},
                        {0x89B8FD2Fu, 765min},
                        {0x98DBA70Eu, 780min},
                        {0xC59585BBu, 840min}
                };

        return hashToOffset;
    }

    std::unordered_map<uint32_t, Minutes, std::identity> const &GetTimezoneOffsetsByHash() {
        static std::unordered_map<uint32_t, Minutes, std::identity> timezoneMap = InitTimezoneHashDb();
        return timezoneMap;
    }

    using ClientSupportedTimezone = std::pair<Minutes, std::string>;
    std::array<ClientSupportedTimezone, 11> const _clientSupportedTimezones =
            {{
                     {-480min, "America/Los_Angeles"},
                     {-420min, "America/Denver"},
                     {-360min, "America/Chicago"},
                     {-300min, "America/New_York"},
                     {-180min, "America/Sao_Paulo"},
                     {0min, "Etc/UTC"},
                     {60min, "Europe/Paris"},
                     {480min, "Asia/Shanghai"},
                     {480min, "Asia/Taipei"},
                     {540min, "Asia/Seoul"},
                     {600min, "Australia/Melbourne"},
             }};
}

namespace Timezone {
    Minutes GetOffsetByHash(uint32_t hash) {
        if (Minutes const *offset = Containers::MapGetValuePtr(GetTimezoneOffsetsByHash(), hash))
            return *offset;

        return 0min;
    }

    Minutes GetSystemZoneOffsetAt(SystemTimePoint date) {
        Seconds offset;
        tm buf = Util::TimeBreakdown(std::chrono::system_clock::to_time_t(date));
        offset = Seconds(buf.tm_gmtoff);
        return std::chrono::duration_cast<Minutes>(offset);
    }

    Minutes GetSystemZoneOffset(bool applyDst /*= true*/) {
        std::chrono::system_clock::time_point date = std::chrono::system_clock::from_time_t(std::time_t(0));
        if (applyDst)
            date = std::chrono::system_clock::now();

        return GetSystemZoneOffsetAt(date);
    }

    std::string GetSystemZoneName() {
        std::unique_ptr<boost::locale::abstract_calendar> p(
                std::use_facet<class boost::locale::calendar_facet>(Locale::GetCalendarLocale()).create_calendar());
        return p->get_timezone();
    }

    std::string_view
    FindClosestClientSupportedTimezone(std::string_view currentTimezone, Minutes currentTimezoneOffset) {
        // try exact match
        auto itr = std::find_if(_clientSupportedTimezones.begin(), _clientSupportedTimezones.end(),
                                [currentTimezone](ClientSupportedTimezone const &tz) {
                                    return tz.second == currentTimezone;
                                });
        if (itr != _clientSupportedTimezones.end())
            return itr->second;

        // try closest offset
        itr = std::min_element(_clientSupportedTimezones.begin(), _clientSupportedTimezones.end(),
                               [currentTimezoneOffset](ClientSupportedTimezone const &left,
                                                       ClientSupportedTimezone const &right) {
                                   Minutes leftDiff = left.first - currentTimezoneOffset;
                                   Minutes rightDiff = right.first - currentTimezoneOffset;
                                   return std::abs(leftDiff.count()) < std::abs(rightDiff.count());
                               });

        return itr->second;
    }
}
