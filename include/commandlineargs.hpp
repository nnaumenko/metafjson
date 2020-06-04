/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef COMMANDLINEARGS_HPP
#define COMMANDLINEARGS_HPP

#include "settings.hpp"

// Processes command line args, checks for unknown options, if needed  
// displays error messages (to stderr), help, and version (to stdout).
class CommandLineArgs : public Settings {
public:
    // Process command line args and set parameters according to ones specified 
    // in the command line
    CommandLineArgs(int argc, char *argv[]);
  
private:
    // Print version to stdout
    void displayVersion();
    // Print help test to stdout
    void displayHelp(std::string commandLineArgsHelp);
    // Print error message to stderr
    void displayError(const char * message);

    // Process the value of --output arg
    OutputFormat getOutputFormat(std::string format);
    // Process the value of --datetime arg
    DateTimeFormat getDateTimeFormat(std::string format);
    // Process the value of --unit arg
    UnitFormat getUnitFormat(std::string format);

    // Set reference date from command line args
    void setRefDate(std::string yyyymmdd);
};

#endif //#ifndef COMMANDLINEARGS_HPP