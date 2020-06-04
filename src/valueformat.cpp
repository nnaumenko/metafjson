/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "valueformat.hpp"

#include <cmath>

#include "metaf.hpp"
#include "nlohmann/json.hpp"
#include "magic_enum.hpp"

#include "utility.hpp"

//////////////////////////////////////////////////////////////////////////////
// ValueFormatBasic
//////////////////////////////////////////////////////////////////////////////

nlohmann::json ValueFormatBasic::format(const metaf::Runway &runway) const
{
	if (runway.isAllRunways())
		return nlohmann::json{{"all_runways", true}};
	if (runway.isMessageRepetition())
		return nlohmann::json{{"msg_repetition", true}};
	nlohmann::json j;
	j["number"] = runway.number();
	if (runway.designator() != metaf::Runway::Designator::NONE)
		j["designator"] = util::toLower(magic_enum::enum_name(runway.designator()));
    if (!runway.isValid())
        j["not_valid"] = true;
	return j;
}

nlohmann::json ValueFormatBasic::format(const metaf::Temperature &temperature,
										bool addNotReported) const
{
	if (!temperature.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}

	const auto unitStr = util::toLower(magic_enum::enum_name(temperature.unit()));
	if (temperature.isPrecise())
		return nlohmann::json{{unitStr, util::formatDecimals(*temperature.temperature(), 1)}};
	nlohmann::json j;
	j[unitStr] = std::trunc(*temperature.temperature());

	if (const auto tc = temperature.toUnit(metaf::Temperature::Unit::C);
		tc.has_value() && !tc.value() && temperature.isFreezing())
	{
		j["freezing"] = true;
	}
	return j;
}

nlohmann::json ValueFormatBasic::format(const metaf::Speed &speed,
										bool addNotReported) const
{
	if (!speed.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}
	const std::string unitStr = [u = speed.unit()]() -> std::string {
		switch (u)
		{
		case metaf::Speed::Unit::KNOTS:
			return ("kt");
		case metaf::Speed::Unit::METERS_PER_SECOND:
			return ("mps");
		case metaf::Speed::Unit::KILOMETERS_PER_HOUR:
			return ("kmh");
		case metaf::Speed::Unit::MILES_PER_HOUR:
			return ("mph");
		}
	}();
	return nlohmann::json{{unitStr, std::trunc(*speed.speed())}};
}

nlohmann::json ValueFormatBasic::format(const metaf::Distance &distance,
										bool heightOrRvr,
										bool addNotReported) const
{
	if (!distance.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}
	nlohmann::json j;
	if (distance.modifier() != metaf::Distance::Modifier::NONE)
		j["modifier"] = util::toLower(magic_enum::enum_name(distance.modifier()));

	auto unit = distance.unit();
	if (unit == metaf::Distance::Unit::STATUTE_MILES && heightOrRvr)
		unit = metaf::Distance::Unit::FEET;

    if (!distance.isValid())
        j["not_valid"] = true;
	if (!distance.toUnit(unit).has_value())
		return j;

	const auto value = *distance.toUnit(unit);
	switch (unit)
	{
	case metaf::Distance::Unit::METERS:
		static const auto metersPerKm = 1000.0;
		if (heightOrRvr)
			j["m"] = static_cast<int>(std::round(value));
		else
			j["km"] = util::formatDecimals(value / metersPerKm, 3);
		break;
	case metaf::Distance::Unit::STATUTE_MILES:
		j["sm"] = util::formatDecimals(value, 3);

		if (const auto miles = distance.miles(); miles.has_value())
		{
			const auto f = std::get<metaf::Distance::MilesFraction>(*miles);
			const auto i = std::get<unsigned int>(*miles);
			auto fractionToString = [](metaf::Distance::MilesFraction fr) {
				using namespace std::string_literals;
				switch (fr)
				{
				case metaf::Distance::MilesFraction::NONE:
					return ""s;
				case metaf::Distance::MilesFraction::F_1_16:
					return "1/16"s;
				case metaf::Distance::MilesFraction::F_1_8:
					return "1/8"s;
				case metaf::Distance::MilesFraction::F_3_16:
					return "3/16"s;
				case metaf::Distance::MilesFraction::F_1_4:
					return "1/4"s;
				case metaf::Distance::MilesFraction::F_5_16:
					return "5/16"s;
				case metaf::Distance::MilesFraction::F_3_8:
					return "3/8"s;
				case metaf::Distance::MilesFraction::F_1_2:
					return "1/2"s;
				case metaf::Distance::MilesFraction::F_5_8:
					return "5/8"s;
				case metaf::Distance::MilesFraction::F_3_4:
					return "3/4"s;
				case metaf::Distance::MilesFraction::F_7_8:
					return "7/8"s;
				}
			};
			std::string s;
			if (i)
				s += std::to_string(i);
			if (const auto fracStr = fractionToString(f); !fracStr.empty())
			{
				if (!s.empty())
					s += ' ';
				s += fracStr;
			}
			j["sm_fraction"] = s;
		}
		break;
	case metaf::Distance::Unit::FEET:
		j["ft"] = static_cast<int>(std::round(value));
		break;
	}
	return j;
}

