/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2020 by Targoman Intelligent Processing <http://tip.co.ir>
#
#   TargomanAPI is free software: you can redistribute it and/or modify
#   it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   TargomanAPI is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU AFFERO GENERAL PUBLIC LICENSE for more details.
#
#   You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
#   along with Targoman. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/
/**
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "intfMTCharts.h"
#include "Interfaces/AAA/PrivHelpers.h"
//#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/clsJWT.hpp"
//#include "Interfaces/Common/QtTypes.hpp"
#include "Interfaces/Helpers/TokenHelper.h"

using namespace Targoman::API;
using namespace Targoman::API::AAA;
using namespace Targoman::API::Helpers;

TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    /* namespace          */ Targoman::API::ModuleHelpers::MT,
    /* type               */ stuMultiProgressChart
);

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

quint64 checkAPITokenOwner(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    QString             _apiToken
) {
    if (APICALLBOOM_PARAM.jwtActorType() != TAPI::enuTokenActorType::USER)
        throw exAuthorization("JWT is not USER type");

    _apiToken = _apiToken.trimmed();
    if (_apiToken.isEmpty())
        throw exHTTPInternalServerError("API Token not provided");

    enuTokenBanType TokenBanType = TokenHelper::GetTokenBanType(_apiToken);
    if (TokenBanType == enuTokenBanType::Block)
        throw exAuthorization("This API Token is blocked");

    TAPI::JWT_t APITokenJWTPayload;
    QJWT::extractAndDecryptPayload(_apiToken, APITokenJWTPayload);

    clsJWT APITokenJWT(APITokenJWTPayload);

    if (APITokenJWT.actorType() != TAPI::enuTokenActorType::API)
        throw exAuthorization("Just API Token is allowed");

    if (APITokenJWT.ownerID() != APICALLBOOM_PARAM.getActorID())
        throw exAuthorization("API Token is not yours");

    quint64 APITokenID = APITokenJWT.actorID();

    return APITokenID;
}

/**************************************************************************************\
|** baseintfMTCharts ******************************************************************|
\**************************************************************************************/
baseintfMTCharts::baseintfMTCharts(
    const QString &_module
) :
    intfPureModule(_module)
{ ; }

