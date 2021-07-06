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
#include "QFieldValidator.h"
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

#include "Classes/PaymentLogic.h"
#include "PaymentGateways/intfPaymentGateway.hpp"

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;
#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuOAuthType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserGender);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuUserApproval);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAuditableStatus);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuSaleableType);
TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    TAPI, stuMultiJWT,
    [](const TAPI::stuMultiJWT& _value) -> QVariant{return QJsonObject({{"ssn",_value.Session}, {"lgn", _value.Login}}).toVariantMap();}
);
namespace Targoman {
namespace API {

using namespace DBManager;
using namespace Common;
using namespace Common::Configuration;

static QSet<QString> InvalidPasswords = {
    "d41d8cd98f00b204e9800998ecf8427e",
    "c4ca4238a0b923820dcc509a6f75849b",
    "c81e728d9d4c2f636f067f89cc14862c",
    "eccbc87e4b5ce2fe28308fd9f2a7baf3",
    "a87ff679a2f3e71d9181a67b7542122c",
    "e4da3b7fbbce2345d7772b0674a318d5",
    "1679091c5a880faf6fb5e6087eb1b2dc",
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

TARGOMAN_DEFINE_ENUM (enuPaymentType,
                      Online='N',
                      Offline='F'
                              )

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Account);

Targoman::Common::Configuration::tmplConfigurableArray<intfPaymentGateway::stuGateWay> intfPaymentGateway::GatewayEndPoints(
        AAA::Accounting::makeConfig("GatewayEndPoints"),
        "Registered payment gateways",
        0
        );


Targoman::Common::Configuration::tmplConfigurable<FilePath_t> intfPaymentGateway::TransactionLogFile(
        AAA::Accounting::makeConfig("TransactionLogFile"),
        "File to store transaction logs",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Writeatble),
        false>,
        "",
        "FILEPATH",
        "transacton-log-file",
        enuConfigSource::Arg | enuConfigSource::File
        );

Targoman::Common::Configuration::tmplConfigurable<FilePath_t> Account::InvalidPasswordsFile(
        AAA::Accounting::makeConfig("InvalidPasswordsFile"),
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

TAPI::stuMultiJWT Account::apiLogin(TAPI::RemoteIP_t _REMOTE_IP,
                                     QString _login,
                                     TAPI::MD5_t _pass,
                                     QString _salt,
                                     TAPI::CommaSeparatedStringList_t _services,
                                     bool _rememberMe,
                                     TAPI::JSON_t _sessionInfo,
                                     TAPI::MD5_t _fingerprint)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");
    QFV.asciiAlNum().maxLenght(20).validate(_salt, "salt");

    Authorization::validateIPAddress(_REMOTE_IP);

    auto LoginInfo = Authentication::login(_REMOTE_IP,
                                           _login,
                                           _pass,
                                           _salt,
                                           _services.split(",", QString::SkipEmptyParts),
                                           _rememberMe,
                                           _sessionInfo.object(),
                                           _fingerprint);

    return TAPI::stuMultiJWT({
                                 this->createLoginJWT(_rememberMe, _login, LoginInfo.Privs["ssnKey"].toString(), _services),
                                 this->createJWT(_login, LoginInfo, _services)
                             });
}

//TODO cache to ban users for every service
//TODO update cache for each module
//TODO JWT lifetime dynamic based on current hour
TAPI::stuMultiJWT Account::apiLoginByOAuth(TAPI::RemoteIP_t _REMOTE_IP,
                                            TAPI::enuOAuthType::Type _type,
                                            QString _oAuthToken,
                                            TAPI::CommaSeparatedStringList_t _services,
                                            TAPI::JSON_t _sessionInfo,
                                            TAPI::MD5_t _fingerprint)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    QString Login;
    Authentication::stuOAuthInfo OAuthInfo;
    //TODO validate _oAuthToken

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

    auto LoginInfo = Authentication::login(_REMOTE_IP, OAuthInfo.Email, nullptr, nullptr, _services.split(","), true, _sessionInfo.object(), _fingerprint);
    return TAPI::stuMultiJWT({
                                 this->createLoginJWT(true, OAuthInfo.Email, LoginInfo.Privs["ssnKey"].toString(), _services),
                                 this->createJWT(OAuthInfo.Email, LoginInfo, _services)
                             });
}

