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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Roles.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant Roles::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool Roles::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool Roles::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 Roles::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

Roles::Roles() :
    clsTable(AAASchema,
              "tblRoles",
              { ///<ColName             Type                 Validation                          Default    RO   Sort  Filter Self  Virt   PK
                {"rolID",               S(quint32),          QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {"rolName",             S(QString),          QFV.unicodeAlNum().maxLenght(50),   QInvalid},
                {"rolParent_rolID",     S(quint32),          QFV.integer().minValue(1),          QNull},
                {"rolPrivileges",       S(TAPI::JSON_t),     QFV,                                QInvalid, false,false,false},
                {"rolSignupAllowedIPs", S(QString),          QFV,                                QNull,    false,false,false}, //OJO This must be validated after splitting by comma
                {"rolCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),          QInvalid, true},
                {"rolCreationDateTime", S(TAPI::DateTime_t), QFV,                                QNull,    true},
                {"rolUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),          QNull},
                {"rolStatus",           S(TAPI::enuRoleStatus::Type), QFV,                       TAPI::enuRoleStatus::Active},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"rolParent_rolID",    "AAA.tblRoles",     "rolID",     "Parent_",  true},
                {"rolCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"rolUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuRoleStatus);
}

}
}
}

