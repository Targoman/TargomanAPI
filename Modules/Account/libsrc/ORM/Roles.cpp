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

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void Roles::init()
{;}

QVariant Roles::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Account:Roles:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool Roles::apiDELETE(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH)
{
    Authorization::checkPriv(_JWT,{"Account:Roles:CRUD~0001"});
    return this->deleteByPKs(AAADACInstance(), {{this->Cols.first().Name, _EXTRAPATH}});
}

bool Roles::apiUPDATE(QHttp::JWT_t _JWT,
                      quint64 _rolID,
                      QString _name,
                      quint64 _parentRolID,
                      QHttp::JSON_t _privs,
                      QStringList _signupAllowdIPs,
                      Targoman::API::enuGenericStatus::Type _status)
{
    Authorization::checkPriv(_JWT,{"Account:Roles:CRUD~0010"});
    return this->update(AAADACInstance(),
                        {{"rolID", _rolID}},
                        {
                            {"rolName", _name},
                            {"rolParent_rolID", _parentRolID == 0 ? QVariant() : _parentRolID},
                            {"rolPrivileges", _privs},
                            {"rolSignupAllowedIPs", _signupAllowdIPs.join(",")},
                            {"rolUpdatedBy_usrID", clsJWT(_JWT).usrID()},
                            {"rolStatus", _status},
                        }
                        );
}

quint64 Roles::apiCREATE(QHttp::JWT_t _JWT,
                         QString _name,
                         quint64 _parentRolID,
                         QHttp::JSON_t _privs,
                         QStringList _signupAllowdIPs)
{
    Authorization::checkPriv(_JWT,{"Account:Roles:CRUD~1000"});
    return this->create(AAADACInstance(),
                        {
                            {"rolName", _name},
                            {"rolParent_rolID", _parentRolID},
                            {"rolPrivileges", _privs},
                            {"rolSignupAllowedIPs", _signupAllowdIPs.join(",")},
                        }
                        ).toUInt();
}


Roles::Roles() :
    clsTable("AAA",
              "tblRoles",
              { ///<ColName            Validation                           Sort  Filter RO   PK
                {"rolID",               QFV.integer().minValue(1),          true, true,  true, true},
                {"rolName",             QFV.unicodeAlNum().maxLenght(50)},
                {"rolParent_rolID",     QFV.integer().minValue(1)},
                {"rolPrivileges",       QFV.json(),                         false, false},
                {"rolSignupAllowedIPs", QFV.allwaysValid(),                 false, false}, //OJO This must be validated after splitting by comma
                {"rolCreatedBy_usrID",  QFV.integer().minValue(1),          true,  true, true},
                {"rolCreationDateTime", QFV.dateTime(),                     true,  true, true},
                {"rolUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"rolStatus",           QFV_Enum(enuGenericStatus)},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"rolParent_rolID",    "AAA.tblRoles",     "rolID",     "Parent_",  true},
                {"rolCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"rolUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}

}
}
}

