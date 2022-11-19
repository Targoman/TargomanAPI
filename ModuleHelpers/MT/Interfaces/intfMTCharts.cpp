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
#include "intfMTAccounting.h"
#include "intfMTModule.h"

using namespace Targoman::API;
using namespace Targoman::API::AAA;
using namespace Targoman::API::Helpers;
using namespace Targoman::API::ORM;

//TAPI_REGISTER_METATYPE_TYPE_STRUCT(
//    /* namespace          */ Targoman::API::ModuleHelpers::MT,
//    /* type               */ stuMultiProgressChart
//);

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

//template <bool _itmplIsTokenBase>
//intfMTModule<_itmplIsTokenBase> *baseintfMTCharts<_itmplIsTokenBase>::mtModule() {
//    return this->MTModule;
//}

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

QVariant baseintfMTCharts::getSchema(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    QString _key
) {
    ///@TODO: read from tblConfigurations

    Targoman::API::ORM::intfI18N* I18N;
    if (dynamic_cast<intfMTModule<false>*>(this->parentModule()))
        I18N = dynamic_cast<intfMTModule<false>*>(this->parentModule())->i18n();
    else
        I18N = dynamic_cast<intfMTModule<true>*>(this->parentModule())->i18n();

    QVariantMap RemainingCreditPercentageMultiProgress = {
        { "title", I18N->translated(APICALLBOOM_PARAM, "Remaining credit percentage") },
        { "type", "multiprogress" },
        { "charts", QVariantMap({
              { "%%ITERATION%%", QVariantMap({
                    { "type", "progressBar" },
                    { "name", "{{NAME}}" },
                    { "title", "{{TITLE}}" },
                    { "min", "{{MIN}}" },
                    { "max", "{{MAX}}" },
                    { "value", "{{VALUE}}" },
              }) },
        }) },
        { "endpoint", QVariantMap({
              { "url", "{{API_URL}}/MT/Charts/usageDataForProgressBar" },
              { "params", QVariantMap({
                    { "apiToken", "{{API_TOKEN}}" },
                    { "key", _key },
              }) },
        }) },
    };

    QVariantMap RemainingCreditPercentagePieChart = {
        { "title", I18N->translated(APICALLBOOM_PARAM, "Remaining credit percentage") },
        { "type", "pie" },
        { "items", QVariantMap({
              { "%%ITERATION%%", QVariantMap({
                    { "name", "{{NAME}}" },
                    { "title", "{{TITLE}}" },
                    { "value", "{{VALUE}}" },
              }) },
        }) },
        { "endpoint", QVariantMap({
              { "url", "{{API_URL}}/MT/Charts/usageDataForPieChart" },
              { "params", QVariantMap({
                    { "apiToken", "{{API_TOKEN}}" },
                    { "key", _key },
              }) },
        }) },
    };

    QVariantMap Charts = {
        { "title", I18N->translated(APICALLBOOM_PARAM, "MT Charts") },
        { "charts", QVariantList({
              RemainingCreditPercentageMultiProgress,
              RemainingCreditPercentagePieChart,
        }) }
    };

    return Charts;

    /*
    QString Schema = R"(
{
    "title": "MT Charts",
    "charts": [
        {
            "type": "multiprogress",
            "title": "Remaining credit percentage",
            "charts": [
                {
                    "type": "progressBar",
                    "title": "Formal",
                    "name": "Formal",
                    "min": "{{MIN}}",
                    "max": "{{MAX}}",
                    "value": "{{VALUE}}",
                    "endpoint": {
                        "url" : "{{API_URL}}/MT/Charts/usageDataForProgressBar",
                        "params" : [
                            "apiToken" : "{{API_TOKEN}}",
                            "key" : "{{KEY}}",
                            "path" : "*::formal",
                        ],
                    },
                },
            ],
        }
    ]
}
)";

        {
            type: pie,
            title: 'Optional title (default null)', //ex. Type of usage
            items: [{
                name: 'name of the field', // ex. Formal
                label: 'label', //ex. Formal
                value: '{{VALUE_PLACE_HOLDER}}', // ex. {{MT_FORMAL}}
            }, {
                name: 'name of the field', // ex. Informal
                label: 'label', //ex. Informal
                value: '{{VALUE_PLACE_HOLDER}}' // ex. {{MT_INFORMAL}}
            }],
            endpoint: 'API endpoint to fetch data', //optional when used in multiPie
            props: {
                key: 'value' //Key values in order to customize chart. TBDL
            }
        },
        {
            type: multiPie,
            title: 'Optional box title (default null)', //ex. Type of usage
            charts: 'Optional array of at least one pie', //optional when used in multiprogress
            endpoint: 'API endpoint to fetch data of data+specs',
            props: {
                overall: 'true or false indicating wheter to generate an overal piechart or not',
                key: 'value' //Key values in order to customize chart. TBDL
            }
        },
        {
            type: '2D',
            title: 'Optional title (default null)', //ex. Historical Usage
            axis: {
                X: {
                    label: 'Optional Main X axis label', //ex. Time
                    type: 'date|int|float| ...',
                    props: {
                        key: 'value' //Key values in order to customize chart. TBDL
                    }
                },
                Y: {
                    label: 'Optional Main Y axis label', //ex. Count
                    type: 'int|float| ...',
                    props: {
                        key: 'value' //Key values in order to customize chart. TBDL
                    }
                },
                X2: { //Optional
                    label: 'Optional Secondary X axis label', //ex. Time
                    type: 'date|int|float| ...',
                    props: {
                        key: 'value' //Key values in order to customize chart. TBDL
                    }
                },
                Y2: { //Optional
                    label: 'Optional Secondary Y axis label', //ex. Count
                    type: 'int|float| ...',
                    props: {
                        key: 'value' //Key values in order to customize chart. TBDL
                    }
                }
            },
            endpoint: 'API endpoint to fetch data',
            type: 'line|bar|stacked|Candle|mixed...',
            items: [ { //this is optinal and can be fetched from endpoint
                    type: 'line|bar|stacked|Candle...', //optinal to be used on mixed type,
                    name: 'name of the field', // ex. Formal
                    value: '{{VALUE_PLACE_HOLDER}}', // ex. {{MT_INFORMAL}}
                    props: {
                        key: 'value' //Key values in order to customize chart. TBDL
                    }
                }
            ],
            props: {
                key: 'value' //Key values in order to customize chart. TBDL
            }
        }
    ]
}
)";
    return Schema
            .replace("{{KEY}}", _key)
            ;
*/

}

