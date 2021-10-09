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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_AAA_GENERICENUMS_HPP
#define TARGOMAN_API_AAA_GENERICENUMS_HPP

#include <QJsonObject>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"
#include "Interfaces/Common/intfAPIArgManipulator.h"

//I used TAPI as namespace in order to make Targoman::API shorter
namespace TAPI {

TARGOMAN_DEFINE_ENUM(enuUserStatus,
                     Active         = 'A',
                     Removed        = 'R',
                     Blocked        = 'B',
                     MustChangePass = 'C',
                     MustValidate   = 'V'
                     )

TARGOMAN_DEFINE_ENUM(enuUserApproval,
                     None       = 'N',
                     All        = 'A',
                     JustMobile = 'M',
                     JustEmail  = 'E'
                     )

TARGOMAN_DEFINE_ENUM(enuGender,
                     Male         = 'M',
                     Female       = 'F',
                     NotExpressed = 'N'
                     )

TARGOMAN_DEFINE_ENUM(enuAuditableStatus,
                     Pending = 'P',
                     Active  = 'A',
                     Banned  = 'B',
                     Removed = 'R'
                     )

TARGOMAN_DEFINE_ENUM (enuSaleableType,
                      Normal  = 'N',
                      Special = 'S',
                      );
}

TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuUserStatus);
TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuUserApproval);
TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuGender);
TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuAuditableStatus);
TARGOMAN_DECLARE_METATYPE_ENUM(TAPI, enuSaleableType);

#endif // TARGOMAN_API_AAA_GENERICENUMS_HPP
