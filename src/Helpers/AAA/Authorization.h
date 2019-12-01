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

#ifndef TARGOMAN_API_HELPERS_AAA_AUTHORIZATION_H
#define TARGOMAN_API_HELPERS_AAA_AUTHORIZATION_H

#include "AAADefs.hpp"
#include "QHttp/GenericTypes.h"

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {
namespace Authorization{
    extern void validateIPAddress(const QString& _ip);
    extern QJsonObject retrieveTokenInfo(const QString& _token, const QString& _ip, const QStringList& _requiredPrivs = {});
    extern bool hasPriv(const QHttp::JWT_t& _jwt, const QStringList& _requiredAccess, bool _isSelf = false);
    extern void checkPriv(const QHttp::JWT_t& _jwt, const QStringList& _requiredAccess, bool _isSelf = false);
    extern QJsonObject privObjectFromInfo(const QJsonObject& _info);
}
}
}
}
}

#endif // TARGOMAN_API_HELPERS_AAA_AUTHORIZATION_H
