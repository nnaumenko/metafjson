/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef VALUEFORMAT_HPP
#define VALUEFORMAT_HPP

#include <string_view>
#include <string>
#include "nlohmann/json_fwd.hpp"

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
    class WeatherPhenomena;
    class CloudType;
} // namespace metaf

class ValueFormat
{
public:
    ValueFormat() = default;
    virtual ~ValueFormat() {}
    // Methods below format various values as JSON objects. Non-reported values 
    // result in empty JSON. If value must be explicitly specified as 
    // non-reported, set parameter addNotReported to true.   

    // Runway identification
    virtual nlohmann::json format(const metaf::Runway &runway) const = 0;
    // Temperature value
    virtual nlohmann::json format(const metaf::Temperature &temperature,
                                  bool addNotReported = false) const = 0;
    // Speed value
    virtual nlohmann::json format(const metaf::Speed &speed,
                                  bool addNotReported = false) const = 0;
    // Distance, height or runway visual range value
    virtual nlohmann::json format(const metaf::Distance &distance,
                                  bool heightOrRvr = false,
                                  bool addNotReported = false) const = 0;
    // Direction value
    virtual nlohmann::json format(const metaf::Direction &direction,
                                  bool addNotReported = false) const = 0;
    // Direction sector
    virtual nlohmann::json format(const metaf::Direction &sectorBegin,
                                  const metaf::Direction &sectorEnd) const = 0;
    // Vector of directions
    virtual nlohmann::json format(const std::vector<metaf::Direction> &directions) const = 0;
    // Pressure value
    virtual nlohmann::json format(const metaf::Pressure &pressure,
                                  bool addNotReported = false) const = 0;
    // Precipitation or snow/ice accumulation value
    virtual nlohmann::json format(const metaf::Precipitation &precipitation,
                                  bool addNotReported = false) const = 0;
    // Surface friction value
    virtual nlohmann::json format(const metaf::SurfaceFriction &surfaceFriction,
                                  bool addNotReported = false) const = 0;
    // Wave height value or descriptive state of sea surface
    virtual nlohmann::json format(const metaf::WaveHeight &waveHeight,
                                  bool addNotReported = false) const = 0;
};

class ValueFormatBasic : public ValueFormat
{
public:
    ValueFormatBasic() = default;
    virtual ~ValueFormatBasic() {}
    virtual nlohmann::json format(const metaf::Runway &runway) const;
    virtual nlohmann::json format(const metaf::Temperature &temperature,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::Speed &speed,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::Distance &distance,
                                  bool heightOrRvr = false,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::Direction &direction,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::Direction &sectorBegin,
                                  const metaf::Direction &sectorEnd) const;
    virtual nlohmann::json format(const std::vector<metaf::Direction> &directions) const;
    virtual nlohmann::json format(const metaf::Pressure &pressure,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::Precipitation &precipitation,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::SurfaceFriction &surfaceFriction,
                                  bool addNotReported = false) const;
    virtual nlohmann::json format(const metaf::WaveHeight &waveHeight,
                                  bool addNotReported = false) const;
};

#endif // #ifndef VALUEFORMAT_HPP
