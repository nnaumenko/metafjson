/*
* Copyright (C) 2020 Nick Naumenko (https://gitlab.com/nnaumenko,
* https:://github.com/nnaumenko)
* All rights reserved.
* This software may be modified and distributed under the terms
* of the MIT license. See the LICENSE file for details.
*/

#include "outputformatbasic.hpp"

#include "nlohmann/json.hpp"
#include "magic_enum.hpp"
#include "metaf.hpp"

#include "utility.hpp"
#include "metafvisitor.hpp"

using namespace metaf;

class OutputFormatBasic::MetafVisitorBasic : public MetafVisitor
{
public:
    MetafVisitorBasic(const metaf::ParseResult &result,
                      const DateTimeFormat *dtFormat,
                      const ValueFormat *valFormat,
                      bool rawStrings,
                      int refYear,
                      unsigned refMonth,
                      unsigned refDay)
        : MetafVisitor(result, dtFormat, valFormat, rawStrings, refYear, refMonth, refDay) {}

protected:
    nlohmann::json visitKeywordGroup(const KeywordGroup &group,
                                     ReportPart reportPart,
                                     const std::string &rawString);
    nlohmann::json visitLocationGroup(const LocationGroup &group,
                                      ReportPart reportPart,
                                      const std::string &rawString);
    nlohmann::json visitReportTimeGroup(const ReportTimeGroup &group,
                                        ReportPart reportPart,
                                        const std::string &rawString);
    nlohmann::json visitTrendGroup(const TrendGroup &group,
                                   ReportPart reportPart,
                                   const std::string &rawString);
    nlohmann::json visitWindGroup(const WindGroup &group,
                                  ReportPart reportPart,
                                  const std::string &rawString);
    nlohmann::json visitVisibilityGroup(const VisibilityGroup &group,
                                        ReportPart reportPart,
                                        const std::string &rawString);
    nlohmann::json visitCloudGroup(const CloudGroup &group,
                                   ReportPart reportPart,
                                   const std::string &rawString);
    nlohmann::json visitWeatherGroup(const WeatherGroup &group,
                                     ReportPart reportPart,
                                     const std::string &rawString);
    nlohmann::json visitTemperatureGroup(const TemperatureGroup &group,
                                         ReportPart reportPart,
                                         const std::string &rawString);
    nlohmann::json visitPressureGroup(const PressureGroup &group,
                                      ReportPart reportPart,
                                      const std::string &rawString);
    nlohmann::json visitRunwayStateGroup(const RunwayStateGroup &group,
                                         ReportPart reportPart,
                                         const std::string &rawString);
    nlohmann::json visitSeaSurfaceGroup(const SeaSurfaceGroup &group,
                                        ReportPart reportPart,
                                        const std::string &rawString);
    nlohmann::json visitMinMaxTemperatureGroup(const MinMaxTemperatureGroup &group,
                                               ReportPart reportPart,
                                               const std::string &rawString);
    nlohmann::json visitPrecipitationGroup(const PrecipitationGroup &group,
                                           ReportPart reportPart,
                                           const std::string &rawString);
    nlohmann::json visitLayerForecastGroup(const LayerForecastGroup &group,
                                           ReportPart reportPart,
                                           const std::string &rawString);
    nlohmann::json visitPressureTendencyGroup(const PressureTendencyGroup &group,
                                              ReportPart reportPart,
                                              const std::string &rawString);
    nlohmann::json visitCloudTypesGroup(const CloudTypesGroup &group,
                                        ReportPart reportPart,
                                        const std::string &rawString);
    nlohmann::json visitLowMidHighCloudGroup(const LowMidHighCloudGroup &group,
                                             ReportPart reportPart,
                                             const std::string &rawString);
    nlohmann::json visitLightningGroup(const LightningGroup &group,
                                       ReportPart reportPart,
                                       const std::string &rawString);
    nlohmann::json visitVicinityGroup(const VicinityGroup &group,
                                      ReportPart reportPart,
                                      const std::string &rawString);
    nlohmann::json visitMiscGroup(const MiscGroup &group,
                                  ReportPart reportPart,
                                  const std::string &rawString);
    nlohmann::json visitUnknownGroup(const UnknownGroup &group,
                                     ReportPart reportPart,
                                     const std::string &rawString);
};

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitKeywordGroup(
    const KeywordGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "keyword"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitLocationGroup(
    const LocationGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "icao_location"},
        {"location", group.toString()}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitReportTimeGroup(
    const ReportTimeGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "report_time"},
        {"report_time", dateTimeFormat->format(reportDateTime)}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitTrendGroup(
    const TrendGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "trend"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.probability())
    {
    case metaf::TrendGroup::Probability::NONE:
        break;
    case metaf::TrendGroup::Probability::PROB_30:
        j["probability_percent"] = 30;
        break;
    case metaf::TrendGroup::Probability::PROB_40:
        j["probability_percent"] = 40;
        break;
    }
    if (const auto t = group.timeFrom(); t.has_value())
        j["time_from"] = dateTimeFormat->format(*t, reportDateTime);
    if (const auto t = group.timeUntil(); t.has_value())
        j["time_until"] = dateTimeFormat->format(*t, reportDateTime);
    if (const auto t = group.timeAt(); t.has_value())
        j["time_at"] = dateTimeFormat->format(*t, reportDateTime);
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitWindGroup(
    const WindGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "wind"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::WindGroup::Type::SURFACE_WIND:
        j["direction"] = valueFormat->format(group.direction(), true);
        j["wind_speed"] = valueFormat->format(group.windSpeed(), true);
        j["gust_speed"] = valueFormat->format(group.gustSpeed());
        break;
    case metaf::WindGroup::Type::SURFACE_WIND_CALM:
        break;
    case metaf::WindGroup::Type::VARIABLE_WIND_SECTOR:
        j["variable_direction_sector"] =
            valueFormat->format(group.varSectorBegin(), group.varSectorEnd());
        break;
    case metaf::WindGroup::Type::SURFACE_WIND_WITH_VARIABLE_SECTOR:
        j["direction"] = valueFormat->format(group.direction(), true);
        j["wind_speed"] = valueFormat->format(group.windSpeed(), true);
        j["gust_speed"] = valueFormat->format(group.gustSpeed());
        j["variable_direction_sector"] =
            valueFormat->format(group.varSectorBegin(), group.varSectorEnd());
        break;
    case metaf::WindGroup::Type::WIND_SHEAR:
        j["direction"] = valueFormat->format(group.direction(), true);
        j["wind_speed"] = valueFormat->format(group.windSpeed(), true);
        j["gust_speed"] = valueFormat->format(group.gustSpeed());
        j["height"] = valueFormat->format(group.height(), true);
        break;
    case metaf::WindGroup::Type::WIND_SHEAR_IN_LOWER_LAYERS:
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::WindGroup::Type::WIND_SHIFT:
    case metaf::WindGroup::Type::WIND_SHIFT_FROPA:
        if (const auto t = group.eventTime(); t.has_value())
            j["begin_time"] = dateTimeFormat->format(*t, reportDateTime);
        break;
    case metaf::WindGroup::Type::PEAK_WIND:
        j["direction"] = valueFormat->format(group.direction(), true);
        j["wind_speed"] = valueFormat->format(group.windSpeed(), true);
        if (const auto t = group.eventTime(); t.has_value())
            j["occurrence_time"] = dateTimeFormat->format(*t, reportDateTime);
        break;
    case metaf::WindGroup::Type::WSCONDS:
    case metaf::WindGroup::Type::WND_MISG:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitVisibilityGroup(
    const VisibilityGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "visibility"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::VisibilityGroup::Type::PREVAILING:
    case metaf::VisibilityGroup::Type::SURFACE:
    case metaf::VisibilityGroup::Type::TOWER:
        j["visibility"] = valueFormat->format(group.visibility(), false, true);
        break;
    case metaf::VisibilityGroup::Type::PREVAILING_NDV:
    case metaf::VisibilityGroup::Type::DIRECTIONAL:
        j["visibility"] = valueFormat->format(group.visibility(), false, true);
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        break;
    case metaf::VisibilityGroup::Type::RUNWAY:
        j["visibility"] = valueFormat->format(group.visibility(), false, true);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::VisibilityGroup::Type::RVR:
        j["rvr"] = valueFormat->format(group.visibility(), true, true);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        j["trend"] = util::toLower(magic_enum::enum_name(group.trend()));
        break;
    case metaf::VisibilityGroup::Type::SECTOR:
        j["visibility"] = valueFormat->format(group.visibility(), false, true);
        j["sector_directions"] = valueFormat->format(group.sectorDirections());
        break;
    case metaf::VisibilityGroup::Type::VARIABLE_PREVAILING:
        j["min_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        j["max_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        break;
    case metaf::VisibilityGroup::Type::VARIABLE_DIRECTIONAL:
        j["min_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        j["max_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        break;
    case metaf::VisibilityGroup::Type::VARIABLE_RUNWAY:
        j["min_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        j["max_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::VisibilityGroup::Type::VARIABLE_RVR:
        j["min_rvr"] = valueFormat->format(group.minVisibility(), true, true);
        j["max_rvr"] = valueFormat->format(group.minVisibility(), true, true);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        j["trend"] = util::toLower(magic_enum::enum_name(group.trend()));
        break;
    case metaf::VisibilityGroup::Type::VARIABLE_SECTOR:
        j["min_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        j["max_visibility"] = valueFormat->format(group.minVisibility(), false, true);
        j["sector_directions"] = valueFormat->format(group.sectorDirections());
        break;
    case metaf::VisibilityGroup::Type::VIS_MISG:
    case metaf::VisibilityGroup::Type::RVR_MISG:
    case metaf::VisibilityGroup::Type::RVRNO:
        break;
    case metaf::VisibilityGroup::Type::VISNO:
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitCloudGroup(
    const CloudGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "cloud"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::CloudGroup::Type::NO_CLOUDS:
        switch (group.amount())
        {
        case metaf::CloudGroup::Amount::NONE_CLR:
            j["clr"] = true;
            break;
        case metaf::CloudGroup::Amount::NONE_SKC:
            j["skc"] = true;
            break;
        case metaf::CloudGroup::Amount::NCD:
            j["ncd"] = true;
            break;
        case metaf::CloudGroup::Amount::NSC:
            j["nsc"] = true;
            break;
        default:
            j["amount"] = util::toLower(magic_enum::enum_name(group.amount()));
            break;
        }
        break;
    case metaf::CloudGroup::Type::CLOUD_LAYER:
        j["amount"] = util::toLower(magic_enum::enum_name(group.amount()));
        j["height"] = valueFormat->format(group.height(), true, true);
        if (const auto ct = group.convectiveType();
            ct != metaf::CloudGroup::ConvectiveType::NONE)
        {
            j["convective_type"] =
                util::toLower(magic_enum::enum_name(group.convectiveType()));
        }
        break;
    case metaf::CloudGroup::Type::VERTICAL_VISIBILITY:
        j["amount"] = util::toLower(magic_enum::enum_name(group.amount()));
        j["vertical_visibility"] = valueFormat->format(group.verticalVisibility(), true, true);
        break;
    case metaf::CloudGroup::Type::CEILING:
        j["height"] = valueFormat->format(group.height(), true, true);
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::CloudGroup::Type::VARIABLE_CEILING:
        j["min_height"] = valueFormat->format(group.minHeight(), true, true);
        j["max_height"] = valueFormat->format(group.maxHeight(), true, true);
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::CloudGroup::Type::CHINO:
        if (const auto d = group.direction(); d.has_value())
            j["direction"] = valueFormat->format(*d);
        if (const auto r = group.runway(); r.has_value())
            j["runway"] = valueFormat->format(*r);
        break;
    case metaf::CloudGroup::Type::CLD_MISG:
        break;
    case metaf::CloudGroup::Type::OBSCURATION:
        j["amount"] = util::toLower(magic_enum::enum_name(group.amount()));
        if (const auto ct = group.cloudType(); ct.has_value())
            j["obscuration"] = util::toLower(magic_enum::enum_name(ct->type()));;
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitWeatherGroup(
    const WeatherGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "weather"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::WeatherGroup::Type::CURRENT:
    case metaf::WeatherGroup::Type::RECENT:
    case metaf::WeatherGroup::Type::EVENT:
        j["weather_phenomena"] = nlohmann::json::array();
        for (const auto w : group.weatherPhenomena())
        {
            nlohmann::json jweather{
                {"qualifier", util::toLower(magic_enum::enum_name(w.qualifier()))},
                {"descriptor", util::toLower(magic_enum::enum_name(w.descriptor()))},
            };
            jweather["weather"] = nlohmann::json::array();
            for (const auto &ww : w.weather())
            {
                jweather["weather"].push_back(ww);
            }
            if (const auto e = w.event(); e != metaf::WeatherPhenomena::Event::NONE)
                jweather["event_type"] = util::toLower(magic_enum::enum_name(e));
            if (const auto t = w.time(); t.has_value())
                jweather["occurrence_time"] = dateTimeFormat->format(*t, reportDateTime);
            if (!w.isValid())
                jweather["not_valid"] = true;
            j["weather_phenomena"].push_back(std::move(jweather));
        }
        break;
    case metaf::WeatherGroup::Type::NSW:
    case metaf::WeatherGroup::Type::PWINO:
    case metaf::WeatherGroup::Type::WX_MISG:
    case metaf::WeatherGroup::Type::TSNO:
    case metaf::WeatherGroup::Type::TS_LTNG_TEMPO_UNAVBL:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitTemperatureGroup(
    const TemperatureGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "temperature"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::TemperatureGroup::Type::TEMPERATURE_AND_DEW_POINT:
        j["air_temperature"] = valueFormat->format(group.airTemperature(), true);
        j["dew_point"] = valueFormat->format(group.dewPoint(), true);
        break;
    case metaf::TemperatureGroup::Type::T_MISG:
    case metaf::TemperatureGroup::Type::TD_MISG:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitPressureGroup(
    const PressureGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "pressure"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::PressureGroup::Type::OBSERVED_QNH:
    case metaf::PressureGroup::Type::FORECAST_LOWEST_QNH:
        j["pressure_qnh"] = valueFormat->format(group.atmosphericPressure(), true);
        break;
    case metaf::PressureGroup::Type::OBSERVED_QFE:
        j["pressure_qfe"] = valueFormat->format(group.atmosphericPressure(), true);
        break;
    case metaf::PressureGroup::Type::SLPNO:
    case metaf::PressureGroup::Type::PRES_MISG:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitRunwayStateGroup(
    const RunwayStateGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "runway_state"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    j["runway"] = valueFormat->format(group.runway());
    switch (group.type())
    {
    case metaf::RunwayStateGroup::Type::RUNWAY_STATE:
        j["deposits"] = util::toLower(magic_enum::enum_name(group.deposits()));
        j["contamination_extent"] =
            util::toLower(magic_enum::enum_name(group.contaminationExtent()));
        j["deposit_depth"] = valueFormat->format(group.depositDepth());
        j["surface_friction"] = valueFormat->format(group.surfaceFriction());
        break;
    case metaf::RunwayStateGroup::Type::RUNWAY_NOT_OPERATIONAL:
        j["deposits"] = util::toLower(magic_enum::enum_name(group.deposits()));
        j["contamination_extent"] =
            util::toLower(magic_enum::enum_name(group.contaminationExtent()));
        j["surface_friction"] = valueFormat->format(group.surfaceFriction());
        break;
    case metaf::RunwayStateGroup::Type::RUNWAY_CLRD:
        j["surface_friction"] = valueFormat->format(group.surfaceFriction());
        break;
    case metaf::RunwayStateGroup::Type::AERODROME_SNOCLO:
    case metaf::RunwayStateGroup::Type::RUNWAY_SNOCLO:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitSeaSurfaceGroup(
    const SeaSurfaceGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "sea_surface"},
        {"temperature", valueFormat->format(group.surfaceTemperature(), true)},
        {"waves", valueFormat->format(group.surfaceTemperature(), true)}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitMinMaxTemperatureGroup(
    const MinMaxTemperatureGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "min_max_temperature"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::MinMaxTemperatureGroup::Type::FORECAST:
        if (const auto t = group.minimumTime(); t.has_value())
        {
            j["min_time"] = dateTimeFormat->format(*t, reportDateTime);
        }
        if (const auto t = group.maximumTime(); t.has_value())
        {
            j["max_time"] = dateTimeFormat->format(*t, reportDateTime);
        }
    case metaf::MinMaxTemperatureGroup::Type::OBSERVED_24_HOURLY:
    case metaf::MinMaxTemperatureGroup::Type::OBSERVED_6_HOURLY:
        j["min_temperature"] = valueFormat->format(group.minimum());
        j["max_temperature"] = valueFormat->format(group.minimum());
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitPrecipitationGroup(
    const PrecipitationGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "precipitation"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))},
        {"total", valueFormat->format(group.total(), true)}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (group.type() == metaf::PrecipitationGroup::Type::SNOW_INCREASING_RAPIDLY)
        j["last_hour_increase"] = valueFormat->format(group.total());
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitLayerForecastGroup(
    const LayerForecastGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "layer_forecast"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))},
        {"base_height", valueFormat->format(group.baseHeight(), true)},
        {"top_height", valueFormat->format(group.topHeight(), true)}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitPressureTendencyGroup(
    const PressureTendencyGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "pressure_tendency"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))},
        {"trend", util::toLower(magic_enum::enum_name(group.trend(group.type())))},
        {"difference", valueFormat->format(group.difference(), true)}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitCloudTypesGroup(
    const CloudTypesGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "cloud_types"}};
    if (!group.isValid())
        j["not_valid"] = true;
    j["cloud_types"] = nlohmann::json::array();
    for (const auto &ct : group.cloudTypes())
    {
        nlohmann::json jcloud{
            {"type", util::toLower(magic_enum::enum_name(ct.type()))},
            {"height", valueFormat->format(ct.height(), true)}};
        if (ct.okta())
            jcloud["okta"] = ct.okta();
        jcloud["cloud_types"].push_back(std::move(jcloud));
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitLowMidHighCloudGroup(
    const LowMidHighCloudGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "low_mid_high_clouds"},
        {"low_layer", util::toLower(magic_enum::enum_name(group.lowLayer()))},
        {"mid_layer", util::toLower(magic_enum::enum_name(group.midLayer()))},
        {"high_layer", util::toLower(magic_enum::enum_name(group.highLayer()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitLightningGroup(
    const LightningGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "lightning"},
        {"frequency", util::toLower(magic_enum::enum_name(group.frequency()))},
        {"distance", valueFormat->format(group.distance())}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (group.isCloudGround())
        j["cloud_to_ground"] = true;
    if (group.isInCloud())
        j["in_cloud"] = true;
    if (group.isCloudCloud())
        j["cloud_to_cloud"] = true;
    if (group.isCloudAir())
        j["cloud_to_air"] = true;
    if (group.isUnknownType())
        j["unknown_lightning_type"] = true;
    if (const auto dirs = group.directions(); dirs.size())
    {
        j["directions"] = nlohmann::json::array();
        for (const auto &d : dirs)
        {
            j["directions"].push_back(valueFormat->format(d));
        }
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitVicinityGroup(
    const VicinityGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "vicinity"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))},
        {"distance", valueFormat->format(group.distance())}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (const auto dirs = group.directions(); dirs.size())
    {
        j["directions"] = nlohmann::json::array();
        for (const auto &d : dirs)
        {
            j["directions"].push_back(valueFormat->format(d));
        }
    }
    if (const auto d = group.movingDirection(); d.isReported())
    {
        j["moving_direction"] = valueFormat->format(d);
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitMiscGroup(
    const MiscGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{
        {"group", "misc"},
        {"type", util::toLower(magic_enum::enum_name(group.type()))}};
    if (!group.isValid())
        j["not_valid"] = true;
    switch (group.type())
    {
    case metaf::MiscGroup::Type::SUNSHINE_DURATION_MINUTES:
        if (const auto v = group.data(); v.has_value())
        {
            j["minutes"] = std::round(*v);
        }
    case metaf::MiscGroup::Type::CORRECTED_WEATHER_OBSERVATION:
        if (const auto v = group.data(); v.has_value())
        {
            j["correction_number"] = std::round(*v);
        }
    case metaf::MiscGroup::Type::DENSITY_ALTITUDE:
        if (const auto v = group.data(); v.has_value())
        {
            j["ft"] = std::round(*v);
        }
        else
        {
            j["density_altitude_misg"] = true;
        }
        break;
    case metaf::MiscGroup::Type::HAILSTONE_SIZE:
        if (const auto v = group.data(); v.has_value())
        {
            j["in"] = util::formatDecimals(*v, 2);
        }
        break;
    case metaf::MiscGroup::Type::COLOUR_CODE_BLUE:
    case metaf::MiscGroup::Type::COLOUR_CODE_WHITE:
    case metaf::MiscGroup::Type::COLOUR_CODE_GREEN:
    case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW1:
    case metaf::MiscGroup::Type::COLOUR_CODE_YELLOW2:
    case metaf::MiscGroup::Type::COLOUR_CODE_AMBER:
    case metaf::MiscGroup::Type::COLOUR_CODE_RED:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKBLUE:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKWHITE:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKGREEN:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW1:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKYELLOW2:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKAMBER:
    case metaf::MiscGroup::Type::COLOUR_CODE_BLACKRED:
    case metaf::MiscGroup::Type::FROIN:
        break;
    }
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::MetafVisitorBasic::visitUnknownGroup(
    const UnknownGroup &group,
    ReportPart reportPart,
    const std::string &rawString)
{
    (void)reportPart;
    nlohmann::json j{{"group", "unknown"}};
    if (!group.isValid())
        j["not_valid"] = true;
    if (includeRawStrings)
        j["raw_string"] = rawString;
    return j;
}

nlohmann::json OutputFormatBasic::toJson(const metaf::ParseResult &parseResult) const
{
    nlohmann::json output;
    output["report"]["type"] =
        util::toLower(magic_enum::enum_name(parseResult.reportMetadata.type));
    if (parseResult.reportMetadata.error != metaf::ReportError::NONE)
        output["report"]["error"] =
            util::toLower(magic_enum::enum_name(parseResult.reportMetadata.error));

    output["groups"] = nlohmann::json::array();
    MetafVisitorBasic visitor(
        parseResult,
        dateTimeFormat.get(),
        valueFormat.get(),
        getIncludeRawStrings(),
        getReferenceYear(),
        getReferenceMonth(),
        getReferenceDay());
    std::string rawReportStr;
    for (const auto &groupInfo : parseResult.groups)
    {
        auto groupOutput = visitor.visit(groupInfo);
        output["groups"].push_back(groupOutput);
        if (getIncludeRawStrings())
        {
            rawReportStr += metaf::groupDelimiterChar;
            rawReportStr += groupInfo.rawString;
        }
    }
    if (getIncludeRawStrings())
        output["report"]["raw_string"] = rawReportStr;
    return output;
}