/*
SELECT
    1 + DATEDIFF(columnDate, @start_date) DIV 7  AS weekNumber
  , @start_date + INTERVAL (DATEDIFF(columnDate, @start_date) DIV 7) WEEK
      AS week_start_date
  , MIN(columnDate) AS actual_first_date
  , MAX(columnDate) AS actual_last_date
  , SUM(otherColumn)
  , AVG(otherColumn)
  ---
FROM
    tableX
WHERE
    columnDate >= @start_date
GROUP BY
    DATEDIFF(columnDate, @start_date) DIV 7 ;
*/
/*
https://canvasjs.com/javascript-charts/chart-axis-scale-break/

chart-json: [
    {
        "chart": {
            "title": "chart 1",
            "type": "progress-bar",
            "min" : 0,
            "max" : 100,
            "axis": {
                "y": {
                    "suffix": "%",
                },
            },
        },
        "data" : [
            {
                "label": "formal",
                "points": [
                    { "y": 40, "hint": { "Remained": 80, "Total": 200 } }
                ],
            },
            {
                "label": "informal",
                "points": [
                    { "y": 60, "hint": { "Remained": 40, "Total": 100 } }
                ],
            },
        ],
    },
    {
        "chart": {
            "title": "chart 2",
            "type": "pie",
        },
        "data" : {
            "points": [
                { "y": 123, "label": "data 1" } },
                { "y": 456, "label": "data 2" } },
                { "y": 789, "label": "data 3" } },
            ],
        },
    },
    {
        "chart": {
            "title": "chart 3",
            "type": "spline",
            "axis": {
                "x": {
                    "label": "date",
                },
                "y": {
                    "label": "usage",
                },
            },
        },
        "data" : [
            {
                "label": "data 1",
                "points": [
                    { "x":..., "y":... },
                    { "x":..., "y":... },
                    { "x":..., "y":... },
                    ...
                ],
            },
            {
                "label": "data 2",
                "points": [
                    { "x":..., "y":... },
                    { "x":..., "y":... },
                    { "x":..., "y":... },
                    ...
                ],
            },
        ],
    },
]




const schema =
{
    title: "Optional Overall title", //ex. Charts
    charts: [
        { // Any of the following keys
            type: progressBar,
            title: "Optional title (default null)", //ex. Formal
            name: "name of the field", // ex. Formal
            min: "optional {{MIN_PLACE_HOLDER}} (default: min)",
            max: "optional {{MAX_PLACE_HOLDER}} (default: max)",
            value: "optional {{VALUE_PLACE_HOLDER}} (default: curr)",
            endpoint: "API endpoint to fetch data", //optional when used in multiprogress
            props: {
                key: "value" //Key values in order to customize chart. TBDL
            }
        },
        {
            type: multiprogress,
            title: "Optional box title (default null)", //ex. Remaining
            charts: "Optional array of at least one progressBar",
            endpoint: "API endpoint to fetch data of data+specs",
            props: {
                key: "value" //Key values in order to customize chart. TBDL
            }
        },
        {
            type: pie,
            title: "Optional title (default null)", //ex. Type of usage
            items: [{
                name: "name of the field", // ex. Formal
                label: "label", //ex. Formal
                value: "{{VALUE_PLACE_HOLDER}}", // ex. {{MT_FORMAL}}
            }, {
                name: "name of the field", // ex. Informal
                label: "label", //ex. Informal
                value: "{{VALUE_PLACE_HOLDER}}" // ex. {{MT_INFORMAL}}
            }],
            endpoint: "API endpoint to fetch data", //optional when used in multiPie
            props: {
                key: "value" //Key values in order to customize chart. TBDL
            }
        },
        {
            type: multiPie,
            title: "Optional box title (default null)", //ex. Type of usage
            charts: "Optional array of at least one pie", //optional when used in multiprogress
            endpoint: "API endpoint to fetch data of data+specs",
            props: {
                overall: "true or false indicating wheter to generate an overal piechart or not",
                key: "value" //Key values in order to customize chart. TBDL
            }
        },
        {
            type: "2D",
            title: "Optional title (default null)", //ex. Historical Usage
            axis: {
                X: {
                    label: "Optional Main X axis label", //ex. Time
                    type: "date|int|float| ...",
                    props: {
                        key: "value" //Key values in order to customize chart. TBDL
                    }
                },
                Y: {
                    label: "Optional Main Y axis label", //ex. Count
                    type: "int|float| ...",
                    props: {
                        key: "value" //Key values in order to customize chart. TBDL
                    }
                },
                X2: { //Optional
                    label: "Optional Secondary X axis label", //ex. Time
                    type: "date|int|float| ...",
                    props: {
                        key: "value" //Key values in order to customize chart. TBDL
                    }
                },
                Y2: { //Optional
                    label: "Optional Secondary Y axis label", //ex. Count
                    type: "int|float| ...",
                    props: {
                        key: "value" //Key values in order to customize chart. TBDL
                    }
                }
            },
            endpoint: "API endpoint to fetch data",
            type: "line|bar|stacked|Candle|mixed...",
            items: [ { //this is optinal and can be fetched from endpoint
                    type: "line|bar|stacked|Candle...", //optinal to be used on mixed type,
                    name: "name of the field", // ex. Formal
                    value: "{{VALUE_PLACE_HOLDER}}", // ex. {{MT_INFORMAL}}
                    props: {
                        key: "value" //Key values in order to customize chart. TBDL
                    }
                }
            ],
            props: {
                key: "value" //Key values in order to customize chart. TBDL
            }
        }
    ]
}

const sampleProgressData = {max: 200, curr: 40}

const sampleMultipProgressBar= [
    {name: "formal", title:"رسمی", value: 40},
    {name: "informal", title:"محاوره", value: 60},
]
const samplePieData = {
    MT_FORMAL: 12,
    MT_INFORMAL: 88,
}

const sampleMultiPieData = [
    {name: "formal", label:"رسمی", value: 40},
    {name: "informal", label:"محاوره", value: 60},
]

const sampleLineData = {
    specs: {
        MT_FORMAL: {label:"رسمی", props:{}},
        MT_INFORMAL: {label:"رسمی", props:{}}
    },
    data: {
        "2020-12-01": {MT_FORMAL:12, MT_INFORMAL: 17},
        "2020-12-02": {MT_FORMAL:14, MT_INFORMAL: 47},
        "2020-12-04": {MT_FORMAL:15, MT_INFORMAL: 77},
    }
}

*/

