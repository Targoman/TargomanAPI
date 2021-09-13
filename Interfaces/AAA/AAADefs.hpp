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

#ifndef TARGOMAN_API_AAA_DEFS_HPP
#define TARGOMAN_API_AAA_DEFS_HPP

#include <QJsonObject>

#include "libTargomanCommon/exTargomanBase.h"
#include "libTargomanDBM/clsDAC.h"

#include "Interfaces/Common/HTTPExceptions.hpp"

using namespace qhttp;

namespace Targoman {
namespace API {
namespace AAA {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_BAD_REQUEST, exAAA);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_UNAUTHORIZED, exAuthentication);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_FORBIDDEN, exAuthorization);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_METHOD_NOT_ALLOWED, exAccounting);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (499, exNoTokenProvided);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (498, exInvalidToken);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_PAYMENT_REQUIRED, exPaymentRequired);
#pragma clang diagnostic pop


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
namespace TOKENItems {
TARGOMAN_CREATE_CONSTEXPR(tokID);
TARGOMAN_CREATE_CONSTEXPR(usrID);
TARGOMAN_CREATE_CONSTEXPR(privs);
}

namespace PRIVItems {
TARGOMAN_CREATE_CONSTEXPR(CRUD);
}

namespace AAACommonItems {
TARGOMAN_CREATE_CONSTEXPR(privs);
TARGOMAN_CREATE_CONSTEXPR(acc);
TARGOMAN_CREATE_CONSTEXPR(usrID);
}

namespace tblUser{
constexpr char Name[] = "tblUser";
constexpr quint64 MinNormalUserID = 100;
TARGOMAN_CREATE_CONSTEXPR(usrID);
TARGOMAN_CREATE_CONSTEXPR(usrGender);
TARGOMAN_CREATE_CONSTEXPR(usrName);
TARGOMAN_CREATE_CONSTEXPR(usrFamily);
TARGOMAN_CREATE_CONSTEXPR(usrEmail);
TARGOMAN_CREATE_CONSTEXPR(usrMobile);
TARGOMAN_CREATE_CONSTEXPR(usrApprovalState);
TARGOMAN_CREATE_CONSTEXPR(usrPass);
TARGOMAN_CREATE_CONSTEXPR(usr_rolID);
TARGOMAN_CREATE_CONSTEXPR(usrSpecialPrivs);
TARGOMAN_CREATE_CONSTEXPR(usrLanguage);
TARGOMAN_CREATE_CONSTEXPR(usrMaxSessions);
TARGOMAN_CREATE_CONSTEXPR(usrActiveSessions);
TARGOMAN_CREATE_CONSTEXPR(usrLastLogin);
TARGOMAN_CREATE_CONSTEXPR(usrCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(usrCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(usrUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(usrStatus);
}
#pragma GCC diagnostic pop

constexpr char AccountDomain[] = "Account";
constexpr char AAASchema[] = "AAA";

constexpr quint64 SYSTEM_USER_ID = 1;
#define makeAAADAC(_varName) DBManager::clsDAC _varName(AccountDomain, AAASchema)
}
}
}

#endif // TARGOMAN_API_AAA_DEFS_HPP
