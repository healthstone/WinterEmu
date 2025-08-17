#include "utils/Duration.hpp"

class WowTime;

namespace GameTime
{
    // Server start time
    time_t GetStartTime();

    // Current server time (unix) in seconds
    time_t GetGameTime();

    // Milliseconds since server start
    uint32_t GetGameTimeMS();

    /// Current chrono system_clock time point
    SystemTimePoint GetSystemTime();

    /// Current chrono steady_clock time point
    TimePoint Now();

    /// Uptime (in secs)
    uint32_t GetUptime();

    WowTime const* GetUtcWowTime();

    WowTime const* GetWowTime();

    void UpdateGameTimers();
}
