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

#ifndef TARGOMAN_API_AAA_AUTHORIZATION_H
#define TARGOMAN_API_AAA_AUTHORIZATION_H

#include "AAADefs.hpp"
#include "Interfaces/Common/GenericTypes.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Authorization{

    extern void validateIPAddress(const QString& _ip);
    extern QJsonObject retrieveTokenInfo(const QString& _token, const QString& _ip, const QStringList& _requiredPrivs = {});
    extern bool hasPriv(const TAPI::JWT_t& _jwt, const QStringList& _requiredAccess, bool _isSelf = false);
    extern void checkPriv(const TAPI::JWT_t& _jwt, const QStringList& _requiredAccess, bool _isSelf = false);
    extern QJsonObject privObjectFromInfo(const QJsonObject& _info);
}
}
}
}

#endif // TARGOMAN_API_AAA_AUTHORIZATION_H
