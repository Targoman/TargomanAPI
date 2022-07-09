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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "Account.h"
#include "libQFieldValidator/QFieldValidator.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "Interfaces/AAA/clsJWT.hpp"
#include "ORM/APITokens.h"
#include "ORM/APITokenValidIPs.h"
#include "ORM/ActiveSessions.h"
#include "ORM/ApprovalRequest.h"
#include "ORM/BlockingRules.h"
#include "ORM/ForgotPassRequest.h"
#include "ORM/Voucher.h"
#include "ORM/IPBin.h"
#include "ORM/Roles.h"
#include "ORM/IPStats.h"
#include "ORM/Payments.h"
#include "ORM/Service.h"
#include "ORM/User.h"
#include "ORM/UserWallets.h"
#include "ORM/WalletTransactions.h"
#include "ORM/Auth.h"
#include "ORM/Currency.h"
#include "Payment/PaymentLogic.h"
#include "Payment/intfPaymentGateway.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Interfaces/ORM/intfAlerts.h"

#include "Interfaces/Helpers/PhoneHelper.h"
#include "Interfaces/Helpers/SecurityHelper.h"
#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"
#include "Interfaces/Helpers/IteratorHelper.hpp"
#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

using namespace Targoman::API::AAA;

#include "Interfaces/Server/QJWT.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuOAuthType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuGender);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserApproval);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAuditableStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuSaleableType);

//TAPI_REGISTER_METATYPE(
//    COMPLEXITY_Complex,
//    Targoman::API::AccountModule,
//    stuMultiJWT,
//    [](const Targoman::API::AccountModule::stuMultiJWT& _value) -> QVariant{return QJsonObject({{"ssn", _value.Session}, {"lgn", _value.Login}}).toVariantMap();}
//);

//namespace Targoman::API {

//using namespace DBManager;
//using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman::API::AccountModule {

using namespace Payment;
using namespace ORM;

///@TODO: move this to config file
static QSet<QString> InvalidPasswords = {
    "d41d8cd98f00b204e9800998ecf8427e",
    "c4ca4238a0b923820dcc509a6f75849b",
    "c81e728d9d4c2f636f067f89cc14862c",
    "eccbc87e4b5ce2fe28308fd9f2a7baf3",
    "a87ff679a2f3e71d9181a67b7542122c",
    "e4da3b7fbbce2345d7772b0674a318d5",
    "1679091c5a880faf6fb5e6087eb1b2dc",
    "8f14e45fceea167a5a36dedd4bea2543",
    "c9f0f895fb98ab9159f51fd0297e236d",
    "45c48cce2e2d7fbdea1afc51c7c6ad26",
    "cfcd208495d565ef66e7dff9f98764da",
    "c20ad4d76fe97759aa27a0c99bff6710",
    "202cb962ac59075b964b07152d234b70",
    "81dc9bdb52d04dc20036dbd8313ed055",
    "827ccb0eea8a706c4c34a16891f84e7b",
    "21232f297a57a5a743894a0e4a801fc3",
    "4eae18cf9e54a0f62b44176d074cbe2f",
    "76419c58730d9f35de7ac538c2fd6737",
    "d8578edf8458ce06fbc5bb76a58c5ca4",
    "5ee43561ed4491c7d2b76f28574093fc"
};

TARGOMAN_DEFINE_ENUM(enuPaymentType,
                     Online = 'N',
                     Offline = 'F'
                     )


//tmplConfigurableArray<intfPaymentGateway::stuGateway> intfPaymentGateway::GatewayEndPoints (
//        AAA::Accounting::makeConfig("GatewayEndPoints"),
//        "Registered payment gateways",
//        0
//        );

tmplConfigurable<FilePath_t> Account::InvalidPasswordsFile (
    AAA::makeConfig("InvalidPasswordsFile"),
    "File where invalid pasword MD5s are stored",
    "",
    Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Readable),
    false>,
    "",
    "FILEPATH",
    "invalid-md5-passwords",
    enuConfigSource::Arg | enuConfigSource::File
);

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
TARGOMAN_IMPL_API_MODULE(Account)
TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Account, AAASchema);

Account::Account() :
    intfSQLBasedWithActionLogsModule(AccountDomain, AAASchema)
{
    TARGOMAN_API_IMPLEMENT_MIGRATIONS(Account, AAASchema)
    TARGOMAN_API_IMPLEMENT_ACTIONLOG(Account, AAASchema)

    this->addSubModule(&ActiveSessions::instance());
    this->addSubModule(&APITokens::instance());
    this->addSubModule(&APITokenValidIPs::instance());
    this->addSubModule(&ApprovalRequest::instance());
    this->addSubModule(&BlockingRules::instance());
    this->addSubModule(&ForgotPassRequest::instance());
    this->addSubModule(&Voucher::instance());
    this->addSubModule(&IPBin::instance());
    this->addSubModule(&IPStats::instance());
    this->addSubModule(&PaymentGatewayTypes::instance());
    this->addSubModule(&PaymentGateways::instance());
    this->addSubModule(&OnlinePayments::instance());
    this->addSubModule(&OfflinePayments::instance());
    this->addSubModule(&OfflinePaymentClaims::instance());
    this->addSubModule(&Roles::instance());
    this->addSubModule(&Service::instance());
    this->addSubModule(&User::instance());
    this->addSubModule(&UserExtraInfo::instance());
    this->addSubModule(&UserWallets::instance());
    this->addSubModule(&WalletTransactions::instance());
    this->addSubModule(&WalletsBalanceHistory::instance());
    this->addSubModule(&Auth::instance());
    this->addSubModule(&Currency::instance());

    if (Account::InvalidPasswordsFile.value().size()) {
        QFile InputFile(Account::InvalidPasswordsFile.value());
        if (InputFile.open(QIODevice::ReadOnly)) {
            QTextStream Stream(&InputFile);
            while (!Stream.atEnd())
                InvalidPasswords.insert(Stream.readLine().replace(QRegularExpression("#.*"), "").trimmed());
            InputFile.close();
        }
    }
}

//TAPI::EncodedJWT_t Account::createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services)
//{
//    return clsJWT::createSignedLogin(_remember, { { JWTItems::usrLogin, _login } }, QJsonObject({ { "svc", _services } }), _ssid);
//}

TAPI::EncodedJWT_t Account::createJWTAndSaveToActiveSession(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const QString _login,
    const stuActiveAccount& _activeAccount,
    const QString& _services
) {
    auto ServerTiming = _APICALLBOOM.createScopeTiming("jwt", "create");

    QJsonObject Payload = {
        { JWTItems::usrLogin,       _login },
        { JWTItems::usrID,          _activeAccount.Privs["usrID"] },
        { JWTItems::usrName,        _activeAccount.Privs["usrName"] },
        { JWTItems::usrFamily,      _activeAccount.Privs["usrFamily"] },
        { JWTItems::rolID,          _activeAccount.Privs["usr_rolID"] },
        { JWTItems::rolName,        _activeAccount.Privs["rolName"] },
        { JWTItems::privs,          _activeAccount.Privs["privs"] },
        { JWTItems::usrApproval,    TAPI::enuUserApproval::toStr(_activeAccount.Privs["usrApprovalState"].toString()) },
        { JWTItems::usrStatus,      TAPI::enuUserStatus::toStr(_activeAccount.Privs["usrStatus"].toString()) },
        { JWTItems::canChangePass,  _activeAccount.Privs["hasPass"] },
        { JWTItems::iat,            _activeAccount.Privs["Issuance"] },
    };

    TAPI::EncodedJWT_t JWT = QJWT::createSigned(
        Payload,
        QJsonObject({ { "svc", _services } }),
        _activeAccount.TTL,
        _activeAccount.Privs["ssnKey"].toString()
    );

    ServerTiming.finish();

    //-- save to active session -----
    this->callSP(APICALLBOOM_PARAM,
                 "spSession_UpdateJWT", {
                     { "iSSID", _activeAccount.Privs["ssnKey"].toString() },
                     { "iJWT", JWT },
                     { "iIssuance", Payload["iat"].toInt() },
                 });

    return JWT;
}

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/
QString IMPL_REST_GET_OR_POST(Account, normalizePhoneNumber, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _phone,
    QString _country
)) {
    return PhoneHelper::NormalizePhoneNumber(_phone, _country);
}

