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

#include "BlockingRules.h"
#include "User.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant BlockingRules::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));

    return Targoman::API::Query::SelectOne(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL); //, ExtraFilters, CACHE_TIME);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 BlockingRules::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool BlockingRules::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool BlockingRules::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));

    return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

BlockingRules::BlockingRules() :
    clsTable(AAASchema,
              tblBlockingRules::Name,
              { ///<ColName                             Type                Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
                {tblBlockingRules::blrID,               ORM_PRIMARY_KEY64},
                {tblBlockingRules::blr_ipbIP,           S(quint32),         QFV.integer().minValue(1),       QNull,    UPAdmin},
                {tblBlockingRules::blr_ipIPReadable,    S(TAPI::IPv4_t),    QFV,                             QInvalid, UPNone,false,false},
                {tblBlockingRules::blrStartingTime,     S(TAPI::DateTime_t),QFV,                             QNull,    UPNone, true},
                {tblBlockingRules::blrEndingTime,       S(TAPI::DateTime_t),QFV,                             QNull,    UPAdmin},
                {tblBlockingRules::blrCause,            S(QString),         QFV,                             QNull,    UPAdmin,false,false},
                {tblBlockingRules::blrCreatedBy_usrID,  ORM_CREATED_BY},
                {tblBlockingRules::blrCreationDateTime, ORM_CREATED_ON},
                {tblBlockingRules::blrUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblBlockingRules::blrStatus,           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active,UPStatus},
              },
              { ///< Col                               Reference Table              ForeignCol          Rename     LeftJoin
                ORM_RELATION_OF_CREATOR(tblBlockingRules::blrCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(tblBlockingRules::blrUpdatedBy_usrID),
              })
{
}

}
}
}
