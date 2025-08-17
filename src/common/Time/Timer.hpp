#include "utils/Duration.hpp"

inline std::chrono::steady_clock::time_point GetApplicationStartTime() {
    using namespace std::chrono;

    static const steady_clock::time_point ApplicationStartTime = steady_clock::now();

    return ApplicationStartTime;
}

inline uint32_t getMSTime() {
    using namespace std::chrono;

    return uint32_t(duration_cast<milliseconds>(steady_clock::now() - GetApplicationStartTime()).count());
}

inline uint32_t getMSTimeDiff(uint32_t oldMSTime, uint32_t newMSTime) {
    // getMSTime() have limited data range and this is case when it overflow in this tick
    if (oldMSTime > newMSTime)
        return (0xFFFFFFFF - oldMSTime) + newMSTime;
    else
        return newMSTime - oldMSTime;
}

inline uint32_t getMSTimeDiff(uint32_t oldMSTime, std::chrono::steady_clock::time_point newTime) {
    using namespace std::chrono;

    uint32_t newMSTime = uint32_t(duration_cast<milliseconds>(newTime - GetApplicationStartTime()).count());
    return getMSTimeDiff(oldMSTime, newMSTime);
}

inline uint32_t GetMSTimeDiffToNow(uint32_t oldMSTime) {
    return getMSTimeDiff(oldMSTime, getMSTime());
}

struct IntervalTimer {
public:

    IntervalTimer()
            : _interval(0), _current(0) {
    }

    void Update(time_t diff) {
        _current += diff;
        if (_current < 0)
            _current = 0;
    }

    bool Passed() {
        return _current >= _interval;
    }

    void Reset() {
        if (_current >= _interval)
            _current %= _interval;
    }

    void SetCurrent(time_t current) {
        _current = current;
    }

    void SetInterval(time_t interval) {
        _interval = interval;
    }

    time_t GetInterval() const {
        return _interval;
    }

    time_t GetCurrent() const {
        return _current;
    }

private:

    time_t _interval;
    time_t _current;
};

struct TimeTracker {
public:
    TimeTracker(int32_t expiry = 0) : _expiryTime(expiry) {}

    TimeTracker(Milliseconds expiry) : _expiryTime(expiry) {}

    void Update(int32_t diff) {
        Update(Milliseconds(diff));
    }

    void Update(Milliseconds diff) {
        _expiryTime -= diff;
    }

    bool Passed() const {
        return _expiryTime <= 0s;
    }

    void Reset(int32_t expiry) {
        Reset(Milliseconds(expiry));
    }

    void Reset(Milliseconds expiry) {
        _expiryTime = expiry;
    }

    Milliseconds GetExpiry() const {
        return _expiryTime;
    }

private:
    Milliseconds _expiryTime;
};

struct PeriodicTimer {
public:

    PeriodicTimer(int32_t period, int32_t start_time)
            : i_period(period), i_expireTime(start_time) {
    }

    bool Update(const uint32_t diff) {
        if ((i_expireTime -= diff) > 0)
            return false;

        i_expireTime += i_period > int32_t(diff) ? i_period : diff;
        return true;
    }

    void SetPeriodic(int32_t period, int32_t start_time) {
        i_expireTime = start_time;
        i_period = period;
    }

    // Tracker interface
    void TUpdate(int32_t diff) { i_expireTime -= diff; }

    bool TPassed() const { return i_expireTime <= 0; }

    void TReset(int32_t diff, int32_t period) { i_expireTime += period > diff ? period : diff; }

private:

    int32_t i_period;
    int32_t i_expireTime;
};