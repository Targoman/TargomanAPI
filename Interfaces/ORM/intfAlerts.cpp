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

#include "intfAlerts.h"
#include "Interfaces/Common/tmplAPIArg.h"
#include "Interfaces/DBM/QueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ORM, enuAlertType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::ORM, enuAlertStatus);

namespace Targoman::API::ORM {

intfAlerts::intfAlerts(
    const QString& _module,
    const QString& _schema
) :
    intfSQLBasedModule(
        _module,
        _schema,
        tblAlerts::Name,
        tblAlerts::Private::ORMFields,
        tblAlerts::Private::Relations,
        tblAlerts::Private::Indexes
    ) { ; }

/*STATIC*/ bool intfAlerts::createNewAlert(
    quint64 _currentUserID,
    enuAlertType::Type _type,
    quint64 _usrID,
    QString _replacedContactInfo,
    QString _altCode,
    QString _replacements
) {
    ///@TODO: complete this

//    auto Query = CreateQuery(Alerts::instance())
//                 .addCol("")
//                 .execute(_currentUserID)
//            ;

    return true;
}

} //namespace Targoman::API::ORM
