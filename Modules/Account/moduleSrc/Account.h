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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_AAA_H
#define TARGOMAN_API_MODULES_ACCOUNT_AAA_H

#include "Interfaces/ORM/intfActionLogs.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedWithActionLogsModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Payments.h"
#include "ORM/Voucher.h"
#include "Classes/Defs.hpp"
#include "Payment/PaymentLogic.h"

namespace Targoman::API::AccountModule {

//struct stuMultiJWT {
//    TAPI::EncodedJWT_t Login;
//    TAPI::EncodedJWT_t Session;
//};

TARGOMAN_ACTIONLOG_PREPARENT;

class Account : public intfSQLBasedWithActionLogsModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Account);
    TARGOMAN_DEFINE_API_MODULE(Account);
    TARGOMAN_API_DEFINE_ACTIONLOG(Account, AAASchema);

public:
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> InvalidPasswordsFile;

    virtual QJsonObject todayPrivs(quint64 _usrID) final { Q_UNUSED(_usrID) return {}; }

public:
    static Targoman::API::AAA::stuVoucher processVoucher(
//        intfAPICallBoom &APICALLBOOM_PARAM,
        quint64 _userID,
        quint64 _voucherID
    );
    static void tryCancelVoucher(
//        intfAPICallBoom &APICALLBOOM_PARAM,
        quint64 _userID,
        quint64 _voucherID,
        bool _setAsError = false
    );

private:
//    TAPI::EncodedJWT_t createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services);
    TAPI::EncodedJWT_t createJWTAndSaveToActiveSession(const QString _login, const stuActiveAccount& _activeAccount, const QString& _services = {});

private slots:
    /*****************************************************************\
    |* User **********************************************************|
    \*****************************************************************/
    QString REST_GET_OR_POST(
        normalizePhoneNumber,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _phone,
            QString _country
        ),
        "Normalize and format phone number as E164 standard"
    )

    QVariantMap REST_PUT(
        signup,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile,
            TAPI::MD5_t _pass,
            QString _role = "BaseUser",
            QString _name = "",
            QString _family = "",
            bool _enableEmailAlerts = true,
            bool _enableSMSAlerts = true,
            TAPI::JSON_t _specialPrivs = {},
            qint8 _maxSessions = -1
        ),
        "Base method for signup with email or mobile. this method can be called just by predefined IPs"
    )

    TAPI::EncodedJWT_t REST_POST(
        approveEmail,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _email,
            TAPI::MD5_t _uuid,
            bool _autoLogin = false,
            TAPI::CommaSeparatedStringList_t _services = {},
            bool _rememberMe = false,
            TAPI::JSON_t _sessionInfo = {},
            TAPI::MD5_t _fingerprint = {}
        ),
        "Approves Email by provided UUID, then login if needed"
    )

    TAPI::EncodedJWT_t REST_POST(
        approveMobile,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::Mobile_t _mobile,
            quint32 _code,
            bool _autoLogin = false,
            TAPI::CommaSeparatedStringList_t _services = {},
            bool _rememberMe = false,
            TAPI::JSON_t _sessionInfo = {},
            TAPI::MD5_t _fingerprint = {}
        ),
        "Approves Mobile by provided mobile and verify code, then login if needed"
    )

    TAPI::EncodedJWT_t REST_GET_OR_POST(
        login,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile,
            TAPI::MD5_t _pass,
            QString _salt,
            TAPI::CommaSeparatedStringList_t _services = {},
            bool _rememberMe = false,
            TAPI::JSON_t _sessionInfo = {},
            TAPI::MD5_t _fingerprint = {}
        ),
        "Login user by email or mobile and return an encoded JWT."
        "if services are provided (as comma separated list) then user must have access to specified services"
    )

    bool REST_GET_OR_POST(
        loginByMobileOnly,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::Mobile_t _mobile,
            bool _signupIfNotExists = false,
            QString _signupRole = "BaseUser",
            bool _signupEnableEmailAlerts = true,
            bool _signupEnableSMSAlerts = true
//            quint32 _verifyCode = 0/* = {}*/,
//            TAPI::MD5_t _pass = {},
//            QString _salt = {},
//            TAPI::CommaSeparatedStringList_t _services = {},
//            bool _rememberMe = false,
//            TAPI::JSON_t _sessionInfo = {},
//            TAPI::MD5_t _fingerprint = {}
        ),
        "Login user by mobile only and return an encoded JWT."
        "A new random code is generated and sent to the user via SMS."
        "After the user submits this code, approveMobile must be called with verifyCode."
        "If services are provided (as comma separated list) then user must have access to specified services."
    )

    bool REST_GET_OR_POST(
        resendApprovalCode,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "Recreate (if expired) approval code and resend last valid code to the email or mobile."
    )

//    bool REST_PUT(
//        requestMobileVerifyCode,
//        (
//            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
//            TAPI::Mobile_t _mobile
//        ),
//        "Send verification code for provided mobile."
//    )

