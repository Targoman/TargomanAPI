/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void ForgotPassRequest::init()
{;}

QVariant ForgotPassRequest::apiGET(GET_METHOD_ARGS_IMPL)
{
//    Authorization::checkPriv(_JWT,{"Account:ForgotPassRequest:CRUD~0100"});

//    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

ForgotPassRequest::ForgotPassRequest() :
    clsTable("AAA",
              "tblForgotPassRequest",
              { ///<ColName             Type                 Validation                                  RO   Sort  Filter Self  Virt   PK
                {"fprUUID",             S(QHttp::MD5_t),     QFV,                                        ORM_PRIMARY_KEY},
                {"fpr_usrID",           S(quint32),          QFV.integer().minValue(1),                  true},
                {"fprRequestedVia",     S(QString),          QFV.asciiAlNum().minLenght(1).maxLenght(10),true},
                {"fprRequestDate",      S(QHttp::DateTime_t),QFV,                                        true},
                {"fprApplyDate",        S(QHttp::DateTime_t),QFV,                                        true},
                {"fprStatus",           S(Targoman::API::enuFPRStatus::Type)},
              },
              { ///< Col       Reference Table   ForeignCol
                {"fpr_usrID",  "AAA.tblUser",    "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuFPRStatus);

    this->registerMyRESTAPIs();
}

}
}
}
