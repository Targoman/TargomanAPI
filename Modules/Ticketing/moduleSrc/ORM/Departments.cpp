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
#include "../Ticketing.h"

//using namespace Targoman::API::ORM;

namespace Targoman::API::TicketingModule::ORM {

TARGOMAN_API_SUBMODULE_IMPLEMENT(Ticketing, Departments)

Departments::Departments() :
    intfSQLBasedModule(
        TicketingSchema,
        tblDepartments::Name,
        tblDepartments::Private::ORMFields,
        tblDepartments::Private::Relations,
        tblDepartments::Private::Indexes
) { ; }

QVariant IMPL_ANONYMOUSE_ORMGET(Departments) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint32 IMPL_ORMCREATE(Departments) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE(Departments) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE(Departments) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

} //namespace Targoman::API::TicketingModule::ORM
