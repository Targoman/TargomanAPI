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

QVariant Departments::apiGET(APISession<true> &_SESSION, GET_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

quint32 Departments::apiCREATE(APISession<true> &_SESSION, CREATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Departments::apiUPDATE(APISession<true> &_SESSION, UPDATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Departments::apiDELETE(APISession<true> &_SESSION, DELETE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::TicketingModule::ORM
