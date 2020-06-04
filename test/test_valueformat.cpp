/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include "valueformat.hpp"

#include "nlohmann/json.hpp"
#include "metaf.hpp"

//////////////////////////////////////////////////////////////////////////////
// Data structures for testing the formatting
//////////////////////////////////////////////////////////////////////////////

class ValueFormats : public ::testing::Test
{
protected:
    ValueFormats();

    void initRunway();
    void initTemperature();
    void initSpeed();
    void initDistance();
    void initDirection();
    void initPressure();
    void initPrecipitation();
    void initSurfaceFriction();
    void initWaveHeight();

    metaf::Runway runway09;
    metaf::Runway runway09R;
    metaf::Runway runway09C;
    metaf::Runway runway09L;
    metaf::Runway runwayAll;
    metaf::Runway runwayMessageRepetition;

    metaf::Temperature temperature17;
    metaf::Temperature temperatureM01;
    metaf::Temperature temperature00;
    metaf::Temperature temperatureM00;
    metaf::Temperature temperatureNotReported;
    metaf::Temperature temperaturePrecise0147;
    metaf::Temperature temperaturePrecise1004;
    metaf::Temperature temperaturePrecise0000;
    metaf::Temperature temperaturePrecise1000;

    metaf::Speed speed10Kt;
    metaf::Speed speed10Mps;
    metaf::Speed speed10Kmh;
    metaf::Speed speed10Mph;
    metaf::Speed speedNotReported;

    metaf::Distance distanceP10km;
    metaf::Distance distance3500m;
    metaf::Distance distance3500ft;
    metaf::Distance distance3sm;
    metaf::Distance distanceP6sm;
    metaf::Distance distanceM1sm;
    metaf::Distance distance3_4sm;
    metaf::Distance distance1_3_4sm;
    metaf::Distance distanceM1_4sm;
    metaf::Distance distanceNotReported;
    metaf::Distance distanceDsnt;
    metaf::Distance distanceVc;
    metaf::Distance distance1_16sm;
    metaf::Distance distance1_8sm;
    metaf::Distance distance3_16sm;
    metaf::Distance distance1_4sm;
    metaf::Distance distance5_16sm;
    metaf::Distance distance3_8sm;
    metaf::Distance distance1_2sm;
    metaf::Distance distance5_8sm;
    metaf::Distance distance7_8sm;

    metaf::Direction directionNotReported;
    metaf::Direction directionVariable;
    metaf::Direction directionNdv;
    metaf::Direction direction270Degrees;
    metaf::Direction directionCardinal_N;
    metaf::Direction directionCardinal_NE;
    metaf::Direction directionCardinal_E;
    metaf::Direction directionCardinal_SE;
    metaf::Direction directionCardinal_S;
    metaf::Direction directionCardinal_SW;
    metaf::Direction directionCardinal_W;
    metaf::Direction directionCardinal_NW;

    metaf::Pressure pressure994hpa;
    metaf::Pressure pressure29_34inhg;
    metaf::Pressure pressure750mmhg;
    metaf::Pressure pressureNotReported;

    metaf::Precipitation precipitation1mm;
    metaf::Precipitation precipitation61_5mm;
    metaf::Precipitation precipitation13_52inches;
    metaf::Precipitation precipitationNotReported;

    metaf::SurfaceFriction surfaceFrictionCoefficient0_80;
    metaf::SurfaceFriction surfaceFrictionBrakingActionPoor;
    metaf::SurfaceFriction surfaceFrictionBrakingActionMediumPoor;
    metaf::SurfaceFriction surfaceFrictionBrakingActionMedium;
    metaf::SurfaceFriction surfaceFrictionBrakingActionMediumGood;
    metaf::SurfaceFriction surfaceFrictionBrakingActionGood;
    metaf::SurfaceFriction surfaceFrictionNotReported;
    metaf::SurfaceFriction surfaceFrictionUnreliable;

    metaf::WaveHeight waveHeightStateOfSurfaceNotReported;
    metaf::WaveHeight waveHeightStateOfSurfaceCalmGlassy;
    metaf::WaveHeight waveHeightStateOfSurfaceCalmRippled;
    metaf::WaveHeight waveHeightStateOfSurfaceSmooth;
    metaf::WaveHeight waveHeightStateOfSurfaceSlight;
    metaf::WaveHeight waveHeightStateOfSurfaceModerate;
    metaf::WaveHeight waveHeightStateOfSurfaceRough;
    metaf::WaveHeight waveHeightStateOfSurfaceVeryRough;
    metaf::WaveHeight waveHeightStateOfSurfaceHigh;
    metaf::WaveHeight waveHeightStateOfSurfaceVeryHigh;
    metaf::WaveHeight waveHeightStateOfSurfacePhenomenal;
    metaf::WaveHeight waveHeightWaveHeight3m;
    metaf::WaveHeight waveHeightWaveHeightNotReported;
};

