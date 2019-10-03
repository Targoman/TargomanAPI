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

#include "Account.h"
#include "QFieldValidator.h"
#include "QHttp/QRESTServer.h"
#include "QHttp/intfAPIArgManipulator.h"
#include "Helpers/AAA/AAA.hpp"
#include "Helpers/AAA/PrivHelpers.h"

#include "ORM/User.h"

using namespace Targoman;
using namespace QHttp;

void Account::init()
{}

QHttp::EncodedJWT_t Account::apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _login, const QHttp::MD5_t& _pass, const QString& _salt, bool _rememberMe, const QHttp::JSON_t& _sessionInfo)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
    QFV.optional(QFV.json()).validate(_sessionInfo, "sessionInfo");

    Authorization::validateIPAddress(_REMOTE_IP);

    QJsonObject Result = Authentication::login(_login, _pass, _salt, QJsonDocument::fromJson(_sessionInfo.toUtf8()).object());
    return this->createSignedJWT({
                                     {JWTItems::usrLogin, _login},
                                     {JWTItems::usrName, Result["usrGivenName"]},
                                     {JWTItems::usrFamily, Result["usrFamilyName"]},
                                     {JWTItems::rolName, Result["rolName"]},
                                     {JWTItems::rolID, Result["rolID"]},
                                     {JWTItems::privs, Result["privs"]},
                                     {JWTItems::usrID, Result["usrID"]},
                                     {JWTItems::usrApproval, enuUserApproval::toStr(Result["usrApprovalState"].toString())},
                                     {JWTItems::usrStatus, enuUserStatus::toStr(Result["usrStatus"].toString())},
                                 },
                                 QJsonObject(),
                                 _rememberMe ? 7*24*3600 : 1800,
                                 Result["ssnKey"].toString()
            );
}

QHttp::EncodedJWT_t Account::apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP, enuOAuthType::Type _type, const QString& _AuthToken, const QHttp::JSON_t& _sessionInfo)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_type) Q_UNUSED(_AuthToken)Q_UNUSED(_sessionInfo)
            throw exHTTPNotImplemented("oh oh!");
}

QHttp::EncodedJWT_t Account::apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP, const QHttp::JSON_t& _sessionInfo)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_sessionInfo)
            throw exHTTPNotImplemented("oh oh!");
}

bool Account::apiLogout(QHttp::JWT_t _JWT)
{
    clsJWT JWT(_JWT);
    AAADACInstance().callSP("","AAA.sp_UPDATE_logout", {
                          {"iByUserID", clsJWT(_JWT).usrID()},
                          {"iSessionGUID", clsJWT(_JWT).session()},
                      });
    return true;
}

QHttp::MD5_t Account::apiCreateForgotPasswordLink(const RemoteIP_t& _REMOTE_IP, const QString& _login, enuForgotPassLinkVia::Type _via)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");

    Authorization::validateIPAddress(_REMOTE_IP);
    return AAADACInstance().callSP ("","AAA.sp_CREATE_forgotPassRequest", {
                                  {"iLogin", _login},
                                  {"iVia", _via},
                              }).spDirectOutputs().value("oUUID").toString();;
}

bool Account::apiChangePass(QHttp::JWT_t _JWT, const QHttp::MD5_t& _oldPass, const QString& _oldPassSalt, const QHttp::MD5_t& _newPass)
{
    QFV.asciiAlNum().maxLenght(20).validate(_oldPassSalt, "salt");

    AAADACInstance().callSP ("","AAA.sp_UPDATE_changePass", {
                           {"iUserID", clsJWT(_JWT).usrID()},
                           {"iOldPass", _oldPass},
                           {"iOldPassSalt", _oldPassSalt},
                           {"iNewPass", _newPass},
                       });
    return true;
}

bool Account::apiChangePassByUUID(const QHttp::RemoteIP_t& _REMOTE_IP, const MD5_t& _uuid, const MD5_t& _newPass)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    AAADACInstance().callSP ("","AAA.sp_UPDATE_changePass", {
                           {"iUUID", _uuid},
                           {"iNewPass", _newPass},
                       });
    return true;
}

Account::Account(){

    QHTTP_REGISTER_ENHANCED_ENUM(enuOAuthType);
    QHTTP_REGISTER_ENHANCED_ENUM(enuForgotPassLinkVia);

    User::instance().init();

    this->registerMyRESTAPIs();
}
