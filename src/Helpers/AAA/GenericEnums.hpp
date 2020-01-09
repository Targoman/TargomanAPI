/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_HELPERS_AAA_USERENUMS_HPP
#define TARGOMAN_API_HELPERS_AAA_USERENUMS_HPP

#include <QJsonObject>
#include <QVariantMap>
#include "libTargomanCommon/Macros.h"

namespace Targoman {
namespace API {
TARGOMAN_DEFINE_ENUM(enuForgotPassLinkVia,
                               Web = 'W',
                               Mobile = 'M',
                               App = 'A'
                               );

TARGOMAN_DEFINE_ENUM(enuUserStatus,
                     Active = 'A',
                     Removed = 'R',
                     Blocked = 'B',
                     MustChangePass = 'C',
                     MustValidate = 'V',
                     )

TARGOMAN_DEFINE_ENUM(enuUserApproval,
                     None = 'N',
                     All = 'A',
                     JustMobile = 'M',
                     JustEmail = 'E',
                     )

TARGOMAN_DEFINE_ENUM(enuUserSex,
                     Male = 'M',
                     Female = 'F',
                     NotExpressed = 'N',
                     )

TARGOMAN_DEFINE_ENUM(enuGenericStatus,
                     Active = 'A',
                     Removed = 'R',
                     )

TARGOMAN_DEFINE_ENUM(enuAuditableStatus,
                     Pending = 'P',
                     Active = 'A',
                     Banned = 'B',
                     Removed = 'R',
                     )

}
}

Q_DECLARE_METATYPE(Targoman::API::enuUserStatus::Type);
Q_DECLARE_METATYPE(Targoman::API::enuUserApproval::Type);
Q_DECLARE_METATYPE(Targoman::API::enuUserSex::Type);
Q_DECLARE_METATYPE(Targoman::API::enuGenericStatus::Type);
Q_DECLARE_METATYPE(Targoman::API::enuAuditableStatus::Type);

#endif // TARGOMAN_API_HELPERS_AAA_USERENUMS_HPP
