#include "utils/Duration.hpp"
#include "utils/advstd.hpp"
#include <compare>
#include <ctime>

class ByteBuffer;

class WowTime
{
public:
    uint32_t GetPackedTime() const;
    void SetPackedTime(uint32_t packedTime);

    std::time_t GetUnixTimeFromUtcTime() const;
    void SetUtcTimeFromUnixTime(std::time_t unixTime);

    int32_t GetYear() const { return _year; }
    void SetYear(int32_t year);

    int8_t GetMonth() const { return _month; }
    void SetMonth(int8_t month);

    int8_t GetMonthDay() const { return _monthDay; }
    void SetMonthDay(int8_t monthDay);

    int8_t GetWeekDay() const { return _weekDay; }
    void SetWeekDay(int8_t weekDay);

    int8_t GetHour() const { return _hour; }
    void SetHour(int8_t hour);

    int8_t GetMinute() const { return _minute; }
    void SetMinute(int8_t minute);

    int8_t GetFlags() const { return _flags; }
    void SetFlags(int8_t flags);

    int8_t GetHolidayOffset() const { return _holidayOffset; }
    void SetHolidayOffset(int8_t holidayOffset) { _holidayOffset = holidayOffset; }

    friend std::strong_ordering operator<=>(WowTime const& left, WowTime const& right);
    friend bool operator==(WowTime const& left, WowTime const& right)
    {
        return advstd::is_eq(left <=> right);
    }

    bool IsInRange(WowTime const& from, WowTime const& to) const;

    WowTime& operator+=(Seconds seconds);
    WowTime operator+(Seconds seconds) const;

    WowTime& operator-=(Seconds seconds);
    WowTime operator-(Seconds seconds) const;

    friend ByteBuffer& operator<<(ByteBuffer& data, WowTime const& wowTime);
    friend ByteBuffer& operator>>(ByteBuffer& data, WowTime& wowTime);

private:
    int32_t _year = -1;
    int8_t _month = -1;
    int8_t _monthDay = -1;
    int8_t _weekDay = -1;
    int8_t _hour = -1;
    int8_t _minute = -1;
    int8_t _flags = -1;
    int8_t _holidayOffset = 0;
};