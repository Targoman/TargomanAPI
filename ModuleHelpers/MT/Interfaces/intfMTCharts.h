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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTCHARTS_H
#define TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTCHARTS_H

#include "Interfaces/API/intfPureModule.h"

namespace Targoman::API::ModuleHelpers::MT {

TAPI_DEFINE_STRUCT(stuChartSeriesItemProps,
    SF_QString(ValueSuffix)
);

TAPI_DEFINE_STRUCT(stuChartSeriesItem,
    SF_QString(Label),
    SF_QListOfVarStruct(Props, stuChartSeriesItemProps)
);
typedef QMap<QString, stuChartSeriesItem> ChartSeries_t;

TAPI_DEFINE_STRUCT(stuYChartData,
    SF_QString(Y)
);
typedef QMap<QString, stuYChartData> YChartDataSet_t;

TAPI_DEFINE_STRUCT(stuMultiProgressChart,
    SF_QString          (Type),
    SF_QMapOfVarStruct  (Series, stuChartSeriesItem, ChartSeries_t),
    SF_QMapOfVarStruct  (Data, stuYChartData, YChartDataSet_t)
);





TAPI_DEFINE_STRUCT(stuXYChartData,
    SF_QString(X),
    SF_QString(Y)
);
typedef QMap<QString, stuXYChartData> XYChartDataSet_t;

}

TAPI_DECLARE_METATYPE(Targoman::API::ModuleHelpers::MT::stuMultiProgressChart)

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

using namespace API;

class baseintfMTCharts : public intfPureModule
{
public:
    baseintfMTCharts(
        const QString &_module
    );

//public:
//    virtual bool IsTokenBase() = 0;

protected:
//    virtual QStringList creditFieldNames() = 0;

    Targoman::API::ModuleHelpers::MT::stuMultiProgressChart usageDataForProgressBar(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _currentActorID
    );

    QVariant usageDataForPieChart(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _currentActorID
    );

    QVariant usageDataForLineChart(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _currentActorID
    );
};

/******************************************************/
class baseintfMTCharts_USER : public baseintfMTCharts
{
    Q_OBJECT

protected:
    baseintfMTCharts_USER(
        const QString &_module
    );

//protected:
//    inline bool IsTokenBase() {
//        return false;
//    }

protected slots:
    Targoman::API::ModuleHelpers::MT::stuMultiProgressChart REST_GET(
        usageDataForProgressBar,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
        ),
        "Get MT Usage chart data for progress bar"
    );

    QVariant REST_GET(
        usageDataForPieChart,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
        ),
        "Get MT Usage chart data for pie chart"
    );

    QVariant REST_GET(
        usageDataForLineChart,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
        ),
        "Get MT Usage chart data for lin chart"
    );
};

/******************************************************/
class baseintfMTCharts_API : public baseintfMTCharts
{
    Q_OBJECT

protected:
    baseintfMTCharts_API(
        const QString &_module
    );

//protected:
//    inline bool IsTokenBase() {
//        return true;
//    }

protected slots:
    Targoman::API::ModuleHelpers::MT::stuMultiProgressChart REST_GET(
        usageDataForProgressBar,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            QString _apiToken
        ),
        "Get MT Usage chart data for progress bar"
    );

    QVariant REST_GET(
        usageDataForPieChart,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            QString _apiToken
        ),
        "Get MT Usage chart data for pie chart"
    );

    QVariant REST_GET(
        usageDataForLineChart,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            QString _apiToken
        ),
        "Get MT Usage chart data for lin chart"
    );
};

/******************************************************/
template <bool _itmplIsTokenBase>
class intfMTCharts : public std::conditional<_itmplIsTokenBase, baseintfMTCharts_API, baseintfMTCharts_USER>::type
{
protected:
    intfMTCharts(
        const QString &_module
    );
};

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces

#endif // TARGOMAN_API_MODULEHELPERS_MT_INTERFACES_INTFMTCHARTS_H
