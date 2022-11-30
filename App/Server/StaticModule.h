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
    TARGOMAN_API_MODULE_DEFINE(StaticModule); //, enuTokenActorType::USER);
    //---------------------------------------------------------

private slots:
    TAPI::RawData_t EXREST_GET_OR_POST(
        openAPI_json,
        (
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext,
            const QString &_module = {}
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
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext
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
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        stats_json,
        (
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext,
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
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        ping,
        (
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
        pinguser,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL &_apiCallContext
        ),
        "",
        {
            EXRESTCONFIG_HIDDEN,
        }
    );

    QVariant EXREST_GET_OR_POST(
         pingapi,
         (
             APICALLCONTEXT_TYPE_JWT_API_DECL &_apiCallContext
         ),
         "",
         {
             EXRESTCONFIG_HIDDEN,
         }
     );
};

} //namespace Targoman::API::Server

#endif // ARGOMAN_API_SERVER_STATICMODULE_H
