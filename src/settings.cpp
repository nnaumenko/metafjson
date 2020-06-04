/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "settings.hpp"

#include <chrono>

#include "date/date.h"

void Settings::setRefDate(int year, unsigned month, unsigned day)
{
    refYear = year;
    refMonth = month;
    refDay = day;
}

void Settings::setRefDate()
{
    using namespace date;
    using namespace std::chrono;
    auto now = system_clock::now();
    setRefDate(
        (int)year_month_day{floor<days>(now)}.year(),
        (unsigned)year_month_day{floor<days>(now)}.month(),
        (unsigned)year_month_day{floor<days>(now)}.day()
    );
}