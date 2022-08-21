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

#ifndef ARGOMAN_API_SERVER_STATICMODULE_H
#define ARGOMAN_API_SERVER_STATICMODULE_H

#include "Interfaces/API/intfPureModule.h"

using namespace Targoman::API::API;

namespace Targoman::API::Server {

class StaticModule : public intfPureModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(StaticModule, enuModuleActorType::User);
    //---------------------------------------------------------

private slots:
    TAPI::RawData_t EXREST_GET_OR_POST(
        openAPI_json,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM
        ),
        "",
        {
            EXRESTCONFIG_ALIAS("openAPI.json"),
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        openAPI_yaml,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM
        ),
        "",
        {
            EXRESTCONFIG_ALIAS("openAPI.yaml"),
            EXRESTCONFIG_HIDDEN,
        }
    );

    //TAPI::ResponseRedirect_t
    //TAPI::FileData_t
    QVariant EXREST_GET_OR_POST(
        swaggerui,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        stats_json,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            bool _full = false
        ),
        "",
        {
            EXRESTCONFIG_ALIAS("stats.json"),
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        version,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        ping,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );
};

} //namespace Targoman::API::Server

#endif // ARGOMAN_API_SERVER_STATICMODULE_H
