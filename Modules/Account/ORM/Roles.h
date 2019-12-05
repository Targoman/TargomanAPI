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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace AAA {

class Roles : public clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get roles information")
    bool API(DELETE,,(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH),
             "Delete Role")

    bool API(UPDATE,,(QHttp::JWT_t _JWT,
                      quint64 _rolID,
                      QString _name = {},
                      quint64 _parentRolID = {},
                      QHttp::JSON_t _privs = {},
                      QStringList _signupAllowdIPs = {},
                      Targoman::API::enuGenericStatus::Type _status = {}),
             "Update Role by priviledged user")

    quint64 API(CREATE,,(QHttp::JWT_t _JWT,
                         QString _name,
                         quint64 _parentRolID = {},
                         QHttp::JSON_t _privs = {},
                         QStringList _signupAllowdIPs = {}),
             "Create a new Role by priviledged user")

private:
    Roles();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,Roles);
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ROLES_H
