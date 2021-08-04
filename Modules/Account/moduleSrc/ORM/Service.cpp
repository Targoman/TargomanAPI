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

#include "Service.h"
#include "User.h"
#include "Roles.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AAA {

using namespace ORM;

Service::Service() :
    clsTable(
        AAASchema,
        tblService::Name,
        {///< ColName                               Type                 Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
            { tblService::svcID,                    ORM_PRIMARYKEY_32 },
            { tblService::svcName,                  S(QString),          QFV,                             QRequired, UPAdmin },
            { tblService::svc_rolID,                S(quint32),          QFV,                             QRequired, UPAdmin },
            { tblService::svcProcessVoucherEndPoint,S(QString),          QFV,                             "INVALID", UPAdmin },
            { tblService::svcStatus,                ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblService::svcCreationDateTime,      ORM_CREATED_ON },
            { tblService::svcCreatedBy_usrID,       ORM_CREATED_BY },
            { tblService::svcUpdatedBy_usrID,       ORM_UPDATED_BY },
        },
        {///< Col                    Reference Table              ForeignCol      Rename     LeftJoin
            { tblService::svc_rolID, R(AAASchema,tblRoles::Name), tblRoles::rolID },
            ORM_RELATION_OF_CREATOR(tblService::svcCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblService::svcUpdatedBy_usrID),
        },
        {
            { {
                  tblService::svcName,
                  ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    )
{}

QVariant Service::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

quint64 Service::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Service::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Service::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return Targoman::API::Query::DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::AAA
