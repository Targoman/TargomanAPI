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

#include "Charts.h"
#include "../MT.h"

//using namespace Targoman::API::AAA;

namespace Targoman::API::MTModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(MT, Charts)

Charts::Charts() :
    intfMTCharts(
        MTDomain
) { ; }

QVariant IMPL_REST_GET(Charts, schema, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _key
)) {
    ///@TODO: read from tblConfigurations

    QString Schema = R"(
{
    title: 'Optional Overall title', //ex. Charts
    charts: [
        { // Any of the following keys
            type: progressBar,
            title: 'Optional title (default null)', //ex. Formal
            name: 'name of the field', // ex. Formal
            min: 'optional {{MIN_PLACE_HOLDER}} (default: min)',
            max: 'optional {{MAX_PLACE_HOLDER}} (default: max)',
            value: 'optional {{VALUE_PLACE_HOLDER}} (default: curr)',
            endpoint: 'API endpoint to fetch data', //optional when used in multiprogress
            props: {
                key: 'value' //Key values in order to customize chart. TBDL
            }
        },
        {
            type: multiprogress,
            title: 'Optional box title (default null)', //ex. Remaining
            charts: 'Optional array of at least one progressBar',
            endpoint: 'API endpoint to fetch data of data+specs',
            props: {
                key: 'value' //Key values in order to customize chart. TBDL
            }
        },
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

    return Schema;
}

} // namespace Targoman::API::MTModule::ORM
