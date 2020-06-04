/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include "datetimeformat.hpp"

#include "nlohmann/json.hpp"
#include "metaf.hpp"

class DateTimeFormats : public ::testing::Test
{
protected:
    DateTimeFormats();
    void initMetafTime();

    metaf::MetafTime mt081835;
    metaf::MetafTime mt081917;
    metaf::MetafTime mt090217;
    metaf::MetafTime mt312255;
    metaf::MetafTime mt010426;

    metaf::MetafTime mt0040;
    metaf::MetafTime mt0630;
    metaf::MetafTime mt1850;
    metaf::MetafTime mt2415;

    metaf::MetafTime mt282400;
    metaf::MetafTime mt292400;
    metaf::MetafTime mt302400;
    metaf::MetafTime mt312400;
};

DateTimeFormats::DateTimeFormats()
{
    initMetafTime();
}

void DateTimeFormats::initMetafTime()
{
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("081835");
        ASSERT_TRUE(t);
        mt081835 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("081917");
        ASSERT_TRUE(t);
        mt081917 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("090217");
        ASSERT_TRUE(t);
        mt090217 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("312255");
        ASSERT_TRUE(t);
        mt312255 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("010426");
        ASSERT_TRUE(t);
        mt010426 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("0040");
        ASSERT_TRUE(t);
        mt0040 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("0630");
        ASSERT_TRUE(t);
        mt0630 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("1850");
        ASSERT_TRUE(t);
        mt1850 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("2415");
        ASSERT_TRUE(t);
        mt2415 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("282400");
        ASSERT_TRUE(t);
        mt282400 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("292400");
        ASSERT_TRUE(t);
        mt292400 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("302400");
        ASSERT_TRUE(t);
        mt302400 = *t;
    }
    {
        const auto t = metaf::MetafTime::fromStringDDHHMM("312400");
        ASSERT_TRUE(t);
        mt312400 = *t;
    }
}

TEST_F(DateTimeFormats, DateTime_defaultConstructor_empty)
{
    DateTimeFormat::DateTime dt;
    EXPECT_TRUE(dt.isEmpty());
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_sameDayMidMonth)
{
    DateTimeFormat::DateTime dt (mt081835, 2019, 10, 8);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 8u);
    EXPECT_EQ(dt.hour, 18u);
    EXPECT_EQ(dt.minute, 35u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 8u);
    EXPECT_EQ(dt.metafTime->hour(), 18u);
    EXPECT_EQ(dt.metafTime->minute(), 35u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_differentDaySameMonth)
{
    DateTimeFormat::DateTime dt (mt081835, 2019, 10, 9);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 8u);
    EXPECT_EQ(dt.hour, 18u);
    EXPECT_EQ(dt.minute, 35u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 8u);
    EXPECT_EQ(dt.metafTime->hour(), 18u);
    EXPECT_EQ(dt.metafTime->minute(), 35u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_sameDayFirstDayOfMonth)
{
    DateTimeFormat::DateTime dt (mt010426, 2019, 11, 01);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 11u);
    EXPECT_EQ(dt.day, 1u);
    EXPECT_EQ(dt.hour, 4u);
    EXPECT_EQ(dt.minute, 26u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 1u);
    EXPECT_EQ(dt.metafTime->hour(), 4u);
    EXPECT_EQ(dt.metafTime->minute(), 26u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_sameDayLastDayOfMonth)
{
    DateTimeFormat::DateTime dt (mt312255, 2019, 10, 31);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 31u);
    EXPECT_EQ(dt.hour, 22u);
    EXPECT_EQ(dt.minute, 55u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 31u);
    EXPECT_EQ(dt.metafTime->hour(), 22u);
    EXPECT_EQ(dt.metafTime->minute(), 55u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_differentMonth)
{
    DateTimeFormat::DateTime dt (mt312255, 2019, 11, 1);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 31u);
    EXPECT_EQ(dt.hour, 22u);
    EXPECT_EQ(dt.minute, 55u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 31u);
    EXPECT_EQ(dt.metafTime->hour(), 22u);
    EXPECT_EQ(dt.metafTime->minute(), 55u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_sameDayFirstDayOfYear)
{
    DateTimeFormat::DateTime dt (mt010426, 2019, 1, 1);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 1u);
    EXPECT_EQ(dt.day, 1u);
    EXPECT_EQ(dt.hour, 4u);
    EXPECT_EQ(dt.minute, 26u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 1u);
    EXPECT_EQ(dt.metafTime->hour(), 4u);
    EXPECT_EQ(dt.metafTime->minute(), 26u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_sameDayLastDayOfYear)
{
    DateTimeFormat::DateTime dt (mt312255, 2019, 12, 31);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 12u);
    EXPECT_EQ(dt.day, 31u);
    EXPECT_EQ(dt.hour, 22u);
    EXPECT_EQ(dt.minute, 55u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 31u);
    EXPECT_EQ(dt.metafTime->hour(), 22u);
    EXPECT_EQ(dt.metafTime->minute(), 55u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeRefDate_differentYear)
{
    DateTimeFormat::DateTime dt (mt312255, 2019, 1, 1);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2018);
    EXPECT_EQ(dt.month, 12u);
    EXPECT_EQ(dt.day, 31u);
    EXPECT_EQ(dt.hour, 22u);
    EXPECT_EQ(dt.minute, 55u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 31u);
    EXPECT_EQ(dt.metafTime->hour(), 22u);
    EXPECT_EQ(dt.metafTime->minute(), 55u);
}

// TODO: try report time 24:10 with different day, different month, different year
// This is NOT a valid format but recovery from it should be tested anyway

//TODO: 28th & 29th February
//TODO: incorrect dates, 29th February on non-leap years, 30th February on leap years
//TODO: 32th August, 31st November...




