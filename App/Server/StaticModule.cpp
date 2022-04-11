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
#include "Interfaces/Common/intfAPIArgManipulator.h"

//using namespace Targoman::API::Server;

//TAPI_REGISTER_METATYPE(
//    /* complexity         */ COMPLEXITY_Complex,
//    /* namespace          */ Targoman::API::Server,
//    /* type               */ APISession,
//    /* toVariantLambda    */ [](const APISession& _value) -> QVariant { return _value.toVariant(); }
//    /* fromVariantLambda  */ // [](const QVariant& _value, const QByteArray&) -> APISession<APISessionTypes<> { APISession<APISessionTypes<> Value; Value=_value.toString(); return Value; }
//);

namespace Targoman::API::Server {

//template class APISession<APISessionTypes<false, false>>;
//template class APISession<APISessionTypes<false, true>>;
//template class APISession<APISessionTypes<true, false>>;
//template class APISession<APISessionTypes<true, true>>;

TARGOMAN_IMPL_API_MODULE(StaticModule)

StaticModule::StaticModule() :
    intfPureModule("")
{ ; }

QVariant IMPL_REST_GET_OR_POST(StaticModule, openAPI_json, (
    APISession_JWT &_SESSION,
    int i
)) {
    gServerStats.Success.inc();

    return OpenAPIGenerator::retrieveJson(
                _SESSION.host(),
                _SESSION.port()
                );
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, openAPI_yaml, (
    APISession &_SESSION
)) {
    Q_UNUSED(_SESSION)

    throw exHTTPMethodNotAllowed("Yaml openAPI is not implemented yet");
}

TAPI::FileData_t IMPL_REST_GET_OR_POST(StaticModule, swaggerui, (
        APISession &_SESSION
)) {
    if (ServerConfigs::SwaggerUI.value().isEmpty())
        throw exHTTPNotFound("Swagger is not configured");

    QString File = _SESSION.requestAPIPath().mid(sizeof("/swaggerUI") - 1).replace(QRegularExpression("//+"), "/");

    if (File.isEmpty() || (File == "/"))
        File = "index.html";

    return TAPI::FileData_t(ServerConfigs::SwaggerUI.value() + "/" + File);
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, stats_json, (
    APISession &_SESSION,
    bool _full
)) {
    Q_UNUSED(_SESSION)

    gServerStats.Success.inc();

    return gServerStats.toJson(_full);
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, version, (
    APISession &_SESSION
)) {
    Q_UNUSED(_SESSION)

    gServerStats.Success.inc();

    QJsonObject Version;

    Version.insert("version", TARGOMAN_M2STR(PROJ_VERSION));

#ifdef GIT_VERSION
    Version.insert("git-tag", TARGOMAN_M2STR(GIT_VERSION));
#endif

    return Version;
}

QVariant IMPL_REST_GET_OR_POST(StaticModule, ping, (
    APISession &_SESSION
)) {
    Q_UNUSED(_SESSION)

    gServerStats.Success.inc();

    return QJsonObject({
                           { "timestamp", QDateTime::currentDateTime().toMSecsSinceEpoch() },
                       });
}

} //namespace Targoman::API::Server