ValueFormats::ValueFormats()
{
    initRunway();
    initTemperature();
    initSpeed();
    initDistance();
    initDirection();
    initPressure();
    initPrecipitation();
    initSurfaceFriction();
    initWaveHeight();
}

void ValueFormats::initRunway()
{
    {
        const auto r = metaf::Runway::fromString("R09");
        ASSERT_TRUE(r);
        runway09 = *r;
    }
    {
        const auto r = metaf::Runway::fromString("R09R");
        ASSERT_TRUE(r);
        runway09R = *r;
    }
    {
        const auto r = metaf::Runway::fromString("R09C");
        ASSERT_TRUE(r);
        runway09C = *r;
    }
    {
        const auto r = metaf::Runway::fromString("R09L");
        ASSERT_TRUE(r);
        runway09L = *r;
    }
    {
        const auto r = metaf::Runway::fromString("R88");
        ASSERT_TRUE(r);
        runwayAll = *r;
    }
    {
        const auto r = metaf::Runway::fromString("R99");
        ASSERT_TRUE(r);
        runwayMessageRepetition = *r;
    }
}

void ValueFormats::initTemperature()
{
    {
        const auto t = metaf::Temperature::fromString("17");
        ASSERT_TRUE(t);
        temperature17 = *t;
    }
    {
        const auto t = metaf::Temperature::fromString("M01");
        ASSERT_TRUE(t);
        temperatureM01 = *t;
    }
    {
        const auto t = metaf::Temperature::fromString("00");
        ASSERT_TRUE(t);
        temperature00 = *t;
    }
    {
        const auto t = metaf::Temperature::fromString("M00");
        ASSERT_TRUE(t);
        temperatureM00 = *t;
    }
    {
        const auto t = metaf::Temperature::fromString("//");
        ASSERT_TRUE(t);
        temperatureNotReported = *t;
    }
    {
        const auto t = metaf::Temperature::fromRemarkString("0147");
        ASSERT_TRUE(t);
        temperaturePrecise0147 = *t;
    }
    {
        const auto t = metaf::Temperature::fromRemarkString("1004");
        ASSERT_TRUE(t);
        temperaturePrecise1004 = *t;
    }
    {
        const auto t = metaf::Temperature::fromRemarkString("0000");
        ASSERT_TRUE(t);
        temperaturePrecise0000 = *t;
    }
    {
        const auto t = metaf::Temperature::fromRemarkString("1000");
        ASSERT_TRUE(t);
        temperaturePrecise1000 = *t;
    }
}

void ValueFormats::initSpeed()
{
    {
        const auto s = metaf::Speed::fromString("10", metaf::Speed::Unit::KNOTS);
        ASSERT_TRUE(s);
        speed10Kt = *s;
    }
    {
        const auto s = metaf::Speed::fromString("10", metaf::Speed::Unit::METERS_PER_SECOND);
        ASSERT_TRUE(s);
        speed10Mps = *s;
    }
    {
        const auto s = metaf::Speed::fromString("10", metaf::Speed::Unit::KILOMETERS_PER_HOUR);
        ASSERT_TRUE(s);
        speed10Kmh = *s;
    }
    {
        const auto s = metaf::Speed::fromString("10", metaf::Speed::Unit::MILES_PER_HOUR);
        ASSERT_TRUE(s);
        speed10Mph = *s;
    }
    {
        const auto s = metaf::Speed::fromString("//", metaf::Speed::Unit::KNOTS);
        ASSERT_TRUE(s);
        speedNotReported = *s;
    }
}

