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


#ifndef TARGOMAN_API_SERVER_CLSREDISCONNECTOR_H
#define TARGOMAN_API_SERVER_CLSREDISCONNECTOR_H

#ifdef TARGOMAN_API_ENABLE_REDIS_PROTOCOL

#include "intfCacheConnector.hpp"

class redisContext;

namespace Targoman {
namespace API {
namespace Server {

class clsRedisConnector : public intfCacheConnector {
public:
    clsRedisConnector(const QUrl& _connector);

    void connect();
    bool reconnect();
    void setKeyValImpl(const QString& _key, const QString& _value, qint32 _ttl);
    QString getValueImpl(const QString& _key);

private:
    QScopedPointer<redisContext> Connection;
};

}
}
}
#endif //TARGOMAN_API_SERVER_REDIS_PROTOCOL

#endif // TARGOMAN_API_SERVER_CLSREDISCONNECTOR_H
