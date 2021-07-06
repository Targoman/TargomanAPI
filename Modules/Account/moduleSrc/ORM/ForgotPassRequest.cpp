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

#include "ForgotPassRequest.h"
#include "User.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuFPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuForgotPassLinkVia);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ForgotPassRequest::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));

    return Targoman::API::Query::SelectOne(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL); //, ExtraFilters, CACHE_TIME);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool ForgotPassRequest::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));

    return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, {}, true);
}

ForgotPassRequest::ForgotPassRequest() :
    clsTable(AAASchema,
              tblForgotPassRequest::Name,
              { ///<ColName                             Type                 Validation                         Default    UpBy   Sort  Filter Self  Virt   PK
                {tblForgotPassRequest::fprUUID,         S(TAPI::MD5_t),      QFV,                               ORM_PRIMARY_KEY},
                {tblForgotPassRequest::fpr_usrID,       S(quint64),          QFV.integer().minValue(1),         QRequired,  UPNone},
                {tblForgotPassRequest::fprRequestedVia, S(TAPI::enuForgotPassLinkVia::Type),QFV,                TAPI::enuForgotPassLinkVia::Email,  UPNone},
                {tblForgotPassRequest::fprRequestDate,  ORM_CREATED_ON},
                {tblForgotPassRequest::fprApplyDate,    S(TAPI::DateTime_t), QFV,                               QNull,     UPNone},
                {tblForgotPassRequest::fprStatus,       S(TAPI::enuFPRStatus::Type), QFV,                       TAPI::enuFPRStatus::New, UPStatus},
              },
              { ///< Col                            Reference Table                 ForeignCol
                {tblForgotPassRequest::fpr_usrID,   R(AAASchema,tblUser::Name),     tblUser::usrID},
              })
{
}

}
}
}