/*

credit:
-- input: -----------------------
{
    'NMT': {
        'ALL': 5000,
        'formal': {
            'ALL': {
                total: 1000
            },
            'en2fa': {
                'day' : 2000,
                'week' : 2100,
                'month' : 2200,
                'total' : 2300
            }
        }
    },
    'ENMT': {
        'formal': {
            'en2fa': 3000,
        },
        'Informal' : {
            month: 1000,
            total: -1  //unlimited total so it can be discarded
        }
    },
    'TST': {
        'month' : 4000
        // this means total is unlimited
    },
    'ALL': 10000
}

-- output: -----------------------
NMT                 -> 5,000
NMT::formal         -> 1,000
NMT::formal::en2fa	-> 2,300
ENMT::formal::en2fa	-> 3,000
ENMT::informal      -> -1
TST                 -> -1
ALL                 -> 10,000

*/

int extractSumOfCreditValues(
    QMap<QString, stuCreditUsageRemained> &_creditUsageRemained,
    const QVariantMap &_creditSpecs,
    const QString &_parentPath = {}
) {
    int NumOfInserted = 0;

    for (auto it = _creditSpecs.constBegin();
         it != _creditSpecs.constEnd();
         it++
    ) {
        QVariant Value = it.value();
        QString Path = it.key().toLower();

        if ((Path == "day") || (Path == "week") || (Path == "month"))
            continue;

        QString FullPath = _parentPath;

        if (Path == "all")
            Path = (FullPath.isEmpty() ? "*" : "");
        else if (Path == "total") {
            if (Value.canConvert<QVariantMap>())
                throw exTargomanBase("'total' credit item must be leaf");
            Path = "";
        }

        if (Path.length()) {
            if (FullPath.isEmpty() == false)
                FullPath += "::";
            FullPath += Path;
        }

        if (Value.canConvert<QVariantMap>()) {
            int n = extractSumOfCreditValues(
                _creditUsageRemained,
                Value.toMap(),
                FullPath
                );

            if (n > 0)
                ++NumOfInserted;
        } else {
            qint64 V = Value.toLongLong();

            if ((V != -1) && _creditUsageRemained.contains(FullPath))
                _creditUsageRemained[FullPath].Credit += V;
            else
                _creditUsageRemained[FullPath].Credit = V;

            ++NumOfInserted;
        }
    }

    if (NumOfInserted == 0) {
        QString Path = "";
        if (_parentPath.isEmpty())
            Path = "*";

        QString FullPath = _parentPath;

        if (Path.length()) {
            if (FullPath.isEmpty() == false)
                FullPath += "::";
            FullPath += Path;
        }

        _creditUsageRemained[FullPath].Credit = -1;

        ++NumOfInserted;
    }

    return NumOfInserted;
}

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