/***
 * report type: multi progress bar
 *
 *   formal: |xxxxxxxxxx-------------------|
 *           used: 40             total: 100
 *
 * informal: |xxxxxxxxxxxxxxxx-------------|
 *           used: 60             total: 200
const sampleMultipProgressBar = [
    { type: },
    { data: [
        { name: "formal", title:"رسمی", value: 40 },
        { name: "informal", title:"محاوره", value: 60 },
    ]},
]
 */

stuMultiProgressChart baseintfMTCharts::usageDataForProgressBar(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _currentActorID
) {
    stuMultiProgressChart Result;

    Result.Type = "multiprogress";

    Result.Series.insert("MT_FORMAL", stuChartSeriesItem(
                             "رسمی",
                             {
                                stuChartSeriesItemProps("%")
                             }
                             ));

    Result.Series.insert("MT_INFORMAL", stuChartSeriesItem(
                             "محاوره",
                             {
                                stuChartSeriesItemProps("%")
                             }
                             ));

    Result.Data.insert("MT_FORMAL", stuYChartData("30"));
    Result.Data.insert("MT_INFORMAL", stuYChartData("60"));

    return Result;

//    return QVariantMap({
//                           { "type", "multiprogress" },
//                           { "series", QVariantMap({
//                                 { "MT_FORMAL", QVariantMap({
//                                       { "label", "رسمی" },
//                                       { "props", QVariantMap({
//                                             { "valueSuffix", "%" }
//                                       }) },
//                                 }) },
//                                 { "MT_INFORMAL", QVariantMap({
//                                       { "label", "محاوره" },
//                                       { "props", QVariantMap({
//                                             { "valueSuffix", "%" }
//                                       }) },
//                                 }) },
//                           }) },
//                           { "data", QVariantMap({
//                                 { "MT_FORMAL", QVariantList({
//                                       30,
//                                       40
//                                 }) },
//                                 { "MT_INFORMAL", 60 },
//                           }) },
//                       });





    QStringList QueryStringParts;
    QueryStringParts.append("SELECT");

//    REGEXP_LIKE(usgKey, '(.*::)(.*::)formal', 'i')




}

QVariant baseintfMTCharts::usageDataForPieChart(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _currentActorID
) {
    return QVariantMap({
                           { "type", "pie" },
                           { "series", QVariantMap({
                                 { "MT_FORMAL", QVariantMap({
                                       { "label", "رسمی" },
                                       { "props", {} },
                                 }) },
                                 { "MT_INFORMAL", QVariantMap({
                                       { "label", "محاوره" },
                                       { "props", {} },
                                 }) },
                           }) },
                           { "data", QVariantMap({
                                 { "MT_FORMAL", 40 },
                                 { "MT_INFORMAL", 60 },
                           }) },
                       });
}

