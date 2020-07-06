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

#include "ApprovalRequest.h"
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuApprovalType);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ApprovalRequest::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool ApprovalRequest::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, true);
}

ApprovalRequest::ApprovalRequest() :
    clsTable(AAASchema,
              tblApprovalRequest::Name,
              { ///<ColName                             Type                    Validation                       Default    UpBy   Sort  Filter Self  Virt   PK
                {tblApprovalRequest::aprID,             S(quint64),             QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {tblApprovalRequest::apr_usrID,         S(quint32),             QFV.integer().minValue(1),       QInvalid,  UPNone},
                {tblApprovalRequest::aprRequestedFor,   S(TAPI::enuApprovalType::Type),    QFV,                  TAPI::enuApprovalType::Email, UPNone},
                {tblApprovalRequest::aprApprovalCode,   S(QString),             QFV.asciiAlNum().maxLenght(32),  QRequired,        UPNone},
                {tblApprovalRequest::aprApprovalValue,  S(QString),             QFV.allwaysInvalid(),            QRequired,        UPNone,false,false},
                {tblApprovalRequest::aprRequestDate,    ORM_CREATED_ON},
                {tblApprovalRequest::aprApplyDate,      S(TAPI::DateTime_t),    QFV,                             QNull,     UPNone},
                {tblApprovalRequest::aprStatus,         S(TAPI::enuAPRStatus::Type), QFV,                        TAPI::enuAPRStatus::New, UPStatus},
              },
              { ///< Col                                 Reference Table                  ForeignCol        Rename     LeftJoin
                {tblApprovalRequest::apr_usrID,          R(AAASchema,tblUser::Name),      tblUser::usrID},
              })
{
}

}
}
}

