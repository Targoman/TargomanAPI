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

#ifndef TARGOMAN_API_SERVER_APICALLBOOM_H
#define TARGOMAN_API_SERVER_APICALLBOOM_H

#include "QString"
#include "QSharedDataPointer"
#include "QHttp/qhttpfwd.hpp"

namespace Targoman::API::Server {

class APICallBoomData;

class intfAPICallBoom
{
public:
    intfAPICallBoom();
    intfAPICallBoom(const intfAPICallBoom& _other);
    virtual ~intfAPICallBoom();

public:
    void initialize(
        const QString &_apiPath,
        const QJsonObject &_JWT,
        const QVariantMap &_headers,
        const QVariantMap &_cookies,
        const QString &_ip
    );

    virtual bool needJWT() = 0;
    void setJWT(/*TAPI::JWT_t*/QJsonObject &_JWT);
    /*TAPI::JWT_t*/QJsonObject &getJWT();
    quint64 getUserID();
    QJsonObject getJWTPrivsObject();

    QString getIP();

//    void setRequestAPIPath(const QString &_path);
    QString requestAPIPath() const;
    QString host() const;
    quint16 port() const;

    void setResponseHeaders(const QVariantMap &_headers);
    QVariantMap getResponseHeaders();
    void addResponseHeader(const QString &_header, const QVariant &_value, bool _multiValue=false);
    void addResponseHeaderNameToExpose(const QString &_header);

protected:
    QExplicitlySharedDataPointer<APICallBoomData> Data;
};

template <bool _needJWT>
class APICallBoom : public intfAPICallBoom
{
public:
    virtual bool needJWT() { return _needJWT; }
};

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVER_APICALLBOOM_H