QVariant baseintfMTCharts::usageDataForLineChart(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _currentActorID
) {
    return QVariantMap({
                           { "type", "line" },
                           { "series", QVariantMap({
                                 { "MT_FORMAL", QVariantMap({
                                       { "label", "رسمی" },
                                       { "props", {} },
                                 }) },
                                 { "MT_INFORMAL", QVariantMap({
                                       { "label", "محاوره" },
                                       { "props", {} },
                                 }) },
                           }) },
                           { "data", QVariantMap({
                                 { "MT_FORMAL", QVariantMap({
                                       { "2020-12-01", 12 },
                                       { "2020-12-02", 14 },
                                       { "2020-12-03", 15 },
                                 }) },
                                 { "MT_INFORMAL", QVariantMap({
                                       { "2020-12-01", 17 },
                                       { "2020-12-02", 47 },
                                       { "2020-12-04", 77 },
                                 }) },
                           }) },
                       });
}

//TAPI::stuTable baseintfMTCharts::charts(
//    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
//    quint64                 _currentActorID,
//    quint64                 _pageIndex,
//    quint16                 _pageSize,
//    bool                    _reportCount,
//    quint64                 _assetID,
//    TAPI::DateTime_t        _fromDate,
//    TAPI::DateTime_t        _toDate,
//    quint16                 _step,
//    enuAssetHistoryReportStepUnit::Type _stepUnit
//) {
//    QStringList QueryStringParts;
//    QueryStringParts.append("SELECT");

//    //--
//    QString StrFromDate = _fromDate.isValid()
//                          ? _fromDate.toString("yyyy-MM-dd hh:mm:ss")
//                          : "";

//    QString StrToDate = _toDate.isValid()
//                        ? _toDate.toString("yyyy-MM-dd hh:mm:ss")
//                        : "";

//    QString DateDiffPart;
//    if (_stepUnit == enuAssetHistoryReportStepUnit::Minute) {
//        if (_fromDate.isValid())
//            DateDiffPart = QString("1 + TIME_TO_SEC(TIMEDIFF(ushLastDateTime, '%1')) DIV %2")
//                           .arg(StrFromDate)
//                           .arg(_step * 60);
//        else
//            DateDiffPart = QString("1 + TIME_TO_SEC(TIMEDIFF(NOW(), ushLastDateTime)) DIV %1")
//                           .arg(_step * 60);
//    } else if (_stepUnit == enuAssetHistoryReportStepUnit::Hour) {
//        if (_fromDate.isValid())
//            DateDiffPart = QString("1 + TIME_TO_SEC(TIMEDIFF(ushLastDateTime, '%1')) DIV %2")
//                           .arg(StrFromDate)
//                           .arg(_step * 60 * 60);
//        else
//            DateDiffPart = QString("1 + TIME_TO_SEC(TIMEDIFF(NOW(), ushLastDateTime)) DIV %1")
//                           .arg(_step * 60 * 60);
//    } else if (_stepUnit == enuAssetHistoryReportStepUnit::Day) {
//        if (_fromDate.isValid())
//            DateDiffPart = QString("1 + DATEDIFF(ushLastDateTime, '%1') DIV %2")
//                           .arg(StrFromDate)
//                           .arg(_step);
//        else
//            DateDiffPart = QString("1 + DATEDIFF(NOW(), ushLastDateTime) DIV %1")
//                           .arg(_step);
//    }
//    QueryStringParts.append(QString("%1 AS counter").arg(DateDiffPart));

//    //--
//    QueryStringParts.append(", COUNT(*) AS pointsCount");
//    QueryStringParts.append(", MIN(ushLastDateTime) AS minDate");
//    QueryStringParts.append(", MAX(ushLastDateTime) AS maxDate");

//    //--
//    foreach (auto CreditField, this->creditFieldNames()) {
//        QueryStringParts.append(QString(", SUM(%1) AS %1").arg(CreditField));
//    }

//    //--
//    QueryStringParts.append("FROM tblMTChartsHistory ush");
//    QueryStringParts.append("INNER JOIN tblAccountUserAssets uas");
//    QueryStringParts.append("ON uas.uasID = ush.ush_uasID");