//    TAPI::EncodedJWT_t REST_PUT(
//        verifyLoginByMobileCode,
//        (
//            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
//            TAPI::Mobile_t _mobile,
//            quint32 _code,
//            TAPI::CommaSeparatedStringList_t _services = {},
//            bool _rememberMe = false,
//            TAPI::JSON_t _sessionInfo = {},
//            TAPI::MD5_t _fingerprint = {}
//        ),
//        "check verification code for provided mobile."
//    )

    TAPI::EncodedJWT_t REST_GET_OR_POST(
        loginByOAuth,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::enuOAuthType::Type _type,
            QString _oAuthToken,
            TAPI::CommaSeparatedStringList_t _services,
            TAPI::JSON_t _sessionInfo = TAPI::JSON_t(),
            TAPI::MD5_t _fingerprint = {}
        ),
        "Login by Open Authentication and return an encoded JWT"
    )

//    Targoman::API::AccountModule::stuMultiJWT REST_GET_OR_POST(
//        refreshJWT,
//        (
//            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
//            TAPI::JWT_t _loginJWT,
//            QString _services = {}
//        ),
//        "Refresh JWT in order to update information or expiry time. Provide services in order to create service specific JWT"
//    )

    bool REST_GET_OR_POST(
        logout,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM
        ),
        "Logout logged in user"
    )

    QString REST_GET_OR_POST(
        createForgotPasswordLink,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "Create a forgot password request returning a UUID for the requiest"
    )

#ifdef QT_DEBUG
    QString REST_POST(
        fixtureGetLastForgotPasswordUUIDAndMakeAsSent,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "fixture: Get Last Forgot Password UUID And Make As Sent"
    )
#endif

    bool REST_GET_OR_POST(
        changePassByUUID,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile,
            QString _uuid,
            TAPI::MD5_t _newPass
        ),
        "Changes password based on a Code provided by createForgotPasswordLink"
    )

    bool REST_GET_OR_POST(
        changePass,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::MD5_t _oldPass,
            QString     _oldPassSalt,
            TAPI::MD5_t _newPass
        ),
        "Changes password of the logged-in user"
    )

    /*****************************************************************\
    |* Voucher & Payments ********************************************|
    \*****************************************************************/
//    QVariantList REST_GET_OR_POST(
//        gatewayTypesForFinalizeBasket,
//        (
//            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
//            Targoman::API::AAA::stuPreVoucher _preVoucher,
//            QString _domain
//        ),
//        "get list of payment gateway types valid for voucher and domain"
//    )

    Targoman::API::AAA::stuVoucher REST_POST(
        finalizeBasket,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuPreVoucher _preVoucher,
            Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
            QString _domain,
            qint64 _walletID = -1,
            QString _paymentVerifyCallback = {}
        ),
        "create a voucher based on preVoucher. "
        "Set gatewayType = COD for offline payment, url for online payment or keep empty for wallet payment,"
        "Also set walletID >0 to use specified wallet, 0 for using default wallet, <0 to discard wallet usage."
        "When callback is set to URL you must specify payment gateway"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOnlinePayment,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
//            Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
            const QString _paymentMD5,
            const QString _domain,
            TAPI::JSON_t _pgResponse
        ),
        "approve payment back from payment gateway"
    )

    quint64 REST_POST(
        claimOfflinePayment,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _vchID,
            const QString& _bank,
            const QString& _receiptCode,
            TAPI::Date_t _receiptDate,
            quint32 _amount,
            const QString& _note = {}
        ),
        "claim offline payment by user"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOfflinePayment,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _offlinePaymentClaimID
        ),
        "approve Voucher by offline payment"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOfflinePayment_withBankInfo,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _vchID,
            const QString& _bank,
            const QString& _receiptCode,
            TAPI::Date_t _receiptDate,
            quint32 _amount,
            const QString& _note = {}
        ),
        "approve Voucher by offline payment"
    )

    ///TODO: create API for cancelBasketItem
    ///TODO: create API for returnBasketItem

    bool REST_POST(
        addPrizeTo,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _targetUsrID,
            quint64 _amount,
            TAPI::JSON_t _desc
        ),
        "add prize to a user by priviledged user. "
        "Description object must contain at least an string field named 'desc'"
    )

    bool REST_POST(
        addIncomeTo,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _targetUsrID,
            quint64 _amount,
            TAPI::JSON_t _desc
        ),
        "add income to a user by priviledged user. "
        "Description object must contain at least an string field named 'desc'"
    )

    bool REST_POST(
        checkVoucherTTL,
        (
            APICALLBOOM_TYPE_JWT_DECL &APICALLBOOM_PARAM,
            quint64 _voucherID
        ),
        "Check voucher and items for ttl"
    )

#ifdef QT_DEBUG
    QVariant REST_POST(
        fixtureSetup,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Create sample data. give random=1 to auto generate random number"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Cleanup sample data"
    )
    bool REST_POST(
        fixtureApproveEmail,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            QString _email
        ),
        "Approve Email Address directly"
    )
    bool REST_POST(
        fixtureApproveMobile,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            TAPI::Mobile_t _mobile
        ),
        "Approve Mobile directly"
    )
#endif

};

TARGOMAN_ACTIONLOG_POSTPARENT(Account, AAASchema);

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(TAPI, enuOAuthType);
//TAPI_DECLARE_METATYPE(Targoman::API::AccountModule::stuMultiJWT);

#endif // TARGOMAN_API_MODULES_ACCOUNT_AAA_H
