/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef OUTPUTFORMATBASIC_HPP
#define OUTPUTFORMATBASIC_HPP

#include "outputformat.hpp"

class OutputFormatBasic : public OutputFormat
{
public:
    OutputFormatBasic(std::unique_ptr<DateTimeFormat> dtFormat,
                 std::unique_ptr<ValueFormat> valFormat,
                 bool rawStrings,
                 int refYear,
                 unsigned refMonth,
                 unsigned refDay)
        : OutputFormat(std::move(dtFormat),
        std::move (valFormat),
        rawStrings,
        refYear,
        refMonth,
        refDay)
    {
    }
    virtual ~OutputFormatBasic() {}

protected:
    virtual nlohmann::json toJson(const metaf::ParseResult &parseResult) const;

private:
    class MetafVisitorBasic;
};

#endif // #ifndef OUTPUTFORMATBASIC_HPP