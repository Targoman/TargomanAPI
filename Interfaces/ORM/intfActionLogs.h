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

#ifndef TARGOMAN_API_ACTIONLOGS_H
#define TARGOMAN_API_ACTIONLOGS_H

#include "Interfaces/DBM/clsTable.h"
#include "Interfaces/API/intfSQLBasedModule.h"

using namespace Targoman::API::DBM;
using namespace Targoman::API::API;

namespace Targoman::API::ORM {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblActionLogs {
constexpr char Name[] = "tblActionLogs";
TARGOMAN_CREATE_CONSTEXPR(atlID);
TARGOMAN_CREATE_CONSTEXPR(atlBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(atlInsertionDateTime);
TARGOMAN_CREATE_CONSTEXPR(atlType);
TARGOMAN_CREATE_CONSTEXPR(atlDescription);
}

#pragma GCC diagnostic pop

class intfActionLogs : public intfSQLBasedModule
{
    Q_OBJECT

public:
    intfActionLogs(
            const QString& _schema,
            const QString& _name
        );

protected slots:
    QVariant ORMGET("Get Action Logs.")
};

} //namespace Targoman::API::ORM

#endif // TARGOMAN_API_ACTIONLOGS_H
