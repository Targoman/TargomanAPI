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
#include "Payment/PaymentLogic.h"
#include "Payment/intfPaymentGateway.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Interfaces/ORM/intfAlerts.h"

#include "Interfaces/Helpers/PhoneHelper.h"
#include "Interfaces/Helpers/SecurityHelper.h"
#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"
using namespace Targoman::API::Helpers;

using namespace Targoman::API::AAA;

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuOAuthType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuGender);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserApproval);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAuditableStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuSaleableType);

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    Targoman::API::AccountModule,
    stuMultiJWT,
    [](const Targoman::API::AccountModule::stuMultiJWT& _value) -> QVariant{return QJsonObject({{"ssn", _value.Session}, {"lgn", _value.Login}}).toVariantMap();}
);

//namespace Targoman::API {

//using namespace DBManager;
//using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman::API::AccountModule {

using namespace Payment;
using namespace ORM;

///TODO: move this to config file
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

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Account, AAASchema);

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

QString ValidateAndNormalizeEmailOrPhoneNumber(QString &_emailOrMobile)
{
    if (QFV.email().isValid(_emailOrMobile))
    {
        if (QFV.emailNotFake().isValid(_emailOrMobile) == false)
            throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");

        _emailOrMobile = _emailOrMobile.toLower();
        return "E";
    }

    if (QFV.mobile().isValid(_emailOrMobile))
    {
        _emailOrMobile = PhoneHelper::NormalizePhoneNumber(_emailOrMobile);
        return "M";
    }

    throw exHTTPBadRequest("emailOrMobile must be a valid email or mobile");
}

/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
Account::Account() :
    intfSQLBasedWithActionLogsModule(AccountDomain, AAASchema)
{
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
    this->addSubModule(&ORM::PaymentGateways::instance());
    this->addSubModule(&OnlinePayments::instance());
    this->addSubModule(&OfflinePayments::instance());
    this->addSubModule(&Roles::instance());
    this->addSubModule(&Service::instance());
    this->addSubModule(&User::instance());
    this->addSubModule(&UserExtraInfo::instance());
    this->addSubModule(&UserWallets::instance());
    this->addSubModule(&WalletTransactions::instance());
    this->addSubModule(&WalletBalances::instance());

    if (Account::InvalidPasswordsFile.value().size()) {
        QFile InputFile(Account::InvalidPasswordsFile.value());
        if (InputFile.open(QIODevice::ReadOnly)) {
            QTextStream Stream(&InputFile);
            while (!Stream.atEnd())
                InvalidPasswords.insert(Stream.readLine().replace(QRegularExpression("#.*"), ""));
            InputFile.close();
        }
    }
}

TAPI::EncodedJWT_t Account::createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services)
{
    return clsJWT::createSignedLogin(_remember, { { JWTItems::usrLogin, _login } }, QJsonObject({ { "svc", _services } }), _ssid);
}

TAPI::EncodedJWT_t Account::createJWT(const QString _login, const stuActiveAccount& _activeAccount, const QString& _services)
{
    return clsJWT::createSigned({
                                    { JWTItems::usrLogin,        _login },
                                    { JWTItems::usrID,           _activeAccount.Privs["usrID"] },
                                    { JWTItems::usrName,         _activeAccount.Privs["usrName"] },
                                    { JWTItems::usrFamily,       _activeAccount.Privs["usrFamily"] },
                                    { JWTItems::rolID,           _activeAccount.Privs["usr_rolID"] },
                                    { JWTItems::rolName,         _activeAccount.Privs["rolName"] },
                                    { JWTItems::privs,           _activeAccount.Privs["privs"] },
                                    { JWTItems::usrApproval,     TAPI::enuUserApproval::toStr(_activeAccount.Privs["usrApprovalState"].toString()) },
                                    { JWTItems::usrStatus,       TAPI::enuUserStatus::toStr(_activeAccount.Privs["usrStatus"].toString()) },
                                    { JWTItems::canChangePass,   _activeAccount.Privs["hasPass"] },
                                },
                                QJsonObject({ { "svc", _services } }),
                                _activeAccount.TTL,
                                _activeAccount.Privs["ssnKey"].toString()
            );
}

/*****************************************************************\
|* User **********************************************************|
\*****************************************************************/
QString Account::apinormalizePhoneNumber(
        QString _phone,
        QString _country
    )
{
    return PhoneHelper::NormalizePhoneNumber(_phone, _country);
}

