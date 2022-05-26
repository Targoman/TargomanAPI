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

#include "intfMigrations.h"
#include "Interfaces/AAA/Authorization.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/clsJWT.hpp"

using namespace Targoman::API::AAA;

namespace Targoman::API::ORM {

intfMigrations::intfMigrations(
    const QString& _schema,
    const QString& _name
) :
    intfSQLBasedModule(
        _schema,
        _name,
        tblMigrations::Name,
        {///< ColName                       Type                Validation  Default  UpBy   Sort  Filter Self  Virt   PK
            { tblMigrations::migName,       S(QString),         QFV,        QNull,   UPNone },
            { tblMigrations::migAppliedAt,  S(TAPI::DateTime_t),QFV,        QNull,   UPNone },
            { tblMigrations::migRunType,    S(QString),         QFV,        QNull,   UPNone },
            { tblMigrations::migStatus,     S(QString),         QFV,        QNull,   UPNone },
        }
    ) { ; }

QVariant IMPL_ANONYMOUSE_ORMGET(intfMigrations) {
//    Authorization::checkPriv(_APICALLBOOM, { this->ModuleName + ":Migrations:CRUD~0100" });
//    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));

    _orderBy = QStringLiteral("migName, migAppliedAt");

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

} //namespace Targoman::API::ORM