void ValueFormats::initDistance()
{
    {
        const auto d = metaf::Distance::fromMeterString("9999");
        ASSERT_TRUE(d);
        distanceP10km = *d;
    }
    distance3500m = metaf::Distance(3500, metaf::Distance::Unit::METERS);
    distance3500ft = metaf::Distance(3500, metaf::Distance::Unit::FEET);
    distance3sm = metaf::Distance(3, metaf::Distance::Unit::STATUTE_MILES);
    {
        const auto d = metaf::Distance::fromMileString("P6SM");
        ASSERT_TRUE(d);
        distanceP6sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("M1SM");
        ASSERT_TRUE(d);
        distanceM1sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("3/4SM");
        ASSERT_TRUE(d);
        distance3_4sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("13/4SM");
        ASSERT_TRUE(d);
        distance1_3_4sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("M1/4SM");
        ASSERT_TRUE(d);
        distanceM1_4sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("////SM");
        ASSERT_TRUE(d);
        distanceNotReported = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("1/16SM");
        ASSERT_TRUE(d);
        distance1_16sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("1/8SM");
        ASSERT_TRUE(d);
        distance1_8sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("3/16SM");
        ASSERT_TRUE(d);
        distance3_16sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("1/4SM");
        ASSERT_TRUE(d);
        distance1_4sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("5/16SM");
        ASSERT_TRUE(d);
        distance5_16sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("3/8SM");
        ASSERT_TRUE(d);
        distance3_8sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("1/2SM");
        ASSERT_TRUE(d);
        distance1_2sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("5/8SM");
        ASSERT_TRUE(d);
        distance5_8sm = *d;
    }
    {
        const auto d = metaf::Distance::fromMileString("7/8SM");
        ASSERT_TRUE(d);
        distance7_8sm = *d;
    }
    {
        distanceDsnt = metaf::Distance::makeDistant();
        distanceVc = metaf::Distance::makeVicinity();
    }
}

void ValueFormats::initDirection()
{
    {
        const auto d = metaf::Direction::fromDegreesString("///");
        ASSERT_TRUE(d);
        directionNotReported = *d;
    }
    {
        const auto d = metaf::Direction::fromDegreesString("VRB");
        ASSERT_TRUE(d);
        directionVariable = *d;
    }
    {
        auto d = metaf::Direction::fromCardinalString("NDV");
        ASSERT_TRUE(d);
        directionNdv = *d;
    }
    {
        const auto d = metaf::Direction::fromDegreesString("270");
        ASSERT_TRUE(d);
        direction270Degrees = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("N");
        ASSERT_TRUE(d);
        directionCardinal_N = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("NE");
        ASSERT_TRUE(d);
        directionCardinal_NE = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("E");
        ASSERT_TRUE(d);
        directionCardinal_E = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("SE");
        ASSERT_TRUE(d);
        directionCardinal_SE = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("S");
        ASSERT_TRUE(d);
        directionCardinal_S = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("SW");
        ASSERT_TRUE(d);
        directionCardinal_SW = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("W");
        ASSERT_TRUE(d);
        directionCardinal_W = *d;
    }
    {
        const auto d = metaf::Direction::fromCardinalString("NW");
        ASSERT_TRUE(d);
        directionCardinal_NW = *d;
    }
}

void ValueFormats::initPressure()
{
    {
        const auto p = metaf::Pressure::fromString("Q0994");
        ASSERT_TRUE(p);
        pressure994hpa = *p;
    }
    {
        const auto p = metaf::Pressure::fromString("A2934");
        ASSERT_TRUE(p);
        pressure29_34inhg = *p;
    }
    {
        const auto p = metaf::Pressure::fromQfeString("QFE750");
        ASSERT_TRUE(p);
        pressure750mmhg = *p;
    }
    {
        const auto p = metaf::Pressure::fromString("Q////");
        ASSERT_TRUE(p);
        pressureNotReported = *p;
    }
}

void ValueFormats::initPrecipitation()
{
    {
        const auto p = metaf::Precipitation::fromRunwayDeposits("01");
        ASSERT_TRUE(p);
        precipitation1mm = *p;
    }
    {
        const auto p = metaf::Precipitation::fromRainfallString("061.5");
        ASSERT_TRUE(p);
        precipitation61_5mm = *p;
    }
    {
        const auto p = metaf::Precipitation::fromRemarkString("1352", 0.01);
        ASSERT_TRUE(p);
        precipitation13_52inches = *p;
    }
    {
        const auto p = metaf::Precipitation::fromRunwayDeposits("//");
        ASSERT_TRUE(p);
        precipitationNotReported = *p;
    }
}

