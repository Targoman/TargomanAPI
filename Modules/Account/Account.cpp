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
#include "Helpers/AAA/GenericEnums.hpp"
#include "Configs.h"

//#include "ORM/ActionLogs.h"
#include "ORM/APITokens.h"
#include "ORM/APITokenValidIPs.h"
#include "ORM/ActiveSessions.h"
#include "ORM/PaymentOrder.h"
#include "ORM/BlockingRules.h"
#include "ORM/ForgotPassRequest.h"
#include "ORM/IPBin.h"
#include "ORM/IPStats.h"
#include "ORM/Invoice.h"
#include "ORM/Roles.h"
#include "ORM/User.h"
#include "ORM/UserWallets.h"
#include "ORM/WalletTransactions.h"

#include "Helpers/ORM/clsActionLogs.h"

using namespace Targoman;
using namespace Targoman::API;
using namespace Targoman::API::AAA;
using namespace Targoman::API::Helpers::AAA;
using namespace QHttp;

void Account::init()
{}

QHttp::EncodedJWT_t Account::apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _login, const QHttp::MD5_t& _pass, const QString& _salt, const QString& _tlps, bool _rememberMe, const QHttp::JSON_t& _sessionInfo)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");
    QFV.optional(QFV.asciiAlNum(false, ",")).validate(_tlps, "tlps");

    Authorization::validateIPAddress(_REMOTE_IP);

    return this->createJWT (_login,
                            Authentication::login(_REMOTE_IP, _login, _pass, _salt, _rememberMe, _tlps.split(","), _sessionInfo.object()),
                            _tlps);
}

QHttp::EncodedJWT_t Account::apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP,
                                             enuOAuthType::Type _type,
                                             const QString& _oAuthToken,
                                             const QString& _tlps,
                                             const QHttp::JSON_t& _sessionInfo)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    QString Login;
    Authentication::stuOAuthInfo OAuthInfo;

    switch(_type){
    case enuOAuthType::Google:
        OAuthInfo = Authentication::retrieveGoogleUserInfo(_oAuthToken);
        break;
    case enuOAuthType::Linkedin:
        OAuthInfo = Authentication::retrieveLinkedinUserInfo(_oAuthToken);
        break;
    case enuOAuthType::Yahoo:
        OAuthInfo = Authentication::retrieveYahooUserInfo(_oAuthToken);
        break;
    case enuOAuthType::Github:
        OAuthInfo = Authentication::retrieveGitHubUserInfo(_oAuthToken);
        break;
    default:
        throw exHTTPNotImplemented("Invalid oAuth type");
    }
    return this->createJWT (OAuthInfo.Email,
                            Authentication::login(_REMOTE_IP, OAuthInfo.Email, nullptr, nullptr, true, _tlps.split(","), _sessionInfo.object()),
                            _tlps);
}

QHttp::EncodedJWT_t Account::apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP, const QHttp::JSON_t& _sessionInfo)
{
    Q_UNUSED(_REMOTE_IP) Q_UNUSED(_sessionInfo)
    throw exHTTPNotImplemented("oh oh!");
}

QHttp::EncodedJWT_t Account::apiRefreshJWT(QHttp::JWT_t _JWT)
{
    clsJWT JWT(_JWT);
    QString TLPs =  JWT.privatePart().value("tlps").toString();

    return this->createJWT (JWT.login(),
                            Authentication::updatePrivs(JWT.session(), TLPs),
                            TLPs);
}

quint32 Account::apiPUTSignup(const QHttp::RemoteIP_t& _REMOTE_IP,
                       const QString& _emailOrMobile,
                       const QHttp::MD5_t& _pass,
                       const QString& _role,
                       const QString& _name,
                       const QString& _family,
                       QHttp::JSON_t _specialPrivs,
                       qint8 _maxSessions)
{
    char Type;
    if(QFV.email().isValid(_emailOrMobile)){
        if(QFV.emailNotFake().isValid(_emailOrMobile))
            Type = 'E';
        else
            throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");
    }else if (QFV.mobile().isValid(_emailOrMobile))
        Type = 'M';
    else
        throw exHTTPBadRequest("signup must be by a valid email or mobile");

    return static_cast<quint32>(AAADACInstance().callSP ("","AAA.sp_CREATE_signup", {
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
    AAADACInstance().callSP ("","AAA.sp_UPDATE_changePassByUUID", {
                                 {"iUUID", _uuid},
                                 {"iNewPass", _newPass},
                             });
    return true;
}

Account::Account() :
    clsRESTAPIWithActionLogs(AAADACInstance(), "AAA", "Account")
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuOAuthType);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuForgotPassLinkVia);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuUserStatus);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuUserApproval);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuGenericStatus);

//    ActionLogs::instance().init();
    ActiveSessions::instance().init();
    APITokens::instance().init();
    APITokenValidIPs::instance().init();
    BlockingRules::instance().init();
    ForgotPassRequest::instance().init();
    Invoice::instance().init();
    IPBin::instance().init();
    IPStats::instance().init();
    PaymentOrders::instance().init();
    Roles::instance().init();
    User::instance().init();
    UserWallets::instance().init();
    WalletTransactions::instance().init();

    this->registerMyRESTAPIs();
}

EncodedJWT_t Account::createJWT(const QString _login, const QJsonObject& _result, const QString& _requiredTLPs)
{
    return this->createSignedJWT({
                                     {JWTItems::usrLogin, _login},
                                     {JWTItems::usrName, _result["usrGivenName"]},
                                     {JWTItems::usrFamily, _result["usrFamilyName"]},
                                     {JWTItems::rolName, _result["rolName"]},
                                     {JWTItems::rolID, _result["rolID"]},
                                     {JWTItems::privs, _result["privs"]},
                                     {JWTItems::usrID, _result["usrID"]},
                                     {JWTItems::usrApproval, enuUserApproval::toStr(_result["usrApprovalState"].toString())},
                                     {JWTItems::usrStatus, enuUserStatus::toStr(_result["usrStatus"].toString())},
                                 },
                                 QJsonObject({{"tlps",_requiredTLPs}}),
                                 Targoman::Apps::gConfigs::JWT::TTL.value(),
                                 _result["ssnKey"].toString()
            );
}

