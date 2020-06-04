/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include <iostream>
#include "commandlineargs.hpp"
#include "utility.hpp"
#include "outputformat.hpp"

int main(int argc, char *argv[])
{
    const auto args = std::make_unique<CommandLineArgs> (argc, argv);
    switch(args->status()) {
        case CommandLineArgs::Status::CONTINUE:     break;
        case CommandLineArgs::Status::EXIT_OK:      return(EXIT_SUCCESS);
        case CommandLineArgs::Status::EXIT_ERROR:   return(EXIT_FAILURE);
    }

    const auto outputFormat = util::makeOutputFormat(*args);
    
    for (std::string report; getline(std::cin, report); ) {
        outputFormat->toJson(report, std::cout);
    }
    return 0;
}
