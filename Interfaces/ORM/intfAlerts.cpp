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

intfAlerts::intfAlerts() :
    clsTable(
        "Common",
        "Alert",
        {///< ColName                           Type                                        Validation                  Default     UpBy       Sort    Filter  Self    Virt    PK      Select  RenameAs
            { tblAlerts::alrID,                 ORM_PRIMARYKEY_64 },
            { tblAlerts::alrType,               S(Targoman::API::ORM::enuAlertType::Type),  QFV,                        QRequired,  UPOwner },
            { tblAlerts::alr_usrID,             S(quint64),                                 QFV.integer().minValue(1),  QRequired,  UPOwner },
            { tblAlerts::alrReplacedContactInfo,S(TAPI::String_t),                          QFV.maxLenght(50),          QNull,      UPAdmin },
            { tblAlerts::alr_altCode,           S(TAPI::String_t),                          QFV.maxLenght(50),          QRequired,  UPAdmin },
            { tblAlerts::alrReplacements,       S(TAPI::DBText_t),                          QFV,                        QRequired,  UPAdmin },
            { tblAlerts::alrCreateDate,         ORM_CREATED_ON },
            { tblAlerts::alrSentDate,           S(TAPI::DateTime_t),                        QFV,                        QNull,      UPAdmin },
            { tblAlerts::alrStatus,             ORM_STATUS_FIELD(Targoman::API::ORM::enuAlertStatus,                    Targoman::API::ORM::enuAlertStatus::New) },
        }
    )
{}

/*STATIC*/ bool intfAlerts::createNewAlert(
        quint64 _currentUserID,
        enuAlertType::Type _type,
        quint64 _usrID,
        QString _replacedContactInfo,
        QString _altCode,
        QString _replacements
    )
{
    ///TODO: complete this

//    auto Query = CreateQuery(Alerts::instance())
//                 .addCol("")
//                 .execute(_currentUserID)
//            ;

    return true;
}

} //namespace Targoman::API::ORM
