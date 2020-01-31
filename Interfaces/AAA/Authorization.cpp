/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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

#include "Authorization.h"
#include "PrivHelpers.h"
#include <QJsonArray>
#include <arpa/inet.h>

namespace Targoman {
namespace API {
namespace AAA {
namespace Authorization{

void validateIPAddress(const QString& _ip)
{
    AAADACInstance().callSP("","AAA.sp_SYSTEM_validateIPAccess", {
                             {"iIP", inet_addr(_ip.toLatin1().constData())},
                         });
}

QJsonObject retrieveTokenInfo(const QString& _token, const QString& _ip, const QStringList& _requiredPrivs)
{
    PrivHelpers::validateToken(_token);

    QJsonObject TokenInfo =  AAADACInstance().callSPCacheable(3600,
                                                           "","AAA.sp_UPDATE_retrieveTokenInfo", {
                                                               {"iToken", _token},
                                                               {"iIP", _ip},
                                                           }).toJson(true).object();
    return PrivHelpers::processObjectPrivs(TokenInfo, _requiredPrivs);
}

bool hasPriv(const TAPI::JWT_t& _jwt, const QStringList& _requiredAccess, bool _isSelf)
{
    if(_requiredAccess.isEmpty())
        return true;

    if ( privObjectFromInfo(_jwt).isEmpty())
        return false;

    foreach(auto AccessItem, _requiredAccess)
        if(PrivHelpers::hasPrivBase(privObjectFromInfo(_jwt), AccessItem, _isSelf) == false)
            return false;
    return true;
}

void checkPriv(const TAPI::JWT_t &_jwt, const QStringList &_requiredAccess, bool _isSelf)
{
    if(!hasPriv(_jwt, _requiredAccess, _isSelf))
        throw exAuthorization("Not enought privileges: required are <" + _requiredAccess.join("|") + ">");
}

QJsonObject privObjectFromInfo(const QJsonObject& _info)
{
    return _info.contains(AAACommonItems::privs) ? _info[AAACommonItems::privs].toObject() : _info;
}

}
}
}
}