nlohmann::json ValueFormatBasic::format(const metaf::Direction &direction,
										bool addNotReported) const
{
	nlohmann::json j;
	switch (direction.type())
	{
	case metaf::Direction::Type::NOT_REPORTED:
		if (addNotReported)
			j["not_reported"] = true;
		break;
	case metaf::Direction::Type::VARIABLE:
		j["variable"] = true;
		break;
	case metaf::Direction::Type::NDV:
		j["ndv"] = true;
		break;
	case metaf::Direction::Type::VALUE_DEGREES:
		j["degrees"] = *direction.degrees();
		break;
	case metaf::Direction::Type::VALUE_CARDINAL:
		j["cardinal"] = util::toLower(magic_enum::enum_name(direction.cardinal()));
		break;
	case metaf::Direction::Type::OVERHEAD:
		j["overhead"] = true;
		break;
	case metaf::Direction::Type::ALQDS:
		j["alqds"] = true;
		break;
	case metaf::Direction::Type::UNKNOWN:
		j["unknown"] = true;
		break;
	}
    if (!direction.isValid())
        j["not_valid"] = true;
	return j;
}

nlohmann::json ValueFormatBasic::format(const metaf::Direction &sectorBegin,
										const metaf::Direction &sectorEnd) const
{
	nlohmann::json j;
	if (const auto d = sectorBegin.degrees(); d.has_value())
		j["begin_degrees"] = *d;
	if (const auto d = sectorEnd.degrees(); d.has_value())
		j["end_degrees"] = *d;
	return j;
}

nlohmann::json ValueFormatBasic::format(const std::vector<metaf::Direction> &directions) const
{
	nlohmann::json j = nlohmann::json::array();
	for (const auto &d : directions)
	{
		j.push_back(format(d));
	}
	return j;
}

nlohmann::json ValueFormatBasic::format(const metaf::Pressure &pressure,
										bool addNotReported) const
{
	if (!pressure.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}
	const std::string unitStr = [u = pressure.unit()]() -> std::string {
		switch (u)
		{
		case metaf::Pressure::Unit::HECTOPASCAL:
			return ("hpa");
		case metaf::Pressure::Unit::INCHES_HG:
			return ("inhg");
		case metaf::Pressure::Unit::MM_HG:
			return ("mmhg");
		}
	}();
	return nlohmann::json{{unitStr, util::formatDecimals(*pressure.pressure(), 2)}};
}

nlohmann::json ValueFormatBasic::format(const metaf::Precipitation &precipitation,
										bool addNotReported) const
{
	if (!precipitation.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}
	const std::string unitStr = [u = precipitation.unit()]() -> std::string {
		switch (u)
		{
		case metaf::Precipitation::Unit::MM:
			return ("mm");
		case metaf::Precipitation::Unit::INCHES:
			return ("in");
		}
	}();
	return nlohmann::json{{unitStr, util::formatDecimals(*precipitation.amount(), 2)}};
}

nlohmann::json ValueFormatBasic::format(const metaf::SurfaceFriction &surfaceFriction,
										bool addNotReported) const
{
	nlohmann::json j;
	switch (surfaceFriction.type())
	{
	case metaf::SurfaceFriction::Type::NOT_REPORTED:
		if (addNotReported) return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	case metaf::SurfaceFriction::Type::SURFACE_FRICTION_REPORTED:
		return nlohmann::json{{"friction_coefficient",
							   util::formatDecimals(*surfaceFriction.coefficient(), 2)}};
	case metaf::SurfaceFriction::Type::BRAKING_ACTION_REPORTED:
		return nlohmann::json{{"braking_action", util::toLower(magic_enum::enum_name(surfaceFriction.brakingAction()))}};
	case metaf::SurfaceFriction::Type::UNRELIABLE:
		return nlohmann::json{{"unreliable", true}};
	}
}

nlohmann::json ValueFormatBasic::format(const metaf::WaveHeight &waveHeight,
										bool addNotReported) const
{
	if (!waveHeight.isReported())
	{
		if (addNotReported)
			return nlohmann::json{{"not_reported", true}};
		return nlohmann::json{};
	}
	const std::string unitStr = [u = waveHeight.unit()]() -> std::string {
		switch (u)
		{
		case metaf::WaveHeight::Unit::METERS:
			return ("m");
		case metaf::WaveHeight::Unit::FEET:
			return ("ft");
		}
	}();
	switch (waveHeight.type())
	{
	case metaf::WaveHeight::Type::STATE_OF_SURFACE:
		return nlohmann::json{{"surface_state", util::toLower(magic_enum::enum_name(waveHeight.stateOfSurface()))}};
	case metaf::WaveHeight::Type::WAVE_HEIGHT:
		return nlohmann::json{{unitStr, *waveHeight.waveHeight()}};
	}
}