//    //--
//    QStringList WhereParts;

//    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
//        WhereParts.append(QString("uas_actorID = %1").arg(_currentActorID));

//    if (_assetID > 0)
//        WhereParts.append(QString("ush_uasID = %1").arg(_assetID));

//    if (_fromDate.isValid())
//        WhereParts.append(QString("ushLastDateTime >= '%1'").arg(StrFromDate));

//    if (_toDate.isValid())
//        WhereParts.append(QString("ushLastDateTime <= '%1'").arg(StrToDate));

//    if (WhereParts.isEmpty() == false) {
//        QueryStringParts.append("WHERE");
//        QueryStringParts.append(WhereParts.join("\n AND "));
//    }

//    //--
//    QueryStringParts.append(QString("GROUP BY %1").arg(DateDiffPart));

//    //--
//    QueryStringParts.append("ORDER BY MIN(ushLastDateTime)");

//    //--
//    QString QueryString = QueryStringParts.join("\n");

//    TAPI::stuTable Result;
//    Result.HasCount = false;

//    if (_reportCount) {
//        QString CountingQueryString = QString("SELECT COUNT(*) AS cnt FROM (\n%1\n) qry").arg(QueryString);
//        QJsonDocument ResultTotalRows = this->execQuery(APICALLBOOM_PARAM, CountingQueryString).toJson(true);

//        Result.HasCount = true;
//        Result.TotalRows = ResultTotalRows
//                           .toVariant()
//                           .toMap()["cnt"]
//                           .toULongLong();

//    }

//    if ((_pageIndex > 0) || (_pageSize > 0)) {
//        if (_pageSize == 0)
//            _pageSize = 100;

//        QueryString += QString("\nLIMIT %1,%2").arg(_pageIndex * _pageSize).arg(_pageSize);
//    }

//    QJsonDocument ResultRows = this->execQuery(APICALLBOOM_PARAM, QueryString).toJson(false);

//    Result.Rows = ResultRows
//                  .toVariant()
//                  .toList();

//    if (_pageSize > 0)
//        Result.PageCount = ceil((double)Result.TotalRows / _pageSize);
//    else
//        Result.PageCount = 1;

//    Result.HasMore = (Result.PageCount > (_pageIndex + 1));

//    return Result;
//}

/**************************************************************************************\
|** baseintfMTCharts_USER *************************************************************|
\**************************************************************************************/
baseintfMTCharts_USER::baseintfMTCharts_USER(
    const QString &_module
) :
    baseintfMTCharts(
        _module
) { ; }

Targoman::API::ModuleHelpers::MT::stuMultiProgressChart IMPL_REST_GET(baseintfMTCharts_USER, usageDataForProgressBar, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForProgressBar(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_USER, usageDataForPieChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForPieChart(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_USER, usageDataForLineChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForLineChart(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

/**************************************************************************************\
|** baseintfMTCharts_API **************************************************************|
\**************************************************************************************/
baseintfMTCharts_API::baseintfMTCharts_API(
    const QString &_module
) :
    baseintfMTCharts(
        _module
) { ; }

Targoman::API::ModuleHelpers::MT::stuMultiProgressChart IMPL_REST_GET(baseintfMTCharts_API, usageDataForProgressBar, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForProgressBar(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_API, usageDataForPieChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForPieChart(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_API, usageDataForLineChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForLineChart(
        APICALLBOOM_PARAM,
        CurrentActorID
    );
}

/**************************************************************************************\
|** intfMTCharts **********************************************************************|
\**************************************************************************************/
template <bool _itmplIsTokenBase>
intfMTCharts<_itmplIsTokenBase>::intfMTCharts(
    const QString &_module
) :
    std::conditional<_itmplIsTokenBase, baseintfMTCharts_API, baseintfMTCharts_USER>::type(
        _module
    )
{ ; }

template class intfMTCharts<false>;
template class intfMTCharts<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
