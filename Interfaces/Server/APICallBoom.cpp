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

#include "APICallBoom.h"
#include "ServerCommon.h"
#include "Interfaces/AAA/clsJWT.hpp"
//#include "App/Server/clsRequestHandler.h"
#include "Interfaces/Common/GenericTypes.h"
//#include "Interfaces/Common/tmplAPIArg.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"

//using namespace TAPI;
//using namespace Targoman::API;
using namespace Targoman::API::Common;

//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuTokenActorType::Type);

namespace Targoman::API::Server {

class clsRequestHandler;

class APICallBoomData : public QSharedData
{
public:
    APICallBoomData(std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> _fnTiming) :
        QSharedData(),
        FNTiming(_fnTiming),
        AcceptLanguage("en")
    { ; }

    APICallBoomData(const APICallBoomData &_other) :
        QSharedData(_other),
//        RequestHandler(_other.RequestHandler),
        FNTiming(_other.FNTiming),
        RequestAPIPath(_other.RequestAPIPath),
        RequestHeaders(_other.RequestHeaders),
        RequestCookies(_other.RequestCookies),
        JWT(_other.JWT),
        IP(_other.IP),
        RequestQueryParams(_other.RequestQueryParams),
        RequestBodyParams(_other.RequestBodyParams),
        ResponseHeaders(_other.ResponseHeaders),
        AcceptLanguage(_other.AcceptLanguage)
    { ; }

    virtual ~APICallBoomData() { ; }

public:
//    clsRequestHandler *RequestHandler;
    std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> FNTiming;

    QString RequestAPIPath;
    QVariantMap RequestHeaders;
    QVariantMap RequestCookies;
    TAPI::JWT_t JWT;
    TAPI::RemoteIP_t IP;
    QList<QPair<QString, QString>> RequestQueryParams;
    QList<QPair<QString, QString>> RequestBodyParams;

