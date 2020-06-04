/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "commandlineargs.hpp"

#include <iostream>
#include <stdexcept>
#include <regex>

#include "cxxopts.hpp"
#include "date/date.h"

#include "version.hpp"

CommandLineArgs::CommandLineArgs(int argc, char *argv[])
{
    try
    {
        cxxopts::Options options("metaf", "Batch-parse METAR weather reports and TAF weather forecasts");
        options.add_options()
            ("v, version", "Display version")
            ("h, help", "Display help")
        ;
        options.add_options()
            ("o, output", "Specifies the data output format",
             cxxopts::value<std::string>()->default_value("basic"), 
             "format"
            )
            ("u, units", "Specifies the measurment units used in data output",
             cxxopts::value<std::string>()->default_value("basic"), 
             "format"
            )
            ("d, datetime", "Specifies the date and time format used in data output",
             cxxopts::value<std::string>()->default_value("basic"), 
             "format"
            )
            ("f, refdate", "Specifies the reference date "
            "(i.e. date when this recent report was received) in YYYYMMDD format. "
            "Since month and year are not included in date and time formats used in METAR or TAF, "
            "month and year must be inferred from the reference date. Note: this option is "
            "ignored if basic time format is used.",
             cxxopts::value<std::string>(), 
             "YYYYMMDD"
            )
            ("w, wrap", 
             "Wrap JSON output into additional layer of JSON to keep certain data, such as "
             "station ICAO code at the beginning of the JSON output and allow easier "
             "sorting and filtering of report batches.")
            ("r, raw", 
             "Add raw report strings to the output.")
            ;
        auto result = options.parse(argc, argv);

        if (result.count("version"))
        {
            displayVersion();
            setStatus(Status::EXIT_OK);
            return;
        }
        if (result.count("help"))
        {
            displayHelp(options.help({""}));
            setStatus(Status::EXIT_OK);
            return;
        }

        if (result.count("output") > 1)
            throw(std::runtime_error("Duplicate parameter --output or -o"));
        if (result.count("output"))
            setOutputFormat(getOutputFormat(result["output"].as<std::string>()));

        if (result.count("units") > 1)
            throw(std::runtime_error("Duplicate parameter --units or -u"));
        if (result.count("units"))
            setUnitFormat(getUnitFormat(result["units"].as<std::string>()));

        if (result.count("datetime") > 1)
            throw(std::runtime_error("Duplicate parameter --datetime or -d"));
        if (result.count("datetime"))
            setDateTimeFormat(getDateTimeFormat(result["datetime"].as<std::string>()));

        if (result.count("refdate") > 1)
            throw(std::runtime_error("Duplicate parameter --refdate or -f"));
        if (result.count("refdate"))
            setRefDate(result["refdate"].as<std::string>());
        else
            Settings::setRefDate();

        if (result.count("wrap")) setWrapJson();
        if (result.count("raw")) setRawStrings();

        setStatus(Status::CONTINUE);
    }
    catch (const std::exception &e)
    {
        displayError(e.what());
    }
    catch (...) {
        displayError("undefined error");
    }
}

void CommandLineArgs::displayError(const char * message)
{
    std::cerr << "Command line argument error: " << message << std::endl;
}

void CommandLineArgs::displayVersion()
{
    std::cout << "Version: ";
    std::cout << Version::major << '.' << Version::minor << '.' << Version::patch;
    std::cout << std::endl << std::endl;
}