QVariantMap IMPL_REST_PUT(Account, signup, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile,
    TAPI::MD5_t _pass,
    QString _role,
    QString _name,
    QString _family,
    bool _enableEmailAlerts,
    bool _enableSMSAlerts,
    TAPI::JSON_t _specialPrivs,
    qint8 _maxSessions
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QFV/*.asciiAlNum()*/.maxLenght(50).validate(_role);

    if (_role.toLower() == "administrator" || _role.toLower() == "system" || _role.toLower() == "baseuser")
        throw exHTTPForbidden("Selected role is not allowed to signup");

    if (_pass.isEmpty())
        throw exHTTPBadRequest("Password must be provided.");

    if (InvalidPasswords.contains(_pass))
        throw exHTTPBadRequest("Invalid simple password");

    quint64 UserID = this->callSP(APICALLBOOM_PARAM,
                                  "spSignup", {
                                      { "iBy", Type },
                                      { "iLogin", _emailOrMobile },
                                      { "iPass", _pass },
                                      { "iRole", _role },
                                      { "iIP", _APICALLBOOM.getIP() },
                                      { "iName", _name.isEmpty()? QVariant() : _name },
                                      { "iFamily", _family.isEmpty()? QVariant() : _family },
                                      { "iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs },
                                      { "iMaxSessions", _maxSessions },
                                      { "iCreatorUserID", QVariant() },
                                      { "iEnableEmailAlerts", _enableEmailAlerts ? 1 : 0 },
                                      { "iEnableSMSAlerts", _enableSMSAlerts ? 1 : 0 },
                                  })
                                  .spDirectOutputs()
                                  .value("oUserID")
                                  .toDouble();

    return {
        { "type", Type == 'E' ? "email" : "mobile" },
        { "usrID", UserID },
    };
}

TAPI::EncodedJWT_t IMPL_REST_POST(Account, approveEmail, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _email,
    TAPI::MD5_t _uuid,
    bool _autoLogin,
    TAPI::CommaSeparatedStringList_t _services,
    bool _rememberMe,
    TAPI::JSON_t _sessionInfo,
    TAPI::MD5_t _fingerprint
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    _email = _email.toLower().trimmed();

    QJsonObject UserInfo = this->callSP(APICALLBOOM_PARAM,
                                        "spApproval_Accept", {
                                            { "iBy", "E" },
                                            { "iKey", _email },
                                            { "iCode", _uuid },
                                            { "iLogin", _autoLogin ? 1 : 0 },
                                            { "iLoginIP", _APICALLBOOM.getIP() },
                                            { "iLoginInfo", _sessionInfo.object() },
                                            { "iLoginRemember", _rememberMe ? 1 : 0 },
                                            { "iFingerPrint", _fingerprint.isEmpty() ? QVariant() : _fingerprint },
                                            { "iTTL", ApprovalRequest::EmailApprovalCodeTTL.value() },
                                        })
                                        .toJson(true)
                                        .object();

    if (_autoLogin == false)
        return TAPI::EncodedJWT_t();

    auto LoginInfo = PrivHelpers::processUserObject(
                         UserInfo,
                         {},
                         _services.split(",", QString::SkipEmptyParts)
                         );

    return this->createJWTAndSaveToActiveSession(APICALLBOOM_PARAM, _email, LoginInfo, _services);
//    return Targoman::API::AccountModule::stuMultiJWT({
//                                                         this->createLoginJWT(_rememberMe, _email, LoginInfo.Privs["ssnKey"].toString(), _services),
//                                                     });
}

TAPI::EncodedJWT_t IMPL_REST_POST(Account, approveMobile, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Mobile_t _mobile,
    quint32 _code,
    bool _autoLogin,
    TAPI::CommaSeparatedStringList_t _services,
    bool _rememberMe,
    TAPI::JSON_t _sessionInfo,
    TAPI::MD5_t _fingerprint
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    QJsonObject UserInfo = this->callSP(APICALLBOOM_PARAM,
                                        "spApproval_Accept", {
                                            { "iBy", "M" },
                                            { "iKey", _mobile },
                                            { "iCode", _code },
                                            { "iLogin", _autoLogin ? 1 : 0 },
                                            { "iLoginIP", _APICALLBOOM.getIP() },
                                            { "iLoginInfo", _sessionInfo.object() },
                                            { "iLoginRemember", _rememberMe ? 1 : 0 },
                                            { "iFingerPrint", _fingerprint.isEmpty() ? QVariant() : _fingerprint },
                                            { "iTTL", ApprovalRequest::MobileApprovalCodeTTL.value() },
                                        })
                                        .toJson(true)
                                        .object();

    if (_autoLogin == false)
        return TAPI::EncodedJWT_t();

    auto LoginInfo = PrivHelpers::processUserObject(
                         UserInfo,
                         {},
                         _services.split(",", QString::SkipEmptyParts)
                         );

    return this->createJWTAndSaveToActiveSession(APICALLBOOM_PARAM, _mobile, LoginInfo, _services);
//    return Targoman::API::AccountModule::stuMultiJWT({
//                                                         this->createLoginJWT(_rememberMe, _mobile, LoginInfo.Privs["ssnKey"].toString(), _services),
//                                                     });
}

TAPI::EncodedJWT_t IMPL_REST_GET_OR_POST(Account, login, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile,
    TAPI::MD5_t _pass,
    QString _salt,
    TAPI::CommaSeparatedStringList_t _services,
    bool _rememberMe,
    TAPI::JSON_t _sessionInfo,
    TAPI::MD5_t _fingerprint
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    stuActiveAccount LoginInfo = Authentication::login(_APICALLBOOM.getIP(),
                                                       _emailOrMobile,
                                                       _pass,
                                                       _salt,
                                                       _services.split(",", QString::SkipEmptyParts),
                                                       _rememberMe,
                                                       _sessionInfo.object(),
                                                       _fingerprint
                                                       );

    return this->createJWTAndSaveToActiveSession(APICALLBOOM_PARAM, _emailOrMobile, LoginInfo, _services);
//    return Targoman::API::AccountModule::stuMultiJWT({
//                                 this->createLoginJWT(_rememberMe, _emailOrMobile, LoginInfo.Privs["ssnKey"].toString(), _services),
//                             });
}

bool IMPL_REST_GET_OR_POST(Account, loginByMobileOnly, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Mobile_t _mobile,
    bool _signupIfNotExists,
    QString _signupRole,
    bool _signupEnableEmailAlerts,
    bool _signupEnableSMSAlerts
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QFV/*.asciiAlNum()*/.maxLenght(50).validate(_signupRole);

    if (_signupIfNotExists
            && (_signupRole.toLower() == "administrator"
                || _signupRole.toLower() == "system"
                || _signupRole.toLower() == "baseuser")
        )
        throw exHTTPForbidden("Selected role is not allowed to signup");

    QFV.mobile().validate(_mobile, "mobile");

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    this->callSP(APICALLBOOM_PARAM,
                 "spMobileVerifyCode_Request", {
                     { "iMobile", _mobile },
                     { "iSignupIfNotExists", _signupIfNotExists ? 1 : 0 },
                     { "iSignupRole", _signupRole },
                     { "iSignupEnableEmailAlerts", _signupEnableEmailAlerts ? 1 : 0 },
                     { "iSignupEnableSMSAlerts", _signupEnableSMSAlerts ? 1 : 0 },
                 });

    return true;
}

bool IMPL_REST_GET_OR_POST(Account, resendApprovalCode, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

//    this->callSP(APICALLBOOM_PARAM,
//    "spApprovalRequestAgain", {
//                     { "iBy", Type },
//                     { "iKey", _emailOrMobile },
//                     { "iIP", _APICALLBOOM.getIP() },
//                     { "iRecreateIfExpired", true },
//                     { "iTTL", Type == 'E' ? Account::EmailApprovalCodeTTL.value() : Account::MobileApprovalCodeTTL.value() },
//                 });
    this->callSP(APICALLBOOM_PARAM,
                 "spApproval_Request", {
                     { "iBy", Type },
                     { "iKey", _emailOrMobile },
                     { "iUserID", {} },
                     { "iPass", {} },
                     { "iSalt", {} }
//                     { "iIP", _APICALLBOOM.getIP() },
//                     { "iRecreateIfExpired", true },
//                     { "iTTL", Type == 'E' ? Account::EmailApprovalCodeTTL.value() : Account::MobileApprovalCodeTTL.value() },
                 });

    return true;
}

///@TODO: cache to ban users for every service
///@TODO: update cache for each module
///@TODO: JWT lifetime dynamic based on current hour
TAPI::EncodedJWT_t IMPL_REST_GET_OR_POST(Account, loginByOAuth, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::enuOAuthType::Type _type,
    QString _oAuthToken,
    TAPI::CommaSeparatedStringList_t _services,
    TAPI::JSON_t _sessionInfo,
    TAPI::MD5_t _fingerprint
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QString Login;

    Authentication::stuOAuthInfo OAuthInfo;
    ///@TODO: validate _oAuthToken

    switch (_type) {
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

//        default:
//            throw exHTTPNotImplemented("Invalid oAuth type");
    }

    auto LoginInfo = Authentication::login(
                         _APICALLBOOM.getIP(),
                         OAuthInfo.Email,
                         nullptr,
                         nullptr,
                         _services.split(","),
                         true,
                         _sessionInfo.object(),
                         _fingerprint
                         );

    return this->createJWTAndSaveToActiveSession(APICALLBOOM_PARAM, OAuthInfo.Email, LoginInfo, _services);
//    return Targoman::API::AccountModule::stuMultiJWT({
//                                 this->createLoginJWT(true, OAuthInfo.Email, LoginInfo.Privs["ssnKey"].toString(), _services),
//                             });
}

//Targoman::API::AccountModule::stuMultiJWT IMPL_REST_GET_OR_POST(Account, refreshJWT(
//        TAPI::JWT_t _loginJWT,
//        QString _services
//    )
//{
//    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

//    QJsonObject Obj;

//    clsJWT LoginJWT(_loginJWT);
//    QString Services = _services;
//    if (_services.isEmpty())
//        Services = LoginJWT.privatePart().value("svc").toString();

//    auto NewPrivs = Authentication::updatePrivs(_APICALLBOOM.getIP(), LoginJWT.session(), Services);
//    return Targoman::API::AccountModule::stuMultiJWT({
//                                 this->createLoginJWT(true, LoginJWT.login(), LoginJWT.session(), Services),
//                                 this->createJWTAndSaveToActiveSession(APICALLBOOM_PARAM, LoginJWT.login(), NewPrivs, Services)
//                             });
//}

bool IMPL_REST_GET_OR_POST(Account, logout, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM
)) {
    clsJWT JWT(_APICALLBOOM.getJWT());

    this->callSP(APICALLBOOM_PARAM,
                 "spLogout", {
                     { "iByUserID", JWT.usrID() },
                     { "iSessionGUID", JWT.session() },
                 });

    return true;
}

QString IMPL_REST_GET_OR_POST(Account, createForgotPasswordLink, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    this->callSP(APICALLBOOM_PARAM,
                 "spForgotPass_Request", {
                     { "iLogin", _emailOrMobile },
                     { "iBy", Type },
                 });

    return (Type == "E" ? "email" : "mobile");
}

#ifdef QT_DEBUG
QString IMPL_REST_POST(Account, fixtureGetLastForgotPasswordUUIDAndMakeAsSent, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile
)) {
    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QVariantMap Data = SelectQuery(ForgotPassRequest::instance())
                       .addCols({
                                    tblForgotPassRequest::Fields::fprCode,
                                    tblForgotPassRequest::Fields::fprStatus,
                                })
                       .innerJoinWith(tblForgotPassRequest::Relation::User)
                       .where({ Type == "E" ? tblUser::Fields::usrEmail : tblUser::Fields::usrMobile, enuConditionOperator::Equal, _emailOrMobile })
                       .andWhere({ tblForgotPassRequest::Fields::fprRequestedVia, enuConditionOperator::Equal, Type.at(0) })
                       .orderBy(tblForgotPassRequest::Fields::fprRequestDate, enuOrderDir::Descending)
                       .one()
                       ;

    QString Code = Data.value(tblForgotPassRequest::Fields::fprCode).toString();

    if (Code.isEmpty())
        throw exHTTPNotFound("No Code could be found");

    QString fprStatus = Data.value(tblForgotPassRequest::Fields::fprStatus).toString();
    if (fprStatus != "Sent") {
        quint64 RowsCount = UpdateQuery(ForgotPassRequest::instance())
                            .set(tblForgotPassRequest::Fields::fprStatus, enuFPRStatus::Sent)
                            .where({ tblForgotPassRequest::Fields::fprCode, enuConditionOperator::Equal, Code })
                            .execute(1)
                            ;
        if (RowsCount == 0)
            throw exHTTPNotFound("error in set as sent");
    }

    return Code;
}
#endif

bool IMPL_REST_GET_OR_POST(Account, changePassByUUID, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _emailOrMobile,
    QString _uuid,
    TAPI::MD5_t _newPass
)) {
    Authorization::validateIPAddress(_APICALLBOOM, _APICALLBOOM.getIP());

    QString Type = PhoneHelper::ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    this->callSP(APICALLBOOM_PARAM,
                 "spPassword_ChangeByCode", {
                     { "iBy", Type },
                     { "iLogin", _emailOrMobile },
                     { "iCode", _uuid },
                     { "iNewPass", _newPass },
                 });

    return true;
}

bool IMPL_REST_GET_OR_POST(Account, changePass, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::MD5_t _oldPass,
    QString _oldPassSalt,
    TAPI::MD5_t _newPass
)) {
    QFV.asciiAlNum().maxLenght(20).validate(_oldPassSalt, "salt");

    clsJWT JWT(_APICALLBOOM.getJWT());

    this->callSP(APICALLBOOM_PARAM,
                 "spPassword_Change", {
                     { "iUserID", JWT.usrID() },
                     { "iSessionGUID", JWT.session() },
                     { "iOldPass", _oldPass },
                     { "iOldPassSalt", _oldPassSalt },
                     { "iNewPass", _newPass },
                 });

    return true;
}

/*****************************************************************\
|* Voucher & Payments ********************************************|
\*****************************************************************/
//Targoman::API::AAA::stuPreVoucher IMPL_REST_POST(Account, mergeBasket, (
//    APICALLBOOM_TYPE_JWT_IMPL           &APICALLBOOM_PARAM,
//    Targoman::API::AAA::stuPreVoucher   _lastPreVoucher
//)) {
//    ///@TODO: must be implemented

//}

//Targoman::API::AAA::stuPreVoucher IMPL_REST_POST(Account, getBasket, (
//    APICALLBOOM_TYPE_JWT_IMPL   &APICALLBOOM_PARAM
//)) {
//    ///@TODO: must be implemented

//}

//bool IMPL_REST_POST(Account, deleteBasket, (
//    APICALLBOOM_TYPE_JWT_IMPL           &APICALLBOOM_PARAM,
//    Targoman::API::AAA::stuPreVoucher   _lastPreVoucher
//)) {
//    ///@TODO: must be implemented

//}

bool IMPL_REST_POST(Account, cancelVoucher, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _voucherID
)) {
    quint64 CurrentUserID = 0;

    if (Authorization::hasPriv(_APICALLBOOM, { "AAA:cancelVoucher" }) == false)
        CurrentUserID = _APICALLBOOM.getUserID();

    ///@TODO: implement this sp

    /*clsDACResult Result = */this->callSP(APICALLBOOM_PARAM,
                                       "spVoucher_Cancel", {
                                           { "iUserID", CurrentUserID },
                                           { "iVoucherID", _voucherID },
                                           { "iSetAsError", 0 },
                                       });
    return true;
}

/*
SELECT *
FROM tblWalletsTransactions
LEFT JOIN tblWalletsBalanceHistory
ON tblWalletsBalanceHistory.wbl_wltID = tblWalletsTransactions.wltID
LEFT JOIN tblVoucher
ON tblVoucher.vchID = tblWalletsTransactions.wlt_vchID
LEFT JOIN tblUserWallets
ON tblUserWallets.walID = tblWalletsTransactions.wlt_walID
ORDER BY wlt_walID, wltID -- , wltDateTime
-- wlt_walID, wltID
*/

