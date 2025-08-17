#include "GameTime.hpp"
#include "Timer.hpp"
#include "Timezone.hpp"
#include "utils/util/Util.hpp"
#include "WowTime.hpp"

namespace GameTime
{
    time_t const StartTime = time(nullptr);

    time_t GameTime = time(nullptr);
    uint32_t GameMSTime = 0;

    SystemTimePoint GameTimeSystemPoint = SystemTimePoint ::min();
    TimePoint GameTimeSteadyPoint = TimePoint::min();

    WowTime UtcWow;
    WowTime Wow;

    time_t GetStartTime()
    {
        return StartTime;
    }

    time_t GetGameTime()
    {
        return GameTime;
    }

    uint32_t GetGameTimeMS()
    {
        return GameMSTime;
    }

    SystemTimePoint GetSystemTime()
    {
        return GameTimeSystemPoint;
    }

    TimePoint Now()
    {
        return GameTimeSteadyPoint;
    }

    uint32_t GetUptime()
    {
        return uint32_t(GameTime - StartTime);
    }

    WowTime const* GetUtcWowTime()
    {
        return &UtcWow;
    }

    WowTime const* GetWowTime()
    {
        return &Wow;
    }

    void UpdateGameTimers()
    {
        GameTime = time(nullptr);
        GameMSTime = getMSTime();
        GameTimeSystemPoint = std::chrono::system_clock::now();
        GameTimeSteadyPoint = std::chrono::steady_clock::now();
        UtcWow.SetUtcTimeFromUnixTime(GameTime);
        Wow = UtcWow + Timezone::GetSystemZoneOffsetAt(GameTimeSystemPoint);
    }
}