TAPI::stuMultiJWT Account::apiRefreshJWT(TAPI::RemoteIP_t _REMOTE_IP, TAPI::JWT_t _loginJWT, QString _services)
{
    QJsonObject Obj;

    Authorization::validateIPAddress(_REMOTE_IP);

    clsJWT LoginJWT(_loginJWT);
    QString Services = _services;
    if(_services.isEmpty())
        Services = LoginJWT.privatePart().value("svc").toString();

    auto NewPrivs = Authentication::updatePrivs(_REMOTE_IP, LoginJWT.session(), Services);
    return TAPI::stuMultiJWT({
                                 this->createLoginJWT(true, LoginJWT.login(), LoginJWT.session(), Services),
                                 this->createJWT(LoginJWT.login(), NewPrivs, Services)
                             });
}

QVariantMap Account::apiPUTSignup(TAPI::RemoteIP_t _REMOTE_IP,
                                  QString _emailOrMobile,
                                  TAPI::MD5_t _pass,
                                  QString _role,
                                  QString _name,
                                  QString _family,
                                  TAPI::JSON_t _specialPrivs,
                                  qint8 _maxSessions)
{
    QString Type;
    if(QFV.email().isValid(_emailOrMobile)){
        if(QFV.emailNotFake().isValid(_emailOrMobile))
            Type = 'E';
        else
            throw exHTTPBadRequest("Email domain is suspicious. Please use a real email.");
    }else if(QFV.mobile().isValid(_emailOrMobile))
        Type = 'M';
    else
        throw exHTTPBadRequest("emailOrMobile must be by a valid email or mobile");

    QFV.asciiAlNum().maxLenght(50).validate(_role);
    if(_role.toLower() == "administrator" || _role.toLower() == "system" || _role.toLower() == "baseuser")
        throw exHTTPForbidden("Selected role is not allowed to signup");

    if(InvalidPasswords.contains(_pass))
        throw exHTTPBadRequest("Invalid simple password");

    return  {
        {"type", Type == 'E' ? "email" : "mobile"},
            {"usrID",
                this->callSP("AAA.sp_CREATE_signup", {
                                 {"iBy", Type},
                                 {"iLogin", _emailOrMobile},
                                 {"iPass", _pass},
                                 {"iRole", _role},
                                 {"iIP", _REMOTE_IP},
                                 {"iName", _name.isEmpty()? QVariant() : _name},
                                 {"iFamily", _family.isEmpty()? QVariant() : _family},
                                 {"iSpecialPrivs", _specialPrivs.isEmpty()? QVariant() : _specialPrivs},
                                 {"iMaxSessions", _maxSessions},
                                 {"iCreatorUserID", QVariant()},
                             }).spDirectOutputs().value("oUserID").toDouble()
            }
        };
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

QString Account::apiCreateForgotPasswordLink(TAPI::RemoteIP_t _REMOTE_IP, QString _login)
{
    QFV.oneOf({QFV.emailNotFake(), QFV.mobile()}).validate(_login, "login");

    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP("AAA.sp_CREATE_forgotPassRequest", {
                     {"iLogin", _login},
                     {"iVia", QString(_login.contains('@') ? 'E' : 'M')},
                 });
    return _login.contains('@') ? "email" : "mobile";
}

bool Account::apiChangePass(TAPI::JWT_t _JWT, TAPI::MD5_t _oldPass, QString _oldPassSalt, TAPI::MD5_t _newPass)
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

bool Account::apiChangePassByUUID(TAPI::RemoteIP_t _REMOTE_IP, TAPI::MD5_t _uuid, TAPI::MD5_t _newPass)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP("AAA.sp_UPDATE_changePassByUUID", {
                     {"iUUID", _uuid},
                     {"iNewPass", _newPass},
                 });
    return true;
}

bool Account::apiPOSTApproveEmail(TAPI::RemoteIP_t _REMOTE_IP,
                                  TAPI::MD5_t _uuid)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP( "AAA.sp_UPDATE_acceptApproval", {
                      {"iUUID", _uuid},
                      {"iMobile", {}},
                  });
    return true;
}

bool Account::apiPOSTApproveMobile(TAPI::RemoteIP_t _REMOTE_IP,
                                   TAPI::Mobile_t _mobile,
                                   quint32 _code)
{
    Authorization::validateIPAddress(_REMOTE_IP);
    this->callSP( "AAA.sp_UPDATE_acceptApproval", {
                      {"iUUID", _code},
                      {"iMobile", _mobile},
                  });
    return true;
}

