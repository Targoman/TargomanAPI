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

#include "ApprovalRequest.h"
#include "User.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuApprovalType);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ApprovalRequest::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool ApprovalRequest::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return Targoman::API::Query::DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, {}, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, {}, true);
}

ApprovalRequest::ApprovalRequest() :
    clsTable(
        AAASchema,
        tblApprovalRequest::Name,
        {///< ColName                               Type                    Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
            { tblApprovalRequest::aprID,            ORM_PRIMARYKEY_64},
            { tblApprovalRequest::apr_usrID,        S(quint64),             QFV.integer().minValue(1),       QInvalid,  UPNone},
            { tblApprovalRequest::aprRequestedFor,  S(TAPI::enuApprovalType::Type),    QFV,                  TAPI::enuApprovalType::Email, UPNone},
            { tblApprovalRequest::aprApprovalCode,  S(QString),             QFV.asciiAlNum().maxLenght(32),  QRequired,        UPNone},
            { tblApprovalRequest::aprApprovalValue, S(QString),             QFV.allwaysInvalid(),            QRequired,        UPNone,false,false},
            { tblApprovalRequest::aprRequestDate,   ORM_CREATED_ON},
            { tblApprovalRequest::aprApplyDate,     S(TAPI::DateTime_t),    QFV,                             QNull,     UPNone},
            { tblApprovalRequest::aprStatus,        ORM_STATUS_FIELD(TAPI::enuAPRStatus, TAPI::enuAPRStatus::New) },
            { ORM_INVALIDATED_AT_FIELD },
        },
        {///< Col                           Reference Table                  ForeignCol        Rename     LeftJoin
            {tblApprovalRequest::apr_usrID, R(AAASchema,tblUser::Name),      tblUser::usrID},
        },
        {
            { {
                tblApprovalRequest::aprApprovalCode,
                tblApprovalRequest::aprApprovalValue,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    )
{}

}
}
}

