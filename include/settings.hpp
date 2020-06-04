/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>

class Settings
{
public:
    // Which content and schema of output JSON is set by command line args
    enum class OutputFormat
    {
        BASIC,    // As close to original report as possible
        COLLATED, // Comlete data from report, semantically grouped and structured
        SIMPLE,   // Simplified format to display simple current weather and forecast
        HOURLY    // Simplified format with hourly forecast rather than trend-based
    };
    // Which format for date and time was set by command line args
    enum class DateTimeFormat
    {
        BASIC,    // Same as in the report, hour, minute, and optional day
        EXTENDED, // Year, month, day of month, hour and minute
        UNIX_TIME // Unix time (seconds since 00:00:00 1st January 1970 UTC)
    };
    // Which measurment unit format was set by command line args
    enum class UnitFormat
    {
        BASIC, // Only use same measurement units as in the report
        ALL    // Use all supported measurement units
    };
    // How program should proceed after command line args are processed
    enum class Status
    {
        CONTINUE,  // Program should continue processing data
        EXIT_OK,   // Program should exit with status OK
        EXIT_ERROR // Program should exit with status ERROR
    };

    Settings() = default;
    
    // Status how the program should proceed afterwards
    Status status() const { return stat; }
    // Content and schema for JSON output
    OutputFormat outputFormat() const { return outFormat; }
    // Format of date and time to use in JSON output
    DateTimeFormat dateTimeFormat() const { return dtFormat; }
    // Which measurement units to use in JSON output
    UnitFormat unitFormat() const { return uFormat; }
    // Reference date day-of-month
    unsigned refDateDay() const { return refDay; }
    // Reference date month
    unsigned refDateMonth() const { return refMonth; }
    // Reference dare year
    int refDateYear() const { return refYear; }
    // Wrap JSON to keep essential parameters in front of the JSON output 
    bool wrapJson() const { return(wrapOption); }
    // Include raw group and report strings in output JSON
    bool includeRawStrings() const { return(rawOption); }

protected:
    // Set program status
    void setStatus(Status s) { stat = s; }

    // Set JSON output format
    void setOutputFormat(OutputFormat f) { outFormat = f; }
    // Set measurement unit format
    void setUnitFormat(UnitFormat f) { uFormat = f; }
    // Set date/time format
    void setDateTimeFormat(DateTimeFormat f) { dtFormat = f; }

    // Set wrapping into additional layer of JSON
    void setWrapJson(bool w = true) { wrapOption = w; }
    // Set including of raw strings
    void setRawStrings(bool r = true) { rawOption = r; }

    // Set reference date year, month, and day
    void setRefDate(int year, unsigned month, unsigned day);
    // Set reference date to today's date
    void setRefDate();

private:
    Status stat = Status::EXIT_ERROR;

    OutputFormat outFormat = OutputFormat::BASIC;
    DateTimeFormat dtFormat = DateTimeFormat::BASIC;
    UnitFormat uFormat = UnitFormat::BASIC;
    
    unsigned refDay = 0;
    unsigned refMonth = 0;
    int refYear = 0;

    bool wrapOption = false;
    bool rawOption = false;

};

#endif //#ifndef SETTINGS_HPP