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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H

#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/API/intfSQLBasedModule.h"

namespace Targoman::API::AccountModule {

//structures and enumes goes here

TARGOMAN_DEFINE_ENUM(enuAPITokensStatus,
                     Active   = 'A',
                     Pending  = 'P',
                     CreditFinished    = 'C',
                     Removed  = 'R'
                                )
namespace ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAPITokens {
constexpr char Name[] = "tblAPITokens";
TARGOMAN_CREATE_CONSTEXPR(aptID);
TARGOMAN_CREATE_CONSTEXPR(aptToken);
TARGOMAN_CREATE_CONSTEXPR(apt_usrID);
TARGOMAN_CREATE_CONSTEXPR(apt_svcID);
TARGOMAN_CREATE_CONSTEXPR(aptLang);
TARGOMAN_CREATE_CONSTEXPR(aptValidateIP);
TARGOMAN_CREATE_CONSTEXPR(aptExtraPriviledges);
TARGOMAN_CREATE_CONSTEXPR(aptExpiryDate);
TARGOMAN_CREATE_CONSTEXPR(aptLastActivity);
TARGOMAN_CREATE_CONSTEXPR(aptAccessCount);
TARGOMAN_CREATE_CONSTEXPR(aptCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(aptCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(aptUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(aptStatus);
}
#pragma GCC diagnostic pop

class APITokens : public intfSQLBasedModule
{
    Q_OBJECT
    TARGOMAN_DEFINE_API_SUBMODULE(Account, APITokens)

private slots:
    QVariant ORMGET("Get APITokens information")
    quint64 ORMCREATE("Create a new APITokens by priviledged user")
    bool ORMUPDATE("Update token info by priviledged user")
    bool ORMDELETE("Delete an APIToken")
};

} //namespace ORM
} //namespace Targoman::API::AccountModule

TARGOMAN_DECLARE_METATYPE_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENS_H
