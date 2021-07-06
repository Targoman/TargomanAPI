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
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblAPITokenValidIPs {
constexpr char Name[] = "tblAPITokenValidIPs";
TARGOMAN_CREATE_CONSTEXPR(tviID);
TARGOMAN_CREATE_CONSTEXPR(tvi_aptID);
TARGOMAN_CREATE_CONSTEXPR(tviIP);
TARGOMAN_CREATE_CONSTEXPR(tviIPReadable);
TARGOMAN_CREATE_CONSTEXPR(tviCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(tviCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(tviUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(tviStatus);
}
#pragma GCC diagnostic pop

class APITokenValidIPs : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get APITokenValidIPs information")
    bool ORMDELETE("Delete an APITokenValidIP")
    bool ORMUPDATE("Update token valid IP info")
    quint64 ORMCREATE("Create a new APITokenValidIP")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,APITokenValidIPs)
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_APITOKENVALIDIPS_H
