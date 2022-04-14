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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "Roles.h"
#include "User.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuRoleStatus);

namespace Targoman::API::AccountModule::ORM {

Roles::Roles() :
    intfSQLBasedModule(
        AAASchema,
        tblRoles::Name,
        {///<ColName                          Type                   Validation                        Default    UpBy   Sort  Filter Self  Virt   PK
            { tblRoles::rolID,                ORM_PRIMARYKEY_32 },
            { tblRoles::rolName,              S(QString),            QFV/*.unicodeAlNum()*/.maxLenght(50), QRequired, UPAdmin },
            { tblRoles::rolParent_rolID,      S(quint32),            QFV.integer().minValue(1),        QNull,     UPAdmin },
            { tblRoles::rolPrivileges,        S(TAPI::PrivObject_t), QFV,                              QRequired, UPAdmin, false, false },
            { tblRoles::rolSignupAllowedIPs,  S(QString),            QFV,                              QNull,     UPAdmin, false, false }, //OJO This must be validated after splitting by comma
            { tblRoles::rolStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuRoleStatus, Targoman::API::AccountModule::enuRoleStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblRoles::rolCreationDateTime,  ORM_CREATED_ON },
            { tblRoles::rolCreatedBy_usrID,   ORM_CREATED_BY_NULLABLE },
            { tblRoles::rolUpdatedBy_usrID,   ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
            { tblRoles::rolParent_rolID, R(AAASchema,tblRoles::Name), tblRoles::rolID, "Parent_", true },
            ORM_RELATION_OF_CREATOR(tblRoles::rolCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblRoles::rolUpdatedBy_usrID),
        },
        {
            { {
                tblRoles::rolName,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    ) { ; }

QVariant Roles::apiGET(GET_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint32 Roles::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Roles::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Roles::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::AccountModule::ORM
