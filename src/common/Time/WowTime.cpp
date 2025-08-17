#include "WowTime.hpp"
#include "packet/ByteBuffer.hpp"
#include "utils/util/Util.hpp"

uint32_t WowTime::GetPackedTime() const
{
    return ((_year % 100) & 0x1F) << 24
           | (_month & 0xF) << 20
           | (_monthDay & 0x3F) << 14
           | (_weekDay & 0x7) << 11
           | (_hour & 0x1F) << 6
           | (_minute & 0x3F)
           | (_flags & 0x3) << 29;
}

void WowTime::SetPackedTime(uint32_t packedTime)
{
    _year = (packedTime >> 24) & 0x1F;
    if (_year == 31)
        _year = -1;

    _month = (packedTime >> 20) & 0xF;
    if (_month == 15)
        _month = -1;

    _monthDay = (packedTime >> 14) & 0x3F;
    if (_monthDay == 63)
        _monthDay = -1;

    _weekDay = (packedTime >> 11) & 0x7;
    if (_weekDay == 7)
        _weekDay = -1;

    _hour = (packedTime >> 6) & 0x1F;
    if (_hour == 31)
        _hour = -1;

    _minute = packedTime & 0x3F;
    if (_minute == 63)
        _minute = -1;

    _flags = (packedTime >> 29) & 0x3;
    if (_flags == 3)
        _flags = -1;
}

std::time_t WowTime::GetUnixTimeFromUtcTime() const
{
    if (_year < 0 || _month < 0 || _monthDay < 0)
        return 0;

    std::tm buf{};
    buf.tm_year = _year + 100;
    buf.tm_mon = _month;
    buf.tm_mday = _monthDay + 1;
    if (_hour >= 0)
    {
        buf.tm_hour = _hour;
        if (_minute >= 0)
            buf.tm_min = _minute;
    }
    buf.tm_isdst = -1;
    buf.tm_wday = _weekDay;

    return timegm(&buf);
}

void WowTime::SetUtcTimeFromUnixTime(std::time_t unixTime)
{
    std::tm buf;
    if (!::gmtime_r(&unixTime, &buf))
        return;

    _year = (buf.tm_year - 100) % 100;
    _month = buf.tm_mon;
    _monthDay = buf.tm_mday - 1;
    _weekDay = buf.tm_wday;
    _hour = buf.tm_hour;
    _minute = buf.tm_min;
}

void WowTime::SetYear(int32_t year)
{
    if (!(year == -1 || (year >= 0 && year < 32)))
        throw std::runtime_error("WowTime::SetYear: invalid year value");
    _year = year;
}

void WowTime::SetMonth(int8_t month)
{
    if (!(month == -1 || (month >= 0 && month < 12)))
        throw std::runtime_error("WowTime::SetMonth: invalid month value");
    _month = month;
}

void WowTime::SetMonthDay(int8_t monthDay)
{
    if (!(monthDay == -1 || (monthDay >= 0 && monthDay < 32)))
        throw std::runtime_error("WowTime::SetMonthDay: invalid monthDay value");
    _monthDay = monthDay;
}

void WowTime::SetWeekDay(int8_t weekDay)
{
    if (!(weekDay == -1 || (weekDay >= 0 && weekDay < 7)))
        throw std::runtime_error("WowTime::SetWeekDay: invalid weekDay value");
    _weekDay = weekDay;
}

void WowTime::SetHour(int8_t hour)
{
    if (!(hour == -1 || (hour >= 0 && hour < 24)))
        throw std::runtime_error("WowTime::SetHour: invalid hour value");
    _hour = hour;
}

void WowTime::SetMinute(int8_t minute)
{
    if (!(minute == -1 || (minute >= 0 && minute < 60)))
        throw std::runtime_error("WowTime::SetMinute: invalid minute value");
    _minute = minute;
}

void WowTime::SetFlags(int8_t flags)
{
    if (!(flags == -1 || (flags >= 0 && flags < 3)))
        throw std::runtime_error("WowTime::SetFlags: invalid flags value");
    _flags = flags;
}

std::strong_ordering operator<=>(WowTime const& left, WowTime const& right)
{
    auto compareFieldIfSet = [&]<typename T>(T WowTime::*field) -> std::strong_ordering
    {
        if (left.*field < 0 || right.*field < 0)
            return std::strong_ordering::equal;

        return left.*field <=> right.*field;
    };

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_year); advstd::is_neq(cmp))
        return cmp;

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_month); advstd::is_neq(cmp))
        return cmp;

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_monthDay); advstd::is_neq(cmp))
        return cmp;

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_weekDay); advstd::is_neq(cmp))
        return cmp;

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_hour); advstd::is_neq(cmp))
        return cmp;

    if (std::strong_ordering cmp = compareFieldIfSet(&WowTime::_minute); advstd::is_neq(cmp))
        return cmp;

    return std::strong_ordering::equal;
}

bool WowTime::IsInRange(WowTime const& from, WowTime const& to) const
{
    if (from > to)
        return *this >= from || *this < to;

    return *this >= from && *this < to;
}

WowTime& WowTime::operator+=(Seconds seconds)
{
    time_t unixTime = GetUnixTimeFromUtcTime();
    unixTime += seconds.count();
    SetUtcTimeFromUnixTime(unixTime);
    return *this;
}

WowTime WowTime::operator+(Seconds seconds) const
{
    return WowTime(*this) += seconds;
}

WowTime& WowTime::operator-=(Seconds seconds)
{
    time_t unixTime = GetUnixTimeFromUtcTime();
    unixTime -= seconds.count();
    SetUtcTimeFromUnixTime(unixTime);
    return *this;
}

WowTime WowTime::operator-(Seconds seconds) const
{
    return WowTime(*this) -= seconds;
}

ByteBuffer& operator<<(ByteBuffer& data, WowTime const& wowTime)
{
    data.write_uint32_le(wowTime.GetPackedTime()); // Запись в little-endian
    return data;
}

ByteBuffer& operator>>(ByteBuffer& data, WowTime& wowTime)
{
    uint32_t packedTime = data.read_uint32_le(); // Чтение little-endian
    wowTime.SetPackedTime(packedTime);
    return data;
}
