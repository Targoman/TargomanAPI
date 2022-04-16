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

#include "StaticModule.h"
#include "ServerConfigs.h"
#include "OpenAPIGenerator.h"
//#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace Targoman::API::Server {

TARGOMAN_IMPL_API_MODULE(StaticModule)

StaticModule::StaticModule() :
    intfPureModule("")
{ ; }

TAPI::RawData_t IMPL_REST_GET_OR_POST(StaticModule, openAPI_json, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM
)) {
    gServerStats.Success.inc();

    return TAPI::RawData_t(
                QJsonDocument(OpenAPIGenerator::retrieveJson(
                    _APICALLBOOM.host(),
                    _APICALLBOOM.port()
                )).toJson(QJsonDocument::Compact),
                "application/json; charset=utf-8"
                );
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, openAPI_yaml, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM
)) {
    throw exHTTPMethodNotAllowed("Yaml openAPI is not implemented yet");
}

//TAPI::ResponseRedirect_t
//TAPI::FileData_t
QVariant IMPL_REST_GET_OR_POST(StaticModule, swaggerui, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM
)) {
    if (ServerConfigs::SwaggerUI.value().isEmpty())
        throw exHTTPNotFound("Swagger is not configured");

    QString API = _APICALLBOOM.requestAPIPath();

    QString File = API.mid(sizeof("/swaggerUI") - 1).replace(QRegularExpression("//+"), "/");

    if (File.isEmpty())
        return TAPI::ResponseRedirect_t(API + "/").toVariant();

    if (File == "/")
        File = "index.html";

    return TAPI::FileData_t(ServerConfigs::SwaggerUI.value() + "/" + File).toVariant();
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, stats_json, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    bool _full
)) {
    gServerStats.Success.inc();

    return gServerStats.toJson(_full);
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, version, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM
)) {
    gServerStats.Success.inc();

    QJsonObject Version;

    Version.insert("version", TARGOMAN_M2STR(PROJ_VERSION));

#ifdef GIT_VERSION
    Version.insert("git-tag", TARGOMAN_M2STR(GIT_VERSION));
#endif

    return Version;
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, ping, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM
)) {
    gServerStats.Success.inc();

    return QJsonObject({
                           { "timestamp", QDateTime::currentDateTime().toMSecsSinceEpoch() },
                       });
}

} //namespace Targoman::API::Server
