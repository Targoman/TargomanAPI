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
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/AAA/clsJWT.hpp"
//#include "App/Server/clsRequestHandler.h"

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
    QVariantMap ResponseHeaders;
    QString AcceptLanguage;
};

intfAPICallBoom::intfAPICallBoom(std::function<void(const QString &_name, const QString &_desc, quint64 _nanoSecs)> _fnTiming) : Data(new APICallBoomData(_fnTiming)) { ; }
intfAPICallBoom::intfAPICallBoom(const intfAPICallBoom& _other) : Data(_other.Data) { ; }
intfAPICallBoom::~intfAPICallBoom() { ; }

void intfAPICallBoom::initialize(
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
quint64 intfAPICallBoom::getUserID() {
    return Targoman::API::AAA::clsJWT(this->Data->JWT).usrID();
}
QJsonObject intfAPICallBoom::getJWTPrivsObject() {
    return Targoman::API::AAA::clsJWT(this->Data->JWT).privsObject();
}

QString intfAPICallBoom::getIP() {
    return this->Data->IP;
}

//void intfAPICallBoom::setRequestAPIPath(const QString &_path) {
//    this->Data->RequestAPIPath = _path;
//}
QString intfAPICallBoom::requestAPIPath() const {
    return this->Data->RequestAPIPath;
}

QString intfAPICallBoom::host() const {
    if (this->Data->RequestHeaders.contains("host") == false)
        return "127.0.0.1";

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

template class APICALLBOOM_TYPE_NO_JWT_DECL;
template class APICALLBOOM_TYPE_JWT_DECL;

} //namespace Targoman::API::Server
