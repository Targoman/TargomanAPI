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

#ifndef TARGOMAN_API_SERVER_CLSJWT_H
#define TARGOMAN_API_SERVER_CLSJWT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman::API::AAA {
class clsJWT;
}

namespace Targoman::API::Server {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuJWTHashAlgs,
                              HS256,
                              HS384,
                              HS512)
class QJWT
{
public:
    static inline QString makeConfig(const QString& _name) { return "/JWT/" + _name; }
    static Targoman::Common::Configuration::tmplConfigurable<QString>                 Secret;
    static Targoman::Common::Configuration::tmplConfigurable<enuJWTHashAlgs::Type>    HashAlgorithm;
    static Targoman::Common::Configuration::tmplConfigurable<quint16>                 TTL;
    static Targoman::Common::Configuration::tmplConfigurable<quint32>                 NormalLoginTTL;
    static Targoman::Common::Configuration::tmplConfigurable<quint32>                 RememberLoginTTL;
    static Targoman::Common::Configuration::tmplConfigurable<quint64>                 SimpleCryptKey;

    static QString createSigned(
        QJsonObject _payload,
        QJsonObject _privatePayload = QJsonObject(),
        const qint32 _expiry = -1,
        const QString &_sessionID = {},
        const QString &_remoteIP = {}
    );

    static QJsonObject verifyReturnPayload(
        /*INOUT*/ QString &_jwt,
        const QString &_remoteIP,
        bool _renewIfExpired = false
    );

private:
    static QByteArray hash(const QByteArray& _data);

    friend class Targoman::API::AAA::clsJWT;
};

} //namespace Targoman::API::Server

ENUM_CONFIGURABLE(Targoman::API::Server::enuJWTHashAlgs)

#endif // TARGOMAN_API_SERVER_CLSJWT_H