void ValueFormats::initSurfaceFriction()
{
    {
        const auto sf = metaf::SurfaceFriction::fromString("80");
        ASSERT_TRUE(sf);
        surfaceFrictionCoefficient0_80 = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("91");
        ASSERT_TRUE(sf);
        surfaceFrictionBrakingActionPoor = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("92");
        ASSERT_TRUE(sf);
        surfaceFrictionBrakingActionMediumPoor = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("93");
        ASSERT_TRUE(sf);
        surfaceFrictionBrakingActionMedium = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("94");
        ASSERT_TRUE(sf);
        surfaceFrictionBrakingActionMediumGood = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("95");
        ASSERT_TRUE(sf);
        surfaceFrictionBrakingActionGood = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("//");
        ASSERT_TRUE(sf);
        surfaceFrictionNotReported = *sf;
    }
    {
        const auto sf = metaf::SurfaceFriction::fromString("99");
        ASSERT_TRUE(sf);
        surfaceFrictionUnreliable = *sf;
    }
}

void ValueFormats::initWaveHeight()
{
    {
        const auto wh = metaf::WaveHeight::fromString("S/");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceNotReported = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S0");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceCalmGlassy = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S1");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceCalmRippled = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S2");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceSmooth = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S3");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceSlight = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S4");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceModerate = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S5");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceRough = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S6");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceVeryRough = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S7");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceHigh = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S8");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfaceVeryHigh = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("S9");
        ASSERT_TRUE(wh);
        waveHeightStateOfSurfacePhenomenal = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("H30");
        ASSERT_TRUE(wh);
        waveHeightWaveHeight3m = *wh;
    }
    {
        const auto wh = metaf::WaveHeight::fromString("H///");
        ASSERT_TRUE(wh);
        waveHeightWaveHeightNotReported = *wh;
    }
}

//////////////////////////////////////////////////////////////////////////////
// Basic format
//////////////////////////////////////////////////////////////////////////////

TEST_F(ValueFormats, basicRunway)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(runway09), (nlohmann::json{{"number", 9}}));
    EXPECT_EQ(vfb.format(runway09R), (nlohmann::json{{"number", 9}, {"designator", "right"}}));
    EXPECT_EQ(vfb.format(runway09C), (nlohmann::json{{"number", 9}, {"designator", "center"}}));
    EXPECT_EQ(vfb.format(runway09L), (nlohmann::json{{"number", 9}, {"designator", "left"}}));
    EXPECT_EQ(vfb.format(runwayAll), (nlohmann::json{{"all_runways", true}}));
    EXPECT_EQ(vfb.format(runwayMessageRepetition), (nlohmann::json{{"msg_repetition", true}}));
}

TEST_F(ValueFormats, basicTemperature)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(temperature17), (nlohmann::json{{"c", 17}}));
    EXPECT_EQ(vfb.format(temperatureM01), (nlohmann::json{{"c", -1}}));
    EXPECT_EQ(vfb.format(temperature00), (nlohmann::json{{"c", 0}}));
    EXPECT_EQ(vfb.format(temperatureM00), (nlohmann::json{{"c", 0}, {"freezing", true}}));
    EXPECT_EQ(vfb.format(temperaturePrecise0147), (nlohmann::json{{"c", 14.7}}));
    EXPECT_EQ(vfb.format(temperaturePrecise1004), (nlohmann::json{{"c", -0.4}}));
    EXPECT_EQ(vfb.format(temperaturePrecise0000), (nlohmann::json{{"c", 0.0}}));
    EXPECT_EQ(vfb.format(temperaturePrecise1000), (nlohmann::json{{"c", 0.0}}));
}

TEST_F(ValueFormats, basicSpeed)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(speed10Kt), (nlohmann::json{{"kt", 10}}));
    EXPECT_EQ(vfb.format(speed10Mps), (nlohmann::json{{"mps", 10}}));
    EXPECT_EQ(vfb.format(speed10Kmh), (nlohmann::json{{"kmh", 10}}));
    EXPECT_EQ(vfb.format(speed10Mps), (nlohmann::json{{"mps", 10}}));
    EXPECT_EQ(vfb.format(speedNotReported), (nlohmann::json{}));
    EXPECT_EQ(vfb.format(speedNotReported, true), (nlohmann::json{{"not_reported", true}}));
}

