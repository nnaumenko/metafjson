/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef DATETIMEFORMAT_HPP
#define DATETIMEFORMAT_HPP

#include "nlohmann/json_fwd.hpp"

namespace metaf
{
    class MetafTime;
}

class DateTimeFormat
{
public:
    struct DateTime
    {
        DateTime() = default;
        DateTime(const metaf::MetafTime &reportTime,
                 int refYear,
                 unsigned refMonth,
                 unsigned refDay);
        DateTime(const DateTime &reportTime,
                 const metaf::MetafTime &time,
                 bool forecast = false);

        // Converts date/time into Unix time
        time_t toUnixTime();
        bool isEmpty() { return !year && !month && !day && !hour && !minute && !metafTime; }

        int year = 0;
        unsigned month = 0;
        unsigned day = 0;
        unsigned hour = 0;
        unsigned minute = 0;
        std::unique_ptr<metaf::MetafTime> metafTime;

    private:
        // decrease month (and year if jumping from January to December),
        // leave day, hour, minute intact
        void previousMonth();
        // increase month (and year if jumping from December to January),
        // leave day, hour, minute intact
        void nextMonth();
        // increase day (also month if last day of month and year if last
        // day of year), leave hour and minute intact
        void nextDay();
    };

    DateTimeFormat() = default;
    virtual ~DateTimeFormat() {}
    virtual nlohmann::json format(const DateTime &dateTime) const = 0;
    nlohmann::json format(const metaf::MetafTime &time,
                          const DateTime &reportTime,
                          bool forecast = false) const;

protected:
};

class DateTimeFormatBasic : public DateTimeFormat
{
public:
    DateTimeFormatBasic() = default;
    virtual ~DateTimeFormatBasic() {}
    virtual nlohmann::json format(const DateTime &dateTime) const;
};

#endif // #ifndef DATETIMEFORMAT_HPP