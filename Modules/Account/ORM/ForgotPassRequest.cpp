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

#include "ForgotPassRequest.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuFPRStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuForgotPassLinkVia);

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant ForgotPassRequest::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool ForgotPassRequest::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, true);
}

ForgotPassRequest::ForgotPassRequest() :
    clsTable(AAASchema,
              "tblForgotPassRequest",
              { ///<ColName             Type                 Validation                         Default    RO   Sort  Filter Self  Virt   PK
                {"fprUUID",             S(TAPI::MD5_t),      QFV,                               ORM_PRIMARY_KEY},
                {"fpr_usrID",           S(quint32),          QFV.integer().minValue(1),         QInvalid,  true},
                {"fprRequestedVia",     S(TAPI::enuForgotPassLinkVia::Type),QFV,                QInvalid,  true},
                {"fprRequestDate",      S(TAPI::DateTime_t), QFV,                               QNull,     true},
                {"fprApplyDate",        S(TAPI::DateTime_t), QFV,                               QNull,     true},
                {"fprStatus",           S(TAPI::enuFPRStatus::Type), QFV,                       TAPI::enuFPRStatus::New},
              },
              { ///< Col       Reference Table   ForeignCol
                {"fpr_usrID",  "AAA.tblUser",    "usrID"},
              })
{
}

}
}
}
