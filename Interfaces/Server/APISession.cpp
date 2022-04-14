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

#include "APISession.h"
#include "ServerCommon.h"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/AAA/clsJWT.hpp"
//#include "App/Server/clsRequestHandler.h"

namespace Targoman::API::Server {

class clsRequestHandler;

class APISessionData : public QSharedData
{
public:
    APISessionData() : QSharedData() { ; }

    APISessionData(const APISessionData &_other) :
        QSharedData(_other),
//        RequestHandler(_other.RequestHandler),
        RequestAPIPath(_other.RequestAPIPath),
        RequestHeaders(_other.RequestHeaders),
        RequestCookies(_other.RequestCookies),
        JWT(_other.JWT),
        IP(_other.IP),
        ResponseHeaders(_other.ResponseHeaders)
    { ; }

    virtual ~APISessionData() { ; }

public:
//    clsRequestHandler *RequestHandler;
    QString RequestAPIPath;
    QVariantMap RequestHeaders;
    QVariantMap RequestCookies;
    TAPI::JWT_t JWT;
    TAPI::RemoteIP_t IP;
    QVariantMap ResponseHeaders;
};

intfAPISession::intfAPISession() : Data(new APISessionData) { ; }
intfAPISession::intfAPISession(const intfAPISession& _other) : Data(_other.Data) { ; }
intfAPISession::~intfAPISession() { ; }

void intfAPISession::initialize(
    const QString &_apiPath,
    const QJsonObject &_JWT,
    const QVariantMap &_headers,
    const QVariantMap &_cookies,
    const QString &_ip
) {
    this->Data->RequestAPIPath = _apiPath;
    this->Data->RequestHeaders = _headers;
    this->Data->RequestCookies = _cookies;
    this->Data->JWT = _JWT;
    this->Data->IP = _ip;
}

void intfAPISession::setJWT(/*TAPI::JWT_t*/QJsonObject &_JWT) {
    this->Data->JWT = _JWT;
}
/*TAPI::JWT_t*/QJsonObject &intfAPISession::getJWT() {
    return this->Data->JWT;
}
quint64 intfAPISession::getUserID() {
    return Targoman::API::AAA::clsJWT(this->Data->JWT).usrID();
}
QJsonObject intfAPISession::getJWTPrivsObject() {
    return Targoman::API::AAA::clsJWT(this->Data->JWT).privsObject();
}

QString intfAPISession::getIP() {
    return this->Data->IP;
}

//void intfAPISession::setRequestAPIPath(const QString &_path) {
//    this->Data->RequestAPIPath = _path;
//}
QString intfAPISession::requestAPIPath() const {
    return this->Data->RequestAPIPath;
}

QString intfAPISession::host() const {
    if (this->Data->RequestHeaders.contains("host") == false)
        return "127.0.0.1";

    QString Host = this->Data->RequestHeaders["host"].toString();

    int idx;
    if ((idx = Host.indexOf(":")) >= 0)
        Host = Host.left(idx);

    return Host;
}

quint16 intfAPISession::port() const {
    if (this->Data->RequestHeaders.contains("host") == false)
        return ServerCommonConfigs::ListenPort.value();

    QString Host = this->Data->RequestHeaders["host"].toString();

    int idx;
    if ((idx = Host.indexOf(":")) < 0)
        return 80;

    return Host.mid(idx+1).toUInt();
}

//void intfAPISession::setResponseHeaders(const QVariantMap &_headers) {
//    this->Data->ResponseHeaders.clear();

//    for (QVariantMap::const_iterator it=_headers.constBegin(); it!=_headers.constEnd(); it++) {
//        this->Data->ResponseHeaders.insert(it.key().toLatin1(), it->toByteArray());
//    }
//}
void intfAPISession::setResponseHeaders(const QVariantMap &_headers) {
    this->Data->ResponseHeaders = _headers;
}
QVariantMap intfAPISession::getResponseHeaders() {
    return this->Data->ResponseHeaders;
}
void intfAPISession::addResponseHeader(const QString &_header, const QVariant &_value, bool _multiValue) {
    if (_multiValue && this->Data->ResponseHeaders.contains(_header))
        this->Data->ResponseHeaders[_header] = this->Data->ResponseHeaders[_header].toString() + _value.toString();
    else
        this->Data->ResponseHeaders.insert(_header, _value.toString());
}
void intfAPISession::addResponseHeaderNameToExpose(const QString &_header) {
    this->addResponseHeader("Access-Control-Expose-Headers", _header, true);
}

template class APISession<false>;
template class APISession<true>;

} //namespace Targoman::API::Server