TEST_F(ValueFormats, basicDistance)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(distanceP10km),
              (nlohmann::json{{"km", 10.000}, {"modifier", "more_than"}}));
    EXPECT_EQ(vfb.format(distance3500m), (nlohmann::json{{"km", 3.5}}));
    EXPECT_EQ(vfb.format(distance3500ft), (nlohmann::json{{"ft", 3500}}));
    EXPECT_EQ(vfb.format(distance3sm), (nlohmann::json{
                                           {"sm", 3},
                                           {"sm_fraction", "3"},
                                       }));
    EXPECT_EQ(vfb.format(distanceP6sm),
              (nlohmann::json{{"sm", 6}, {"sm_fraction", "6"}, {"modifier", "more_than"}}));
    EXPECT_EQ(vfb.format(distanceM1sm),
              (nlohmann::json{{"sm", 1}, {"sm_fraction", "1"}, {"modifier", "less_than"}}));
    EXPECT_EQ(vfb.format(distance1_3_4sm),
              (nlohmann::json{{"sm", 1.750}, {"sm_fraction", "1 3/4"}}));
    EXPECT_EQ(vfb.format(distanceM1_4sm),
              (nlohmann::json{{"sm", 0.250}, {"modifier", "less_than"}, {"sm_fraction", "1/4"}}));
    EXPECT_EQ(vfb.format(distanceNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(distanceNotReported, false, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(distanceNotReported, true, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(distanceNotReported, false),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(distanceNotReported, true),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(distance1_16sm),
              (nlohmann::json{{"sm", 0.063}, {"sm_fraction", "1/16"}}));
    EXPECT_EQ(vfb.format(distance1_8sm),
              (nlohmann::json{{"sm", 0.125}, {"sm_fraction", "1/8"}}));
    EXPECT_EQ(vfb.format(distance3_16sm),
              (nlohmann::json{{"sm", 0.188}, {"sm_fraction", "3/16"}}));
    EXPECT_EQ(vfb.format(distance1_4sm),
              (nlohmann::json{{"sm", 0.250}, {"sm_fraction", "1/4"}}));
    EXPECT_EQ(vfb.format(distance5_16sm),
              (nlohmann::json{{"sm", 0.313}, {"sm_fraction", "5/16"}}));
    EXPECT_EQ(vfb.format(distance3_8sm),
              (nlohmann::json{{"sm", 0.375}, {"sm_fraction", "3/8"}}));
    EXPECT_EQ(vfb.format(distance1_2sm),
              (nlohmann::json{{"sm", 0.500}, {"sm_fraction", "1/2"}}));
    EXPECT_EQ(vfb.format(distance5_8sm),
              (nlohmann::json{{"sm", 0.625}, {"sm_fraction", "5/8"}}));
    EXPECT_EQ(vfb.format(distance3_4sm),
              (nlohmann::json{{"sm", 0.750}, {"sm_fraction", "3/4"}}));
    EXPECT_EQ(vfb.format(distance7_8sm),
              (nlohmann::json{{"sm", 0.875}, {"sm_fraction", "7/8"}}));
    EXPECT_EQ(vfb.format(distanceDsnt),
              (nlohmann::json{{"modifier", "distant"}}));
    EXPECT_EQ(vfb.format(distanceVc),
              (nlohmann::json{{"modifier", "vicinity"}}));
}

TEST_F(ValueFormats, basicHeightOrRvr)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(distanceP10km, true),
              (nlohmann::json{{"m", 10000}, {"modifier", "more_than"}}));
    EXPECT_EQ(vfb.format(distance3500m, true), (nlohmann::json{{"m", 3500}}));
    EXPECT_EQ(vfb.format(distance3500ft, true), (nlohmann::json{{"ft", 3500}}));
    EXPECT_EQ(vfb.format(distance3sm, true), (nlohmann::json{{"ft", 15840}}));
    EXPECT_EQ(vfb.format(distanceP6sm, true),
              (nlohmann::json{{"ft", 31680}, {"modifier", "more_than"}}));
    EXPECT_EQ(vfb.format(distanceM1sm, true),
              (nlohmann::json{{"ft", 5280}, {"modifier", "less_than"}}));
    EXPECT_EQ(vfb.format(distance1_3_4sm, true),
              (nlohmann::json{{"ft", 9240}}));
    EXPECT_EQ(vfb.format(distanceM1_4sm, true),
              (nlohmann::json{{"ft", 1320}, {"modifier", "less_than"}}));
}

