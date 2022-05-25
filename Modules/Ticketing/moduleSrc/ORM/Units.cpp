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

#include "Units.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Departments.h"

using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule::ORM {

/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
Units::Units() :
    intfSQLBasedModule(
        TicketingSchema,
        tblUnits::Name,
        {///< ColName                           Type                    Validation                  Default     UpBy   Sort  Filter Self  Virt   PK
            { tblUnits::untID,                  ORM_PRIMARYKEY_32 },
            { tblUnits::unt_depID,              S(quint32),             QFV.integer().minValue(1),  QRequired,  UPNone },
            { tblUnits::untName,                S(QString),             QFV,                        QRequired,  UPNone },
            { tblUnits::untCreationDateTime,    ORM_CREATED_ON },
            { tblUnits::untCreatedBy_usrID,     ORM_CREATED_BY },
        },
        {///< Col                   Reference Table                             ForeignCol              Rename      LeftJoin
            { tblUnits::unt_depID,  R(TicketingSchema, tblDepartments::Name),   tblDepartments::depID },
            ORM_RELATION_OF_CREATOR(tblUnits::untCreatedBy_usrID),
        }
    ) { ; }

QVariant IMPL_ORMGET(Units) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(*this, GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(Units) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(*this, CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(Units) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(*this, UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(Units) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(*this, DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::TicketingModule::ORM