TAPI::stuVoucher processVoucher(quint64 _vchID){
    try {
        return PaymentLogic::processVoucher(_vchID);
    }  catch (...) {
        ///TODO create cancel voucher and credit to wallet
        throw;
    }
}

///TODO remove _gateway from parameters
///TODO select gateway (null|single|multiple) from service
///TODO check for common gateway voucher
TAPI::stuVoucher Account::apiPOSTfinalizeBasket(TAPI::JWT_t _JWT,
                                                TAPI::stuPreVoucher _preVoucher,
                                                QString _callBack,
                                                qint64 _walletID,
                                                TAPI::enuPaymentGateway::Type _gateway)
{
    if(_callBack.size() && _callBack != "OFFLINE")
        QFV.url().validate(_callBack, "callBack");

    if(_preVoucher.Items.isEmpty())
        throw exHTTPBadRequest("seems that pre-Voucher is empty");

    Accounting::checkPreVoucherSanity(_preVoucher);

    TAPI::stuVoucher Voucher;
    Voucher.Info = _preVoucher;
    ///TODO remove sign from prevoucher before converting to JSON
    ///TODO recalculate prevoucher to check either price change/package expiry/coupon limits/ etc.
    ///TODO reserve saleables before returning voucher
    ///TODO implement overall coupon at the end of checkout steps

//    Voucher.ID   = Voucher::instance().create(clsJWT(_JWT).usrID(),
    Voucher.ID = Targoman::API::Query::Create(Voucher::instance(),
                                              clsJWT(_JWT).usrID(),
                                              TAPI::ORMFields_t({
                                                  { tblVoucher::vch_usrID,clsJWT(_JWT).usrID() },
                                                  { tblVoucher::vchDesc, _preVoucher.toJson() },
                                                  { tblVoucher::vchTotalAmount, _preVoucher.ToPay }
                                              })); //.toULongLong();
    try {
        qint64 RemainingAfterWallet = static_cast<qint64>(_preVoucher.ToPay);
        if (_walletID>=0 && RemainingAfterWallet>0) {
            clsDACResult Result = this->callSP("sp_CREATE_walletTransaction", {
                                                   {"iWalletID", _walletID},
                                                   {"iVoucherID", Voucher.ID},
                                               });
            RemainingAfterWallet -= Result.spDirectOutputs().value("oAmount").toUInt();
            if(RemainingAfterWallet < 0)
                throw exHTTPInternalServerError("Remaining after wallet transaction is negative!");
        }

        if (RemainingAfterWallet > 0) {
            ///TODO rename OFFLINE to COD (as constant)
            if (_callBack == "OFFLINE") {
                //Do nothing as it will be created after information upload.
            } else {
                Voucher.PaymentLink = PaymentLogic::createOnlinePaymentLink(_gateway, Voucher.ID, _preVoucher.Summary, RemainingAfterWallet, _callBack);
            }
        } else {
            return processVoucher(Voucher.ID);
        }
    } catch (...) {
        Targoman::API::Query::Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, TAPI::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, Voucher.ID }
                                     });
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
TAPI::stuVoucher Account::apiPOSTapproveOnlinePayment(TAPI::enuPaymentGateway::Type _gateway, const QString _domain, TAPI::JSON_t _pgResponse)
{
    quint64 VoucherID = PaymentLogic::approveOnlinePayment(_gateway, _pgResponse, _domain);
    try {
        this->callSP("sp_CREATE_walletTransactionOnPayment", {
                         {"iVoucherID", VoucherID},
                         {"iPaymentType", enuPaymentType::Online}
                     });
        return processVoucher(VoucherID);
    } catch(...) {
        Targoman::API::Query::Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, TAPI::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, VoucherID }
                                     });
        throw;
    }
}

///TODO implement auto verify daemon OJO on failed payments in the daemon

TAPI::stuVoucher Account::apiPOSTapproveOfflinePayment(TAPI::JWT_t _JWT,
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

    Targoman::API::Query::Create(OfflinePayments::instance(),
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
        this->callSP("sp_CREATE_walletTransactionOnPayment", {
                         {"iVoucherID", _vchID},
                         {"iPaymentType", enuPaymentType::Offline}
                     });
        return processVoucher(_vchID);
    }  catch (...) {
        Targoman::API::Query::Update(Voucher::instance(),
                                     SYSTEM_USER_ID,
                                     {},
                                     TAPI::ORMFields_t({
                                        { tblVoucher::vchStatus, TAPI::enuVoucherStatus::Error }
                                     }),
                                     {
                                        { tblVoucher::vchID, _vchID }
                                     });
        throw;
    }
}

