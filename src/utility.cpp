/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "utility.hpp"

#include <algorithm>

#include "settings.hpp"
#include "valueformat.hpp"
#include "datetimeformat.hpp"
#include "outputformat.hpp"
#include "outputformatbasic.hpp"

namespace util
{

double formatDecimals(double value, size_t decimals)
{
	double pow10 = 1.0;
	switch (decimals)
	{
	case 0:
		break;
	case 1:
		pow10 = 10;
		break;
	case 2:
		pow10 = 100;
		break;
	case 3:
		pow10 = 1000;
		break;
	default:
		for (auto i = 0u; i < decimals; i++)
			pow10 *= 10.0;
		break;
	}
	if (value < 0)
		return static_cast<int>(value * pow10 - 0.5) / pow10;
	return static_cast<int>(value * pow10 + 0.5) / pow10;
}

std::string toLower(std::string_view s)
{
	return toLower(std::string(s));
}

std::string toLower(std::string s)
{
	std::transform(s.begin(),
				   s.end(),
				   s.begin(),
				   [](unsigned char c) -> unsigned char { return std::tolower(c); });
	return (s);
}

std::unique_ptr<OutputFormat> makeOutputFormat(const Settings & settings)
{
	switch (settings.outputFormat())
	{
	case Settings::OutputFormat::BASIC:
		return std::make_unique<OutputFormatBasic>(
			makeDateTimeFormat(settings),
			makeValueFormat(settings),
			settings.includeRawStrings(),
			settings.refDateYear(),
			settings.refDateMonth(),
			settings.refDateDay());
	default:
		throw std::runtime_error("Output format not implemented in this version");
	}
}

std::unique_ptr<ValueFormat> makeValueFormat(const Settings & settings)
{
	switch (settings.unitFormat())
	{
	case Settings::UnitFormat::BASIC:
		return std::make_unique<ValueFormatBasic>();
	default:
		throw std::runtime_error("Value format not implemented in this version");
	}
}

std::unique_ptr<DateTimeFormat> makeDateTimeFormat(const Settings & settings)
{
	switch(settings.dateTimeFormat())
	{
	case Settings::DateTimeFormat::BASIC:
		return std::make_unique<DateTimeFormatBasic>();
	default:
		throw std::runtime_error("Date / time format not implemented in this version");
	}
}

} // namespace util