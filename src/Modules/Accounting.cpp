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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Translation.h"
#include "Accounting.h"
#include "libTargomanAAA/AAA.h"
#include "QFieldValidator.h"
#include "QHttp/QRESTServer.h"
#include "QHttp/intfAPIArgManipulator.h"

using namespace Targoman;
using namespace QHttp;

TARGOMAN_DEFINE_ENUM(enuUserStatus,
                     Active = 'A',
                     Remove = 'R',
                     Blocked = 'B',
                     MustChangePass = 'C',
                     MustValidate = 'V'
                    )

TARGOMAN_DEFINE_ENUM(enuUserApproval,
                     None = 'N',
                     All = 'A',
                     JustMobile = 'M',
                     JustEmail = 'E',
                    )

namespace JWT{
    TARGOMAN_CREATE_CONSTEXPR(usrLogin);
    TARGOMAN_CREATE_CONSTEXPR(usrName);
    TARGOMAN_CREATE_CONSTEXPR(usrFamily);
    TARGOMAN_CREATE_CONSTEXPR(rolName);
    TARGOMAN_CREATE_CONSTEXPR(roleID);
    TARGOMAN_CREATE_CONSTEXPR(privs);
    TARGOMAN_CREATE_CONSTEXPR(usrID);
    TARGOMAN_CREATE_CONSTEXPR(usrApproval);
    TARGOMAN_CREATE_CONSTEXPR(usrStatus);
    TARGOMAN_CREATE_CONSTEXPR(jti);
}

void Accounting::init()
{}

QHttp::EncodedJWT_t Accounting::apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _login, const QHttp::MD5_t& _pass, const QString& _salt, bool _rememberMe, const QHttp::JSON_t& _sessionInfo)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
    QFV.optional(QFV.json()).validate(_sessionInfo, "sessionInfo");

    AAA::validateIPAddress(_REMOTE_IP);

    QJsonObject Result = AAA::login(_login, _pass, _salt, QJsonDocument::fromJson(_sessionInfo.toUtf8()).object());
    return this->createSignedJWT({
                                     {JWT::usrLogin, _login},
                                     {JWT::usrName, Result["usrGivenName"]},
                                     {JWT::usrFamily, Result["usrFamilyName"]},
                                     {JWT::rolName, Result["rolName"]},
                                     {JWT::roleID, Result["rolID"]},
                                     {JWT::privs, Result["privs"]},
                                     {JWT::usrID, Result["usrID"]},
                                     {JWT::usrApproval, enuUserApproval::toStr(Result["usrApprovalState"].toString())},
                                     {JWT::usrStatus, enuUserStatus::toStr(Result["usrStatus"].toString())},
                                 },
                                 QJsonObject(),
                                 _rememberMe ? 7*24*3600 : 1800,
                                 Result["ssnKey"].toString()
            );
}

QHttp::EncodedJWT_t Accounting::apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP, enuOAuthType::Type _type, const QString& _AuthToken, const QHttp::JSON_t& _sessionInfo)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_type) Q_UNUSED(_AuthToken)Q_UNUSED(_sessionInfo)
    throw exHTTPNotImplemented("oh oh!");
}

QHttp::EncodedJWT_t Accounting::apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP, const QHttp::JSON_t& _sessionInfo)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_sessionInfo)
    throw exHTTPNotImplemented("oh oh!");
}

bool Accounting::apiLogout(QHttp::JWT_t _JWT)
{
    return AAA::logout(static_cast<quint64>(_JWT[JWT::usrID].toDouble()), _JWT[JWT::jti].toString());
}

bool Accounting::apiCreateForgotPasswordLink(const RemoteIP_t& _REMOTE_IP, const QString& _login)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_login)
    throw exHTTPNotImplemented("oh oh!");
}

bool Accounting::apiChangePass(QHttp::JWT_t _JWT, const QHttp::MD5_t& _oldPass, const QString& _oldPassSalt, const QHttp::MD5_t& _newPass)
{
    Q_UNUSED(_JWT) Q_UNUSED(_oldPass) Q_UNUSED(_oldPassSalt)Q_UNUSED(_newPass)
    throw exHTTPNotImplemented("oh oh!");
}

bool Accounting::apiChangePassByUUID(const QHttp::RemoteIP_t& _REMOTE_IP, const MD5_t& _uuid, const MD5_t& _newPass)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_uuid) Q_UNUSED(_newPass)
    throw exHTTPNotImplemented("oh oh!");
}

QHttp::stuTable Accounting::apiGETLastSessions(QHttp::JWT_t _JWT, quint16 _fromID, quint16 _maxItems)
{
    Q_UNUSED(_JWT) Q_UNUSED(_fromID) Q_UNUSED(_maxItems)
    throw exHTTPNotImplemented("oh oh!");
}


Accounting::Accounting() :
    DAC(new DBManager::clsDAC){

    QHTTP_REGISTER_ENHANCED_ENUM(enuOAuthType);

    this->registerMyRESTAPIs();
}
