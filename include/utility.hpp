/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string_view>
#include <string>
#include <memory>

class OutputFormat;
class ValueFormat;
class DateTimeFormat;
class Settings;

namespace util
{
// Truncate a value to specified amount of digits after decimal point
double formatDecimals(double value, size_t decimals);

// Convert a string to lowercase
std::string toLower(std::string_view s);

// Convert a string_view to lowercase
std::string toLower(std::string s);

// Create an OutputFormat object specified in settings
std::unique_ptr<OutputFormat> makeOutputFormat(const Settings & settings);

// Create an ValueFormat object specified in settings
std::unique_ptr<ValueFormat> makeValueFormat(const Settings & settings);

// Create a DateTimeFormat object specified in settings
std::unique_ptr<DateTimeFormat> makeDateTimeFormat(const Settings & settings);

} // namespace util

#endif // #ifndef UTILITY_HPP