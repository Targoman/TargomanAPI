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

#include "ApprovalRequest.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {
using namespace QHttp;

void ApprovalRequest::init()
{;}

QVariant ApprovalRequest::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,
                            {"Account:ApprovalRequest:CRUD~0100"},
                             this->isSelf({{"apr_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _DIRECTFILTERS, _filters));

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}


ApprovalRequest::ApprovalRequest() :
    clsTable("AAA",
              "tblApprovalRequest",
              { ///<ColName             Type                    Validation                       RO   Sort  Filter Self  Virt   PK
                {"aprID",               S(quint64),             QFV.integer().minValue(1),       ORM_PRIMARY_KEY},
                {"apr_usrID",           S(quint32),             QFV.integer().minValue(1),       true},
                {"aprRequestedFor",     S(Targoman::API::enuApprovalType::Type),    QFV,         true},
                {"aprApprovalCode",     S(QString),             QFV.asciiAlNum().maxLenght(32),  true},
                {"aprApprovalValue",    S(QString),             QFV.allwaysInvalid(),            true,false,false},
                {"aprRequestDate",      S(QHttp::DateTime_t),   QFV,                             true},
                {"aprApplyDate",        S(QHttp::DateTime_t),   QFV,                             true},
                {"aprStatus",           S(Targoman::API::enuAPRStatus::Type)},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"apr_usrID",          "AAA.tblUser",      "usrID"},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAPRStatus);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuApprovalType);
    this->registerMyRESTAPIs();
}

}
}
}