bool Account::apiPOSTaddPrizeTo(TAPI::JWT_t _JWT, quint64 _targetUsrID, quint64 _amount, TAPI::JSON_t _desc)
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
                     {"iType", QString(static_cast<char>(TAPI::enuVoucherType::Prize))},
                     {"iAmount", _amount},
                     {"iDesc", _desc.toJson()},
                 });
    return true;
}

bool Account::apiPOSTaddIncomeTo(TAPI::JWT_t _JWT, quint64 _targetUsrID, quint64 _amount, TAPI::JSON_t _desc)
{
    qint64 Limit = Authorization::getPrivValue(_JWT, "AAA:addIncomeTo:maxAmount").toLongLong();
    if(Limit == 0)
        throw exAuthorization("Not enough access to add income");

    if(Limit > 0 && _amount > static_cast<quint64>(Limit))
        throw exAuthorization("Amount is greater than your limits");

    QFV.hasKey("desc").validate(_desc, "desc");

    this->callSP("sp_CREATE_increaseWallet", {
                     {"iWalletID", 0},
                     {"iToUsrID", _targetUsrID},
                     {"iType", QString(static_cast<char>(TAPI::enuVoucherType::Income))},
                     {"iAmount", _amount},
                     {"iDesc", _desc.toJson()},
                 });
    return true;
}

Account::Account() :
    clsRESTAPIWithActionLogs("AAA", "Account")
{
    this->addSubModule(&ActiveSessions::instance());
    this->addSubModule(&APITokens::instance());
    this->addSubModule(&APITokenValidIPs::instance());
    this->addSubModule(&ApprovalRequest::instance());
    this->addSubModule(&BlockingRules::instance());
    this->addSubModule(&ForgotPassRequest::instance());
    this->addSubModule(&Voucher::instance());
    this->addSubModule(&IPBin::instance());
    this->addSubModule(&IPStats::instance());
    this->addSubModule(&OnlinePayments::instance());
    this->addSubModule(&OfflinePayments::instance());
    this->addSubModule(&Roles::instance());
    this->addSubModule(&Service::instance());
    this->addSubModule(&User::instance());
    this->addSubModule(&UserExtraInfo::instance());
    this->addSubModule(&UserWallets::instance());
    this->addSubModule(&WalletTransactions::instance());
    this->addSubModule(&WalletBalances::instance());

    if(Account::InvalidPasswordsFile.value().size()){
        QFile InputFile(Account::InvalidPasswordsFile.value());
        if (InputFile.open(QIODevice::ReadOnly)) {
            QTextStream Stream(&InputFile);
            while (!Stream.atEnd())
                InvalidPasswords.insert(Stream.readLine().replace(QRegularExpression("#.*"), ""));
            InputFile.close();
        }
    }
}

TAPI::EncodedJWT_t Account::createJWT(const QString _login, const stuActiveAccount& _activeAccount, const QString& _services)
{
    return clsJWT::createSigned({
                                    {JWTItems::usrLogin,        _login},
                                    {JWTItems::usrName,         _activeAccount.Privs["usrGivenName"]},
                                    {JWTItems::usrFamily,       _activeAccount.Privs["usrFamilyName"]},
                                    {JWTItems::rolName,         _activeAccount.Privs["rolName"]},
                                    {JWTItems::rolID,           _activeAccount.Privs["rolID"]},
                                    {JWTItems::privs,           _activeAccount.Privs["privs"]},
                                    {JWTItems::usrID,           _activeAccount.Privs["usrID"]},
                                    {JWTItems::canChangePass,   _activeAccount.Privs["hasPass"]},
                                    {JWTItems::usrApproval,     TAPI::enuUserApproval::toStr(_activeAccount.Privs["usrApprovalState"].toString())},
                                    {JWTItems::usrStatus,       TAPI::enuUserStatus::toStr(_activeAccount.Privs["usrStatus"].toString())},
                                },
                                QJsonObject({{"svc", _services}}),
                                _activeAccount.TTL,
                                _activeAccount.Privs["ssnKey"].toString()
            );
}

TAPI::EncodedJWT_t Account::createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services)
{
    return clsJWT::createSignedLogin(_remember, { {JWTItems::usrLogin, _login} }, QJsonObject({{"svc", _services}}), _ssid);
}

}
}
