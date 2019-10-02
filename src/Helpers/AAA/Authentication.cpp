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

#include "Authentication.h"
#include "PrivHelpers.h"

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {
namespace Authentication{

QJsonObject login(const QString& _login, const QString& _pass, const QString& _salt, const QJsonObject& _info)
{
    QJsonObject UserInfo =  DACInstance().callSP ("","AAA.sp_UPDATE_login", {
                                                      {"iLogin", _login},
                                                      {"iPass", _pass},
                                                      {"iSalt", _salt},
                                                      {"iInfo", _info}
                                                  }).toJson(true).object();
    return PrivHelpers::processObjectPrivs(UserInfo);
}


}
}
}
}
}
