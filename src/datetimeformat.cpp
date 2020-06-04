/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "datetimeformat.hpp"

#include <stdexcept>

#include "nlohmann/json.hpp"
#include "metaf.hpp"
#include "date/date.h"

//////////////////////////////////////////////////////////////////////////////
// DateTimeFormat::DateTime
//////////////////////////////////////////////////////////////////////////////

DateTimeFormat::DateTime::DateTime(const metaf::MetafTime &reportTime,
                                   int refYear,
                                   unsigned refMonth,
                                   unsigned refDay)
{
    metafTime = std::make_unique<metaf::MetafTime>(reportTime);
    const metaf::MetafTime::Date refDate(refYear, refMonth, refDay);
    const auto reportDate = reportTime.dateBeforeRef(refDate);
    year = reportDate.year;
    month = reportDate.month;
    day = reportDate.day;
    hour = reportTime.hour();
    minute = reportTime.minute();
}

DateTimeFormat::DateTime::DateTime(const DateTime &reportTime,
                                   const metaf::MetafTime &time,
                                   bool isForecast)
{
    metafTime = std::make_unique<metaf::MetafTime>(time);
    year = reportTime.year;
    month = reportTime.month;
    hour = time.hour();
    minute = time.minute();
    if (time.day().has_value())
    { // day is specified in metafTime
        day = time.day().value();
        if (day < reportTime.day)
            nextMonth();
        const auto dayEndOfMonth = 27u; // Month can have minimum 28 days
        const auto dayBeginningOfMonth = 2u;
        if (day >= dayEndOfMonth && reportTime.day < dayBeginningOfMonth)
        {
            previousMonth();
        }
    }
    else
    { // day is not specified in metafTime
        day = reportTime.day;
        const auto reportMinutesSinceMidnight = reportTime.hour * 60 + reportTime.minute;
        const auto timeMinutesSinceMidnight = time.hour() * 60 + time.minute();
        if (isForecast && timeMinutesSinceMidnight < reportMinutesSinceMidnight)
            nextDay();
    }
    // Normalise 24 hours (= 00 hours of next day)
    if (hour == 24)
    {
        hour = 0;
        nextDay();
    }
}

void DateTimeFormat::DateTime::previousMonth()
{
    using namespace date;
    using namespace std::chrono;
    auto ymd = year_month_day(date::year{this->year},
                              date::month{this->month},
                              date::day{this->day});
    ymd -= months{1};
    this->year = (int)ymd.year();
    this->month = (unsigned)ymd.month();
    this->day = (unsigned)ymd.day();
}

void DateTimeFormat::DateTime::nextMonth()
{
    using namespace date;
    using namespace std::chrono;
    auto ymd = year_month_day(date::year{this->year},
                              date::month{this->month},
                              date::day{this->day});
    ymd += months{1};
    this->year = (int)ymd.year();
    this->month = (unsigned)ymd.month();
    this->day = (unsigned)ymd.day();
}

void DateTimeFormat::DateTime::nextDay()
{
    using namespace date;
    using namespace std::chrono;
    const sys_days sd = year_month_day(date::year{this->year},
                                       date::month{this->month},
                                       date::day{this->day});
    const year_month_day ymd = sd + days{1};
    this->year = (int)ymd.year();
    this->month = (unsigned)ymd.month();
    this->day = (unsigned)ymd.day();
}

time_t DateTimeFormat::DateTime::toUnixTime()
{
    using namespace date;
    using namespace std::chrono;
    const auto s = date::sys_seconds(
        sys_days{date::year{this->year} / date::month{this->month} / date::day{this->day}} +
        hours{this->hour} +
        minutes{this->minute});
    return (s.time_since_epoch().count());
}

nlohmann::json DateTimeFormat::format(const metaf::MetafTime &time,
                      const DateTime & reportTime,
                      bool forecast) const
{
    DateTime dt (reportTime, time, forecast);
    return format(dt);
}

//////////////////////////////////////////////////////////////////////////////
// DateTimeFormatBasic
//////////////////////////////////////////////////////////////////////////////

nlohmann::json DateTimeFormatBasic::format(const DateTime &dateTime) const
{
    nlohmann::json j;
    if (const auto d = dateTime.metafTime->day(); d.has_value())
        j["day"] = *d;
    j["hour"] = dateTime.metafTime->hour();
    j["minute"] = dateTime.metafTime->minute();
    return j;
}
