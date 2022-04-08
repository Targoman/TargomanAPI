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

#include "intfAPIArgManipulator.h"

namespace Targoman::API::Common {

intfAPIObject::~intfAPIObject() { ; }

intfAPIArgManipulator::intfAPIArgManipulator(const QString& _realTypeName) {
    Q_ASSERT_X(_realTypeName.startsWith("QSharedPointer") == false, "intfAPIArgManipulator::ctor()", "aah!! ooh!!");
    this->PrettyTypeName = _realTypeName.startsWith(TAPI_HELEPER_QSP_M2STR_PREFIX) ? _realTypeName.mid(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX) - 1, _realTypeName.size() - static_cast<int>(sizeof(TAPI_HELEPER_QSP_M2STR_PREFIX))) : _realTypeName;
    this->PrettyTypeName = (this->PrettyTypeName.startsWith('Q') ? this->PrettyTypeName.mid(1) : this->PrettyTypeName).toLower();
    QByteArray RealTypeByteArray = _realTypeName.toLatin1();
    this->RealTypeName = new char[static_cast<uint>(RealTypeByteArray.size()+1)];
    strncpy(this->RealTypeName,
            RealTypeByteArray.constData(),
            static_cast<uint>(RealTypeByteArray.size()+1));
    this->RealTypeName[RealTypeByteArray.size()] = 0;
}

intfAPIArgManipulator::~intfAPIArgManipulator() { ; }

} //namespace Targoman::API::Common