///@TODO: select gateway (null|single|multiple) from service
///@TODO: check for common gateway voucher
/**
 * call Account/PaymentGateways/availableGatewayTypes for list of gateway types
 */
Targoman::API::AAA::stuVoucher IMPL_REST_POST(Account, finalizeBasket, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuPreVoucher _preVoucher,
    enuPaymentGatewayType::Type _gatewayType,
    QString _domain,
    qint64 _walID,
    QString _paymentVerifyCallback,
    bool _allowChargeWallet
)) {
    _domain = URLHelper::domain(_domain, true);

    ///scenario:
    ///1: create main expense voucher
    ///?: create pending vouchers per item
    ///2: compute wallet remaining
    ///2.1: process voucher
    ///2.2: create online/offline payment

    if (_gatewayType != enuPaymentGatewayType::COD) {
        if (_paymentVerifyCallback.isEmpty())
            throw exHTTPBadRequest("callback for non COD is mandatory");

        QFV.url().validate(_paymentVerifyCallback, "callBack");
    }
#ifndef QT_DEBUG
    if (_gatewayType == enuPaymentGatewayType::_DeveloperTest)
        throw exHTTPBadRequest("DeveloperTest not available");
#endif

    if (_preVoucher.Items.isEmpty())
        throw exHTTPBadRequest("Pre-Voucher is empty");

    checkPreVoucherSanity(_preVoucher);

    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    if (_preVoucher.UserID != CurrentUserID)
        throw exHTTPBadRequest("invalid pre-Voucher owner");

    ///1: create main expense voucher
//    Targoman::API::AAA::stuVoucher Voucher;

//    Voucher.Info = _preVoucher;

    ///@TODO: remove sign from prevoucher before converting to JSON
    ///@TODO: recalculate prevoucher to check either price change/package expiry/coupon limits/ etc.
    ///@TODO: reserve saleables before returning voucher
    ///@TODO: implement overall coupon at the end of checkout steps

    quint64 TotalPayed = 0;
    quint64 TotalFreezed = 0;

    //check prevoucher changes
    if (_preVoucher.VoucherID > 0) {
        QVariantMap VoucherInfo = SelectQuery(Voucher::instance())
                                     .addCols(Voucher::instance().SelectableColumnNames())
                                     .leftJoin(SelectQuery(Voucher::instance())
                                               .addCol(tblVoucher::Fields::vch_rootVchID)
                                               .addCol(DBExpression::VALUE("SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END)"), "TotalFreezed")
                                               .where({ tblVoucher::Fields::vchType, enuConditionOperator::In, QString("'%1','%2'")
                                                        .arg(QChar(enuVoucherType::Freeze)).arg(QChar(enuVoucherType::UnFreeze)) })
                                               .andWhere({ tblVoucher::Fields::vch_rootVchID, enuConditionOperator::NotNull })
                                               .groupBy(tblVoucher::Fields::vch_rootVchID)
                                               , "tmpFreeze"
                                               , { "tmpFreeze", tblVoucher::Fields::vch_rootVchID,
                                                   enuConditionOperator::Equal,
                                                   tblVoucher::Name, tblVoucher::Fields::vchID }
                                     )
                                     .addCol("tmpFreeze.TotalFreezed")
                                     .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, _preVoucher.VoucherID })
                                     .tryOne();

        //voucher (expense, new) deleted by calling cancelVoucher, but prevoucher in localDB exists:
        //  create new voucher
        if (VoucherInfo.isEmpty() == true)
            _preVoucher.VoucherID = 0;
        else {
            tblVoucher::DTO VoucherDTO;
            VoucherDTO.fromJson(QJsonObject::fromVariantMap(VoucherInfo));

            Targoman::API::AAA::stuPreVoucher PreVoucher;
            PreVoucher.fromJson(VoucherDTO.vchDesc.object());

            if ((PreVoucher.Round + PreVoucher.ToPay) != (_preVoucher.Round + _preVoucher.ToPay)) {
                clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                                   "spVoucher_Cancel", {
                                                       { "iUserID", CurrentUserID },
                                                       { "iVoucherID", _preVoucher.VoucherID },
                                                       { "iSetAsError", 0 },
                                                   });
                _preVoucher.VoucherID = 0;
            } else {
                TotalPayed = NULLABLE_GET_OR_DEFAULT(VoucherDTO.vchTotalPayed, 0);

                if (VoucherInfo.contains("TotalFreezed") && VoucherInfo["TotalFreezed"].isValid())
                    TotalFreezed = VoucherInfo["TotalFreezed"].toUInt();
            }
        }
    }

    if (_preVoucher.VoucherID == 0) {
        _preVoucher.VoucherID = Voucher::instance().Create(
                                    _APICALLBOOM,
                                    TAPI::ORMFields_t({
                                                          { tblVoucher::Fields::vch_usrID,      CurrentUserID },
                                                          { tblVoucher::Fields::vchType,        enuVoucherType::Invoice },
                                                          { tblVoucher::Fields::vchDesc,        _preVoucher.toJson().toVariantMap() },
                                                          { tblVoucher::Fields::vchTotalAmount, _preVoucher.ToPay },
                                                          { tblVoucher::Fields::vchStatus,      enuVoucherStatus::New },
                                                      }));

        _preVoucher.Sign.clear();
        _preVoucher.Sign = QString(voucherSign(QJsonDocument(_preVoucher.toJson()).toJson()).toBase64());

        Voucher::instance().Update(_APICALLBOOM,
                                  {},
                                  TAPI::ORMFields_t({
                                     { tblVoucher::Fields::vchDesc, _preVoucher.toJson().toVariantMap() },
                                  }),
                                  {
                                     { tblVoucher::Fields::vchID, _preVoucher.VoucherID }
                                  });
    }

    /// --------------------
//    Targoman::API::AAA::stuVoucher Voucher = payAndProcessBasket(
//        APICALLBOOM_PARAM,
//        _domain,
//        _preVoucher.VoucherID,
//        _gatewayType,
//        _preVoucher.ToPay,
//        _walID,
//        _paymentVerifyCallback
//    );

    //is basket free?
    if (_preVoucher.ToPay == 0)
        return Account::processVoucher(APICALLBOOM_PARAM, _preVoucher.VoucherID);

    //compute wallet remaining
    qint64 RemainingAfterWallet = static_cast<qint64>(_preVoucher.ToPay);

    RemainingAfterWallet -= TotalPayed;
    RemainingAfterWallet -= TotalFreezed;

    if (RemainingAfterWallet < 0)
        throw exHTTPInternalServerError("Remaining after wallet transaction is negative.");

    quint64 MustFreeze = 0;

    if ((RemainingAfterWallet > 0) && (_walID >= 0)) {
        auto Query = SelectQuery(UserWallets::instance())
                     .andWhere({ tblUserWallets::Fields::wal_usrID, enuConditionOperator::Equal, CurrentUserID });
                     ;

        if (_walID == 0)
            Query.andWhere({ tblUserWallets::Fields::walDefault, enuConditionOperator::Equal, true });
        else
            Query.andWhere({ tblUserWallets::Fields::walID, enuConditionOperator::Equal, _walID });

        tblUserWallets::DTO UserWalletsDTO = Query.one<tblUserWallets::DTO>();
        if (_walID == 0)
            _walID = UserWalletsDTO.walID;

        if (UserWalletsDTO.walStatus != enuUserWalletStatus::Active)
            throw exHTTPInternalServerError("The selected wallet can not be used");

//        if (UserWalletsDTO.walBalance < 0)
//            throw exHTTPInternalServerError("Wallet balance is negative");

        qint64 WalletAvailableAmount =
                UserWalletsDTO.walBalance
                - static_cast<qint64>(UserWalletsDTO.walFreezedAmount)
                - static_cast<qint64>(UserWalletsDTO.walMinBalance);

        if (WalletAvailableAmount <= 0)
            throw exHTTPInternalServerError("Not enough credit in your wallet.");

        MustFreeze = qMin(RemainingAfterWallet, WalletAvailableAmount);

        RemainingAfterWallet -= MustFreeze;

        if (RemainingAfterWallet < 0)
            throw exHTTPInternalServerError("Remaining after wallet transaction is negative.");
    }

    //process voucher
    if ((RemainingAfterWallet == 0) || (_gatewayType == enuPaymentGatewayType::COD)) {
        if (TotalFreezed > 0) {
            clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                               "spWallet_unFreezeAndDoTransaction", {
                                                   { "iUserID",             CurrentUserID },
                                                   { "iVoucherID",          _preVoucher.VoucherID },
                                                   { "iCheckTotalFreezed",  TotalFreezed },
                                               });
        }

        if (MustFreeze > 0) { //_preVoucher.ToPay > TotalPayed) {
            clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                               "spWalletTransaction_Create", {
                                                   { "iWalletID", _walID },
                                                   { "iVoucherID", _preVoucher.VoucherID },
                                                   { "ioAmount", 0 },
                                               });

//            RemainingAfterWallet -= Result.spDirectOutputs().value("ioAmount").toUInt();

