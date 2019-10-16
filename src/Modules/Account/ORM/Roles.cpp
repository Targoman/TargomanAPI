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

#include "Roles.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void Roles::init()
{;}

QVariant Roles::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Roles::Roles() :
    intfTable("AAA",
              "tblRoles",
              "rol",
              { ///<ColName            Validation                           Sort  Filter AS  RO   PK
                {"rolID",               QFV.integer().minValue(1),          true, true, {},  true, true},
                {"rolName",             QFV.unicodeAlNum().maxLenght(50)},
                {"rolParent_rolID",     QFV.integer().minValue(1)},
                {"rolPrivileges",       QFV.json(),                         false, false},
                {"rolSignupAllowedIPs", QFV.allwaysValid(),                 false, false}, //OJO This must be validated after splitting by comma
                {"rolCreatedBy_usrID",  QFV.integer().minValue(1),          true,  true, "", true},
                {"rolCreationDateTime", QFV.dateTime(),                     true,  true, "", true},
                {"rolUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"rolStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuGenericStatus::options().join("|"))))},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"rolParent_rolID",    "AAA.tblRoles",     "rolID",     "Parent_",  true},
                {"rolCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"rolUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}