TEST_F(DateTimeFormats, DateTime_constructorReportTimeTrendTimeWithDate_sameDayMidMonth)
{
    DateTimeFormat::DateTime reportDt (mt081835, 2019, 10, 8);
    DateTimeFormat::DateTime dt (reportDt, mt081917);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 8u);
    EXPECT_EQ(dt.hour, 19u);
    EXPECT_EQ(dt.minute, 17u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 8u);
    EXPECT_EQ(dt.metafTime->hour(), 19u);
    EXPECT_EQ(dt.metafTime->minute(), 17u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeTrendTimeWithDate_differentDaySameMonth)
{
    DateTimeFormat::DateTime reportDt (mt081835, 2019, 10, 8);
    DateTimeFormat::DateTime dt (reportDt, mt090217);
    EXPECT_FALSE(dt.isEmpty());
    EXPECT_EQ(dt.year, 2019);
    EXPECT_EQ(dt.month, 10u);
    EXPECT_EQ(dt.day, 9u);
    EXPECT_EQ(dt.hour, 2u);
    EXPECT_EQ(dt.minute, 17u);
    EXPECT_TRUE(dt.metafTime->day().has_value());
    EXPECT_EQ(dt.metafTime->day().value(), 9u);
    EXPECT_EQ(dt.metafTime->hour(), 2u);
    EXPECT_EQ(dt.metafTime->minute(), 17u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeTrendTimeWithDate_differentMonth)
{
    DateTimeFormat::DateTime reportDt1 (mt010426, 2019, 11, 1);
    DateTimeFormat::DateTime dt1 (reportDt1, mt312255);
    EXPECT_FALSE(dt1.isEmpty());
    EXPECT_EQ(dt1.year, 2019);
    EXPECT_EQ(dt1.month, 10u);
    EXPECT_EQ(dt1.day, 31u);
    EXPECT_EQ(dt1.hour, 22u);
    EXPECT_EQ(dt1.minute, 55u);
    EXPECT_TRUE(dt1.metafTime->day().has_value());
    EXPECT_EQ(dt1.metafTime->day().value(), 31u);
    EXPECT_EQ(dt1.metafTime->hour(), 22u);
    EXPECT_EQ(dt1.metafTime->minute(), 55u);

    DateTimeFormat::DateTime reportDt2 (mt312255, 2019, 10, 31);
    DateTimeFormat::DateTime dt2 (reportDt2, mt010426);
    EXPECT_FALSE(dt2.isEmpty());
    EXPECT_EQ(dt2.year, 2019);
    EXPECT_EQ(dt2.month, 11u);
    EXPECT_EQ(dt2.day, 1u);
    EXPECT_EQ(dt2.hour, 4u);
    EXPECT_EQ(dt2.minute, 26u);
    EXPECT_TRUE(dt2.metafTime->day().has_value());
    EXPECT_EQ(dt2.metafTime->day().value(), 1u);
    EXPECT_EQ(dt2.metafTime->hour(), 4u);
    EXPECT_EQ(dt2.metafTime->minute(), 26u);
}

TEST_F(DateTimeFormats, DateTime_constructorReportTimeTrendTimeWithDate_differentYear)
{
    DateTimeFormat::DateTime reportDt1 (mt010426, 2019, 1, 1);
    DateTimeFormat::DateTime dt1 (reportDt1, mt312255);
    EXPECT_FALSE(dt1.isEmpty());
    EXPECT_EQ(dt1.year, 2018);
    EXPECT_EQ(dt1.month, 12u);
    EXPECT_EQ(dt1.day, 31u);
    EXPECT_EQ(dt1.hour, 22u);
    EXPECT_EQ(dt1.minute, 55u);
    EXPECT_TRUE(dt1.metafTime->day().has_value());
    EXPECT_EQ(dt1.metafTime->day().value(), 31u);
    EXPECT_EQ(dt1.metafTime->hour(), 22u);
    EXPECT_EQ(dt1.metafTime->minute(), 55u);

    DateTimeFormat::DateTime reportDt2 (mt312255, 2019, 12, 31);
    DateTimeFormat::DateTime dt2 (reportDt2, mt010426);
    EXPECT_FALSE(dt2.isEmpty());
    EXPECT_EQ(dt2.year, 2020);
    EXPECT_EQ(dt2.month, 1u);
    EXPECT_EQ(dt2.day, 1u);
    EXPECT_EQ(dt2.hour, 4u);
    EXPECT_EQ(dt2.minute, 26u);
    EXPECT_TRUE(dt2.metafTime->day().has_value());
    EXPECT_EQ(dt2.metafTime->day().value(), 1u);
    EXPECT_EQ(dt2.metafTime->hour(), 4u);
    EXPECT_EQ(dt2.metafTime->minute(), 26u);
}


//TODO: time only
//TODO: mid-month, same day, time before and after report datetime
//TODO: mid-month, different day, time 1 day before and 1 day after report datetime
//TODO: mid-month, time 24:xx
//TODO: first day of month, same day, time before and after report datetime
//TODO: first day of month, previous day
//TODO: last day of month, same day, time before and after report datetime
//TODO: last day of month, same day before report time, next day time and 24:xx time
//TODO: first day of year, same day, time before and after report datetime
//TODO: first day of year, previous day
//TODO: last day of year, same day, time before and after report datetime
//TODO: last day of year, same day before report time, next day time and 24:xx time



//TODO: first & last day of month
//TODO: different month
//TODO: last & first day of year
//TODO: different year
//TODO: time 24:00, 24:10
//TODO: 28th & 29th February
//TODO: incorrect dates, 29th February on non-leap years, 32th August, 31st November, 32nd December


//TODO: test method format(const metaf::MetafTime &, const DateTimeFormat::DateTime &, bool)