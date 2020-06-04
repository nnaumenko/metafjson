/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "outputformat.hpp"

#include <iostream>
#include <memory>

#include "nlohmann/json.hpp"
#include "metaf.hpp"

static_assert(metaf::Version::major >= 4, "Metaf version 4.0.0 or later is required");

#include "utility.hpp"

OutputFormat::Result OutputFormat::toJson(const std::string &report,
                                          std::ostream &out) const
{
    try
    {
        const auto parseResult = metaf::Parser::parse(report);
        const auto j = toJson(parseResult);
        out << j << "\n"; //not std::endl because it does std::flush as well
        return Result::OK;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Exception " << e.what();
        std::cerr << " occurred when parsing or serialising the following report:" << std::endl;
        std::cerr << report << std::endl;
        return Result::EXCEPTION;
    }
}