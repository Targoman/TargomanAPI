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

#ifndef TARGOMAN_API_SERVER_INTFCACHECONNECTOR_HPP
#define TARGOMAN_API_SERVER_INTFCACHECONNECTOR_HPP

#include <QUrl>
#include <QVariant>
#include "libTargomanCommon/exTargomanBase.h"

#include "ServerConfigs.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"

namespace Targoman {
namespace API {
namespace Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exCacheConnector, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop

class intfCacheConnector{
public:
    intfCacheConnector(const QUrl& _connector) :
        ConnectorURL(_connector)
    { ; }
    virtual ~intfCacheConnector();

    virtual void connect() = 0;
    void setKeyVal(const QString& _key, const QVariant& _value, qint32 _ttl);

    QVariant getValue(const QString& _key) {
        return this->getValueImpl (_key);
    }

private:
    virtual void setKeyValImpl(const QString& _key, const QString& _value, qint32 _ttl) = 0;
    virtual QString getValueImpl(const QString& _key) = 0;

protected:
    QUrl ConnectorURL;
};


}
}
}

#endif // TARGOMAN_API_SERVER_INTFCACHECONNECTOR_HPP
