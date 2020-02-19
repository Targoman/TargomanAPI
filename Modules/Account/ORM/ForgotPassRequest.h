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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuFPRStatus,
                     New      = 'N',
                     Sent     = 'S',
                     Applied  = 'A',
                     Removed  = 'R'
                                )
TARGOMAN_DEFINE_ENUM(enuForgotPassLinkVia,
                     Email    = 'E',
                     Mobile   = 'M',
                                )
}

namespace Targoman {
namespace API {
namespace AAA {

namespace tblForgotPassRequest {
constexpr char Name[] = "tblForgotPassRequest";
TARGOMAN_CREATE_CONSTEXPR(fprUUID);
TARGOMAN_CREATE_CONSTEXPR(fpr_usrID);
TARGOMAN_CREATE_CONSTEXPR(fprRequestedVia);
TARGOMAN_CREATE_CONSTEXPR(fprRequestDate);
TARGOMAN_CREATE_CONSTEXPR(fprApplyDate);
TARGOMAN_CREATE_CONSTEXPR(fprStatus);
}

class ForgotPassRequest : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get ForgotPassRequest information")
    bool ORMDELETE("Delete a ForgotPassRequest")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,ForgotPassRequest)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuFPRStatus::Type);
TAPI_DECLARE_METATYPE(TAPI::enuForgotPassLinkVia::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_FORGOTPASSREQUEST_H
