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

#include "Authorization.h"
#include "PrivHelpers.h"
#include <QJsonArray>
#include <arpa/inet.h>

namespace Targoman {
namespace API {
namespace Helpers {
namespace AAA {
namespace Authorization{

void validateIPAddress(const QString& _ip)
{
    DACInstance().callSP("","AAA.sp_SYSTEM_validateIPAccess", {
                             {"iIP", inet_addr(_ip.toLatin1().constData())},
                         });
}

QJsonObject retrieveTokenInfo(const QString& _token, const QString& _ip, const QStringList& _requiredPrivs)
{
    PrivHelpers::validateToken(_token);

    QJsonObject TokenInfo =  DACInstance().callSPCacheable(3600,
                                                           "","AAA.sp_UPDATE_retrieveTokenInfo", {
                                                               {"iToken", _token},
                                                               {"iIP", _ip},
                                                           }).toJson(true).object();
    return PrivHelpers::processObjectPrivs(TokenInfo, _requiredPrivs);
}

void checkCredit(const QJsonObject& _privs, const QString& _selector, double _value)
{
    QVariant Credit = PrivHelpers::getPrivValue(_privs, _selector);
    if(Credit.isValid() &&
       Credit.canConvert<double>() &&
       Credit.toDouble() >= 0 &&
       Credit.toDouble() < _value)
        throw new exAccounting("You have exceeded your "+_selector+" quota.");
}

bool hasPriv(const QJsonObject& _privs, const QStringList& _requiredAccess)
{
    if (_requiredAccess.isEmpty() || _privs.isEmpty())
        return false;

    foreach(auto AccessItem, _requiredAccess)
        if(PrivHelpers::hasPrivBase(privObjectFromInfo(_privs), AccessItem) == false)
            return false;
    return true;
}

QJsonObject privObjectFromInfo(const QJsonObject& _info)
{
    return _info.contains(AAACommonItems::privs) ? _info[AAACommonItems::privs].toObject() : _info;
}

}
}
}
}
}
