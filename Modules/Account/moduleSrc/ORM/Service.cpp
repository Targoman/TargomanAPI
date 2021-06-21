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

#include "Service.h"
#include "User.h"
#include "Roles.h"

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant Service::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);

    ApiSelectQuery query = ApiSelectQuery(*this, GET_METHOD_CALL_ARGS);

    return query.one();
}

bool Service::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}


bool Service::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint64 Service::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toULongLong();
}

Service::Service() :
    clsTable(AAASchema,
              tblService::Name,
              { ///<ColName                        Type                 Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
                {tblService::svcID,               ORM_PRIMARY_KEY32},
                {tblService::svcName,             S(QString),          QFV,                             QRequired, UPAdmin},
                {tblService::svc_rolID,           S(quint32),          QFV,                             QRequired, UPAdmin},
                {tblService::svcCreatedBy_usrID,  ORM_CREATED_BY},
                {tblService::svcCreationDateTime, ORM_CREATED_ON},
                {tblService::svcUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblService::svcStatus,           S(TAPI::enuGenericStatus::Type), QFV,                 TAPI::enuGenericStatus::Active, UPStatus},
              },
              { ///< Col                           Reference Table                  ForeignCol          Rename     LeftJoin
                {tblService::svc_rolID,           R(AAASchema,tblRoles::Name),     tblRoles::rolID},
                ORM_RELATION_OF_CREATOR(tblService::svcCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(tblService::svcUpdatedBy_usrID),
              })
{
}

}
}
}