//            if (RemainingAfterWallet != 0)
//                throw exHTTPInternalServerError("Error in wallet transaction");
        }

        if ((_gatewayType == enuPaymentGatewayType::COD) && (RemainingAfterWallet > 0)) {
            //create COD related vouchers

            //1: COD Credit
            stuPreVoucher PreVoucher_COD_Credit;
            PreVoucher_COD_Credit.UserID = CurrentUserID;
            PreVoucher_COD_Credit.Items.append(Targoman::API::AAA::stuVoucherItem(VOUCHER_ITEM_NAME_COD_CREDIT));
            PreVoucher_COD_Credit.Summary = "COD Credit";
            PreVoucher_COD_Credit.ToPay = RemainingAfterWallet;
            PreVoucher_COD_Credit.Type = enuPreVoucherType::CODCreadit;
            PreVoucher_COD_Credit.Sign = QString(voucherSign(QJsonDocument(PreVoucher_COD_Credit.toJson()).toJson()).toBase64());

//            quint64 COD_CreditVoucherID = Voucher::instance().Create(
//                                              _APICALLBOOM,
//                                              TAPI::ORMFields_t({
//                                                   { tblVoucher::Fields::vch_usrID,         CurrentUserID },
//                                                   { tblVoucher::Fields::vchType,           enuVoucherType::Credit },
//                                                   { tblVoucher::Fields::vchDesc,           PreVoucher_COD_Credit.toJson().toVariantMap() },
//                                                   { tblVoucher::Fields::vch_rootVchID,     _preVoucher.VoucherID },
//                                                   { tblVoucher::Fields::vchTotalAmount,    PreVoucher_COD_Credit.ToPay },
//                                                   { tblVoucher::Fields::vchStatus,         enuVoucherStatus::Finished },
//                                               }));

            quint64 COD_CreditVoucherID = this->callSP(APICALLBOOM_PARAM,
                                "spWallet_Increase", {
                                    { "iWalletID",      0 },
                                    { "iForUsrID",      CurrentUserID },
                                    { "iByUserID",      CurrentUserID },
                                    { "iType",          QString(static_cast<char>(enuVoucherType::Credit)) },
                                    { "iAmount",        PreVoucher_COD_Credit.ToPay },
                                    { "iDesc",          QJsonDocument(PreVoucher_COD_Credit.toJson()).toJson() },
                                    { "iRootVoucherID", _preVoucher.VoucherID },
                                }).spDirectOutputs().value("oVoucherID").toULongLong();

            //2:
            clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                               "spWalletTransaction_Create", {
                                                   { "iWalletID", 0 },
                                                   { "iVoucherID", _preVoucher.VoucherID },
                                                   { "ioAmount", PreVoucher_COD_Credit.ToPay },
                                               });

            //3: COD Debit
            stuPreVoucher PreVoucher_COD_Debit;
            PreVoucher_COD_Debit.UserID = CurrentUserID;
            PreVoucher_COD_Debit.Items.append(Targoman::API::AAA::stuVoucherItem(VOUCHER_ITEM_NAME_COD_DEBIT));
            PreVoucher_COD_Debit.Summary = "COD Debit";
            PreVoucher_COD_Debit.ToPay = RemainingAfterWallet;
            PreVoucher_COD_Debit.Type = enuPreVoucherType::CODDebit;
            PreVoucher_COD_Debit.Sign = QString(voucherSign(QJsonDocument(PreVoucher_COD_Debit.toJson()).toJson()).toBase64());

            quint64 COD_DebitVoucherID = Voucher::instance().Create(
                                              _APICALLBOOM,
                                              TAPI::ORMFields_t({
                                                   { tblVoucher::Fields::vch_usrID,         CurrentUserID },
                                                   { tblVoucher::Fields::vchType,           enuVoucherType::Debit },
                                                   { tblVoucher::Fields::vchDesc,           PreVoucher_COD_Debit.toJson().toVariantMap() },
                                                   { tblVoucher::Fields::vch_rootVchID,     _preVoucher.VoucherID },
                                                   { tblVoucher::Fields::vchTotalAmount,    PreVoucher_COD_Debit.ToPay },
                                                   { tblVoucher::Fields::vchStatus,         enuVoucherStatus::Finished },
                                               }));
        }

        return Account::processVoucher(APICALLBOOM_PARAM, _preVoucher.VoucherID);
    }

    //rem > 0 and non cod:

    //check max gateway amount per pay
    tblPaymentGatewayTypes::DTO PaymentGatewayTypesDTO = SelectQuery(PaymentGatewayTypes::instance())
                                                         .where({ tblPaymentGatewayTypes::Fields::pgtType, enuConditionOperator::Equal, _gatewayType })
                                                         .one<tblPaymentGatewayTypes::DTO>();
    stuVoucher Voucher;
    quint64 MustPay = 0;

    if (NULLABLE_HAS_VALUE(PaymentGatewayTypesDTO.pgtMaxRequestAmount)
        && (RemainingAfterWallet > NULLABLE_VALUE(PaymentGatewayTypesDTO.pgtMaxRequestAmount))
    ) {
        if (_allowChargeWallet == false)
            throw exHTTPInternalServerError("Permission to charge the wallet is not given");

        MustPay = NULLABLE_VALUE(PaymentGatewayTypesDTO.pgtMaxRequestAmount);

        Voucher.Info.UserID = CurrentUserID;
        Voucher.Info.Type = enuPreVoucherType::IncreaseWallet;
        Voucher.Info.Items.append(Targoman::API::AAA::stuVoucherItem(VOUCHER_ITEM_NAME_INC_WALLET, 0 /*_walID*/));
        Voucher.Info.Summary = "Increase wallet";
        Voucher.Info.ToPay = MustPay;
        Voucher.Info.Sign = QString(voucherSign(QJsonDocument(Voucher.Info.toJson()).toJson()).toBase64());

        Voucher.ID = Voucher::instance().Create(
                         _APICALLBOOM,
                         TAPI::ORMFields_t({
                                               { tblVoucher::Fields::vch_usrID,         CurrentUserID },
                                               { tblVoucher::Fields::vchType,           enuVoucherType::Credit },
                                               { tblVoucher::Fields::vchDesc,           Voucher.Info.toJson().toVariantMap() },
                                               { tblVoucher::Fields::vch_rootVchID,     _preVoucher.VoucherID },
                                               { tblVoucher::Fields::vchTotalAmount,    Voucher.Info.ToPay },
                                               { tblVoucher::Fields::vchStatus,         enuVoucherStatus::New },
                                           }));

        Voucher.Remained = Voucher.Info.ToPay;
        Voucher.Payed = 0;
    } else {
        MustPay = RemainingAfterWallet;
        Voucher.ID = _preVoucher.VoucherID;
        Voucher.Info = _preVoucher;
    }

    //freeze MustFreeze
    if (MustFreeze > 0) {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spWallet_Freeze", {
                                               { "iUserID",     CurrentUserID },
                                               { "iWalletID",   _walID },
                                               { "iVoucherID",  _preVoucher.VoucherID },
                                               { "iAomount",    MustFreeze },
                                           });
    }

    //goto online payment
    TAPI::MD5_t PaymentKey;
    Voucher.PaymentLink = PaymentLogic::createOnlinePaymentLink(
                              APICALLBOOM_PARAM,
                              _gatewayType,
                              _domain,
                              Voucher.ID,
                              Voucher.Info.Summary,
                              MustPay,
                              _paymentVerifyCallback,
                              PaymentKey,
                              0 /* _walID */
                              );

    //send master voucher info back to the client
    Voucher.Info = _preVoucher;
    Voucher.Remained = RemainingAfterWallet;
    Voucher.Payed = Voucher.Info.ToPay - Voucher.Remained;

    Voucher.PaymentKey = PaymentKey;

    return Voucher;
}

//Targoman::API::AAA::stuVoucher IMPL_REST_POST(Account, payForBasket, (
//    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
//    QString _domain,
//    quint64 _voucherID,
//    NULLABLE_TYPE(Targoman::API::AccountModule::enuPaymentGatewayType::Type) _gatewayType,
//    qint64 _amount,
//    qint64 _walID,
//    QString _paymentVerifyCallback
//)) {
//    return this->payAndProcessBasket(
//        APICALLBOOM_PARAM,
//        _domain,
//        _voucherID,
//        _gatewayType,
//        _amount,
//        _walID,
//        _paymentVerifyCallback
//    );
//}

/*
Targoman::API::AAA::stuVoucher Account::payAndProcessBasket(
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString _domain,
    quint64 _voucherID,
    NULLABLE_TYPE(enuPaymentGatewayType::Type) _gatewayType,
    qint64 _amount,
    qint64 _walID,
    QString _paymentVerifyCallback
) {
    if (NULLABLE_IS_NULL(_gatewayType)) {
        if (_walID == -1)
            throw exHTTPBadRequest("At least one of gatewayType or walID must be provided");
    } else if (NULLABLE_VALUE(_gatewayType) != enuPaymentGatewayType::COD) {
        if (_paymentVerifyCallback.isEmpty())
            throw exHTTPBadRequest("callback for non COD gatewayType is mandatory");

        QFV.url().validate(_paymentVerifyCallback, "callBack");
    }

//    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    tblVoucher::DTO VoucherDTO = SelectQuery(Voucher::instance())
                                 .addCols(Voucher::instance().SelectableColumnNames())
                                 .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, _voucherID })
                                 .one<tblVoucher::DTO>();

    if (VoucherDTO.vchStatus != enuVoucherStatus::New)
        throw exHTTPBadRequest("Only New vouchers allowed");

    if (VoucherDTO.vchType != enuVoucherType::Invoice)
        throw exHTTPBadRequest("Only Expense vouchers allowed");

    if (_amount < 0) {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spVoucher_GetRemaining", {
                                               { "iVoucherID", _voucherID },
                                           });

        _amount = Result.spDirectOutputs().value("oRemainingAmount").toUInt();
    }

    //compute wallet remaining
    qint64 RemainingAfterWallet = static_cast<qint64>(_amount);
    if ((_walID >= 0) && (RemainingAfterWallet > 0)) {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spWalletTransaction_Create", {
                                               { "iWalletID", _walID },
                                               { "iVoucherID", _voucherID },
                                               { "ioAmount", 0 },
                                           });

        RemainingAfterWallet -= Result.spDirectOutputs().value("ioAmount").toUInt();

        if (RemainingAfterWallet < 0)
            throw exHTTPInternalServerError("Remaining after wallet transaction is negative.");
    }

    //process voucher
    if (RemainingAfterWallet == 0)
        return Account::processVoucher(APICALLBOOM_PARAM, _voucherID);

    Targoman::API::AAA::stuVoucher Voucher;

    Voucher.ID = VoucherDTO.vchID;
    Voucher.Info.fromJson(VoucherDTO.vchDesc.object());

    //create online/offline payment
    switch (NULLABLE_VALUE(_gatewayType)) {
        case enuPaymentGatewayType::COD:
            //Do nothing as it will be created after information upload.
            break;

        default:
            TAPI::MD5_t PaymentKey;
            Voucher.PaymentLink = PaymentLogic::createOnlinePaymentLink(
                                      APICALLBOOM_PARAM,
                                      NULLABLE_VALUE(_gatewayType),
                                      _domain,
                                      _voucherID,
                                      Voucher.Info.Summary, //VoucherDTO.vchDesc.toJson(), //_preVoucher.Summary,
                                      RemainingAfterWallet,
                                      _paymentVerifyCallback,
                                      PaymentKey,
                                      _walID < 0 ? 0 : abs(_walID)
                                      );
            Voucher.PaymentKey = PaymentKey;
            break;
    }

    return Voucher;
}
*/

/**
 * @brief apiPOSTapproveOnlinePayment: called back from callback
 * @param _gateway
 * @param _domain
 * @param _pgResponse: ... voucherID ...
 * @return
 */
