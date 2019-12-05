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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "UserWallets.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void UserWallets::init()
{;}

QVariant UserWallets::apiGET(GET_METHOD_ARGS_IMPL)
{
//    bool IsSelf = _EXTRAPATH.split(',').size() == 2 && clsJWT(_JWT).usrID() != _EXTRAPATH.split(',').last().toUInt();
//    if(_EXTRAPATH.isEmpty() || IsSelf == false)
//        Authorization::checkPriv(_JWT,{"Account:UserWallets:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {},
//                                 IsSelf ? "" : QString("+wal_usrID=%1").arg(clsJWT(_JWT).usrID()),
//                                 GET_METHOD_CALL_ARGS);
}

UserWallets::UserWallets() :
    clsTable("AAA",
              "tblUserWallets",
              { ///<ColName         Type                        Validation                          RO   Sort  Filter Self  Virt   PK
                {"walID",           S(quint64),                 QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"wal_usrID",       S(quint32),                 QFV.integer().minValue(1),          true},
                {"walName",         S(quint32),                 QFV.unicodeAlNum().maxLenght(100)},
                {"walMinBalance",   S(qint64),                  QFV,                               false,false,false},
                {"walLastBalance",  S(qint64),                  QFV,                                true,false,false},
                {"walStatus",       S(Targoman::API::enuUserWalletStatus::Type)},
              },
              { ///< Col       Reference Table             ForeignCol    Rename   LeftJoin
                {"wal_usrID",  "AAA.tblUser",              "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuUserWalletStatus);
    this->registerMyRESTAPIs();
}

}
}
}

