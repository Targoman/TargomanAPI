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

#ifndef TARGOMAN_API_HELPERS_AAA_DEFS_HPP
#define TARGOMAN_API_HELPERS_AAA_DEFS_HPP

#include <QJsonObject>
#include "libTargomanCommon/exTargomanBase.h"
#include "QHttp/HTTPExceptions.h"
#include "QHttp/qhttpfwd.hpp"

using namespace qhttp;

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_BAD_REQUEST, exAAA);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_UNAUTHORIZED, exAuthentication);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_FORBIDDEN, exAuthorization);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_METHOD_NOT_ALLOWED, exAccounting);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (499, exNoTokenProvided);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (498, exInvalidToken);
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_PAYMENT_REQUIRED, exPaymentRequired);
#pragma GCC diagnostic pop

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
}

/*constexpr char AAA_TOKENID[]= "tokID";
constexpr char AAA_USERID[]= "usrID";
constexpr char AAA_PRIVS_CRUD[]= "CRUD";
*/

}
}
}
}

#endif // TARGOMAN_API_HELPERS_AAA_DEFS_HPP