Targoman::API::AAA::stuVoucher IMPL_REST_POST(Account, approveOnlinePayment, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _paymentKey,
    QString _domain,
    TAPI::JSON_t _pgResponse
)) {
    _domain = URLHelper::domain(_domain, true);

    auto [PaymentID, VoucherID, TargetWalletID, PaymentAmount] = PaymentLogic::approveOnlinePayment(
            APICALLBOOM_PARAM,
            _paymentKey,
            _pgResponse,
            _domain);

    //----------------------------------
    QVariantMap VoucherInfo = SelectQuery(Voucher::instance())
                                 .addCols(Voucher::instance().SelectableColumnNames())
                                 .leftJoin(SelectQuery(Voucher::instance())
                                           .addCol(tblVoucher::Fields::vch_rootVchID)
                                           .addCol(DBExpression::VALUE("SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END)"), "TotalFreezed")
                                           .where({ tblVoucher::Fields::vchType, enuConditionOperator::In, QString("'%1','%2'")
                                                    .arg(QChar(enuVoucherType::Freeze)).arg(QChar(enuVoucherType::UnFreeze)) })
                                           .andWhere({ tblVoucher::Fields::vch_rootVchID, enuConditionOperator::NotNull })
                                           .groupBy(tblVoucher::Fields::vch_rootVchID)
                                           , "tmpFreeze"
                                           , { "tmpFreeze", tblVoucher::Fields::vch_rootVchID,
                                               enuConditionOperator::Equal,
                                               tblVoucher::Name, tblVoucher::Fields::vchID }
                                 )
                                 .addCol("tmpFreeze.TotalFreezed")
                                 .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, VoucherID })
                                 .one();

    tblVoucher::DTO VoucherDTO;
    VoucherDTO.fromJson(QJsonObject::fromVariantMap(VoucherInfo));

    quint32 CurrentUserID = VoucherDTO.vch_usrID;

    //----------------------------------
    QVariantMap RootVoucherInfo;
    tblVoucher::DTO RootVoucherDTO;
    if (NULLABLE_HAS_VALUE(VoucherDTO.vch_rootVchID)) {
        RootVoucherInfo = SelectQuery(Voucher::instance())
                          .addCols(Voucher::instance().SelectableColumnNames())
                          .leftJoin(SelectQuery(Voucher::instance())
                                    .addCol(tblVoucher::Fields::vch_rootVchID)
                                    .addCol(DBExpression::VALUE("SUM(tblVoucher.vchTotalAmount * CASE tblVoucher.vchType WHEN 'R' THEN 1 ELSE -1 END)"), "TotalFreezed")
                                    .where({ tblVoucher::Fields::vchType, enuConditionOperator::In, QString("'%1','%2'")
                                             .arg(QChar(enuVoucherType::Freeze)).arg(QChar(enuVoucherType::UnFreeze)) })
                                    .andWhere({ tblVoucher::Fields::vch_rootVchID, enuConditionOperator::NotNull })
                                    .groupBy(tblVoucher::Fields::vch_rootVchID)
                                    , "tmpFreeze"
                                    , { "tmpFreeze", tblVoucher::Fields::vch_rootVchID,
                                        enuConditionOperator::Equal,
                                        tblVoucher::Name, tblVoucher::Fields::vchID }
                          )
                          .addCol("tmpFreeze.TotalFreezed")
                          .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, NULLABLE_VALUE(VoucherDTO.vch_rootVchID) })
                          .one();

        RootVoucherDTO.fromJson(QJsonObject::fromVariantMap(RootVoucherInfo));
    }

    if ((VoucherDTO.vchType == enuVoucherType::Invoice)
        || (RootVoucherDTO.vchType == enuVoucherType::Invoice)
    ) {
        quint64 TotalAmount = 0;
        quint64 TotalPayed = 0;
        quint64 TotalFreezed = 0;

        stuPreVoucher PreVoucher;

        if (VoucherDTO.vchType == enuVoucherType::Invoice) {
            PreVoucher.fromJson(VoucherDTO.vchDesc.object());

            TotalAmount = VoucherDTO.vchTotalAmount;
            TotalPayed = NULLABLE_GET_OR_DEFAULT(VoucherDTO.vchTotalPayed, 0);
            TotalFreezed = VoucherInfo["TotalFreezed"].toUInt();
        } else {
            PreVoucher.fromJson(RootVoucherDTO.vchDesc.object());

            TotalAmount = RootVoucherDTO.vchTotalAmount;
            TotalPayed = NULLABLE_GET_OR_DEFAULT(RootVoucherDTO.vchTotalPayed, 0);
            TotalFreezed = RootVoucherInfo["TotalFreezed"].toUInt();
        }

        //----------------------
        clsDACResult DACResult1 = this->callSP(APICALLBOOM_PARAM,
                                 "spWalletTransactionOnPayment_Create", {
                                     { "iPaymentID",        PaymentID },
                                     { "iPaymentType",      QChar(enuPaymentType::Online) },
                                     { "iVoucherID",        VoucherID },
                                     { "ioTargetWalletID",  TargetWalletID },
                                     { "iAddExpense",       0 }, //just add to wallet
                                 });

        QVariantMap spOutputs = DACResult1.spDirectOutputs();
//        qint64 RemainingAfterWallet = spOutputs.value("oRemainingAfterWallet").toInt();
        if (TargetWalletID == 0)
            TargetWalletID = spOutputs.value("ioTargetWalletID").toUInt();

//        if (RemainingAfterWallet > 0)
//            return Targoman::API::AAA::stuVoucher();

//        return Account::processVoucher(_APICALLBOOM, VoucherID);

        quint64 MainExpenseVoucherID = (VoucherDTO.vchType == enuVoucherType::Invoice
                                        ? VoucherDTO.vchID
                                        : NULLABLE_VALUE(VoucherDTO.vch_rootVchID));

        //----------------------
        clsDACResult DACResult2 = this->callSP(APICALLBOOM_PARAM,
                                 "spWallet_Freeze", {
                                     { "iUserID",     CurrentUserID },
                                     { "iWalletID",   TargetWalletID },
                                     { "iVoucherID",  MainExpenseVoucherID },
                                     { "ioAmount",    PaymentAmount }, //0:freeze all avaiable amount of wallet
                                 });
        spOutputs = DACResult2.spDirectOutputs();
        TotalFreezed += spOutputs.value("ioAmount").toInt();

        //----------------------
        qint64 RemainingAfterWallet =
                TotalAmount
                 - TotalPayed
                 - TotalFreezed;

        if (RemainingAfterWallet < 0)
            throw exHTTPInternalServerError("Remaining after wallet transaction is negative.");

        if (RemainingAfterWallet > 0) {
            stuVoucher Voucher;

            Voucher.ID = MainExpenseVoucherID;
//            Voucher.ID = VoucherID;
            Voucher.Info = PreVoucher;
//            Voucher.PaymentLink
//            Voucher.PaymentKey
            Voucher.Payed = TotalPayed + TotalFreezed;
            Voucher.Remained = RemainingAfterWallet;
//            Voucher.Status

            return Voucher;
        }

        this->callSP(APICALLBOOM_PARAM,
                                 "spWallet_unFreezeAndDoTransaction", {
                                     { "iUserID",             CurrentUserID },
                                     { "iVoucherID",          MainExpenseVoucherID },
                                     { "iCheckTotalFreezed",  TotalFreezed },
                                 });

        return Account::processVoucher(_APICALLBOOM, MainExpenseVoucherID);
    }

    //otherwise
    clsDACResult DACResultO = this->callSP(APICALLBOOM_PARAM,
                             "spWalletTransactionOnPayment_Create", {
                                 { "iPaymentID", PaymentID },
                                 { "iPaymentType", QChar(enuPaymentType::Online) },
                                 { "iVoucherID", VoucherID },
                                 { "ioTargetWalletID", TargetWalletID },
                                 { "iAddExpense", 1 },
                             });

    QVariantMap spOutputs = DACResultO.spDirectOutputs();
    qint64 RemainingAfterWallet = spOutputs.value("oRemainingAfterWallet").toInt();
    if (TargetWalletID == 0)
        TargetWalletID = spOutputs.value("ioTargetWalletID").toUInt();

    if (RemainingAfterWallet > 0)
        return Targoman::API::AAA::stuVoucher();

    return Account::processVoucher(_APICALLBOOM, VoucherID);




//    } catch (...) {
//        ///@TODO: VERY IMPORTANT: reject gateway verify if 'C' type of tblWalletsTransactions not created in sp yet


//        this->Update(Voucher::instance(),
//                     SYSTEM_USER_ID,
//                     {},
//                     TAPI::ORMFields_t({
//                                           { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
//                                       }),
//                     {
//                         { tblVoucher::Fields::vchID, VoucherID }
//                     });
//        throw;
//    }
}

///@TODO: implement auto verify daemon OJO on failed payments in the daemon

/**
 * @callby:
 *     operator
 *     owner
 */
quint64 IMPL_REST_POST(Account, claimOfflinePayment, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    QString _bank,
    QString _receiptCode,
    TAPI::Date_t _receiptDate,
    quint32 _amount,
//    NULLABLE_TYPE(quint64) voucherID,
    quint64 _walID,
    QString _note
)) {
    /*if (NULLABLE_HAS_VALUE(voucherID)) {
        QJsonObject VoucherInfo = QJsonObject::fromVariantMap(SelectQuery(Voucher::instance())
//            .addCol(tblVoucher::Fields::vchTotalAmount)
            .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, NULLABLE_VALUE(voucherID) })
            .one()
        );
        tblVoucher::DTO VoucherDTO;
        VoucherDTO.fromJson(VoucherInfo);

        //check operator or owner
        if (Authorization::hasPriv(_APICALLBOOM, { "AAA:claimOfflinePayment" }) == false) {
            if (VoucherDTO.vch_usrID != _APICALLBOOM.getUserID())
                throw exAuthorization("Voucher is not yours");
        }

//        if (Voucher.vchTotalAmount > _amount)
//            throw exAuthorization("Voucher total amount is greater than provided amount");
    }*/

    QFV.unicodeAlNum(true).maxLenght(50).validate(_bank, "bank");
    QFV.unicodeAlNum(true).maxLenght(50).validate(_receiptCode, "receiptCode");

    QVariantMap CreateParams = {
        { "ofpcBank", _bank },
        { "ofpcReceiptCode", _receiptCode },
        { "ofpcReceiptDate", _receiptDate },
        { "ofpcAmount", _amount },
    };

    if ((_note.isNull() == false) && (_note.trimmed().length() > 0))
        CreateParams.insert("ofpcNotes", _note.trimmed());

    /*if (NULLABLE_HAS_VALUE(voucherID))
        CreateParams.insert("ofpc_vchID", NULLABLE_VALUE(voucherID));
    */

    if (_walID > 0)
        CreateParams.insert("ofpcTarget_walID", _walID);

    return this->Create(OfflinePaymentClaims::instance(),
                        _APICALLBOOM,
                        TAPI::ORMFields_t(CreateParams));
}

/**
 * @callby:
 *     operator
 *     owner
 */
bool IMPL_REST_POST(Account, rejectOfflinePayment, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _offlinePaymentClaimID
)) {
    QJsonObject PaymentInfo = QJsonObject::fromVariantMap(SelectQuery(OfflinePaymentClaims::instance())
        .addCols(OfflinePaymentClaims::instance().SelectableColumnNames())
//        .addCols(Voucher::instance().SelectableColumnNames())
//        .leftJoin(tblVoucher::Name)
        .where({ tblOfflinePaymentClaims::Fields::ofpcID, enuConditionOperator::Equal, _offlinePaymentClaimID })
        .one()
    );

    tblOfflinePaymentClaims::DTO OfflinePaymentClaimDTO;
    OfflinePaymentClaimDTO.fromJson(PaymentInfo);

//    tblVoucher::DTO VoucherDTO;
//    VoucherDTO.fromJson(PaymentInfo);

    //check operator or owner
    if (Authorization::hasPriv(_APICALLBOOM, { "AAA:rejectOfflinePayment" }) == false) {
        if (OfflinePaymentClaimDTO.ofpcCreatedBy_usrID != _APICALLBOOM.getUserID())
            throw exAuthorization("This voucher is not yours");
    }

    if (OfflinePaymentClaimDTO.ofpcStatus != enuPaymentStatus::New)
        throw exAuthorization("Only new offline payments are allowed.");

    this->Update(OfflinePaymentClaims::instance(),
                 _APICALLBOOM,
                 {},
                 TAPI::ORMFields_t({
                     { tblOfflinePaymentClaims::Fields::ofpcStatus, enuPaymentStatus::Rejected }
                 }),
                 {
                     { tblOfflinePaymentClaims::Fields::ofpcID, _offlinePaymentClaimID }
                 });

    return true;
}

/**
 * @callby:
 *     operator
 */