QVariantMap Account::apiPUTsignup(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile,
        TAPI::MD5_t _pass,
        QString _role,
        QString _name,
        QString _family,
        TAPI::JSON_t _specialPrivs,
        qint8 _maxSessions
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QString Type = ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QFV/*.asciiAlNum()*/.maxLenght(50).validate(_role);

    if (_role.toLower() == "administrator" || _role.toLower() == "system" || _role.toLower() == "baseuser")
        throw exHTTPForbidden("Selected role is not allowed to signup");

    if (_pass.isEmpty())
        throw exHTTPBadRequest("Password must be provided.");

    if (InvalidPasswords.contains(_pass))
        throw exHTTPBadRequest("Invalid simple password");

    quint64 UserID = this->callSP("spSignup", {
            { "iBy", Type },
            { "iLogin", _emailOrMobile },
            { "iPass", _pass },
            { "iRole", _role },
            { "iIP", _REMOTE_IP },
            { "iName", _name.isEmpty()? QVariant() : _name },
            { "iFamily", _family.isEmpty()? QVariant() : _family },
            { "iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs },
            { "iMaxSessions", _maxSessions },
            { "iCreatorUserID", QVariant() },
        })
        .spDirectOutputs()
        .value("oUserID")
        .toDouble();

    return {
        { "type", Type == 'E' ? "email" : "mobile" },
        { "usrID", UserID },
    };
}
/*
QVariantMap Account::apiPUTsignupByMobileOnly(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::Mobile_t _mobile,
//        quint32 _verifyCode,
//        TAPI::MD5_t _pass,
        QString _role,
        QString _name,
        QString _family,
        TAPI::JSON_t _specialPrivs,
        qint8 _maxSessions
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    if (QFV.mobile().isValid(_mobile) == false)
        throw exHTTPBadRequest("Incorrect mobile.");

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    QFV.asciiAlNum().maxLenght(50).validate(_role);

    if (_role.toLower() == "administrator" || _role.toLower() == "system" || _role.toLower() == "baseuser")
        throw exHTTPForbidden("Selected role is not allowed to signup");

    quint64 UserID = this->callSP("spSignup", {
            { "iBy", "M" },
            { "iLogin", _mobile },
            { "iPass", "" },
            { "iRole", _role },
            { "iIP", _REMOTE_IP },
            { "iName", _name.isEmpty()? QVariant() : _name },
            { "iFamily", _family.isEmpty()? QVariant() : _family },
            { "iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs },
            { "iMaxSessions", _maxSessions },
            { "iCreatorUserID", QVariant() },
        })
        .spDirectOutputs()
        .value("oUserID")
        .toDouble();

    return {
        { "type", "mobile" },
        { "usrID", UserID },
    };
}
*/
Targoman::API::AccountModule::stuMultiJWT Account::apiPOSTapproveEmail(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _email,
        TAPI::MD5_t _uuid,
        bool _autoLogin,
        TAPI::CommaSeparatedStringList_t _services,
        bool _rememberMe,
        TAPI::JSON_t _sessionInfo,
        TAPI::MD5_t _fingerprint
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    _email = _email.toLower().trimmed();

    QJsonObject UserInfo = this->callSP("spApproval_Accept", {
            { "iBy", "E" },
            { "iKey", _email },
            { "iCode", _uuid },
            { "iLogin", _autoLogin ? 1 : 0 },
            { "iLoginIP", _REMOTE_IP },
            { "iLoginInfo", _sessionInfo.object() },
            { "iLoginRemember", _rememberMe ? 1 : 0 },
            { "iFingerPrint", _fingerprint.isEmpty() ? QVariant() : _fingerprint },
            { "iTTL", ApprovalRequest::EmailApprovalCodeTTL.value() },
        })
        .toJson(true)
        .object();

    if (_autoLogin == false)
        return Targoman::API::AccountModule::stuMultiJWT();

    auto LoginInfo = PrivHelpers::processUserObject(
                         UserInfo,
                         {},
                         _services.split(",", QString::SkipEmptyParts)
                         );

    return Targoman::API::AccountModule::stuMultiJWT({
                                                         this->createLoginJWT(_rememberMe, _email, LoginInfo.Privs["ssnKey"].toString(), _services),
                                                         this->createJWT(_email, LoginInfo, _services)
                                                     });
}

Targoman::API::AccountModule::stuMultiJWT Account::apiPOSTapproveMobile(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::Mobile_t _mobile,
        quint32 _code,
        bool _autoLogin,
        TAPI::CommaSeparatedStringList_t _services,
        bool _rememberMe,
        TAPI::JSON_t _sessionInfo,
        TAPI::MD5_t _fingerprint
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    QJsonObject UserInfo = this->callSP("spApproval_Accept", {
            { "iBy", "M" },
            { "iKey", _mobile },
            { "iCode", _code },
            { "iLogin", _autoLogin ? 1 : 0 },
            { "iLoginIP", _REMOTE_IP },
            { "iLoginInfo", _sessionInfo.object() },
            { "iLoginRemember", _rememberMe ? 1 : 0 },
            { "iFingerPrint", _fingerprint.isEmpty() ? QVariant() : _fingerprint },
            { "iTTL", ApprovalRequest::MobileApprovalCodeTTL.value() },
        })
        .toJson(true)
        .object();

    if (_autoLogin == false)
        return Targoman::API::AccountModule::stuMultiJWT();

    auto LoginInfo = PrivHelpers::processUserObject(
                         UserInfo,
                         {},
                         _services.split(",", QString::SkipEmptyParts)
                         );

    return Targoman::API::AccountModule::stuMultiJWT({
                                                         this->createLoginJWT(_rememberMe, _mobile, LoginInfo.Privs["ssnKey"].toString(), _services),
                                                         this->createJWT(_mobile, LoginInfo, _services)
                                                     });
}

Targoman::API::AccountModule::stuMultiJWT Account::apilogin(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile,
        TAPI::MD5_t _pass,
        QString _salt,
        TAPI::CommaSeparatedStringList_t _services,
        bool _rememberMe,
        TAPI::JSON_t _sessionInfo,
        TAPI::MD5_t _fingerprint
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

//    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_emailOrMobile, "login");
//    if (QFV.mobile().isValid(_emailOrMobile))
//        _emailOrMobile = PhoneHelper::NormalizePhoneNumber(_emailOrMobile);
    ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    auto LoginInfo = Authentication::login(_REMOTE_IP,
                                           _emailOrMobile,
                                           _pass,
                                           _salt,
                                           _services.split(",", QString::SkipEmptyParts),
                                           _rememberMe,
                                           _sessionInfo.object(),
                                           _fingerprint);

    return Targoman::API::AccountModule::stuMultiJWT({
                                 this->createLoginJWT(_rememberMe, _emailOrMobile, LoginInfo.Privs["ssnKey"].toString(), _services),
                                 this->createJWT(_emailOrMobile, LoginInfo, _services)
                             });
}

bool Account::apiloginByMobileOnly(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::Mobile_t _mobile,
        bool _signupIfNotExists,
        QString _signupRole
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QFV/*.asciiAlNum()*/.maxLenght(50).validate(_signupRole);

    if (_signupIfNotExists
            && (_signupRole.toLower() == "administrator"
                || _signupRole.toLower() == "system"
                || _signupRole.toLower() == "baseuser")
        )
        throw exHTTPForbidden("Selected role is not allowed to signup");

    QFV.mobile().validate(_mobile, "mobile");

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    this->callSP("spMobileVerifyCode_Request", {
                     { "iMobile", _mobile },
                     { "iSignupIfNotExists", _signupIfNotExists ? 1 : 0 },
                     { "iSignupRole", _signupRole },
                 });

    return true;
}

bool Account::apiresendApprovalCode(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QString Type = ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

//    this->callSP("spApprovalRequestAgain", {
//                     { "iBy", Type },
//                     { "iKey", _emailOrMobile },
//                     { "iIP", _REMOTE_IP },
//                     { "iRecreateIfExpired", true },
//                     { "iTTL", Type == 'E' ? Account::EmailApprovalCodeTTL.value() : Account::MobileApprovalCodeTTL.value() },
//                 });
    this->callSP("spApproval_Request", {
                     { "iBy", Type },
                     { "iKey", _emailOrMobile },
                     { "iUserID", {} },
                     { "iPass", {} },
                     { "iSalt", {} }
//                     { "iIP", _REMOTE_IP },
//                     { "iRecreateIfExpired", true },
//                     { "iTTL", Type == 'E' ? Account::EmailApprovalCodeTTL.value() : Account::MobileApprovalCodeTTL.value() },
                 });

    return true;
}

//bool Account::apiPUTrequestMobileVerifyCode(
//        TAPI::RemoteIP_t _REMOTE_IP,
//        TAPI::Mobile_t _mobile
//    )
//{
//    Authorization::validateIPAddress(_REMOTE_IP);

//    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

//    quint64 aprID = this->callSP("spMobileVerifyCode_Request", {
//                                     { "iMobile", _mobile },
//                                 })
//                    .spDirectOutputs()
//                    .value("oAprID")
//                    .toDouble();

//    return (aprID > 0);
//}

/*
Targoman::API::AccountModule::stuMultiJWT Account::apiPUTverifyLoginByMobileCode(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::Mobile_t _mobile,
        quint32 _code,
        TAPI::CommaSeparatedStringList_t _services,
        bool _rememberMe,
        TAPI::JSON_t _sessionInfo,
        TAPI::MD5_t _fingerprint
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    _mobile = PhoneHelper::NormalizePhoneNumber(_mobile);

    QJsonObject UserInfo = this->callSP("spLogin_VerifyByMobileCode", {
                                            { "iMobile", _mobile },
                                            { "iCode", _code },
                                            { "iIP", _REMOTE_IP },
                                            { "iInfo", _sessionInfo.object() },
                                            { "iRemember", _rememberMe ? "1" : "0" },
                                            { "iFingerPrint", _fingerprint.isEmpty() ? QVariant() : _fingerprint },
                                        })
                           .toJson(true)
                           .object();

    auto LoginInfo = PrivHelpers::processUserObject(UserInfo, {}, _services.split(",", QString::SkipEmptyParts));

    return Targoman::API::AccountModule::stuMultiJWT({
                                 this->createLoginJWT(_rememberMe, _mobile, LoginInfo.Privs["ssnKey"].toString(), _services),
                                 this->createJWT(_mobile, LoginInfo, _services)
                             });
}
*/

///TODO: cache to ban users for every service
///TODO: update cache for each module
///TODO: JWT lifetime dynamic based on current hour
Targoman::API::AccountModule::stuMultiJWT Account::apiloginByOAuth(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::enuOAuthType::Type _type,
        QString _oAuthToken,
        TAPI::CommaSeparatedStringList_t _services,
        TAPI::JSON_t _sessionInfo,
        TAPI::MD5_t _fingerprint
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QString Login;

    Authentication::stuOAuthInfo OAuthInfo;
    ///TODO: validate _oAuthToken

    switch(_type)
    {
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
                         _REMOTE_IP,
                         OAuthInfo.Email,
                         nullptr,
                         nullptr,
                         _services.split(","),
                         true,
                         _sessionInfo.object(),
                         _fingerprint
                         );

    return Targoman::API::AccountModule::stuMultiJWT({
                                 this->createLoginJWT(true, OAuthInfo.Email, LoginInfo.Privs["ssnKey"].toString(), _services),
                                 this->createJWT(OAuthInfo.Email, LoginInfo, _services)
                             });
}

Targoman::API::AccountModule::stuMultiJWT Account::apirefreshJWT(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::JWT_t _loginJWT,
        QString _services
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QJsonObject Obj;

    clsJWT LoginJWT(_loginJWT);
    QString Services = _services;
    if(_services.isEmpty())
        Services = LoginJWT.privatePart().value("svc").toString();

    auto NewPrivs = Authentication::updatePrivs(_REMOTE_IP, LoginJWT.session(), Services);
    return Targoman::API::AccountModule::stuMultiJWT({
                                 this->createLoginJWT(true, LoginJWT.login(), LoginJWT.session(), Services),
                                 this->createJWT(LoginJWT.login(), NewPrivs, Services)
                             });
}

bool Account::apilogout(TAPI::JWT_t _JWT)
{
    clsJWT JWT(_JWT);

    this->callSP("spLogout", {
                     { "iByUserID", clsJWT(_JWT).usrID() },
                     { "iSessionGUID", clsJWT(_JWT).session() },
                 });

    return true;
}

QString Account::apicreateForgotPasswordLink(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QString Type = ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    this->callSP("spForgotPass_Request", {
                     { "iLogin", _emailOrMobile },
                     { "iVia", Type },
                 });

    return (Type == "E" ? "email" : "mobile");
}

#ifdef QT_DEBUG
QString Account::apiPOSTfixtureGetLastForgotPasswordUUIDAndMakeAsSent(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile
    )
{
    Q_UNUSED(_REMOTE_IP);

    QString Type = ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    QVariantMap Data = SelectQuery(ForgotPassRequest::instance())
                       .addCol(tblForgotPassRequest::fprUUID)
                       .addCol(tblForgotPassRequest::fprStatus)
                       .innerJoinWith(tblForgotPassRequest::Relation::User)
                       .where({ Type == "E" ? tblUser::usrEmail : tblUser::usrMobile, enuConditionOperator::Equal, _emailOrMobile })
                       .andWhere({ tblForgotPassRequest::fprRequestedVia, enuConditionOperator::Equal, Type.at(0) })
                       .orderBy(tblForgotPassRequest::fprRequestDate, enuOrderDir::Descending)
                       .one()
                       ;

    QString UUID = Data.value(tblForgotPassRequest::fprUUID).toString();

    if (UUID.isEmpty())
        throw exHTTPNotFound("No UUID could be found");

    QString fprStatus = Data.value(tblForgotPassRequest::fprStatus).toString();
    if (fprStatus != "Sent")
    {
        quint64 RowsCount = UpdateQuery(ForgotPassRequest::instance())
                            .set(tblForgotPassRequest::fprStatus, enuFPRStatus::Sent)
                            .where({ tblForgotPassRequest::fprUUID, enuConditionOperator::Equal, UUID })
                            .execute(1)
                            ;
        if (RowsCount == 0)
            throw exHTTPNotFound("error in set as sent");
    }

    return UUID;
}
#endif

bool Account::apichangePassByUUID(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _emailOrMobile,
        TAPI::MD5_t _uuid,
        TAPI::MD5_t _newPass
    )
{
    Authorization::validateIPAddress(_REMOTE_IP);

    QString Type = ValidateAndNormalizeEmailOrPhoneNumber(_emailOrMobile);

    this->callSP("spPassword_ChangeByUUID", {
                     { "iVia", Type },
                     { "iLogin", _emailOrMobile },
                     { "iUUID", _uuid },
                     { "iNewPass", _newPass },
                 });

    return true;
}

bool Account::apichangePass(
        TAPI::JWT_t _JWT,
        TAPI::MD5_t _oldPass,
        QString _oldPassSalt,
        TAPI::MD5_t _newPass
    )
{
    QFV.asciiAlNum().maxLenght(20).validate(_oldPassSalt, "salt");

    this->callSP("spPassword_Change", {
                     { "iUserID", clsJWT(_JWT).usrID() },
                     { "iOldPass", _oldPass },
                     { "iOldPassSalt", _oldPassSalt },
                     { "iNewPass", _newPass },
                 });

    return true;
}

/*****************************************************************\
|* Voucher & Payments ********************************************|
\*****************************************************************/
Targoman::API::AAA::stuVoucher Account::processVoucher(
        TAPI::JWT_t _JWT,
        quint64 _voucherID
    )
{
    try
    {
        QVariant VoucherDesc = SelectQuery(Voucher::instance())
                               .addCol(tblVoucher::vchDesc)
                               .where({ tblVoucher::vchID, enuConditionOperator::Equal, _voucherID })
                               .one()
                               .value(tblVoucher::vchDesc);

        Targoman::API::AAA::stuPreVoucher PreVoucher;

        if (VoucherDesc.canConvert<QJsonObject>())
            PreVoucher.fromJson(VoucherDesc.toJsonObject());
        else if (VoucherDesc.canConvert<QVariantMap>())
            PreVoucher.fromJson(QJsonObject::fromVariantMap(VoucherDesc.toMap()));
        else
            throw exHTTPInternalServerError(QString("Voucher with ID: %1 not found or invalid json.").arg(_voucherID));

        if (PreVoucher.Items.isEmpty())
            throw exHTTPInternalServerError(QString("Voucher with ID: %1 has not any items.").arg(_voucherID));

        QVariantList Services = SelectQuery(Service::instance())
                .addCol(tblService::svcID)
                .addCol(tblService::svcName)
                .addCol(tblService::svcProcessVoucherItemEndPoint)
                .all();
        if (Services.isEmpty())
            throw exHTTPInternalServerError("There is no services registered.");

        //1: process voucher items
        foreach(Targoman::API::AAA::stuVoucherItem VoucherItem, PreVoucher.Items)
        {
            //lookup services
            foreach (QVariant Service, Services)
            {
                QVariantMap ServiceInfo = Service.toMap();

                if (ServiceInfo.value(tblService::svcName) == VoucherItem.Service)
                {
                    NULLABLE_TYPE(QString) ProcessVoucherItemEndPoint;
                    TAPI::setFromVariant(ProcessVoucherItemEndPoint, ServiceInfo.value(tblService::svcProcessVoucherItemEndPoint));

                    //bypass process by end point?
                    if (NULLABLE_HAS_VALUE(ProcessVoucherItemEndPoint))
                    {
                        QVariant Result = RESTClientHelper::callAPI(
                            _JWT,
                            RESTClientHelper::POST,
                            NULLABLE_GET_OR_DEFAULT(ProcessVoucherItemEndPoint, ""),
                            {},
                            {
                                { "voucherItem", VoucherItem.toJson().toVariantMap() },
                                { "voucherID", _voucherID },
                            }
                        );

                        if ((Result.isValid() == false) || (Result.toBool() == false))
                            throw exHTTPInternalServerError("error in process voucher");
                    }

                    break;
                }
            } //foreach (QVariant Service, Services)
        }

        //2: change voucher status to Targoman::API::AAA::enuVoucherStatus::Finished
        /*Targoman::API::Query::*/Voucher::instance().Update(
                                     Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::Finished }
                                     }),
                                     {
                                        { tblVoucher::vchID, _voucherID }
                                     });

        //--------------------------
        return Targoman::API::AAA::stuVoucher(
                    _voucherID,
                    PreVoucher,
                    QString(),
                    QString(),
                    Targoman::API::AAA::enuVoucherStatus::Finished
                    );
    }
    catch (...)
    {
        Account::tryCancelVoucher(_JWT, _voucherID);
        throw;
    }
}

