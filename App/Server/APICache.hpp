/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#ifndef TARGOMAN_API_SERVER_APIRESULTCACHE_H
#define TARGOMAN_API_SERVER_APIRESULTCACHE_H

#include <QObject>
#include <QTime>
#include <QString>
#include <QVariant>
#include <QMutexLocker>
#include <QHash>

#include "intfCacheConnector.hpp"

namespace Targoman {
namespace API {
namespace Server {

struct stuCacheValue {
    QTime InsertionTime;
    QVariant Value;
    qint32   TTL;

    stuCacheValue(){}
    stuCacheValue(const QVariant& _value, qint32 _ttl) : InsertionTime(QTime::currentTime()),Value(_value), TTL(_ttl){}
    stuCacheValue(const stuCacheValue& _other) : InsertionTime(_other.InsertionTime),Value(_other.Value), TTL(_other.TTL){}
};
typedef QHash<QString, stuCacheValue> Cache_t;

class InternalCache
{
public:
    static void setValue(const QString& _key, const QVariant& _value, qint32 _ttl){
        QMutexLocker Locker(&InternalCache::Lock);
        if(InternalCache::Cache.size() < static_cast<qint32>(ServerConfigs::MaxCachedItems.value()))
           InternalCache::Cache.insert(_key, stuCacheValue(_value, _ttl));
    }
    static QVariant storedValue(const QString& _key){
        QMutexLocker Locker(&InternalCache::Lock);
        auto StoredValue = InternalCache::Cache.find(_key);
        if(StoredValue == InternalCache::Cache.end())
            return QVariant();
        Locker.unlock();
        if(StoredValue->InsertionTime.secsTo(QTime::currentTime()) > StoredValue->TTL)
            return QVariant();
        return StoredValue->Value;
    }

public:
    static Cache_t Cache;
    static QMutex  Lock;

    friend class clsUpdateAndPruneThread;
};

class CentralCache
{
public:
    static bool isValid(){return CentralCache::Connector.isNull() == false;}
    static void setup(intfCacheConnector* _connector){ CentralCache::Connector.reset(_connector); }
    static void setValue(const QString& _key, const QVariant& _value, qint32 _ttl){
        if(CentralCache::Connector.isNull() == false)
            CentralCache::Connector->setKeyVal(_key, _value, _ttl);
    }
    static QVariant storedValue(const QString& _key){
        return CentralCache::Connector.isNull() ? QVariant() : CentralCache::Connector->getValue(_key);
    }

private:
    static QScopedPointer<intfCacheConnector> Connector;
};

}
}
}
#endif // CLSAPIRESULTCACHE_H
