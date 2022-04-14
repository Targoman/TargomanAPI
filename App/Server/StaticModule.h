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
    TARGOMAN_DEFINE_API_MODULE(StaticModule);

private slots:
    TAPI::RawData_t REST_GET_OR_POST_ALIAS(
        openAPI_json,
        "openAPI.json",
        (
            APISession<false> &_SESSION
        ),
        ""
    );

    QVariant REST_GET_OR_POST_ALIAS(
        openAPI_yaml,
        "openAPI.yaml",
        (
            APISession<false> &_SESSION
        ),
        ""
    );

    //TAPI::ResponseRedirect_t
    //TAPI::FileData_t
    QVariant REST_GET_OR_POST(
        swaggerui,
        (
            APISession<false> &_SESSION
        ),
        ""
    );

    QVariant REST_GET_OR_POST_ALIAS(
        stats_json,
        "stats.json",
        (
            APISession<false> &_SESSION,
            bool _full = false
        ),
        ""
    );

    QVariant REST_GET_OR_POST(
        version,
        (
            APISession<false> &_SESSION
        ),
        ""
    );

    QVariant REST_GET_OR_POST(
        ping,
        (
            APISession<false> &_SESSION
        ),
        ""
    );
};

} //namespace Targoman::API::Server

#endif // ARGOMAN_API_SERVER_STATICMODULE_H