Targoman::API::AAA::stuVoucher IMPL_REST_POST(Account, approveOfflinePayment, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _offlinePaymentClaimID
)) {
    tblOfflinePaymentClaims::DTO OfflinePaymentClaimDTO = SelectQuery(OfflinePaymentClaims::instance())
        .addCols(OfflinePaymentClaims::instance().SelectableColumnNames())
//        .addCols(Voucher::instance().SelectableColumnNames())
//        .innerJoin(tblVoucher::Name)
        .where({ tblOfflinePaymentClaims::Fields::ofpcID, enuConditionOperator::Equal, _offlinePaymentClaimID })
        .one<tblOfflinePaymentClaims::DTO>();

    //check operator
    if (Authorization::hasPriv(_APICALLBOOM, { "AAA:approveOfflinePayment" }) == false)
        throw exAuthorization("Access denied");

    if (OfflinePaymentClaimDTO.ofpcStatus != enuPaymentStatus::New)
        throw exAuthorization("Only new offline payments are allowed.");

//    if (ApprovalLimit > 0) {
//        if (Voucher.value(tblVoucher::Fields::vchTotalAmount).toLongLong() > ApprovalLimit)
//            throw exAuthorization("Voucher total amount is greater than your approval limit");
//    }

//    QFV.unicodeAlNum(true).maxLenght(50).validate(_bank, "bank");
//    QFV.unicodeAlNum(true).maxLenght(50).validate(_receiptCode, "receiptCode");

    quint64 _walID = NULLABLE_GET_OR_DEFAULT(OfflinePaymentClaimDTO.ofpcTarget_walID, 0);
    quint64 VoucherID;

    if (NULLABLE_IS_NULL(OfflinePaymentClaimDTO.ofpc_vchID)) {
        Targoman::API::AAA::stuVoucher Voucher;

        Voucher.Info.UserID = _APICALLBOOM.getUserID();
        Voucher.Info.Type = enuPreVoucherType::IncreaseWallet;
        Voucher.Info.Items.append(Targoman::API::AAA::stuVoucherItem(VOUCHER_ITEM_NAME_INC_WALLET, _walID));
        Voucher.Info.Summary = "Increase wallet";
        Voucher.Info.ToPay = OfflinePaymentClaimDTO.ofpcAmount;
        Voucher.Info.Sign = QString(voucherSign(QJsonDocument(Voucher.Info.toJson()).toJson()).toBase64());

        Voucher.ID = this->Create(Voucher::instance(),
                                  _APICALLBOOM,
                                  TAPI::ORMFields_t({
                                                        { tblVoucher::Fields::vch_usrID, _APICALLBOOM.getUserID() },
    //                                                    { tblVoucher::Fields::vchDesc, QJsonDocument(Voucher.Info.toJson()).toJson().constData() },
                                                        { tblVoucher::Fields::vchDesc, Voucher.Info.toJson().toVariantMap() },
                                                        { tblVoucher::Fields::vchTotalAmount, Voucher.Info.ToPay },
                                                        { tblVoucher::Fields::vchType, Targoman::API::AAA::enuVoucherType::Credit },
                                                        { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::New },
                                                    }));

        VoucherID = Voucher.ID;
    } else
        VoucherID = NULLABLE_VALUE(OfflinePaymentClaimDTO.ofpc_vchID);

    QVariantMap CreateParams = {
        { tblOfflinePayments::Fields::ofp_vchID, VoucherID },
        { tblOfflinePayments::Fields::ofpBank, OfflinePaymentClaimDTO.ofpcBank },
        { tblOfflinePayments::Fields::ofpReceiptCode, OfflinePaymentClaimDTO.ofpcReceiptCode },
        { tblOfflinePayments::Fields::ofpReceiptDate, OfflinePaymentClaimDTO.ofpcReceiptDate },
        { tblOfflinePayments::Fields::ofpAmount, OfflinePaymentClaimDTO.ofpcAmount },
        { tblOfflinePayments::Fields::ofpNotes, OfflinePaymentClaimDTO.ofpcNotes.trimmed().size() ? OfflinePaymentClaimDTO.ofpcNotes.trimmed() : QVariant() },
        { tblOfflinePayments::Fields::ofpStatus, enuPaymentStatus::Payed },
    };

    if (NULLABLE_HAS_VALUE(OfflinePaymentClaimDTO.ofpcTarget_walID))
            CreateParams.insert(tblOfflinePayments::Fields::ofpTarget_walID, NULLABLE_VALUE(OfflinePaymentClaimDTO.ofpcTarget_walID));

    quint64 PaymentID = this->Create(OfflinePayments::instance(),
                 _APICALLBOOM,
                 TAPI::ORMFields_t(CreateParams));

    this->Update(OfflinePaymentClaims::instance(),
                 _APICALLBOOM,
                 {},
                 TAPI::ORMFields_t({
                     { tblOfflinePaymentClaims::Fields::ofpcStatus, enuPaymentStatus::Succeded }
                 }),
                 {
                     { tblOfflinePaymentClaims::Fields::ofpcID, _offlinePaymentClaimID }
                 });

    try {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spWalletTransactionOnPayment_Create", {
                                               { "iPaymentID", PaymentID },
                                               { "iPaymentType", QChar(enuPaymentType::Offline) },
                                               { "iVoucherID", VoucherID },
                                               { "ioTargetWalletID", _walID },
                                               { "iAddExpense", 1 },
                                           });

        qint64 RemainingAfterWallet = Result.spDirectOutputs().value("oRemainingAfterWallet").toInt();

        if (RemainingAfterWallet > 0)
            return Targoman::API::AAA::stuVoucher();

        return Account::processVoucher(_APICALLBOOM, VoucherID);

    }  catch (...) {
        this->Update(OfflinePaymentClaims::instance(),
                     _APICALLBOOM,
                     {},
                     TAPI::ORMFields_t({
                         { tblOfflinePaymentClaims::Fields::ofpcStatus, enuPaymentStatus::Error }
                     }),
                     {
                         { tblOfflinePaymentClaims::Fields::ofpcID, _offlinePaymentClaimID }
                     });

        this->Update(OfflinePayments::instance(),
                     _APICALLBOOM,
                     {},
                     TAPI::ORMFields_t({
                         { tblOfflinePayments::Fields::ofpStatus, enuPaymentStatus::Error }
                     }),
                     {
                         { tblOfflinePayments::Fields::ofpID, PaymentID }
                     });


        //        this->Update(Voucher::instance(),
//                    SYSTEM_USER_ID,
//                    {},
//                    TAPI::ORMFields_t({
//                        { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
//                    }),
//                    {
//                        { tblVoucher::Fields::vchID, OfflinePaymentClaim.ofpc_vchID }
//                    });
        throw;
    }
}

/*
Targoman::API::AAA::stuVoucher IMPL_REST_POST(Account, approveOfflinePayment_withBankInfo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _vchID,
    const QString& _bank,
    const QString& _receiptCode,
    TAPI::Date_t _receiptDate,
    quint32 _amount,
    quint64 _walID,
    const QString& _note
)) {
    qint64 ApprovalLimit = Authorization::getPrivValue(_APICALLBOOM, "AAA:approveOffline:maxAmount").toLongLong();
    if (ApprovalLimit == 0)
        throw exAuthorization("Not enough access for offline approval");

    if (ApprovalLimit > 0) {
//        QVariantMap Voucher = Voucher::instance().selectFromTable({}, {}, QString::number(_vchID), 0, 1, tblVoucher::Fields::vchTotalAmount).toMap();
        QVariantMap Voucher = SelectQuery(Voucher::instance())
            .addCol(tblVoucher::Fields::vchTotalAmount)
            .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, _vchID })
            .one();

        if (Voucher.value(tblVoucher::Fields::vchTotalAmount).toLongLong() > ApprovalLimit)
            throw exAuthorization("Voucher total amount is greater than your approval limit");
    }

    QFV.unicodeAlNum(true).maxLenght(50).validate(_bank, "bank");
    QFV.unicodeAlNum(true).maxLenght(50).validate(_receiptCode, "receiptCode");

    QVariantMap CreateParams = {
        { "ofp_vchID", _vchID },
        { "ofpBank", _bank },
        { "ofpReceiptCode", _receiptCode },
        { "ofpReceiptDate", _receiptDate },
        { "ofpAmount", _amount },
        { "ofpNote", _note.trimmed().size() ? _note.trimmed() : QVariant() }
    };
    if (_walID > 0)
        CreateParams.insert("ofpTarget_walID", _walID);

    quint64 PaymentID = this->Create(OfflinePayments::instance(),
        _APICALLBOOM,
        TAPI::ORMFields_t(CreateParams));

    try {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spWalletTransactionOnPayment_Create", {
                                               { "iPaymentID", PaymentID },
                                               { "iPaymentType", QChar(enuPaymentType::Offline) },
                                               { "iVoucherID", _vchID },
                                               { "ioTargetWalletID", _walID },
                                               { "iAddExpense", 1 },
                                           });

        qint64 RemainingAfterWallet = Result.spDirectOutputs().value("oRemainingAfterWallet").toInt();

        if (RemainingAfterWallet > 0)
            return Targoman::API::AAA::stuVoucher();

        return Account::processVoucher(_APICALLBOOM.getUserID(), _vchID);

    }  catch (...) {
        this->Update(OfflinePayments::instance(),
                     _APICALLBOOM,
                     {},
                     TAPI::ORMFields_t({
                         { tblOfflinePayments::Fields::ofpStatus, enuPaymentStatus::Error }
                     }),
                     {
                         { tblOfflinePayments::Fields::ofpID, PaymentID }
                     });

//        this->Update(Voucher::instance(),
//                    SYSTEM_USER_ID,
//                    {},
//                    TAPI::ORMFields_t({
//                        { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
//                    }),
//                    {
//                        { tblVoucher::Fields::vchID, _vchID }
//                    });
        throw;
    }
}
*/

Targoman::API::AAA::stuVoucher Account::processVoucher(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    quint64 _userID,
    quint64 _voucherID
) {
//    try {
        clsDACResult Result = this->callSP(APICALLBOOM_PARAM,
                                           "spVoucher_GetRemaining", {
                                               { "iVoucherID", _voucherID },
                                           });
        quint64 RemainingAmount = Result.spDirectOutputs().value("oRemainingAmount").toUInt();
        if (RemainingAmount != 0)
            throw exHTTPInternalServerError("This voucher has not been paid in full");

        tblVoucher::DTO VoucherDTO = SelectQuery(Voucher::instance())
//                                     .addCols(Voucher::instance().SelectableColumnNames())
                                     .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, _voucherID })
                                     .one<tblVoucher::DTO>();

        if (VoucherDTO.vchStatus != enuVoucherStatus::New)
            throw exHTTPInternalServerError("only NEW vouchers are allowed to process");

        Targoman::API::AAA::stuPreVoucher PreVoucher;
        PreVoucher.fromJson(VoucherDTO.vchDesc.object());
//        if (VoucherInfo.vchDesc.canConvert<QJsonObject>())
//            PreVoucher.fromJson(VoucherInfo.vchDesc.toJsonObject());
//        else if (VoucherDesc.canConvert<QVariantMap>())
//            PreVoucher.fromJson(QJsonObject::fromVariantMap(VoucherDesc.toMap()));
//        else
//            throw exHTTPInternalServerError(QString("Voucher with ID: %1 not found or invalid json.").arg(_voucherID));

