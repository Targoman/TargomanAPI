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
#include "ServerConfigs.h"

using namespace Targoman::API::API;

namespace Targoman::API::Server {

//class intfAPISession
//{
//public:
//    virtual QVariant toVariant() const = 0;
//    virtual bool needJWT() = 0;
//    virtual bool needIP() = 0;
//};

//template <bool _needJWT, bool _needIP>
//struct APISessionTypes
//{
//    typedef std::conditional<_needJWT, TAPI::JWT_t, QVariant> JWT_Type;
//    typedef std::conditional<_needIP, TAPI::RemoteIP_t, QVariant> IP_Type;
//};

//template <class T_APISessionTypes>
class APISession //: public intfAPISession
{
public:

    APISession() = default;
    virtual ~APISession() = default;
    APISession(const APISession &) = default;
    APISession &operator=(const APISession &) = default;

//    APISession(
//            QString &_requestAPIPath,
//            qhttp::THeaderHash &_requestHeaders,
//            qhttp::THeaderHash &_requestCookies,
//            typename T_APISessionTypes::JWT_Type &_JWT,
//            typename T_APISessionTypes::IP_Type &_IP,
//            qhttp::THeaderHash &_responseHeaders
//        ) :
//        RequestAPIPath(_requestAPIPath),
//        RequestHeaders(_requestHeaders),
//        RequestCookies(_requestCookies),
//        JWT(_JWT),
//        IP(_IP),
//        ResponseHeaders(_responseHeaders)
//    { ; }

public:
//    QVariant toVariant() const { return QVariant(); }
    virtual bool needJWT() { return false; }

    QString requestAPIPath() const
    {
        return RequestAPIPath;
    }
    QString host() const
    {
        if (RequestHeaders.has("host") == false)
            return "127.0.0.1";

        QString Host = RequestHeaders["host"];

        int idx;
        if ((idx = Host.indexOf(":")) >= 0)
            Host = Host.left(idx);

        return Host;
    }
    quint16 port() const
    {
        if (RequestHeaders.has("host") == false)
            return ServerConfigs::ListenPort.value();

        QString Host = RequestHeaders["host"];

        int idx;
        if ((idx = Host.indexOf(":")) < 0)
            return 80;

        return Host.mid(idx+1).toUInt();
    }

public:
//    clsRequestHandler *RequestHandler;
    ///@TODO: complete this
    QString RequestAPIPath;
    qhttp::THeaderHash RequestHeaders;
    qhttp::THeaderHash RequestCookies;
    TAPI::RemoteIP_t IP;
    qhttp::THeaderHash ResponseHeaders;
};

class APISession_JWT : public APISession
{
public:
    virtual bool needJWT() { return true; }

public:
    typename TAPI::JWT_t JWT;
};

//typedef APISession<APISessionTypes<false, false>> APISession;
//typedef APISession<APISessionTypes<false, true>> APISession_ip;
//typedef APISession<APISessionTypes<true, false>> APISession_JWT;
//typedef APISession<APISessionTypes<true, true>> APISession_JWT_ip;

class StaticModule : public intfPureModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_DEFINE_API_MODULE(StaticModule);

private slots:
    QVariant REST_GET_OR_POST_ALIAS(
        openAPI_json,
        "openAPI.json",
        (
            APISession_JWT &_SESSION,
            int i = 345
        ),
        ""
    );

    QVariant REST_GET_OR_POST_ALIAS(
        openAPI_yaml,
        "openAPI.yaml",
        (
            APISession &_SESSION
        ),
        ""
    );

    TAPI::FileData_t REST_GET_OR_POST(
        swaggerui,
        (
            APISession &_SESSION
        ),
        ""
    );

    QVariant REST_GET_OR_POST_ALIAS(
        stats_json,
        "stats.json",
        (
            APISession &_SESSION,
            bool _full = false
        ),
        ""
    );

    QVariant REST_GET_OR_POST(
        version,
        (
            APISession &_SESSION
        ),
        ""
    );

    QVariant REST_GET_OR_POST(
        ping,
        (
            APISession &_SESSION
        ),
        ""
    );
};

//typedef APISession APISession;
//typedef APISession<APISessionTypes<false, true>> APISession_ip;
//typedef APISession<APISessionTypes<true, false>> APISession_JWT;
//typedef APISession<APISessionTypes<true, true>> APISession_JWT_ip;

} //namespace Targoman::API::Server

//typedef Targoman::API::Server::APISession<false, false> APISession_f_f;

//Q_DECLARE_METATYPE_TEMPLATE_1ARG(Targoman::API::Server::APISession)

//TAPI_DECLARE_METATYPE(Targoman::API::Server::APISession);
//TAPI_DECLARE_METATYPE(Targoman::API::Server::APISession_ip);
//TAPI_DECLARE_METATYPE(Targoman::API::Server::APISession_JWT);
//TAPI_DECLARE_METATYPE(Targoman::API::Server::APISession_JWT_ip);

#endif // ARGOMAN_API_SERVER_STATICMODULE_H