void Account::tryCancelVoucher(
        TAPI::JWT_t _JWT,
        quint64 _voucherID,
        bool _setAsError
    )
{
    //1: cancel voucher items
    try
    {
        QVariant VoucherDesc = SelectQuery(Voucher::instance())
                               .addCol(tblVoucher::vchDesc)
                               .where({ tblVoucher::vchID, enuConditionOperator::Equal, _voucherID })
                               .tryOne()
                               .value(tblVoucher::vchDesc);

        Targoman::API::AAA::stuPreVoucher PreVoucher;

        if (VoucherDesc.canConvert<QJsonObject>())
            PreVoucher.fromJson(VoucherDesc.toJsonObject());
        else if (VoucherDesc.canConvert<QVariantMap>())
            PreVoucher.fromJson(QJsonObject::fromVariantMap(VoucherDesc.toMap()));

        if (PreVoucher.Items.length())
        {
            QVariantList Services = SelectQuery(Service::instance())
                    .addCol(tblService::svcID)
                    .addCol(tblService::svcName)
                    .addCol(tblService::svcCancelVoucherItemEndPoint)
                    .all();

            if (Services.isEmpty() == false)
            {
                foreach(Targoman::API::AAA::stuVoucherItem VoucherItem, PreVoucher.Items)
                {
                    //lookup services
                    foreach (QVariant Service, Services)
                    {
                        QVariantMap ServiceInfo = Service.toMap();

                        if (ServiceInfo.value(tblService::svcName) == VoucherItem.Service)
                        {
                            NULLABLE_TYPE(QString) CancelVoucherItemEndPoint;
                            TAPI::setFromVariant(CancelVoucherItemEndPoint, ServiceInfo.value(tblService::svcCancelVoucherItemEndPoint));

                            //bypass process by end point?
                            if (NULLABLE_HAS_VALUE(CancelVoucherItemEndPoint))
                            {
                                try
                                {
                                    QVariant Result = RESTClientHelper::callAPI(
                                        _JWT,
                                        RESTClientHelper::POST,
                                        NULLABLE_GET_OR_DEFAULT(CancelVoucherItemEndPoint, ""),
                                        {},
                                        {
                                            { "voucherItem", VoucherItem.toJson().toVariantMap() },
                                        }
                                    );
                                }
                                catch (...)
                                {
                                }
                            }

                            break;
                        }
                    }
                }
            } //if (Services.isEmpty() == false)
        } //if (PreVoucher.Items.length())
    }
    catch (...)
    {
    }

    //2: cancel voucher

    ///TODO: complete spVoucher_Cancel: create cancel voucher and credit to wallet

    clsDACResult Result = Voucher::instance().callSP("spVoucher_Cancel", {
        { "iUserID", SYSTEM_USER_ID },
        { "iVoucherID", _voucherID },
        { "iSetAsError", _setAsError },
    });

//    /*Targoman::API::Query::*/this->Update(Voucher::instance(),
//                                 SYSTEM_USER_ID,
//                                 {},
//                                 TAPI::ORMFields_t({
//                                    { tblVoucher::vchStatus, _setAsError ? Accounting::enuVoucherStatus::Error : Accounting::enuVoucherStatus::Canceled }
//                                 }),
//                                 {
//                                    { tblVoucher::vchID, _voucherID }
//                                 });
}