//CreditUsageRemained_t, I18NMap
std::tuple<CreditUsageRemained_t, QMap<QString, QString>> baseintfMTCharts::getUsageData(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    QString _key
) {
    baseintfAccountingBasedModule* ParentModule = dynamic_cast<baseintfAccountingBasedModule*>(this->parentModule());

    //credits
    //------------------------------------------------
    ORMSelectQuery SelectQuery = ParentModule->accountUserAssets()->makeSelectQuery(APICALLBOOM_PARAM)
                                 .where({ tblAccountUserAssetsBase::Fields::uasStatus,
                                          enuConditionOperator::Equal,
                                          enuAuditableStatus::Active })
//                                 .addCols(this->accountUserAssets()->selectableColumnNames())
                                 ;

    SelectQuery
        .andWhere({ tblAccountUserAssetsBase::Fields::uas_actorID, enuConditionOperator::Equal, _actorID })

        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidFromDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidFromDate, enuConditionOperator::LessEqual,
                DBExpression::NOW() })
        )
        .andWhere(clsCondition({ tblAccountUserAssetsBase::Fields::uasValidToDate, enuConditionOperator::Null })
            .orCond({ tblAccountUserAssetsBase::Fields::uasValidToDate, enuConditionOperator::GreaterEqual,
                DBExpression::NOW() })
        )
    ;

    TAPI::stuTable Table = SelectQuery
                           .pageSize(0)
                           .all();

    if (Table.Rows.count() == 0)
        return {};

    QStringList AssetsIDs;
    CreditUsageRemained_t CreditUsageRemained;

    foreach (QVariant Row, Table.Rows) {
        QVariantMap UserAssetInfo = Row.toMap();

        AssetsIDs.append(UserAssetInfo[tblAccountUserAssetsBase::Fields::uasID].toString());

        if (UserAssetInfo.contains(tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs) == false)
            continue;

        QVariantMap CreditSpecs = UserAssetInfo[tblAccountUserAssetsMTBase::ExtraFields::uasCreditSpecs].toMap();
        extractSumOfCreditValues(CreditUsageRemained, CreditSpecs);
    }

    //usages
    //------------------------------------------------
    ORMSelectQuery UsageSelectQuery = ParentModule->accountAssetUsage()->makeSelectQuery(APICALLBOOM_PARAM)
                                      .where({ tblAccountAssetUsageBase::Fields::usg_uasID,
                                               enuConditionOperator::In,
                                               AssetsIDs.join(",") })
                                      .andWhere({ tblAccountAssetUsageBase::Fields::usgResolution,
                                                  enuConditionOperator::Equal,
                                                  Targoman::API::AAA::enuAssetUsageResolution::Total
                                                })
                                      ;

    TAPI::stuTable UsageTable = UsageSelectQuery.pageSize(0).all();

    foreach (QVariant Row, UsageTable.Rows) {
        QVariantMap AssetUsageInfo = Row.toMap();

        tblAccountAssetUsageMTBase::DTO AccountAssetUsageMTBaseDTO;
        AccountAssetUsageMTBaseDTO.fromJson(QJsonObject::fromVariantMap(AssetUsageInfo));

        QString UsageKey = AccountAssetUsageMTBaseDTO.usgKey.toLower();

        while (UsageKey.length()) {
            if (CreditUsageRemained.contains(UsageKey)) {
                CreditUsageRemained[UsageKey].Usage += AccountAssetUsageMTBaseDTO.usgUsedTotalWords;

                break;
            }

            int idx = UsageKey.lastIndexOf("::");
            if (idx < 0)
                break;

            UsageKey = UsageKey.left(idx);
        }
    }

