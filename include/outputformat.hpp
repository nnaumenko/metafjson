/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef OUTPUTFORMAT_HPP
#define OUTPUTFORMAT_HPP

#include <iostream>
#include <memory>

#include "nlohmann/json_fwd.hpp"

#include "datetimeformat.hpp"
#include "valueformat.hpp"

class Settings;

namespace metaf
{
class Runway;
class Temperature;
class Speed;
class Distance;
class Direction;
class Pressure;
class Precipitation;
class SurfaceFriction;
class WaveHeight;
enum class Weather;
enum class WeatherDescriptor;
struct ParseResult;
} // namespace metaf

class OutputFormat
{
public:
    OutputFormat(std::unique_ptr<const DateTimeFormat> dtFormat,
                 std::unique_ptr<const ValueFormat> valFormat,
                 bool rawStrings,
                 int refYear,
                 unsigned refMonth,
                 unsigned refDay)
        : dateTimeFormat(std::move(dtFormat)),
          valueFormat(std::move(valFormat)),
          includeRawStrings(rawStrings),
          referenceYear(refYear),
          referenceMonth(refMonth),
          referenceDay(refDay)
    {
    }
    virtual ~OutputFormat() {}
    // Result of METAR or TAF report parsing and serialising to JSON
    enum class Result
    {
        OK,       // Result parsed and serialised OK
        EXCEPTION // Exception occurred during parsing or serialising
    };
    //
    Result toJson(const std::string &report, std::ostream &out = std::cout) const;

protected:
    // Parse a METAR or TAF report and serialise to JSON
    virtual nlohmann::json toJson(
        const metaf::ParseResult &parseResult) const = 0;

    std::unique_ptr<const DateTimeFormat> dateTimeFormat;
    std::unique_ptr<const ValueFormat> valueFormat;

    bool getIncludeRawStrings() const { return includeRawStrings; }
    int getReferenceYear() const { return referenceYear; }
    int getReferenceMonth() const { return referenceMonth; }
    int getReferenceDay() const { return referenceDay; }
private:
    bool includeRawStrings = false;
    int referenceYear = 0;
    int referenceMonth = 0;
    int referenceDay = 0;
};

#endif //#ifndef OUTPUTFORMAT_HPP