//        this->Update(Voucher::instance(),
//                     SYSTEM_USER_ID,
//                     {},
//                     TAPI::ORMFields_t({
//                                           { tblVoucher::Fields::vchStatus, Targoman::API::AAA::enuVoucherStatus::WaitForProcess }
//                                       }),
//                     {
//                         { tblVoucher::Fields::vchID, _voucherID }
//                     });

        if (PreVoucher.Items.isEmpty())
            throw exHTTPInternalServerError(QString("Voucher with ID: %1 has not any items.").arg(_voucherID));

        QVariantList Services = SelectQuery(Service::instance())
                .addCols({
                             tblService::Fields::svcID,
                             tblService::Fields::svcName,
                             tblService::Fields::svcProcessVoucherItemEndPoint,
                         })
                .all();

        if (Services.isEmpty())
            throw exHTTPInternalServerError("There is no services registered.");

        QVariantMap vchProcessResult = VoucherDTO.vchProcessResult.object().toVariantMap();
        quint8 ErrorCount = 0;

        //1: process voucher items
        foreach (stuVoucherItem VoucherItem, PreVoucher.Items) {

            QVariantMap ItemResult;

            if (vchProcessResult.contains(VoucherItem.UUID)) {
                ItemResult = vchProcessResult[VoucherItem.UUID].toMap();

                if (ItemResult.contains("status")
                    && (ItemResult["status"] == enuVoucherItemProcessStatus::Processed)
                ) {
                    continue;
                }
            }

            //lookup services
            try {
                /*auto [RunCount, OkCount] = */IteratorHelper::ConstIterator(Services)
                        .where([&VoucherItem](auto _service) {
                            QVariantMap ServiceInfo = _service.toMap();
                            return (ServiceInfo.value(tblService::Fields::svcName) == VoucherItem.Service);
                        })
                        .runFirst([/*&APICALLBOOM_PARAM, */&VoucherDTO, &_voucherID, &VoucherItem, &ItemResult, &vchProcessResult](auto _service) -> bool {
                            QVariantMap ServiceInfo = _service.toMap();

                            NULLABLE_TYPE(QString) ProcessVoucherItemEndPoint;
                            TAPI::setFromVariant(ProcessVoucherItemEndPoint, ServiceInfo.value(tblService::Fields::svcProcessVoucherItemEndPoint));

                            //bypass process by end point?
                            if (NULLABLE_HAS_VALUE(ProcessVoucherItemEndPoint)) {
                                stuVoucherItemForTrustedAction VoucherItemForTrustedAction;
                                VoucherItemForTrustedAction.UserID = VoucherDTO.vch_usrID; //APICALLBOOM_PARAM.getUserID();
                                VoucherItemForTrustedAction.VoucherID = _voucherID;
                                VoucherItemForTrustedAction.VoucherItem = VoucherItem;
                                VoucherItemForTrustedAction.Sign.clear();
                                VoucherItemForTrustedAction.Sign = QString(voucherSign(QJsonDocument(VoucherItemForTrustedAction.toJson()).toJson()).toBase64());

                                QVariant Result = RESTClientHelper::callAPI(
                                    RESTClientHelper::POST,
                                    NULLABLE_GET_OR_DEFAULT(ProcessVoucherItemEndPoint, ""),
                                    {},
                                    {
                                        { "data", VoucherItemForTrustedAction.toJson().toVariantMap() },
                                    }
                                );

                                if ((Result.isValid() == false) || (Result.toBool() == false))
                                    throw exHTTPInternalServerError(QString("error in process voucher item %1:%2").arg(_voucherID).arg(VoucherItem.UUID));

                                ItemResult["status"] = QChar(enuVoucherItemProcessStatus::Processed);
                                if (ItemResult.contains("error"))
                                    ItemResult.remove("error");

                                vchProcessResult[VoucherItem.UUID] = ItemResult;
                            } //if (NULLABLE_HAS_VALUE(ProcessVoucherItemEndPoint))
                            else {
                                throw exHTTPInternalServerError("Item service has not ProcessVoucherItemEndPoint");
                            }

                            return true;
                        });
            } catch (std::exception &_exp) {
                ++ErrorCount;

                ItemResult["status"] = QChar(enuVoucherItemProcessStatus::Error);
                ItemResult["error"] = _exp.what();

                vchProcessResult[VoucherItem.UUID] = ItemResult;
            }
        } // foreach (Targoman::API::AAA::stuVoucherItem VoucherItem, PreVoucher.Items)

        //2: change voucher status to Targoman::API::AAA::enuVoucherStatus::Finished
        Voucher::instance().Update(APICALLBOOM_PARAM, //SYSTEM_USER_ID,
                                   {},
                                   TAPI::ORMFields_t({
                                      { tblVoucher::Fields::vchStatus, (ErrorCount == 0
                                        ? Targoman::API::AAA::enuVoucherStatus::Finished
                                        : Targoman::API::AAA::enuVoucherStatus::Error
                                      )},
                                      { tblVoucher::Fields::vchProcessResult, vchProcessResult },
                                      { tblVoucher::Fields::vchSettled, true }
                                   }),
                                   {
                                      { tblVoucher::Fields::vchID, _voucherID }
                                   });

        //--------------------------
        /*
            //if noerror
            ///TODO: create pending vouchers per item

            //vchProcessResult not filled before and it is empty
            QVariantMap vchProcessResult;

            foreach (stuVoucherItem VoucherItem, _preVoucher.Items) {
                if (VoucherItem.PendingVouchers.isEmpty())
                    continue;

                QVariantMap AdditionalVouchers;

                foreach (stuPendingVoucher PendingVoucher, VoucherItem.PendingVouchers) {
                    quint64 vchid;
                    if ((PendingVoucher.Type == enuVoucherType::Income)
                        || (PendingVoucher.Type == enuVoucherType::Prize)
                    ) {
                        PendingVoucher.Info.insert("mainVoucherID", static_cast<double>(_preVoucher.VoucherID));

                        QJsonDocument Doc;
                        Doc.setObject(PendingVoucher.Info);
                        vchid = this->callSP(APICALLBOOM_PARAM,
                                             "spWallet_Increase", {
                                                 { "iWalletID", 0 },
                                                 { "iForUsrID", CurrentUserID },
                                                 { "iByUserID", CurrentUserID },
                                                 { "iType", QString(static_cast<char>(PendingVoucher.Type)) },
                                                 { "iAmount", PendingVoucher.Amount },
                                                 { "iDesc", Doc.toJson(QJsonDocument::Compact) },
                                    { "iRootVoucherID", 0 },
                                             }).spDirectOutputs().value("oVoucherID").toULongLong();
                    } else {
                        vchid = Voucher::instance().Create(_APICALLBOOM,
                                                           TAPI::ORMFields_t({
                                                                                 { tblVoucher::Fields::vch_usrID, CurrentUserID },
                                                                                 { tblVoucher::Fields::vchDesc, PendingVoucher.Info.toVariantMap() },
                                                                                 { tblVoucher::Fields::vchTotalAmount, PendingVoucher.Amount },
                                                                                 { tblVoucher::Fields::vchType, PendingVoucher.Type },
                                                                                 { tblVoucher::Fields::vchStatus, enuVoucherStatus::New },
                                                                             }));
                    }

                    AdditionalVouchers.insert(PendingVoucher.Name, vchid);
                }

                vchProcessResult[VoucherItem.UUID] = QVariantMap({
                                                                     { "addVouchers", AdditionalVouchers },
                                                                 });
            }

            if (vchProcessResult.isEmpty() == false) {
                Voucher::instance().Update(APICALLBOOM_PARAM,
                                           {},
                                           TAPI::ORMFields_t({
                                              { tblVoucher::Fields::vchProcessResult, vchProcessResult }
                                           }),
                                           {
                                              { tblVoucher::Fields::vchID, _preVoucher.VoucherID }
                                           });
            }
        */

        //--------------------------
        return stuVoucher(
                    _voucherID,
                    PreVoucher,
                    QString(),
                    QString(),
                    VoucherDTO.vchTotalAmount,
                    0,
                    Targoman::API::AAA::enuVoucherStatus::Finished
                    );
//    } catch (...) {
//        Account::tryCancelVoucher(
////                    _APICALLBOOM,
//                    _userID,
//                    _voucherID);
//        throw;
//    }
}

void Account::tryCancelVoucher(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
//    quint64 _userID,
    quint64 _voucherID,
    bool _setAsError
) {
    //1: cancel voucher items
    try {
        QVariant VoucherDesc = SelectQuery(Voucher::instance())
                               .addCol(tblVoucher::Fields::vchDesc)
                               .where({ tblVoucher::Fields::vchID, enuConditionOperator::Equal, _voucherID })
                               .tryOne()
                               .value(tblVoucher::Fields::vchDesc);

        Targoman::API::AAA::stuPreVoucher PreVoucher;

        if (VoucherDesc.canConvert<QJsonObject>())
            PreVoucher.fromJson(VoucherDesc.toJsonObject());
        else if (VoucherDesc.canConvert<QVariantMap>())
            PreVoucher.fromJson(QJsonObject::fromVariantMap(VoucherDesc.toMap()));

        if (PreVoucher.Items.length()) {
            QVariantList Services = SelectQuery(Service::instance())
                    .addCols({
                                 tblService::Fields::svcID,
                                 tblService::Fields::svcName,
                                 tblService::Fields::svcCancelVoucherItemEndPoint,
                             })
                    .all();

            if (Services.isEmpty() == false) {
                foreach (Targoman::API::AAA::stuVoucherItem VoucherItem, PreVoucher.Items) {
                    //lookup services
                    foreach (QVariant Service, Services) {
                        QVariantMap ServiceInfo = Service.toMap();

                        if (ServiceInfo.value(tblService::Fields::svcName) == VoucherItem.Service) {
                            NULLABLE_TYPE(QString) CancelVoucherItemEndPoint;
                            TAPI::setFromVariant(CancelVoucherItemEndPoint, ServiceInfo.value(tblService::Fields::svcCancelVoucherItemEndPoint));

                            //bypass process by end point?
                            if (NULLABLE_HAS_VALUE(CancelVoucherItemEndPoint)) {
                                try {
                                    stuVoucherItemForTrustedAction VoucherItemForTrustedAction;
                                    VoucherItemForTrustedAction.UserID = APICALLBOOM_PARAM.getUserID();
                                    VoucherItemForTrustedAction.VoucherID = _voucherID;
                                    VoucherItemForTrustedAction.VoucherItem = VoucherItem;
                                    VoucherItemForTrustedAction.Sign.clear();
                                    VoucherItemForTrustedAction.Sign = QString(voucherSign(QJsonDocument(VoucherItemForTrustedAction.toJson()).toJson()).toBase64());

                                    QVariant Result = RESTClientHelper::callAPI(
                                        RESTClientHelper::POST,
                                        NULLABLE_GET_OR_DEFAULT(CancelVoucherItemEndPoint, ""),
                                        {},
                                        {
                                            { "data", VoucherItemForTrustedAction.toJson().toVariantMap() },
                                        }
                                    );
                                } catch (...) { ; }
                            }

                            break;
                        }
                    }
                }
            } //if (Services.isEmpty() == false)
        } //if (PreVoucher.Items.length())
    } catch (...) { ; }

    //2: cancel voucher

    clsDACResult Result = Voucher::instance().callSP(APICALLBOOM_PARAM,
                                                     "spVoucher_Cancel", {
                                                         { "iUserID", SYSTEM_USER_ID },
                                                         { "iVoucherID", _voucherID },
                                                         { "iSetAsError", _setAsError ? 1 : 0 },
                                                     });

//    this->Update(Voucher::instance(),
//                                 SYSTEM_USER_ID,
//                                 {},
//                                 TAPI::ORMFields_t({
//                                    { tblVoucher::Fields::vchStatus, _setAsError ? Accounting::enuVoucherStatus::Error : Accounting::enuVoucherStatus::Canceled }
//                                 }),
//                                 {
//                                    { tblVoucher::Fields::vchID, _voucherID }
//                                 });
}

/**
 * @callby:
 *     operator
 */
quint64 IMPL_REST_POST(Account, addPrizeTo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _targetUsrID,
    quint64 _amount,
    QString _desc
)) {
    qint64 Limit = Authorization::getPrivValue(_APICALLBOOM, "AAA:addPrizeTo:maxAmount", -1).toLongLong();
    if (Limit == 0)
        throw exAuthorization("Not enough access to add prize");

    if (Limit > 0 && _amount > static_cast<quint64>(Limit))
        throw exAuthorization("Prize amount is greater than your limits");

//    QFV.hasKey("desc").validate(_desc, "desc");

    return this->callSP(APICALLBOOM_PARAM,
                        "spWallet_Increase", {
                            { "iWalletID", 0 },
                            { "iForUsrID", _targetUsrID },
                            { "iByUserID", APICALLBOOM_PARAM.getUserID() },
                            { "iType", QString(static_cast<char>(enuVoucherType::Prize)) },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                            { "iRootVoucherID", 0 },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

/**
 * @callby:
 *     operator
 */
quint64 IMPL_REST_POST(Account, addIncomeTo, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _targetUsrID,
    quint64 _amount,
    QString _desc
)) {
    qint64 Limit = Authorization::getPrivValue(_APICALLBOOM, "AAA:addIncomeTo:maxAmount", -1).toLongLong();
    if (Limit == 0)
        throw exAuthorization("Not enough access to add income");

    if (Limit > 0 && _amount > static_cast<quint64>(Limit))
        throw exAuthorization("Amount is greater than your limits");

//    QFV.hasKey("desc").validate(_desc, "desc");

    return this->callSP(APICALLBOOM_PARAM,
                        "spWallet_Increase", {
                            { "iWalletID", 0 },
                            { "iForUsrID", _targetUsrID },
                            { "iByUserID", APICALLBOOM_PARAM.getUserID() },
                            { "iType", QString(static_cast<char>(enuVoucherType::Income)) },
                            { "iAmount", _amount },
                            { "iDesc", _desc },
                            { "iRootVoucherID", 0 },
                        }).spDirectOutputs().value("oVoucherID").toULongLong();
}

bool IMPL_REST_POST(Account, checkVoucherTTL, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint64 _voucherID
)) {
}

/****************************************************************\
|** fixture *****************************************************|
\****************************************************************/
#ifdef QT_DEBUG
QVariant IMPL_REST_POST(Account, fixtureSetup, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
    QVariantMap Result;

    if (_random == "1")
        _random = QString::number(QRandomGenerator::global()->generate());

    if (_random.isEmpty() == false)
        Result.insert("Random", _random);

    constexpr quint64 UT_SystemUserID = 1;
    constexpr quint32 UT_AdminRoleID = 3;
//    constexpr char UT_RoleName[] = "fixture_role";
    QString RoleName = FixtureHelper::MakeRandomizeName(_random, "-", "fixture", "role");

    clsDAC DAC;

    //-- create role --------------------------------------
    DAC.execQuery("",
                  "INSERT IGNORE"
                  "  INTO tblRoles"
                  "   SET rolName=?"
                  "     , rolCreatedBy_usrID=?",
                  {
                      RoleName,
                      UT_SystemUserID
                  });
    Result.insert("Role", RoleName);

    //-- user --------------------------------------
    QString UserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "user@dev.test");

    clsDACResult UserDACResult = DAC.execQuery("",
        "SELECT usrID"
        "  FROM tblUser"
        " WHERE usrEmail=?",
        {
            UserEmail
        });

    if (UserDACResult.isValid()) {
        Result.insert("User", QVariantMap({
                                              { "usrID", UserDACResult.value("usrID") },
                                              { "email", UserEmail },
                                          }));
    } else {
        //-- create user --------------------------------------
        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVariantMap SignupUserResult = this->apiPUTsignup(
                                        _APICALLBOOM,
                                        UserEmail,
                                        { "df6d2338b2b8fce1ec2f6dda0a630eb0" },
                                        RoleName,
                                        "fixture test",
                                        "user",
                                        false,
                                        false
                                       );

        SignupUserResult.insert("email", UserEmail);

        Result.insert("User", SignupUserResult);

        quint64 UserID = SignupUserResult["usrID"].toUInt();

        //-- approve user email --------------------------------------
        QString Code = DAC.execQuery("",
                                     "SELECT aprApprovalCode"
                                     "  FROM tblApprovalRequest"
                                     " WHERE apr_usrID=?",
                                     {
                                         UserID
                                     })
                       .toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("",
                      "UPDATE tblApprovalRequest"
                      "   SET aprStatus=?"
                      "     , aprSentDate=NOW()"
                      " WHERE apr_usrID=?",
                      {
                          QChar(enuAPRStatus::Sent),
                          UserID
                      });

        this->apiPOSTapproveEmail(_APICALLBOOM, UserEmail, Code);
    }

    //-- admin --------------------------------------
    QString AdminUserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "admin@dev.test");

    clsDACResult AdminDACResult = DAC.execQuery("",
        "SELECT usrID"
        "  FROM tblUser"
        " WHERE usrEmail=?",
        {
            AdminUserEmail
        });

    if (AdminDACResult.isValid()) {
        Result.insert("Admin", QVariantMap({
                                               { "usrID", AdminDACResult.value("usrID") },
                                               { "email", AdminUserEmail },
                                           }));
    } else {
        //-- create admin --------------------------------------
        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVariantMap SignupAdminUserResult = this->apiPUTsignup(
                                                _APICALLBOOM,
                                                AdminUserEmail,
                                                { "df6d2338b2b8fce1ec2f6dda0a630eb0" },
                                                RoleName,
                                                "fixture test",
                                                "admin",
                                                false,
                                                false
                                                );

        SignupAdminUserResult.insert("email", AdminUserEmail);

        Result.insert("Admin", SignupAdminUserResult);

        quint64 AdminUserID = SignupAdminUserResult["usrID"].toUInt();

        DAC.execQuery("",
                      "UPDATE tblUser"
                      "   SET tblUser.usr_rolID=?"
                      " WHERE tblUser.usrID=?",
                      {
                          UT_AdminRoleID,
                          AdminUserID
                      });

        //-- approve admin email --------------------------------------
        QString Code = DAC.execQuery("",
                             "SELECT aprApprovalCode"
                             "  FROM tblApprovalRequest"
                             " WHERE apr_usrID=?",
                             {
                                 AdminUserID
                             })
               .toJson(true).object().value("aprApprovalCode").toString();

        DAC.execQuery("",
                      "UPDATE tblApprovalRequest"
                      "   SET aprStatus=?"
                      "     , aprSentDate=NOW()"
                      " WHERE apr_usrID=?",
                      {
                          QChar(enuAPRStatus::Sent),
                          AdminUserID
                      });

        this->apiPOSTapproveEmail(_APICALLBOOM, AdminUserEmail, Code);
    }

    //-- payment gateway --------------------------------------
    quint32 pgwTotalRows = DAC.execQuery("",
                                         "SELECT COUNT(*) AS cnt"
                                         "  FROM tblPaymentGateways"
                                         " WHERE pgwType=?"
                                         "   AND pgwAllowedDomainName=?",
                                         {
                                             QChar(enuPaymentGatewayType::_DeveloperTest),
                                             "dev.test"
                                         })
                           .toJson(true)