///TODO: select gateway (null|single|multiple) from service
///TODO: check for common gateway voucher
Targoman::API::AAA::stuVoucher Account::apiPOSTfinalizeBasket(
        TAPI::JWT_t _JWT,
        Targoman::API::AAA::stuPreVoucher _preVoucher,
        Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
        QString _domain,
        qint64 _walletID,
        QString _paymentVerifyCallback
    )
{
    ///scenario:
    ///1: create voucher
    ///2: compute wallet remaining
    ///2.1: process voucher
    ///2.2: create online/offline payment

    if (_gatewayType != Targoman::API::AccountModule::enuPaymentGatewayType::COD)
    {
        if (_paymentVerifyCallback.isEmpty())
            throw exHTTPBadRequest("callback for non COD is mandatory");
        QFV.url().validate(_paymentVerifyCallback, "callBack");
    }

    if (_preVoucher.Items.isEmpty())
        throw exHTTPBadRequest("seems that pre-Voucher is empty");

    checkPreVoucherSanity(_preVoucher);

    //1: create voucher
    Targoman::API::AAA::stuVoucher Voucher;

    Voucher.Info = _preVoucher;

    ///TODO: remove sign from prevoucher before converting to JSON
    ///TODO: recalculate prevoucher to check either price change/package expiry/coupon limits/ etc.
    ///TODO: reserve saleables before returning voucher
    ///TODO: implement overall coupon at the end of checkout steps

    Voucher.ID = /*Targoman::API::Query::*/this->Create(
                     Voucher::instance(),
                     clsJWT(_JWT).usrID(),
                     TAPI::ORMFields_t({
                                           { tblVoucher::vch_usrID, clsJWT(_JWT).usrID() },
                                           { tblVoucher::vchDesc, _preVoucher.toJson().toVariantMap() },
                                           { tblVoucher::vchType, Targoman::API::AccountModule::enuVoucherType::Expense },
                                           { tblVoucher::vchTotalAmount, _preVoucher.ToPay },
                                           { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::New },
                                       })
                     );

    try
    {
        //2: compute wallet remaining
        qint64 RemainingAfterWallet = static_cast<qint64>(_preVoucher.ToPay);
        if ((_walletID >= 0) && (RemainingAfterWallet > 0))
        {
            clsDACResult Result = this->callSP("spWalletTransaction_Create", {
                                                   { "iWalletID", _walletID },
                                                   { "iVoucherID", Voucher.ID },
                                               });
            RemainingAfterWallet -= Result.spDirectOutputs().value("oAmount").toUInt();
            if (RemainingAfterWallet < 0)
                throw exHTTPInternalServerError("Remaining after wallet transaction is negative.");
        }

        //2.1: process voucher
        if (RemainingAfterWallet == 0)
            return Account::processVoucher(_JWT, Voucher.ID);

        //2.2: create online/offline payment
        if (_gatewayType == Targoman::API::AccountModule::enuPaymentGatewayType::COD)
        {
            //Do nothing as it will be created after information upload.
        }
        else
        {
            TAPI::MD5_t PaymentMD5;
            Voucher.PaymentLink = PaymentLogic::createOnlinePaymentLink(
                                      _gatewayType,
                                      _domain,
                                      Voucher.ID,
                                      _preVoucher.Summary,
                                      RemainingAfterWallet,
                                      _paymentVerifyCallback,
                                      PaymentMD5
                                      );
            Voucher.PaymentMD5 = PaymentMD5;
        }
    }
    catch (std::exception &exp) {
        qDebug() << "**********************************" << exp.what();

        Account::tryCancelVoucher(_JWT, Voucher.ID, true);
//        /*Targoman::API::Query::*/this->Update(Voucher::instance(),
//                                     SYSTEM_USER_ID,
//                                     {},
//                                     TAPI::ORMFields_t({
//                                        { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
//                                     }),
//                                     {
//                                        { tblVoucher::vchID, Voucher.ID }
//                                     });
        throw;
    }

    return Voucher;
}

