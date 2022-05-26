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

#include "Departments.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"

using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule::ORM {

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
Departments::Departments() :
    intfSQLBasedModule(
        TicketingSchema,
        tblDepartments::Name,
        {///< ColName                               Type                    Validation      Default     UpBy   Sort  Filter Self  Virt   PK
            { tblDepartments::depID,                ORM_PRIMARYKEY_32 },
            { tblDepartments::depName,              S(QString),             QFV,            QRequired,  UPNone },
            { tblDepartments::depCreationDateTime,  ORM_CREATED_ON },
            { tblDepartments::depCreatedBy_usrID,   ORM_CREATED_BY },
        },
        {///< Col                                   Reference Table         ForeignCol      Rename      LeftJoin
            ORM_RELATION_OF_CREATOR(tblDepartments::depCreatedBy_usrID),
        }
    ) { ; }

QVariant IMPL_ORMGET(Departments) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(Departments) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(Departments) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(Departments) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::TicketingModule::ORM