//    QString Schema = getSchema(APICALLBOOM_PARAM, _key).toString();
//    QStringList ChartPaths = extract from schema

    QStringList FullKeys;
    QStringList Engines;
    QStringList Classes;
    QStringList Directions;

    for (auto it = CreditUsageRemained.begin();
         it != CreditUsageRemained.end();
         ++it
    ) {
        ///@TODO: remove item if not exists in schema

        //----------
        FullKeys.append(it.key());
        auto Parts = it.key().split("::");
        if (Parts.length() >= 3) Directions.append(Parts[2]);
        if (Parts.length() >= 2) Classes.append(Parts[1]);
        if (Parts.length() >= 1) Engines.append(Parts[0]);

        //----------
        if ((it->Credit == -1) || (it->Usage == 0))
            continue;

        it->Remained = it->Credit - it->Usage;
    }

    //output
    //------------------------------------------------
    Targoman::API::ORM::intfI18N* I18N;
    if (dynamic_cast<intfMTModule<false>*>(this->parentModule()))
        I18N = dynamic_cast<intfMTModule<false>*>(this->parentModule())->i18n();
    else
        I18N = dynamic_cast<intfMTModule<true>*>(this->parentModule())->i18n();

    auto I18NSelectQuery = I18N->makeSelectQuery(APICALLBOOM_PARAM)
                           .addCol(tblI18N::Fields::i18nKey)
                           .addCol(DBExpression::VALUE(QString("COALESCE("
                                                               "JSON_UNQUOTE(JSON_EXTRACT(%1.%2, '$.\"%3\"')),"
                                                               "JSON_UNQUOTE(JSON_EXTRACT(%1.%2, '$.default'))"
                                                               ")")
                                                       .arg(tblI18N::Name)
                                                       .arg(tblI18N::Fields::i18nValue)
                                                       .arg(APICALLBOOM_PARAM.language())
                                                       ),
                                   "Translated")

                           .where({ tblI18N::Fields::i18nKey,
                                    enuConditionOperator::In,
                                    "'" + FullKeys.join("','") + "'"
                                  });
    if (Engines.length())
        I18NSelectQuery.orWhere({ tblI18N::Fields::i18nKey,
                                  enuConditionOperator::In,
                                  "'" + Engines.join("','") + "'"
                                });
    if (Classes.length())
        I18NSelectQuery.orWhere({ tblI18N::Fields::i18nKey,
                                  enuConditionOperator::In,
                                  "'" + Classes.join("','") + "'"
                                });
    if (Directions.length())
        I18NSelectQuery.orWhere({ tblI18N::Fields::i18nKey,
                                  enuConditionOperator::In,
                                  "'" + Directions.join("','") + "'"
                                });

    auto I18NRows = I18NSelectQuery
                    .pageSize(0)
                    .all();

    QMap<QString, QString> I18NMap;

    foreach (QVariant Row, I18NRows.Rows) {
        QVariantMap Info = Row.toMap();

        if (Info["Translated"].toString().isEmpty())
            continue;

        I18NMap.insert(Info[tblI18N::Fields::i18nKey].toString(), Info["Translated"].toString());
    }

    //------------------------------------------------
    return { CreditUsageRemained, I18NMap };
}