TEST_F(ValueFormats, basicDirection)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(directionNotReported, false),
              (nlohmann::json()));
    EXPECT_EQ(vfb.format(directionNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(directionVariable),
              (nlohmann::json{{"variable", true}}));
    EXPECT_EQ(vfb.format(directionNdv),
              (nlohmann::json{{"ndv", true}}));
    EXPECT_EQ(vfb.format(direction270Degrees),
              (nlohmann::json{{"degrees", 270}}));
    EXPECT_EQ(vfb.format(directionCardinal_N),
              (nlohmann::json{{"cardinal", "n"}}));
    EXPECT_EQ(vfb.format(directionCardinal_NE),
              (nlohmann::json{{"cardinal", "ne"}}));
    EXPECT_EQ(vfb.format(directionCardinal_E),
              (nlohmann::json{{"cardinal", "e"}}));
    EXPECT_EQ(vfb.format(directionCardinal_SE),
              (nlohmann::json{{"cardinal", "se"}}));
    EXPECT_EQ(vfb.format(directionCardinal_S),
              (nlohmann::json{{"cardinal", "s"}}));
    EXPECT_EQ(vfb.format(directionCardinal_SW),
              (nlohmann::json{{"cardinal", "sw"}}));
    EXPECT_EQ(vfb.format(directionCardinal_W),
              (nlohmann::json{{"cardinal", "w"}}));
    EXPECT_EQ(vfb.format(directionCardinal_NW),
              (nlohmann::json{{"cardinal", "nw"}}));
}

TEST_F(ValueFormats, basicPressure)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(pressure994hpa),
              (nlohmann::json{{"hpa", 994.0}}));
    EXPECT_EQ(vfb.format(pressure29_34inhg),
              (nlohmann::json{{"inhg", 29.34}}));
    EXPECT_EQ(vfb.format(pressure750mmhg),
              (nlohmann::json{{"mmhg", 750.0}}));
    EXPECT_EQ(vfb.format(pressureNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(pressureNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(pressureNotReported),
              (nlohmann::json{}));
}

TEST_F(ValueFormats, basicPrecipitation)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(precipitation1mm),
              (nlohmann::json{{"mm", 1.0}}));
    EXPECT_EQ(vfb.format(precipitation61_5mm),
              (nlohmann::json{{"mm", 61.5}}));
    EXPECT_EQ(vfb.format(precipitation13_52inches),
              (nlohmann::json{{"in", 13.52}}));
    EXPECT_EQ(vfb.format(precipitationNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(precipitationNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
}

TEST_F(ValueFormats, basicSurfaceFriction)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(surfaceFrictionCoefficient0_80),
              (nlohmann::json{{"friction_coefficient", 0.8}}));
    EXPECT_EQ(vfb.format(surfaceFrictionBrakingActionPoor),
              (nlohmann::json{{"braking_action", "poor"}}));
    EXPECT_EQ(vfb.format(surfaceFrictionBrakingActionMediumPoor),
              (nlohmann::json{{"braking_action", "medium_poor"}}));
    EXPECT_EQ(vfb.format(surfaceFrictionBrakingActionMedium),
              (nlohmann::json{{"braking_action", "medium"}}));
    EXPECT_EQ(vfb.format(surfaceFrictionBrakingActionMediumGood),
              (nlohmann::json{{"braking_action", "medium_good"}}));
    EXPECT_EQ(vfb.format(surfaceFrictionBrakingActionGood),
              (nlohmann::json{{"braking_action", "good"}}));
    EXPECT_EQ(vfb.format(surfaceFrictionNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(surfaceFrictionNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(surfaceFrictionUnreliable),
              (nlohmann::json{{"unreliable", true}}));
}

TEST_F(ValueFormats, basicWaveHeight)
{
    ValueFormatBasic vfb;
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceCalmGlassy),
              (nlohmann::json{{"surface_state", "calm_glassy"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceCalmRippled),
              (nlohmann::json{{"surface_state", "calm_rippled"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceSmooth),
              (nlohmann::json{{"surface_state", "smooth"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceSlight),
              (nlohmann::json{{"surface_state", "slight"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceModerate),
              (nlohmann::json{{"surface_state", "moderate"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceRough),
              (nlohmann::json{{"surface_state", "rough"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceVeryRough),
              (nlohmann::json{{"surface_state", "very_rough"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceHigh),
              (nlohmann::json{{"surface_state", "high"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfaceVeryHigh),
              (nlohmann::json{{"surface_state", "very_high"}}));
    EXPECT_EQ(vfb.format(waveHeightStateOfSurfacePhenomenal),
              (nlohmann::json{{"surface_state", "phenomenal"}}));
    EXPECT_EQ(vfb.format(waveHeightWaveHeight3m),
              (nlohmann::json{{"m", 3}}));
    EXPECT_EQ(vfb.format(waveHeightWaveHeightNotReported),
              (nlohmann::json{}));
    EXPECT_EQ(vfb.format(waveHeightWaveHeightNotReported, true),
              (nlohmann::json{{"not_reported", true}}));
}
