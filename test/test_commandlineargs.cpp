/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "gtest/gtest.h"

#include <sstream>
#include <iostream>

#include "commandlineargs.hpp"
#include "version.hpp"
#include "date/date.h"

// Version and help

TEST(CommandLineArgs, version) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--version";
    char * argv[] = {arg0, arg1};
    const std::string expectedoutput = 
        "Version: " + 
        std::to_string(Version::major) + 
        '.' + 
        std::to_string(Version::minor) + 
        '.' + 
        std::to_string(Version::patch) + 
        "\n\n";

    testing::internal::CaptureStdout();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(testing::internal::GetCapturedStdout(), expectedoutput);

    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_OK);
}

TEST(CommandLineArgs, help) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--help";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStdout();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStdout().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_OK);
}

// Default parameters

TEST(CommandLineArgs, defaultParameters) {
    const int argn = 1;
    char arg0[] = "metafjson";
    char * argv[] = {arg0};

    const auto cla = CommandLineArgs(argn, argv);

    const auto now = std::chrono::system_clock::now();

    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);

    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::BASIC);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::BASIC);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::BASIC);

    using namespace date;
    EXPECT_EQ(cla.refDateDay(), (unsigned)year_month_day{floor<days>(now)}.day());
    EXPECT_EQ(cla.refDateMonth(), (unsigned)year_month_day{floor<days>(now)}.month());
    EXPECT_EQ(cla.refDateYear(), (int)year_month_day{floor<days>(now)}.year());

    EXPECT_FALSE(cla.wrapJson());
    EXPECT_FALSE(cla.includeRawStrings());
}

// output formats

TEST(CommandLineArgs, outputFormatBasic) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--output=basic";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::BASIC);
}

TEST(CommandLineArgs, outputFormatBasicShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-o";
    char arg2[] = "b";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::BASIC);
}

TEST(CommandLineArgs, outputFormatCollated) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--output=collated";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::COLLATED);
}

TEST(CommandLineArgs, outputFormatCollatedShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-o";
    char arg2[] = "c";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::COLLATED);
}

TEST(CommandLineArgs, outputFormatSimple) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--output=simple";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::SIMPLE);
}

TEST(CommandLineArgs, outputFormatSimpleShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-o";
    char arg2[] = "s";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::SIMPLE);
}

TEST(CommandLineArgs, outputFormatHourly) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--output=hourly";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::HOURLY);
}

TEST(CommandLineArgs, outputFormatHourlyShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-o";
    char arg2[] = "h";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::HOURLY);
}

TEST(CommandLineArgs, outputFormatUnrecognised) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--output=other";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::BASIC);
}

TEST(CommandLineArgs, outputFormatDuplicate) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "--output=simple";
    char arg2[] = "--output=simple";
    char * argv[] = {arg0, arg1, arg2};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.outputFormat(), CommandLineArgs::OutputFormat::BASIC);
}

// DateTime formats

TEST(CommandLineArgs, dateTimeFormatBasic) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--datetime=basic";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::BASIC);
}

TEST(CommandLineArgs, dateTimeFormatBasicShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-d";
    char arg2[] = "b";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::BASIC);
}

TEST(CommandLineArgs, dateTimeFormatExtended) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--datetime=extended";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::EXTENDED);
}

TEST(CommandLineArgs, dateTimeFormatExtendedShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-d";
    char arg2[] = "x";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::EXTENDED);
}

TEST(CommandLineArgs, dateTimeFormatTimestamp) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--datetime=unix";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::UNIX_TIME);
}

TEST(CommandLineArgs, dateTimeFormatTimestampShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-d";
    char arg2[] = "u";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::UNIX_TIME);
}

TEST(CommandLineArgs, dateTimeFormatUnrecognised) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--datetime=other";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::BASIC);
}

TEST(CommandLineArgs, dateTimeFormatDuplicate) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "--datetime=timestamp";
    char arg2[] = "--datetime=timestamp";
    char * argv[] = {arg0, arg1, arg2};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.dateTimeFormat(), CommandLineArgs::DateTimeFormat::BASIC);
}

// Measurement unit formats

TEST(CommandLineArgs, unitFormatBasic) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--units=basic";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::BASIC);
}

TEST(CommandLineArgs, unitFormatBasicShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-u";
    char arg2[] = "b";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::BASIC);
}

TEST(CommandLineArgs, unitFormatAll) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--units=all";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::ALL);
}

TEST(CommandLineArgs, unitFormatAllShort) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "-u";
    char arg2[] = "a";
    char * argv[] = {arg0, arg1, arg2};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::ALL);
}

TEST(CommandLineArgs, unitFormatUnrecognised) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--units=other";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::BASIC);
}

TEST(CommandLineArgs, unitFormatDuplicate) {
    const int argn = 3;
    char arg0[] = "metafjson";
    char arg1[] = "--units=all";
    char arg2[] = "--units=all";
    char * argv[] = {arg0, arg1, arg2};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
    EXPECT_EQ(cla.unitFormat(), CommandLineArgs::UnitFormat::BASIC);
}

// Reference date

TEST(CommandLineArgs, refdateValid) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--refdate=20190925";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);
    EXPECT_EQ(cla.refDateDay(), 25u);
    EXPECT_EQ(cla.refDateMonth(), 9u);
    EXPECT_EQ(cla.refDateYear(), 2019);
}

TEST(CommandLineArgs, refdateInvalid) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--refdate=201900925";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
}

// Flags

TEST(CommandLineArgs, flagsWrapJson) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--wrap";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);

    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);

    EXPECT_TRUE(cla.wrapJson());
}

TEST(CommandLineArgs, flagsRaw) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--raw";
    char * argv[] = {arg0, arg1};

    const auto cla = CommandLineArgs(argn, argv);

    EXPECT_EQ(cla.status(), CommandLineArgs::Status::CONTINUE);

    EXPECT_TRUE(cla.includeRawStrings());
}

// Unrecognised options

TEST(CommandLineArgs, unrecognisedFlag) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--other";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
}

TEST(CommandLineArgs, unrecognisedParameter) {
    const int argn = 2;
    char arg0[] = "metafjson";
    char arg1[] = "--other=basic";
    char * argv[] = {arg0, arg1};

    testing::internal::CaptureStderr();
    const auto cla = CommandLineArgs(argn, argv);
    EXPECT_FALSE(testing::internal::GetCapturedStderr().empty());
    EXPECT_EQ(cla.status(), CommandLineArgs::Status::EXIT_ERROR);
}
