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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Account.h"
#include "QFieldValidator.h"

#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/APITokens.h"
#include "ORM/APITokenValidIPs.h"
#include "ORM/ActiveSessions.h"
#include "ORM/ApprovalRequest.h"
#include "ORM/BlockingRules.h"
#include "ORM/ForgotPassRequest.h"
#include "ORM/Invoice.h"
#include "ORM/IPBin.h"
#include "ORM/IPStats.h"
#include "ORM/PaymentOrders.h"
#include "ORM/Roles.h"
#include "ORM/Services.h"
#include "ORM/User.h"
#include "ORM/UserWallets.h"
#include "ORM/WalletTransactions.h"

namespace Targoman {
namespace API {

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Account);

TAPI::EncodedJWT_t Account::apiLogin(const TAPI::RemoteIP_t& _REMOTE_IP,
                                     const QString& _login,
                                     const TAPI::MD5_t& _pass,
                                     const QString& _salt,
                                     const QString& _tlps,
                                     bool _rememberMe,
                                     const TAPI::JSON_t& _sessionInfo,
                                     const TAPI::MD5_t& _fingerprint)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_tlps, "tlps");

    Authorization::validateIPAddress(_REMOTE_IP);

    return this->createJWT (_login,
                            Authentication::login(_REMOTE_IP, _login, _pass, _salt, _rememberMe, _tlps.split(",", QString::SkipEmptyParts), _sessionInfo.object(), _fingerprint),
                            _tlps);
}

TAPI::EncodedJWT_t Account::apiLoginByOAuth(const TAPI::RemoteIP_t& _REMOTE_IP,
                                            TAPI::enuOAuthType::Type _type,
                                            const QString& _oAuthToken,
                                            const QString& _tlps,
                                            const TAPI::JSON_t& _sessionInfo,
                                            const TAPI::MD5_t& _fingerprint)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    QString Login;
    Authentication::stuOAuthInfo OAuthInfo;

    switch(_type){
    case TAPI::enuOAuthType::Google:
        OAuthInfo = Authentication::retrieveGoogleUserInfo(_oAuthToken);
        break;
    case TAPI::enuOAuthType::Linkedin:
        OAuthInfo = Authentication::retrieveLinkedinUserInfo(_oAuthToken);
        break;
    case TAPI::enuOAuthType::Yahoo:
        OAuthInfo = Authentication::retrieveYahooUserInfo(_oAuthToken);
        break;
    case TAPI::enuOAuthType::Github:
        OAuthInfo = Authentication::retrieveGitHubUserInfo(_oAuthToken);
        break;
        /*    default:
        throw exHTTPNotImplemented("Invalid oAuth type");*/
    }
    return this->createJWT (OAuthInfo.Email,
                            Authentication::login(_REMOTE_IP, OAuthInfo.Email, nullptr, nullptr, true, _tlps.split(","), _sessionInfo.object(), _fingerprint),
                            _tlps);
}

TAPI::EncodedJWT_t Account::apiRefreshJWT(const TAPI::RemoteIP_t& _REMOTE_IP, TAPI::JWT_t _JWT)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    clsJWT JWT(_JWT);
    QString TLPs =  JWT.privatePart().value("tlps").toString();

    return this->createJWT (JWT.login(),
                            Authentication::updatePrivs(_REMOTE_IP, JWT.session(), TLPs),
                            TLPs);
}

quint32 Account::apiPUTSignup(const TAPI::RemoteIP_t& _REMOTE_IP,
                              const QString& _emailOrMobile,
                              const TAPI::MD5_t& _pass,
                              const QString& _role,
                              const QString& _name,
                              const QString& _family,
                              TAPI::JSON_t _specialPrivs,
                              qint8 _maxSessions)
{
    QString Type;
    if(QFV.email().isValid(_emailOrMobile)){
        if(QFV.emailNotFake().isValid(_emailOrMobile))
            Type = 'E';
        else
            throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");
    }else if (QFV.mobile().isValid(_emailOrMobile))
        Type = 'M';
    else
        throw exHTTPBadRequest("signup must be by a valid email or mobile");

    return static_cast<quint32>(this->callSP("AAA.sp_CREATE_signup", {
                                                 {"iBy", Type},
                                                 {"iLogin", _emailOrMobile},
                                                 {"iPass", _pass},
                                                 {"iRole", _role},
                                                 {"iIP", _REMOTE_IP},
                                                 {"iName", _name.isEmpty()? QVariant() : _name},
                                                 {"iFamily", _family.isEmpty()? QVariant() : _family},
                                                 {"iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs},
                                                 {"iMaxSessions", _maxSessions},
                                             }).spDirectOutputs().value("oUserID").toDouble());
}

bool Account::apiLogout(TAPI::JWT_t _JWT)
{
    clsJWT JWT(_JWT);
    this->callSP("AAA.sp_UPDATE_logout", {
                     {"iByUserID", clsJWT(_JWT).usrID()},
                     {"iSessionGUID", clsJWT(_JWT).session()},
                 });
    return true;
}

bool Account::apiCreateForgotPasswordLink(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _login, TAPI::enuForgotPassLinkVia::Type _via)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");

    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP("AAA.sp_CREATE_forgotPassRequest", {
                     {"iLogin", _login},
                     {"iVia", TAPI::enuForgotPassLinkVia::toStr(_via)},
                 });
    return true;
}