/**
 * @brief Account::apiPOSTapproveOnlinePayment: called back from callback
 * @param _gateway
 * @param _domain
 * @param _pgResponse: ... voucherID ...
 * @return
 */
Targoman::API::AAA::stuVoucher Account::apiPOSTapproveOnlinePayment(
        TAPI::JWT_t _JWT,
//        Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
        const QString _paymentMD5,
        const QString _domain,
        TAPI::JSON_t _pgResponse
    )
{
    quint64 VoucherID = PaymentLogic::approveOnlinePayment(_paymentMD5, _pgResponse, _domain);

    try
    {
        this->callSP("spWalletTransactionOnPayment_Create", {
                         { "iVoucherID", VoucherID },
                         { "iPaymentType", QChar(enuPaymentType::Online) }
                     });
        return Account::processVoucher(_JWT, VoucherID);
    }
    catch(...)
    {
        /*Targoman::API::Query::*/this->Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, VoucherID }
                                     });
        throw;
    }
}

///TODO: implement auto verify daemon OJO on failed payments in the daemon

Targoman::API::AAA::stuVoucher Account::apiPOSTapproveOfflinePayment(
        TAPI::JWT_t _JWT,
        quint64 _vchID,
        const QString& _bank,
        const QString& _receiptCode,
        TAPI::Date_t _receiptDate,
        quint32 _amount,
        const QString& _note
    )
{
    qint64 ApprovalLimit = Authorization::getPrivValue(_JWT, "AAA:approveOffline:maxAmount").toLongLong();
    if(ApprovalLimit == 0)
        throw exAuthorization("Not enough access for offline approval");

    if (ApprovalLimit > 0) {
//        QVariantMap Voucher = Voucher::instance().selectFromTable({}, {}, QString("%1").arg(_vchID), 0, 1, tblVoucher::vchTotalAmount).toMap();
        QVariantMap Voucher = SelectQuery(Voucher::instance())
            .addCol(tblVoucher::vchTotalAmount)
            .where({ tblVoucher::vchID, enuConditionOperator::Equal, _vchID })
            .one();

        if (Voucher.value(tblVoucher::vchTotalAmount).toLongLong() > ApprovalLimit)
            throw exAuthorization("Voucher total amount is greater than your approval limit");
    }

    QFV.unicodeAlNum(true).maxLenght(50).validate(_bank, "bank");
    QFV.unicodeAlNum(true).maxLenght(50).validate(_receiptCode, "receiptCode");

    /*Targoman::API::Query::*/this->Create(OfflinePayments::instance(),
                                 clsJWT(_JWT).usrID(),
                                 TAPI::ORMFields_t({
                                    { "ofp_vchID",_vchID },
                                    { "ofpBank",_bank },
                                    { "ofpReceiptCode",_receiptCode },
                                    { "ofpReceiptDate",_receiptDate },
                                    { "ofpAmount",_amount },
                                    { "ofpNote",_note.trimmed().size() ? _note.trimmed() : QVariant() }
                                 }));
//    OfflinePayments::instance().create(clsJWT(_JWT).usrID(), TAPI::ORMFields_t({
//                                           {"ofp_vchID",_vchID},
//                                           {"ofpBank",_bank},
//                                           {"ofpReceiptCode",_receiptCode},
//                                           {"ofpReceiptDate",_receiptDate},
//                                           {"ofpAmount",_amount},
//                                           {"ofpNote",_note.trimmed().size() ? _note.trimmed() : QVariant()}
//                                       }));

    try {
        this->callSP("spWalletTransactionOnPayment_Create", {
                         { "iVoucherID", _vchID },
                         { "iPaymentType", QChar(enuPaymentType::Offline) }
                     });
        return Account::processVoucher(_JWT, _vchID);
    }  catch (...) {
        /*Targoman::API::Query::*/this->Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, Targoman::API::AAA::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, _vchID }
                                     });
        throw;
    }
}

