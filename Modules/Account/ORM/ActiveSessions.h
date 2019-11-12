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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
#define TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "Helpers/ORM/clsTable.h"

namespace Targoman {
namespace API {
TARGOMAN_DEFINE_ENUM(enuSessionStatus,
                     Active    = 'A',
                     LoggedOut = 'G',
                     FiredOut  = 'F',
                     Expired   = 'E'
                     )
namespace AAA {


class ActiveSessions : public QHttp::intfRESTAPIHolder, private clsTable
{
    Q_OBJECT
public:
    void init();

private slots:
    QVariant ORMGET("Get ActiveSessions information. Primary key is ssnKey (stored as jti in JWT)")
    bool API(DELETE,,(QHttp::JWT_t _JWT, QHttp::ExtraPath_t _EXTRAPATH),
             "Delete an active session. PKs are sessionKey and userID "
             "both must be provided in oreder to remove self session")

private:
    ActiveSessions();
    TARGOMAN_DEFINE_SINGLETON_SUBMODULE(Account,ActiveSessions);
};

}
}
}

Q_DECLARE_METATYPE(Targoman::API::enuSessionStatus::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ORM_ACTIVESESSIONS_H
