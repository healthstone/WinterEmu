#include "utils/Duration.hpp"
#include <string>

namespace Timezone {
    Minutes GetOffsetByHash(uint32_t hash);

    // Returns the time offset that must be added to UTC time to get localtime
    Minutes GetSystemZoneOffsetAt(SystemTimePoint date);

    Minutes GetSystemZoneOffset(bool applyDst = true);

    std::string GetSystemZoneName();

    std::string_view
    FindClosestClientSupportedTimezone(std::string_view currentTimezone, Minutes currentTimezoneOffset);
}