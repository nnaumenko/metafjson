/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#ifndef METAFVISITOR_HPP
#define METAFVISITOR_HPP

#include <stdexcept>
#include "metaf.hpp"
#include "datetimeformat.hpp"

class MetafVisitor : public metaf::Visitor<nlohmann::json>
{
public:
    MetafVisitor() = delete;
    MetafVisitor(const metaf::ParseResult &result,
                 const DateTimeFormat *dtFormat,
                 const ValueFormat *valFormat,
                 bool rawStrings,
                 int refYear,
                 unsigned refMonth,
                 unsigned refDay)
        : dateTimeFormat(dtFormat),
          valueFormat(valFormat),
          includeRawStrings(rawStrings),
          referenceYear(refYear),
          referenceMonth(refMonth),
          referenceDay(refDay)
    {
        if (!dtFormat)
            throw(std::runtime_error("dateTimeFormat is null when creating MetafVisitor"));
        if (!valFormat)
            throw(std::runtime_error("valueFormat is null when creating MetafVisitor"));
        
        reportDateTime.year = refYear;
        reportDateTime.month = refMonth;
        reportDateTime.day = refDay;
        if (const auto rt = result.reportMetadata.reportTime; rt.has_value()) {
            reportDateTime = DateTimeFormat::DateTime(*rt, refYear, refMonth, refDay);
        }
    }

protected:

    const DateTimeFormat *dateTimeFormat;
    const ValueFormat *valueFormat;
    bool includeRawStrings = false;

    int referenceYear = 0;
    int referenceMonth = 0;
    int referenceDay = 0;

    DateTimeFormat::DateTime reportDateTime;     
};

#endif //#ifndef METAFVISITOR_HPP