QVariant baseintfMTCharts::usageDataForProgressBar(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    QString _key
) {
    auto [CreditUsageRemained, I18NMap] = getUsageData(
            APICALLBOOM_PARAM,
            _actorID,
            _key
            );

    QVariantList Result;

    Targoman::API::ORM::intfI18N* I18N;
    if (dynamic_cast<intfMTModule<false>*>(this->parentModule()))
        I18N = dynamic_cast<intfMTModule<false>*>(this->parentModule())->i18n();
    else
        I18N = dynamic_cast<intfMTModule<true>*>(this->parentModule())->i18n();

    for (auto it = CreditUsageRemained.constBegin();
         it != CreditUsageRemained.constEnd();
         ++it
    ) {
        QString Title = it.key().split("::").join(" - ");
        if (I18NMap.contains(it.key()))
            Title = I18NMap[it.key()];
        else {
            auto KeyParts = it.key().split("::");
            QStringList TitleParts;

            if (KeyParts.length() >= 1) {
                if (I18NMap.contains(KeyParts[0]))
                    TitleParts.append(I18NMap[KeyParts[0]]);
                else
                    TitleParts.append(KeyParts[0]);
            }

            if (KeyParts.length() >= 2) {
                if (I18NMap.contains(KeyParts[1]))
                    TitleParts.append(I18NMap[KeyParts[1]]);
                else
                    TitleParts.append(KeyParts[1]);
            }

            if (KeyParts.length() >= 3) {
                if (I18NMap.contains(KeyParts[2]))
                    TitleParts.append(I18NMap[KeyParts[2]]);
                else
                    TitleParts.append(KeyParts[2]);
            }

            Title = TitleParts.join(" - ");
        }

        quint32 Value;

//        if (it->Remained == -1)
        if (it->Credit == -1)
            Value = 50;
        else if (it->Remained == -1)
            Value = 0;
        else if ((it->Credit == 0) || (it->Remained == 0))
            Value = 100;
        else
            Value = (quint32)(((double)it->Remained * 100.0) / (double)it->Credit);

        Result.append(QVariantMap({
                                      { "NAME", it.key() },
                                      { "TITLE", Title },
                                      { "VALUE", Value },
                                      { "MIN", 0 },
                                      { "MAX", 100 },
                                  }));
    }

    return Result;
}

QVariant baseintfMTCharts::usageDataForPieChart(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    QString _key
) {
    auto [CreditUsageRemained, I18NMap] = getUsageData(
            APICALLBOOM_PARAM,
            _actorID,
            _key
            );

    QVariantMap Result;

    for (auto it = CreditUsageRemained.constBegin();
         it != CreditUsageRemained.constEnd();
         ++it
    ) {
//        QString Title = it.key().split("::").join(" - ");
//        if (I18NMap.contains(it.key()))
//            Title = I18NMap[it.key()];
//        else {
//            auto KeyParts = it.key().split("::");
//            QStringList TitleParts;

//            if (KeyParts.length() >= 1) {
//                if (I18NMap.contains(KeyParts[0]))
//                    TitleParts.append(I18NMap[KeyParts[0]]);
//                else
//                    TitleParts.append(KeyParts[0]);
//            }

//            if (KeyParts.length() >= 2) {
//                if (I18NMap.contains(KeyParts[1]))
//                    TitleParts.append(I18NMap[KeyParts[1]]);
//                else
//                    TitleParts.append(KeyParts[1]);
//            }

//            if (KeyParts.length() >= 3) {
//                if (I18NMap.contains(KeyParts[2]))
//                    TitleParts.append(I18NMap[KeyParts[2]]);
//                else
//                    TitleParts.append(KeyParts[2]);
//            }

//            Title = TitleParts.join(" - ");
//        }

        quint32 Value;

//        if (it->Remained == -1)
        if (it->Credit == -1)
            Value = 50;
        else if (it->Remained == -1)
            Value = 0;
        else if ((it->Credit == 0) || (it->Remained == 0))
            Value = 100;
        else
            Value = (quint32)(((double)it->Remained * 100.0) / (double)it->Credit);

        Result.insert(it.key(), Value);
    }

    return Result;
}

QVariant baseintfMTCharts::usageDataForLineChart(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    QString _key
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
//    quint64                 _actorID,
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
//        WhereParts.append(QString("uas_actorID = %1").arg(_actorID));

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

QVariant IMPL_REST_GET(baseintfMTCharts_USER, schema, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _key
)) {
    return getSchema(APICALLBOOM_PARAM, _key);
}

