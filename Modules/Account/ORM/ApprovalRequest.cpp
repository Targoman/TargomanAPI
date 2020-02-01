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

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ApprovalRequest::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

bool ApprovalRequest::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(AAADACInstance(), DELETE_METHOD_CALL_ARGS, true);
}

ApprovalRequest::ApprovalRequest() :
    clsTable("AAA",
              "tblApprovalRequest",
              { ///<ColName             Type                    Validation                       Default    RO   Sort  Filter Self  Virt   PK
                {"aprID",               S(quint64),             QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"apr_usrID",           S(quint32),             QFV.integer().minValue(1),       QInvalid,  true},
                {"aprRequestedFor",     S(TAPI::enuApprovalType::Type),    QFV,         "",        true},
                {"aprApprovalCode",     S(QString),             QFV.asciiAlNum().maxLenght(32),  "",        true},
                {"aprApprovalValue",    S(QString),             QFV.allwaysInvalid(),            "",        true,false,false},
                {"aprRequestDate",      S(TAPI::DateTime_t),    QFV,                             QNull,     true},
                {"aprApplyDate",        S(TAPI::DateTime_t),    QFV,                             QNull,     true},
                {"aprStatus",           S(TAPI::enuAPRStatus::Type), QFV,                        TAPI::enuAPRStatus::New},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"apr_usrID",          "AAA.tblUser",      "usrID"},
              })
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuAPRStatus);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuApprovalType);
}

}
}
}

