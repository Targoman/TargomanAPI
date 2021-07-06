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



#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace TAPI{
TARGOMAN_DEFINE_ENUM(enuRoleStatus,
                     Active   = 'A',
                     Blocked  = 'B',
                     Removed  = 'R'
                     )
}

namespace Targoman {
namespace API {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace tblRoles {
constexpr char Name[] = "tblRoles";
TARGOMAN_CREATE_CONSTEXPR(rolID);
TARGOMAN_CREATE_CONSTEXPR(rolName);
TARGOMAN_CREATE_CONSTEXPR(rolParent_rolID);
TARGOMAN_CREATE_CONSTEXPR(rolPrivileges);
TARGOMAN_CREATE_CONSTEXPR(rolSignupAllowedIPs);
TARGOMAN_CREATE_CONSTEXPR(rolCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(rolCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(rolUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(rolStatus);
}
#pragma GCC diagnostic pop

class Roles : public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get roles information")
    bool ORMDELETE("Delete a Role")
    bool ORMUPDATE("Update role info by priviledged user")
    quint32 ORMCREATE("Create a new Role by priviledged user")

    private:
        TARGOMAN_DEFINE_API_SUBMODULE(Account,Roles)
};

}
}
}

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuRoleStatus);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H