QVariant IMPL_REST_GET(baseintfMTCharts_USER, usageDataForProgressBar, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _key
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForProgressBar(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_USER, usageDataForPieChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _key
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForPieChart(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_USER, usageDataForLineChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _key
)) {
    quint64 CurrentActorID = APICALLBOOM_PARAM.getActorID();

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForLineChart(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
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

QVariant IMPL_REST_GET(baseintfMTCharts_API, schema, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _apiToken,
    QString _key
)) {
    return getSchema(APICALLBOOM_PARAM, _key);
}

QVariant IMPL_REST_GET(baseintfMTCharts_API, usageDataForProgressBar, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken,
    QString _key
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForProgressBar(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_API, usageDataForPieChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken,
    QString _key
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForPieChart(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
    );
}

QVariant IMPL_REST_GET(baseintfMTCharts_API, usageDataForLineChart, (
    APICALLBOOM_TYPE_JWT_USER_IMPL  &APICALLBOOM_PARAM,
    QString _apiToken,
    QString _key
)) {
    quint64 CurrentActorID = checkAPITokenOwner(APICALLBOOM_PARAM, _apiToken);

//    bool CompactList = APICALLBOOM_PARAM.requestHeader("compact-list", false).toBool();

    return this->usageDataForLineChart(
        APICALLBOOM_PARAM,
        CurrentActorID,
        _key
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

/**************************************************************************************/
template class intfMTCharts<false>;
template class intfMTCharts<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces


/*

SELECT
tblAccountAssetUsage_total.usg_uasID
-- , tblAccountAssetUsage.usgResolution
, tblAccountAssetUsage_total.SumUsed AS TotalUsed
, tblAccountAssetUsage_perYear.DateRes AS YearDateRes
, tblAccountAssetUsage_perYear.SumUsed AS UsedPerYear
, tblAccountAssetUsage_perMonth.DateRes AS MonthDateRes
, tblAccountAssetUsage_perMonth.SumUsed AS UsedPerMonth
, tblAccountAssetUsage_perDay.DateRes AS DayDateRes
, tblAccountAssetUsage_perDay.SumUsed AS UsedPerDay
, tblAccountAssetUsage_perHour.DateRes AS HourDateRes
, tblAccountAssetUsage_perHour.SumUsed AS UsedPerHour
-- , tblAccountAssetUsage.usgLastDateTime
-- , tblAccountAssetUsage.usgKey
, tblAccountUserAssets.*

FROM tblAccountUserAssets

LEFT JOIN (
SELECT usg_uasID
, SUM(usgUsedTotalWords) AS SumUsed
FROM tblAccountAssetUsage
WHERE REGEXP_LIKE(usgKey, '(.*::)formal(::.*)', 'i')
AND usgResolution = 'T'
GROUP BY usg_uasID
) tblAccountAssetUsage_total
ON tblAccountAssetUsage_total.usg_uasID = tblAccountUserAssets.uasID

LEFT JOIN (
SELECT usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y') AS DateRes
, SUM(usgUsedTotalWords) AS SumUsed
FROM tblAccountAssetUsage
WHERE REGEXP_LIKE(usgKey, '(.*::)formal(::.*)', 'i')
AND usgResolution = 'Y'
GROUP BY usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y')
) tblAccountAssetUsage_perYear
ON tblAccountAssetUsage_perYear.usg_uasID = tblAccountUserAssets.uasID

LEFT JOIN (
SELECT usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m') AS DateRes
, SUM(usgUsedTotalWords) AS SumUsed
FROM tblAccountAssetUsage
WHERE REGEXP_LIKE(usgKey, '(.*::)formal(::.*)', 'i')
AND usgResolution = 'M'
GROUP BY usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m')
) tblAccountAssetUsage_perMonth
ON tblAccountAssetUsage_perMonth.usg_uasID = tblAccountUserAssets.uasID

LEFT JOIN (
SELECT usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m-%d') AS DateRes
, SUM(usgUsedTotalWords) AS SumUsed
FROM tblAccountAssetUsage
WHERE REGEXP_LIKE(usgKey, '(.*::)formal(::.*)', 'i')
AND usgResolution = 'D'
GROUP BY usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m-%d')
) tblAccountAssetUsage_perDay
ON tblAccountAssetUsage_perDay.usg_uasID = tblAccountUserAssets.uasID

LEFT JOIN (
SELECT usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m-%d %H') AS DateRes
, SUM(usgUsedTotalWords) AS SumUsed
FROM tblAccountAssetUsage
WHERE REGEXP_LIKE(usgKey, '(.*::)formal(::.*)', 'i')
AND usgResolution = 'H'
GROUP BY usg_uasID
, DATE_FORMAT(usgLastDateTime, '%Y-%m-%d %H')
) tblAccountAssetUsage_perHour
ON tblAccountAssetUsage_perHour.usg_uasID = tblAccountUserAssets.uasID

WHERE tblAccountAssetUsage_total.usg_uasID IS NOT null

-- AND uas_actorID = 542

*/
