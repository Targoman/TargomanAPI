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

#ifdef TARGOMAN_API_REDIS_PROTOCOL

#include "clsRedisConnector.h"
#include "libTargomanCommon/Logger.h"

extern "C" {
    #include "hiredis/hiredis.h"
}

namespace Targoman {
namespace API {
namespace Server {

clsRedisConnector::clsRedisConnector(const QUrl& _connector) :
    intfCacheConnector(_connector)
{ ; }

void clsRedisConnector::connect() {
    struct timeval Timeout = { 1, 500000 }; // 1.5 seconds
    if (this->ConnectorURL.port() == 1)
        this->Connection.reset(redisConnectUnixWithTimeout(this->ConnectorURL.host().toLatin1().constData(), Timeout));
    else
        this->Connection.reset(redisConnectWithTimeout(this->ConnectorURL.host().toLatin1().constData(), this->ConnectorURL.port(), Timeout));

    if (this->Connection.isNull() || this->Connection->err)
        throw exCacheConnector(this->Connection.isNull() ? "can't allocate Redis context" : this->Connection->errstr);

    redisSetTimeout(this->Connection.data(), Timeout);
}

bool clsRedisConnector::reconnect() {
    try {
        if (this->Connection.isNull()) {
            this->connect();
        } else
            return redisReconnect(this->Connection.data());
    } catch (exCacheConnector& ex) {
        TargomanLogWarn(1, ex.what());
        return false;
    }
    return true;
}

void clsRedisConnector::setKeyValImpl(const QString& _key, const QString& _value, qint32 _ttl) {
    if (this->Connection.isNull() || this->Connection->err)
        this->reconnect();

    if (!redisCommand(this->Connection.data(), "SETEX %s %d %s", qPrintable(_key), _ttl, _value.toUtf8().constData()))
        TargomanWarn(1, this->Connection->errstr);
}

QString clsRedisConnector::getValueImpl(const QString& _key) {
    void *Reply = nullptr;

    Reply = redisCommand(this->Connection.data(), "GET %s", qPrintable(_key)); // You must define "context" previously
    if (!Reply) {
        TargomanWarn(1, this->Connection->errstr);
        return "";
    }

    QString Result = QString((static_cast<redisReply*>(Reply))->str);
    freeReplyObject(Reply);
    return Result;
}

}
}
}
#endif