bool Account::apiPOSTaddPrizeTo(
        TAPI::JWT_t _JWT,
        quint64 _targetUsrID,
        quint64 _amount,
        TAPI::JSON_t _desc
    )
{
    qint64 Limit = Authorization::getPrivValue(_JWT, "AAA:addPrizeTo:maxAmount").toLongLong();
    if(Limit == 0)
        throw exAuthorization("Not enough access to add prize");

    if(Limit > 0 && _amount > static_cast<quint64>(Limit))
        throw exAuthorization("Prize amount is greater than your limits");

    QFV.hasKey("desc").validate(_desc, "desc");

    this->callSP("sp_CREATE_increaseWallet", {
                     {"iWalletID", 0},
                     {"iToUsrID", _targetUsrID},
                     {"iType", QString(static_cast<char>(Targoman::API::AccountModule::enuVoucherType::Prize))},
                     {"iAmount", _amount},
                     {"iDesc", _desc.toJson()},
                 });
    return true;
}

bool Account::apiPOSTaddIncomeTo(
        TAPI::JWT_t _JWT,
        quint64 _targetUsrID,
        quint64 _amount,
        TAPI::JSON_t _desc
    )
{
    qint64 Limit = Authorization::getPrivValue(_JWT, "AAA:addIncomeTo:maxAmount").toLongLong();
    if (Limit == 0)
        throw exAuthorization("Not enough access to add income");

    if (Limit > 0 && _amount > static_cast<quint64>(Limit))
        throw exAuthorization("Amount is greater than your limits");

    QFV.hasKey("desc").validate(_desc, "desc");

    this->callSP("sp_CREATE_increaseWallet", {
                     {"iWalletID", 0},
                     {"iToUsrID", _targetUsrID},
                     {"iType", QString(static_cast<char>(Targoman::API::AccountModule::enuVoucherType::Income))},
                     {"iAmount", _amount},
                     {"iDesc", _desc.toJson()},
                 });
    return true;
}

