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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Account.h"

namespace TAPI{
TARGOMAN_DEFINE_ENUM(enuSessionStatus,
                     Active    = 'A',
                     LoggedOut = 'G',
                     FiredOut  = 'F',
                     Expired   = 'E',
                     Removed   = 'R'
                                 )
}

namespace Targoman {
namespace API {
namespace AAA {

namespace tblActiveSessions {
constexpr char Name[] = "tblActiveSessions";
TARGOMAN_CREATE_CONSTEXPR(ssnKey);
TARGOMAN_CREATE_CONSTEXPR(ssn_usrID);
TARGOMAN_CREATE_CONSTEXPR(ssnIP);
TARGOMAN_CREATE_CONSTEXPR(ssnIPReadable);
TARGOMAN_CREATE_CONSTEXPR(ssnCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(ssnInfo);
TARGOMAN_CREATE_CONSTEXPR(ssnFingerPrint);
TARGOMAN_CREATE_CONSTEXPR(ssnLastActivity);
TARGOMAN_CREATE_CONSTEXPR(ssnRemember);
TARGOMAN_CREATE_CONSTEXPR(ssnUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(ssnStatus);
}

class ActiveSessions : public ORM::clsTable
{
    Q_OBJECT

private slots:
    QVariant ORMGET("Get ActiveSessions information.")
    bool ORMDELETE("Delete an active session. Destroying current session is not allowed.")

    TARGOMAN_DEFINE_API_SUBMODULE(Account,ActiveSessions)
};

}
}
}

TAPI_DECLARE_METATYPE(TAPI::enuSessionStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