//                           .toVariant()
//                           .toMap()
                           .object()
                           .value("cnt")
                           .toInt();

    QVariantMap PaymentGatewayReport = {
        { "RowsCount", pgwTotalRows }
    };

    if (pgwTotalRows < 3) {
//        QVariantMap CreatedPaymentGateways;

        for (int i=pgwTotalRows; i<3; ++i) {
            try {
                QVariantMap PaymentGatewayValues = {
                    { tblPaymentGateways::Fields::pgwName,     FixtureHelper::MakeRandomizeName(_random, " ", "fixture.devtest") },
                    { tblPaymentGateways::Fields::pgwType,     enuPaymentGatewayType::toStr(enuPaymentGatewayType::_DeveloperTest) },
                    { tblPaymentGateways::Fields::pgwDriver,   "DevTest" },
                    { tblPaymentGateways::Fields::pgwMetaInfo, QVariantMap({
                          { "username", "hello" },
                          { "password", "123" },
                      })
                    },
                    { tblPaymentGateways::Fields::pgw_curID,   1 },
                    { tblPaymentGateways::Fields::pgwAllowedDomainName, "dev.test" },
                };
                quint32 PaymentGatewayID = CreateQuery(ORM::PaymentGateways::instance())
                                           .addCols({
//                                                        tblPaymentGateways::Fields::pgwID,
                                                        tblPaymentGateways::Fields::pgwName,
                                                        tblPaymentGateways::Fields::pgwType,
                                                        tblPaymentGateways::Fields::pgwDriver,
                                                        tblPaymentGateways::Fields::pgwMetaInfo,
                                                        tblPaymentGateways::Fields::pgw_curID,
                                                        tblPaymentGateways::Fields::pgwAllowedDomainName,
//                                                        tblPaymentGateways::Fields::pgwTransactionFeeValue,
//                                                        tblPaymentGateways::Fields::pgwTransactionFeeType,
//                                                        tblPaymentGateways::Fields::pgwMaxPerDayAmount,
//                                                        tblPaymentGateways::Fields::pgwLastPaymentDateTime,
//                                                        tblPaymentGateways::Fields::pgwSumTodayPaidAmount,
//                                                        tblPaymentGateways::Fields::pgwSumRequestCount,
//                                                        tblPaymentGateways::Fields::pgwSumRequestAmount,
//                                                        tblPaymentGateways::Fields::pgwSumFailedCount,
//                                                        tblPaymentGateways::Fields::pgwSumOkCount,
//                                                        tblPaymentGateways::Fields::pgwSumPaidAmount,
//                                                        tblPaymentGateways::Fields::pgwStatus,
                                                    })
                                           .values(PaymentGatewayValues)
                                           .execute(1);

                PaymentGatewayValues.insert(tblPaymentGateways::Fields::pgwID, PaymentGatewayID);
//                CreatedPaymentGateways.in(PaymentGatewayValues);
            } catch (std::exception &exp) {
                qDebug() << exp.what();
            }
        }
//        qDebug() << CreatedPaymentGateways;
//        PaymentGatewayReport.insert("PaymentGateways", QVariant::fromValue(CreatedPaymentGateways));
    }

    Result.insert("PaymentGateway", PaymentGatewayReport);

    //----------------------------------------
    return Result;
}

QVariant IMPL_REST_POST(Account, fixtureCleanup, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
    QVariantMap Result;

//    clsDAC DAC;

    QString UserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "user@dev.test");
    QString AdminUserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "admin@dev.test");

//    OR u.usrMobile LIKE '+98999887%'

    try {
        QString QueryString = R"(
            DELETE wb
              FROM tblWalletsBalanceHistory wb
        INNER JOIN tblWalletsTransactions wt
                ON wt.wltID = wb.wbl_wltID
        INNER JOIN tblUserWallets uw
                ON uw.walID = wt.wlt_walID
        INNER JOIN tblUser u
                ON u.usrID = uw.wal_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });

        Result.insert("tblWalletsBalanceHistory", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE wt
              FROM tblWalletsTransactions wt
        INNER JOIN tblUserWallets uw
                ON uw.walID = wt.wlt_walID
        INNER JOIN tblUser u
                ON u.usrID = uw.wal_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblWalletsTransactions", QVariantMap({
                                                                { "numRowsAffected", DACResult.numRowsAffected() },
                                                            }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE uw
              FROM tblUserWallets uw
        INNER JOIN tblUser u
                ON u.usrID = uw.wal_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblUserWallets", QVariantMap({
                                                        { "numRowsAffected", DACResult.numRowsAffected() },
                                                    }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE op
              FROM tblOnlinePayments op
        INNER JOIN tblVoucher vch
                ON vch.vchID = op.onp_vchID
        INNER JOIN tblUser u
                ON u.usrID = vch.vch_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblOnlinePayments", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE fp
              FROM tblOfflinePayments fp
        INNER JOIN tblVoucher vch
                ON vch.vchID = fp.ofp_vchID
        INNER JOIN tblUser u
                ON u.usrID = vch.vch_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblOfflinePayments", QVariantMap({
                                                            { "numRowsAffected", DACResult.numRowsAffected() },
                                                        }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE vch
              FROM tblVoucher vch
        INNER JOIN tblUser u
                ON u.usrID = vch.vch_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblVoucher", QVariantMap({
                                                    { "numRowsAffected", DACResult.numRowsAffected() },
                                                }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE apr
              FROM tblApprovalRequest apr
        INNER JOIN tblUser u
                ON u.usrID = apr.apr_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblApprovalRequest", QVariantMap({
                                                            { "numRowsAffected", DACResult.numRowsAffected() },
                                                        }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE sn
              FROM tblActiveSessions sn
        INNER JOIN tblUser u
                ON u.usrID = sn.ssn_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblActiveSessions", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    } catch (...) { ; }

    try {
        QString QueryString = R"(
            DELETE u
              FROM tblUser u
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     UserEmail,
                                                     AdminUserEmail
                                                 });
        Result.insert("tblUser", QVariantMap({
                                                 { "items", QStringList({ UserEmail, AdminUserEmail }).join(",") },
                                                 { "numRowsAffected", DACResult.numRowsAffected() },
                                             }));
    } catch (...) { ; }

    try {
        QString RoleName = FixtureHelper::MakeRandomizeName(_random, "-", "fixture", "role");
        QString QueryString = R"(
            DELETE r
              FROM tblRoles r
             WHERE r.rolName=?
        ;)";
        clsDACResult DACResult = this->execQuery(APICALLBOOM_PARAM,
                                                 QueryString, {
                                                     RoleName
                                                 });
        Result.insert("tblRoles", QVariantMap({
                                                  { "items", RoleName },
                                                  { "numRowsAffected", DACResult.numRowsAffected() },
                                              }));
    } catch (...) { ; }

    return Result;
}

///@TODO: not tested
bool IMPL_REST_POST(Account, fixtureApproveEmail, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _email
)) {
    clsDAC DAC;

    QJsonObject Result = DAC.execQuery("",
                                       "SELECT aprApprovalCode"
                                       "     , usrID"
                                       "  FROM tblApprovalRequest "
                                       " INNER JOIN tblUser"
                                       "    ON tblUser.usrID = tblApprovalRequest.apr_usrID "
                                       " WHERE usrEmail=?",
                                       {
                                           _email
                                       })
                         .toJson(true).object();

    QString Code = Result.value("aprApprovalCode").toString();
    if (Code.isEmpty())
        return false;

    quint64 UserID = Result.value("usrID").toInt();

    DAC.execQuery("",
                  "UPDATE tblApprovalRequest"
                  "   SET aprStatus=?"
                  "     , aprSentDate=NOW()"
                  " WHERE apr_usrID=?",
                  {
                      QChar(enuAPRStatus::Sent),
                      UserID
                  });

    this->apiPOSTapproveEmail(_APICALLBOOM, _email, Code);

    return true;
}

///@TODO: not tested
bool IMPL_REST_POST(Account, fixtureApproveMobile, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::Mobile_t _mobile
)) {
    clsDAC DAC;

    QJsonObject Result = DAC.execQuery("",
                                       "SELECT aprApprovalCode"
                                       "     , usrID"
                                       "  FROM tblApprovalRequest "
                                       " WHERE aprApprovalKey=?",
                                       {
                                           _mobile
                                       })
                         .toJson(true).object();

    QString Code = Result.value("aprApprovalCode").toString();
    if (Code.isEmpty())
        return false;

    quint64 UserID = Result.value("usrID").toInt();

    DAC.execQuery("",
                  "UPDATE tblApprovalRequest"
                  "   SET aprStatus=?"
                  "     , aprSentDate=NOW()"
                  " WHERE apr_usrID=?",
                  {
                      QChar(enuAPRStatus::Sent),
                      UserID
                  });

    this->apiPOSTapproveMobile(_APICALLBOOM, _mobile, Code.toUInt());

    return true;
}

#endif

} //namespace Targoman::API::AccountModule