bool Account::apiPOSTcheckVoucherTTL(
        TAPI::JWT_t _JWT,
        quint64 _voucherID
    )
{
}

/****************************************************************\
|** fixture *****************************************************|
\****************************************************************/
#ifdef QT_DEBUG
QVariant Account::apiPOSTfixtureSetup(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _random
    )
{
    QVariantMap Result;

    if (_random == "1")
        _random = QString("%1").arg(QRandomGenerator::global()->generate());

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

    if (UserDACResult.isValid())
    {
        Result.insert("User", QVariantMap({
                                              { "usrID", UserDACResult.value("usrID") },
                                              { "email", UserEmail },
                                          }));
    }
    else
    {
        //-- create user --------------------------------------
        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVariantMap SignupUserResult = this->apiPUTsignup(
                                        _REMOTE_IP,
                                        UserEmail,
                                        { "df6d2338b2b8fce1ec2f6dda0a630eb0" },
                                        RoleName,
                                        "fixture test",
                                        "user"
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

        this->apiPOSTapproveEmail(_REMOTE_IP, UserEmail, Code);
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

    if (AdminDACResult.isValid())
    {
        Result.insert("Admin", QVariantMap({
                                               { "usrID", AdminDACResult.value("usrID") },
                                               { "email", AdminUserEmail },
                                           }));
    }
    else
    {
        //-- create admin --------------------------------------
        //df6d2338b2b8fce1ec2f6dda0a630eb0 # 987
        QVariantMap SignupAdminUserResult = this->apiPUTsignup(
                                                _REMOTE_IP,
                                                AdminUserEmail,
                                                { "df6d2338b2b8fce1ec2f6dda0a630eb0" },
                                                RoleName,
                                                "fixture test",
                                                "admin"
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

        this->apiPOSTapproveEmail(_REMOTE_IP, AdminUserEmail, Code);
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

    if (pgwTotalRows < 3)
    {
//        QVariantMap CreatedPaymentGateways;

        for (int i=pgwTotalRows; i<3; ++i)
        {
            try
            {
                QVariantMap PaymentGatewayValues = {
                    { tblPaymentGateways::pgwName,     FixtureHelper::MakeRandomizeName(_random, " ", "fixture.devtest") },
                    { tblPaymentGateways::pgwType,     enuPaymentGatewayType::toStr(enuPaymentGatewayType::_DeveloperTest) },
                    { tblPaymentGateways::pgwDriver,   "DevTest" },
                    { tblPaymentGateways::pgwMetaInfo, QVariantMap({
                          { "username", "hello" },
                          { "password", "123" },
                      })
                    },
                    { tblPaymentGateways::pgwAllowedDomainName, "dev.test" },
                };
                quint32 PaymentGatewayID = CreateQuery(ORM::PaymentGateways::instance())
//                                           .addCol(tblPaymentGateways::pgwID)
                                           .addCol(tblPaymentGateways::pgwName)
                                           .addCol(tblPaymentGateways::pgwType)
                                           .addCol(tblPaymentGateways::pgwDriver)
                                           .addCol(tblPaymentGateways::pgwMetaInfo)
                                           .addCol(tblPaymentGateways::pgwAllowedDomainName)
//                                           .addCol(tblPaymentGateways::pgwTransactionFeeValue)
//                                           .addCol(tblPaymentGateways::pgwTransactionFeeType)
//                                           .addCol(tblPaymentGateways::pgwMinRequestAmount)
//                                           .addCol(tblPaymentGateways::pgwMaxRequestAmount)
//                                           .addCol(tblPaymentGateways::pgwMaxPerDayAmount)
//                                           .addCol(tblPaymentGateways::pgwLastPaymentDateTime)
//                                           .addCol(tblPaymentGateways::pgwSumTodayPaidAmount)
//                                           .addCol(tblPaymentGateways::pgwSumRequestCount)
//                                           .addCol(tblPaymentGateways::pgwSumRequestAmount)
//                                           .addCol(tblPaymentGateways::pgwSumFailedCount)
//                                           .addCol(tblPaymentGateways::pgwSumOkCount)
//                                           .addCol(tblPaymentGateways::pgwSumPaidAmount)
//                                           .addCol(tblPaymentGateways::pgwStatus)
                                           .values(PaymentGatewayValues)
                                           .execute(1);

                PaymentGatewayValues.insert(tblPaymentGateways::pgwID, PaymentGatewayID);
//                CreatedPaymentGateways.in(PaymentGatewayValues);
            }
            catch(std::exception &exp)
            {
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

QVariant Account::apiPOSTfixtureCleanup(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _random
    )
{
    Q_UNUSED(_REMOTE_IP);

    QVariantMap Result;

    clsDAC DAC;

    QString UserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "user@dev.test");
    QString AdminUserEmail = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "admin@dev.test");

//    OR u.usrMobile LIKE '+98999887%'

    try
    {
        QString QueryString = R"(
            DELETE wb
              FROM tblWalletBalances wb
        INNER JOIN tblWalletsTransactions wt
                ON wt.wltID = wb.wbl_wltID
        INNER JOIN tblUserWallets uw
                ON uw.walID = wt.wlt_walID
        INNER JOIN tblUser u
                ON u.usrID = uw.wal_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblWalletBalances", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    }
    catch(...)
    {
    }

    try
    {
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
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblWalletsTransactions", QVariantMap({
                                                                { "numRowsAffected", DACResult.numRowsAffected() },
                                                            }));
    }
    catch(...)
    {
    }

    try
    {
        QString QueryString = R"(
            DELETE uw
              FROM tblUserWallets uw
        INNER JOIN tblUser u
                ON u.usrID = uw.wal_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblUserWallets", QVariantMap({
                                                        { "numRowsAffected", DACResult.numRowsAffected() },
                                                    }));
    }
    catch(...)
    {
    }

    try
    {
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
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblOnlinePayments", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    }
    catch(...)
    {
    }

    try
    {
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
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblOfflinePayments", QVariantMap({
                                                            { "numRowsAffected", DACResult.numRowsAffected() },
                                                        }));
    }
    catch(...)
    {
    }

    try
    {
        QString QueryString = R"(
            DELETE vch
              FROM tblVoucher vch
        INNER JOIN tblUser u
                ON u.usrID = vch.vch_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblVoucher", QVariantMap({
                                                    { "numRowsAffected", DACResult.numRowsAffected() },
                                                }));
    }
    catch(...)
    {
    }

    try
    {
        QString QueryString = R"(
            DELETE apr
              FROM tblApprovalRequest apr
        INNER JOIN tblUser u
                ON u.usrID = apr.apr_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblApprovalRequest", QVariantMap({
                                                            { "numRowsAffected", DACResult.numRowsAffected() },
                                                        }));
    }
    catch(...)
    {
    }

    try
    {
        QString QueryString = R"(
            DELETE sn
              FROM tblActiveSessions sn
        INNER JOIN tblUser u
                ON u.usrID = sn.ssn_usrID
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblActiveSessions", QVariantMap({
                                                           { "numRowsAffected", DACResult.numRowsAffected() },
                                                       }));
    }
    catch(...)
    {
    }

    try
    {
        QString QueryString = R"(
            DELETE u
              FROM tblUser u
             WHERE u.usrEmail=?
                OR u.usrEmail=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   UserEmail,
                                                   AdminUserEmail
                                               });
        Result.insert("tblUser", QVariantMap({
                                                 { "items", QStringList({ UserEmail, AdminUserEmail }).join(",") },
                                                 { "numRowsAffected", DACResult.numRowsAffected() },
                                             }));
    }
    catch(...)
    {
    }

    try
    {
        QString RoleName = FixtureHelper::MakeRandomizeName(_random, "-", "fixture", "role");
        QString QueryString = R"(
            DELETE r
              FROM tblRoles r
             WHERE r.rolName=?
        ;)";
        clsDACResult DACResult = DAC.execQuery("", QueryString, {
                                                   RoleName
                                               });
        Result.insert("tblRoles", QVariantMap({
                                                  { "items", RoleName },
                                                  { "numRowsAffected", DACResult.numRowsAffected() },
                                              }));
    }
    catch(...)
    {
    }

    return Result;
}

///TODO: not tested
bool Account::apiPOSTfixtureApproveEmail(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _email
    )
{
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

    this->apiPOSTapproveEmail(_REMOTE_IP, _email, Code);

    return true;
}

///TODO: not tested
bool Account::apiPOSTfixtureApproveMobile(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::Mobile_t _mobile
    )
{
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

    this->apiPOSTapproveMobile(_REMOTE_IP, _mobile, Code.toUInt());

    return true;
}

#endif

} //namespace Targoman::API::AccountModule
