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
 */


#ifndef TARGOMAN_API_SERVER_CLSJWT_H
#define TARGOMAN_API_SERVER_CLSJWT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman {
namespace API {
namespace AAA {
class clsJWT;
}
namespace Server {

TARGOMAN_DEFINE_ENHANCED_ENUM(enuJWTHashAlgs,
                              HS256,
                              HS384,
                              HS512)
class QJWT
{
    static inline QString makeConfig(const QString& _name){return "/JWT/" + _name;}
    static Common::Configuration::tmplConfigurable<QString>                 Secret;
    static Common::Configuration::tmplConfigurable<enuJWTHashAlgs::Type>    HashAlgorithm;
    static Common::Configuration::tmplConfigurable<quint16>                 TTL;
    static Common::Configuration::tmplConfigurable<quint32>                 NormalLoginTTL;
    static Common::Configuration::tmplConfigurable<quint32>                 RememberLoginTTL;
public:
    static Common::Configuration::tmplConfigurable<quint64>                 SimpleCryptKey;

public:
    static QString createSigned(QJsonObject _payload,
                                QJsonObject _privatePayload = QJsonObject(),
                                const qint32 _expiry = -1,
                                const QString& _sessionID = QString());
    static QJsonObject verifyReturnPayload(const QString& _jwt);

private:
    static QByteArray hash(const QByteArray& _data);

    friend class Targoman::API::AAA::clsJWT;
};

}
}
}

ENUM_CONFIGURABLE(Targoman::API::Server::enuJWTHashAlgs)

#endif // TARGOMAN_API_SERVER_CLSJWT_H