bool Account::apiChangePass(TAPI::JWT_t _JWT, const TAPI::MD5_t& _oldPass, const QString& _oldPassSalt, const TAPI::MD5_t& _newPass)
{
    QFV.asciiAlNum().maxLenght(20).validate(_oldPassSalt, "salt");

    this->callSP("AAA.sp_UPDATE_changePass", {
                     {"iUserID", clsJWT(_JWT).usrID()},
                     {"iOldPass", _oldPass},
                     {"iOldPassSalt", _oldPassSalt},
                     {"iNewPass", _newPass},
                 });
    return true;
}

bool Account::apiChangePassByUUID(const TAPI::RemoteIP_t& _REMOTE_IP, const TAPI::MD5_t& _uuid, const TAPI::MD5_t& _newPass)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP("AAA.sp_UPDATE_changePassByUUID", {
                     {"iUUID", _uuid},
                     {"iNewPass", _newPass},
                 });
    return true;
}

bool Account::apiPOSTApproveEmail(const TAPI::RemoteIP_t& _REMOTE_IP,
                                  const TAPI::MD5_t& _uuid)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP( "AAA.sp_UPDATE_acceptApproval", {
                      {"iUUID", _uuid},
                      {"iMobile", {}},
                  });
    return true;
}

bool Account::apiPOSTApproveMobile(const TAPI::RemoteIP_t& _REMOTE_IP,
                                   const TAPI::Mobile_t _mobile,
                                   const quint16& _code)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP( "AAA.sp_UPDATE_acceptApproval", {
                      {"iUUID", _code},
                      {"iMobile", _mobile},
                  });
    return true;
}

Account::Account() :
    clsRESTAPIWithActionLogs("AAA", "Account")
{
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuOAuthType);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuForgotPassLinkVia);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuUserStatus);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuUserSex);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuUserApproval);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuGenericStatus);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuAuditableStatus);

    this->addSubModule(new ActiveSessions);
    this->addSubModule(new APITokens);
    this->addSubModule(new APITokenValidIPs);
    this->addSubModule(new ApprovalRequest);
    this->addSubModule(new BlockingRules);
    this->addSubModule(new ForgotPassRequest);
    this->addSubModule(new Invoice);
    this->addSubModule(new IPBin);
    this->addSubModule(new IPStats);
    this->addSubModule(new PaymentOrders);
    this->addSubModule(new Roles);
    this->addSubModule(new Services);
    this->addSubModule(new User);
    this->addSubModule(new UserExtraInfo);
    this->addSubModule(new UserWallets);
    this->addSubModule(new WalletTransactions);
    this->addSubModule(new WalletBalances);
}

TAPI::EncodedJWT_t Account::createJWT(const QString _login, const QJsonObject& _result, const QString& _requiredTLPs)
{
    return clsJWT::createSigned({
                                    {JWTItems::usrLogin, _login},
                                    {JWTItems::usrName, _result["usrGivenName"]},
                                    {JWTItems::usrFamily, _result["usrFamilyName"]},
                                    {JWTItems::rolName, _result["rolName"]},
                                    {JWTItems::rolID, _result["rolID"]},
                                    {JWTItems::privs, _result["privs"]},
                                    {JWTItems::usrID, _result["usrID"]},
                                    {JWTItems::canChangePass, _result["hasPass"]},
                                    {JWTItems::usrApproval, TAPI::enuUserApproval::toStr(_result["usrApprovalState"].toString())},
                                    {JWTItems::usrStatus, TAPI::enuUserStatus::toStr(_result["usrStatus"].toString())},
                                },
                                QJsonObject({{"tlps",_requiredTLPs}}),
                                _result["ssnKey"].toString()
            );
}

}
}
