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
    bool IsSelf = _EXTRAPATH.split(',').size() == 2 && clsJWT(_JWT).usrID() == _EXTRAPATH.split(',').last().toUInt();
    if(IsSelf == false)
        Authorization::checkPriv(_JWT,{"Account:ApprovalRequest:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {},
                                 IsSelf ? "" : QString("+ssn_usrID=%1").arg(clsJWT(_JWT).usrID()),
                                 GET_METHOD_CALL_ARGS);
}


ApprovalRequest::ApprovalRequest() :
    clsTable("AAA",
              "tblApprovalRequest",
              { ///<ColName            Validation                       Sort   Filter RO   PK
                {"aprID",              QFV.integer().minValue(1),       true,  true, true, true},
                {"apr_usrID",          QFV.integer().minValue(1),       true,  true, true, true},
                {"aprRequestedFor",    QFV_Enum(enuApprovalType),       true,  true, true},
                {"aprApprovalCode",    QFV.asciiAlNum().maxLenght(32),  true,  true, true},
                {"aprApprovalValue",   QFV.allwaysInvalid(),            false,false, true},
                {"aprRequestDate",     QFV.dateTime(),                  true,  true, true},
                {"aprApplyDate",       QFV.dateTime(),                  true,  true, true},
                {"aprStatus",          QFV_Enum(enuAPRStatus)},
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