void CommandLineArgs::displayHelp(std::string commandLineArgsHelp)
{
    std::cout << std::move(commandLineArgsHelp) << std::endl;
    std::cout << "The METAR or TAF data are received from standard input stream (one report per" << std::endl;
    std::cout << "line), for example: " << std::endl;
    std::cout << "cat metar.txt | metafjson" << std::endl;
    std::cout << "In this example file metar.txt is expected to contain one METAR or TAF per line." << std::endl;
    std::cout << std::endl;

    std::cout << "The data output formats (specified with --format option):" << std::endl;
    std::cout << " b or basic: output all METAR/TAF groups without changes in the same order." << std::endl;
//    std::cout << " c or collated: output semantically structured collated data." << std::endl;
//    std::cout << " s or simple: output only essential data needed for human-readable report and" << std::endl;
//    std::cout << "              forecast." << std::endl;
//    std::cout << " h or hourly: similar to 'simple' except hourly forecast is produces instead of" << std::endl;
//    std::cout << "              trends." << std::endl;
    std::cout << std::endl;

    std::cout << "The date and time output formats (specified with --datetime option):" << std::endl;
    std::cout << " b or basic: include only hour, minute, and optional day (if reported) only." << std::endl;
//    std::cout << " x or extended: include year, month, day, hour and minute." << std::endl;
//    std::cout << " u or unix: include Unix Time (seconds since 1st Jan 1970) only." << std::endl;
//    std::cout << std::endl;
//    std::cout << "Note: year and month are not specified on METAR and TAF reports and must be" << std::endl;
//    std::cout << "inferred from the reference date. For recent reports the current date can be" << std::endl;
//    std::cout << "used (no need to specify an additional option). For historical reports the date" << std::endl;
//    std::cout << "or report retreival or any date not later than 1 month from report release " << std::endl;
//    std::cout << "date must be specified with --refdate option." << std::endl;
    std::cout << std::endl;

    std::cout << "The measurement units (specified with --units option):" << std::endl;
    std::cout << " b or basic: include only values in measurement units used in report." << std::endl;
    std::cout << " a or all: include values in all supported measurement units." << std::endl;
    std::cout << std::endl;

    std::cout << "Please refer to https://gitlab.com/nnaumenko/metafjson/ for documentation, " << std::endl;
    std::cout << "more examples, and JSON output specification." << std::endl;
}

CommandLineArgs::OutputFormat CommandLineArgs::getOutputFormat(std::string format)
{
    if (format == "basic" || format == "b") return OutputFormat::BASIC; 
    if (format == "collated" || format == "c") return OutputFormat::COLLATED;
    if (format == "simple" || format == "s") return OutputFormat::SIMPLE; 
    if (format == "hourly" || format == "h") return OutputFormat::HOURLY; 
    throw (std::runtime_error("Output data format " + format + " is not recognised"));
}

CommandLineArgs::DateTimeFormat CommandLineArgs::getDateTimeFormat(std::string format)
{
    if (format == "basic" || format == "b") return DateTimeFormat::BASIC; 
    if (format == "extended" || format == "x") return DateTimeFormat::EXTENDED;
    if (format == "unix" || format == "u") return DateTimeFormat::UNIX_TIME; 
    throw (std::runtime_error("Date and time format " + format + " is not recognised"));
}

CommandLineArgs::UnitFormat CommandLineArgs::getUnitFormat(std::string format)
{
    if (format == "basic" || format == "b") return UnitFormat::BASIC;
    if (format == "all" || format == "a") return UnitFormat::ALL;
    throw (std::runtime_error("Unit format " + format + " is not recognised"));
}

void CommandLineArgs::setRefDate(std::string yyyymmdd)
{
    static const std::regex dateTimeRegex("(\\d\\d\\d\\d)(\\d\\d)(\\d\\d)");
    static const size_t matchYear = 1, matchMonth = 2, matchDay = 3;
    std::smatch match;
    if (!std::regex_match(yyyymmdd, match, dateTimeRegex)) 
        throw (std::runtime_error("Date format " +yyyymmdd + " is not recognised"));
    Settings::setRefDate(
        std::stoi(match.str(matchYear)),
        std::stoi(match.str(matchMonth)),
        std::stoi(match.str(matchDay))
    );
    //TODO: check validity of day and month (4-digit year is always valid)
}