    QVariantMap ResponseHeaders;
    QString AcceptLanguage;
};

intfAPICallBoom::intfAPICallBoom(std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> _fnTiming) :
    Data(new APICallBoomData(_fnTiming)) { ; }

intfAPICallBoom::intfAPICallBoom(const intfAPICallBoom& _other) :
    Data(_other.Data) { ; }

intfAPICallBoom::~intfAPICallBoom() { ; }

void intfAPICallBoom::initialize(
    const QString &_apiPath,
    const QJsonObject &_JWT,
    const QVariantMap &_headers,
    const QVariantMap &_cookies,
    const QString &_ip,
    const QStringList &_requestQueryParams,
    const QList<QPair<QString, QString>> &_requestBodyParams
) {
    this->Data->RequestAPIPath = _apiPath;
    this->Data->RequestHeaders = _headers;
    this->Data->RequestCookies = _cookies;
    this->Data->JWT = _JWT;
    this->Data->IP = _ip;

    foreach (auto Item, _requestQueryParams) {
        QStringList Parts = Item.split('=');
        QString Name = Parts.at(0);

        QString Value;
        if (Parts.length() > 1) {
            Parts.removeAt(0);
            Value = QUrl::fromPercentEncoding(Parts.join('=').toUtf8());
        }

        this->Data->RequestQueryParams.append({ Name, Value });
    }
    this->Data->RequestBodyParams = _requestBodyParams;

    this->Data->AcceptLanguage = "en";

    if (this->Data->RequestHeaders.contains("accept-language")) {
        QStringList Languages = this->Data->RequestHeaders["accept-language"].toString().split(',');
        this->Data->AcceptLanguage = Languages.at(0).split(';').at(0);
    }
}

void intfAPICallBoom::setJWT(/*TAPI::JWT_t*/QJsonObject &_JWT) {
    this->Data->JWT = _JWT;
}

/*TAPI::JWT_t*/QJsonObject &intfAPICallBoom::getJWT() {
    return this->Data->JWT;
}

bool intfAPICallBoom::isAnonymouse() {
    return this->Data->JWT.isEmpty();
}
TAPI::enuTokenActorType::Type intfAPICallBoom::jwtActorType() {
    if (this->isAnonymouse())
        return TAPI::enuTokenActorType::ANONYMOUSE;

    return Targoman::API::AAA::clsJWT(this->Data->JWT).actorType();
}

quint64 intfAPICallBoom::getActorID(quint64 _default) {
    quint64 ActorID = Targoman::API::AAA::clsJWT(this->Data->JWT).actorID();
    if (ActorID == 0)
        ActorID = _default;
    return ActorID;
}
QJsonObject intfAPICallBoom::getJWTPrivsObject() {
    return Targoman::API::AAA::clsJWT(this->Data->JWT).privsObject();
}

QString intfAPICallBoom::getIP() {
    return this->Data->IP;
}

QList<QPair<QString, QString>> intfAPICallBoom::getRequestQueryParams() {
    return this->Data->RequestQueryParams;
}

QList<QPair<QString, QString>> intfAPICallBoom::getRequestBodyParams() {
    return this->Data->RequestBodyParams;
}

//void intfAPICallBoom::setRequestAPIPath(const QString &_path) {
//    this->Data->RequestAPIPath = _path;
//}
QString intfAPICallBoom::requestAPIPath() const {
    return this->Data->RequestAPIPath;
}

const QVariantMap intfAPICallBoom::requestHeaders() const {
    return this->Data->RequestHeaders;
}

QVariant intfAPICallBoom::requestHeader(const QString &_name, const QVariant &_default) const {
    for (auto It = this->Data->RequestHeaders.constBegin();
         It != this->Data->RequestHeaders.constEnd();
         It++
    ) {
        if (It.key().compare(_name, Qt::CaseInsensitive) == 0)
            return It.value();
    }

    return _default;
}

QString intfAPICallBoom::host(const QString &_default) const {
    if (this->Data->RequestHeaders.contains("host") == false)
        return _default;

    QString Host = this->Data->RequestHeaders["host"].toString();

    int idx;
    if ((idx = Host.indexOf(":")) >= 0)
        Host = Host.left(idx);

    return Host;
}

quint16 intfAPICallBoom::port() const {
    if (this->Data->RequestHeaders.contains("host") == false)
        return ServerCommonConfigs::ListenPort.value();

    QString Host = this->Data->RequestHeaders["host"].toString();

    int idx;
    if ((idx = Host.indexOf(":")) < 0)
        return 80;

    return Host.mid(idx+1).toUInt();
}

QString intfAPICallBoom::hostAndPort(const QString &_defaultHost) const {
    QString Host = this->host(_defaultHost);
    quint16 Port = this->port();

    if (Host.startsWith('/')) Host = Host.mid(1);
    if (Host.endsWith('/')) Host = Host.chopped(1);

    QString HostPort;
    if ((Port == 80) || (Port == 443))
        HostPort = Host;
    else
        HostPort = QString("%1:%2").arg(Host).arg(Port);

    return HostPort;
}

QString intfAPICallBoom::language() const {
    return this->Data->AcceptLanguage;
}

//void intfAPICallBoom::setResponseHeaders(const QVariantMap &_headers) {
//    this->Data->ResponseHeaders.clear();

//    for (QVariantMap::const_iterator it=_headers.constBegin(); it!=_headers.constEnd(); it++) {
//        this->Data->ResponseHeaders.insert(it.key().toLatin1(), it->toByteArray());
//    }
//}
void intfAPICallBoom::setResponseHeaders(const QVariantMap &_headers) {
    this->Data->ResponseHeaders = _headers;
}
QVariantMap intfAPICallBoom::getResponseHeaders() {
    return this->Data->ResponseHeaders;
}
void intfAPICallBoom::addResponseHeader(const QString &_header, const QVariant &_value, bool _multiValue) {
    if (_multiValue && this->Data->ResponseHeaders.contains(_header))
        this->Data->ResponseHeaders[_header] = this->Data->ResponseHeaders[_header].toString() + "," + _value.toString();
    else
        this->Data->ResponseHeaders.insert(_header, _value.toString());
}
void intfAPICallBoom::addResponseHeaderNameToExpose(const QString &_header) {
    this->addResponseHeader("Access-Control-Expose-Headers", _header, true);
}

void intfAPICallBoom::addToTimings(const QString &_name, const QString &_desc, quint64 _nanoSecs) {
    this->Data->FNTiming(_name, _desc, _nanoSecs);
}

//--------------------------------------------------------------------
template class APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL;
template class APICALLBOOM_TYPE_JWT_USER_DECL;
template class APICALLBOOM_TYPE_JWT_API_DECL;

template class APICALLBOOM_TYPE_JWT_ANONYMOUSE_OR_USER_DECL;
template class APICALLBOOM_TYPE_JWT_ANONYMOUSE_OR_API_DECL;

} //namespace Targoman::API::Server
