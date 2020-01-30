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

#include "intfAPIArgManipulator.h"

#include "Server/QJWT.h"
#include "Server/RESTAPIRegistry.h"
#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"
#include "Interfaces/ORM/clsORMField.h"

namespace Targoman {
namespace API {

using namespace Server;

/*void intfAPIModule::registerMyRESTAPIs(){
    for (int i=0; i<this->metaObject()->methodCount(); ++i)
        RESTAPIRegistry::registerRESTAPI(this, this->metaObject()->method(i));
}

TAPI::EncodedJWT_t intfAPIModule::createSignedJWT(QJsonObject _payload, QJsonObject _privatePayload, const qint32 _expiry, const QString& _sessionID) {
    return QJWT::createSigned(_payload, _privatePayload, _expiry, _sessionID);
}
*/
intfAPIArgManipulator::intfAPIArgManipulator(const QString& _realTypeName) {
    this->PrettyTypeName = (_realTypeName.startsWith('Q') ? _realTypeName.mid(1) : _realTypeName).toLower();
    QByteArray RealTypeByteArray = _realTypeName.toLatin1();
    this->RealTypeName = new char[static_cast<uint>(RealTypeByteArray.size()+1)];
    strncpy(this->RealTypeName,
            _realTypeName.toLatin1().constData(),
            static_cast<uint>(RealTypeByteArray.size()));
    this->RealTypeName[RealTypeByteArray.size()] = 0;
}

intfAPIArgManipulator::~intfAPIArgManipulator()
{;}

intfAPIObject::~intfAPIObject()
{;}

}
}
