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
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuRoleStatus);

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
              tblRoles::Name,
              { ///<ColName                      Type                 Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblRoles::rolID,                S(quint32),          QFV.integer().minValue(1),          ORM_PRIMARY_KEY},
                {tblRoles::rolName,              S(QString),          QFV.unicodeAlNum().maxLenght(50),   QInvalid, UPAdmin},
                {tblRoles::rolParent_rolID,      S(quint32),          QFV.integer().minValue(1),          QNull,    UPAdmin},
                {tblRoles::rolPrivileges,        S(TAPI::JSON_t),     QFV,                                QInvalid, UPAdmin,false,false},
                {tblRoles::rolSignUPOwnerowedIPs,S(QString),          QFV,                                QNull,    UPAdmin,false,false}, //OJO This must be validated after splitting by comma
                {tblRoles::rolCreatedBy_usrID,   ORM_CREATED_BY},
                {tblRoles::rolCreationDateTime,  ORM_CREATED_ON},
                {tblRoles::rolUpdatedBy_usrID,   ORM_UPDATED_BY},
                {tblRoles::rolStatus,            S(TAPI::enuRoleStatus::Type), QFV,                       TAPI::enuRoleStatus::Active, UPAdmin},
              },
              { ///< Col                       Reference Table                ForeignCol        Rename      LeftJoin
                {tblRoles::rolParent_rolID,    R(AAASchema,tblRoles::Name),   tblRoles::rolID,  "Parent_",  true},
                {tblRoles::rolCreatedBy_usrID, ORM_JOIN_CREATOR},
                {tblRoles::rolUpdatedBy_usrID, ORM_JOIN_UPDATER},
              })
{
}

}
}
}

