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

#ifndef TARGOMAN_API_ALERTS_H
#define TARGOMAN_API_ALERTS_H

#include "Interfaces/DBM/clsTable.h"

using namespace Targoman::API::DBM;

namespace Targoman::API::ORM {

TARGOMAN_DEFINE_ENUM(enuAlertType,
                     Informational = 'I',
                     Critical = 'C',
                     Warning = 'W'
                     )

TARGOMAN_DEFINE_ENUM(enuAlertStatus,
                     New = 'N',
                     Sent = 'S',
                     Error = 'E'
                     )

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAlerts {
constexpr char Name[] = "tblAlerts";
TARGOMAN_CREATE_CONSTEXPR(alrID);
TARGOMAN_CREATE_CONSTEXPR(alrType);
TARGOMAN_CREATE_CONSTEXPR(alr_usrID);
TARGOMAN_CREATE_CONSTEXPR(alrEmail);
TARGOMAN_CREATE_CONSTEXPR(alrMobile);
TARGOMAN_CREATE_CONSTEXPR(alrReplacedContactInfo);
TARGOMAN_CREATE_CONSTEXPR(alr_altCode);
TARGOMAN_CREATE_CONSTEXPR(alrReplacements);
TARGOMAN_CREATE_CONSTEXPR(alrCreateDate);
TARGOMAN_CREATE_CONSTEXPR(alrLockedAt);
TARGOMAN_CREATE_CONSTEXPR(alrLastTryAt);
TARGOMAN_CREATE_CONSTEXPR(alrSentDate);
TARGOMAN_CREATE_CONSTEXPR(alrStatus);
}

#pragma GCC diagnostic pop

class intfAlerts : public clsTable
{
public:
    intfAlerts();
//    instanceGetter(intfAlerts)

public:
    static bool createNewAlert(
        quint64 _currentUserID,
        enuAlertType::Type _type,
        quint64 _usrID,
        QString _replacedContactInfo,
        QString _altCode,
        QString _replacements
    );
};

} //namespace Targoman::API::ORM

TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuAlertType);
TAPI_DECLARE_METATYPE_ENUM(Targoman::API::ORM, enuAlertStatus);

#endif // TARGOMAN_API_ALERTS_H
