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
#include "Interfaces/ORM/intfMigrations.h"
#include "Interfaces/ObjectStorage/ORM/ObjectStorage.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "Interfaces/API/intfSQLBasedModule.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Payments.h"
#include "ORM/Voucher.h"
#include "Classes/Defs.hpp"
#include "Payment/PaymentLogic.h"
#include "Interfaces/ORM/intfFAQ.h"

namespace Targoman::API::AccountModule {

//struct stuMultiJWT {
//    TAPI::EncodedJWT_t Login;
//    TAPI::EncodedJWT_t Session;
//};

TARGOMAN_MIGRATIONS_PREPARENT;
TARGOMAN_ACTIONLOG_PREPARENT;
TARGOMAN_OBJECTSTORAGE_PREPARENT;
TARGOMAN_FAQ_PREPARENT;

class Account : public intfSQLBasedModule //intfSQLBasedWithActionLogsModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFPUREMODULE_IID)
    Q_INTERFACES(Targoman::API::API::intfPureModule)
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE(Account); //, enuTokenActorType::USER);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_DB_CONFIGS(Account);
    //---------------------------------------------------------
    TARGOMAN_API_MODULE_DEFINE_MIGRATIONS(Account, AAASchema);
    TARGOMAN_API_MODULE_DEFINE_ACTIONLOG(Account, AAASchema);
    TARGOMAN_API_MODULE_DEFINE_OBJECTSTORAGE(Account, AAASchema);
    TARGOMAN_API_MODULE_DEFINE_FAQ(Account, AAASchema);

public:
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> InvalidPasswordsFile;
    void initializeModule();

    virtual QJsonObject todayPrivs(quint64 _usrID) final { Q_UNUSED(_usrID) return {}; }

private:
//    TAPI::EncodedJWT_t createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services);
    TAPI::EncodedJWT_t createJWTAndSaveToActiveSession(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const QString _login,
        const stuActiveAccount& _activeAccount,
        const QString& _services = {}
    );

private slots:
    /*****************************************************************\
    |* User **********************************************************|
    \*****************************************************************/
    QString REST_GET_OR_POST(
        normalizePhoneNumber,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _phone,
            QString _country
        ),
        "Normalize and format phone number as E164 standard"
    )

    QVariantMap REST_PUT(
        signup,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
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

    /*TAPI::EncodedJWT_t*/QVariantMap REST_POST(
        approveEmail,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _email,
            TAPI::MD5_t _code,
            bool _autoLogin = false,
            TAPI::CommaSeparatedStringList_t _services = {},
            bool _rememberMe = false,
            TAPI::JSON_t _sessionInfo = {},
            TAPI::MD5_t _fingerprint = {}
        ),
        "Approves Email by provided UUID, then login if needed"
    )

    /*TAPI::EncodedJWT_t*/QVariantMap REST_POST(
        approveMobile,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
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

    /*TAPI::EncodedJWT_t*/QVariantMap REST_GET_OR_POST(
        login,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
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
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            TAPI::Mobile_t _mobile,
            bool _signupIfNotExists = false,
            QString _signupRole = "BaseUser",
            bool _signupEnableEmailAlerts = true,
            bool _signupEnableSMSAlerts = true
        ),
        "Login user by mobile only and return an encoded JWT."
        "A new random code is generated and sent to the user via SMS."
        "After the user submits this code, approveMobile must be called with verifyCode."
        "If services are provided (as comma separated list) then user must have access to specified services."
    )

    bool REST_GET_OR_POST(
        resendApprovalCode,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "Recreate (if expired) approval code and resend last valid code to the email or mobile."
    )

//    bool REST_PUT(
//        requestMobileVerifyCode,
//        (
//            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
//            TAPI::Mobile_t _mobile
//        ),
//        "Send verification code for provided mobile."
//    )

//    TAPI::EncodedJWT_t REST_PUT(
//        verifyLoginByMobileCode,
//        (
//            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
//            TAPI::Mobile_t _mobile,
//            quint32 _code,
//            TAPI::CommaSeparatedStringList_t _services = {},
//            bool _rememberMe = false,
//            TAPI::JSON_t _sessionInfo = {},
//            TAPI::MD5_t _fingerprint = {}
//        ),
//        "check verification code for provided mobile."
//    )

    /*TAPI::EncodedJWT_t*/QVariantMap REST_GET_OR_POST(
        loginByOAuth,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            TAPI::enuOAuthType::Type _type,
            QString _oAuthToken,
            TAPI::CommaSeparatedStringList_t _services = {},
            TAPI::JSON_t _sessionInfo = TAPI::JSON_t(),
            TAPI::MD5_t _fingerprint = {}
        ),
        "Login by Open Authentication and return an encoded JWT"
    )

//    Targoman::API::AccountModule::stuMultiJWT REST_GET_OR_POST(
//        refreshJWT,
//        (
//            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
//            TAPI::JWT_t _loginJWT,
//            QString _services = {}
//        ),
//        "Refresh JWT in order to update information or expiry time. Provide services in order to create service specific JWT"
//    )

    bool REST_GET_OR_POST(
        logout,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
        ),
        "Logout logged in user"
    )

    bool REST_GET_OR_POST(
        createForgotPasswordLink,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "Create a forgot password request returning a UUID for the request"
    )

    bool REST_GET_OR_POST(
        changePass,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            TAPI::MD5_t _newPass,
            TAPI::MD5_t _oldPass = {},
            QString     _oldPassSalt = {}
        ),
        "Changes password of the logged-in user. "
        "Do not set oldPass to set a new password when the current password is empty."
    )

    bool REST_GET_OR_POST(
        changePassByUUID,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile,
            QString _uuid,
            TAPI::MD5_t _newPass
        ),
        "Changes password based on a Code provided by createForgotPasswordLink"
    )

    /*****************************************************************\
    |* Voucher & Payments ********************************************|
    \*****************************************************************/
private:
    Targoman::API::AAA::stuVoucher processVoucher(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        quint64 _userID,
        quint64 _voucherID
    );

    void tryCancelVoucher(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
//        quint64 _userID,
        quint64 _voucherID,
        bool _setAsError = false
    );

//    Targoman::API::AAA::stuVoucher payAndProcessBasket(
//        APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
//        QString _domain,
//        quint64 _voucherID,
//        NULLABLE_TYPE(Targoman::API::AccountModule::enuPaymentGatewayType::Type) _gatewayType = NULLABLE_NULL_VALUE,
//        qint64 _amount = -1, //-1: rest of voucher's remained amount
//        qint64 _walID = -1, //-1: no wallet
//        QString _paymentVerifyCallback = {}
//    );

private slots:
//    Targoman::API::AAA::stuPreVoucher REST_POST(
//        mergeBasket,
//        (
//            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
//            Targoman::API::AAA::stuPreVoucher _lastPreVoucher
//        ),
//        "Merge given pre-Voucher and user's new asset items by service"
//    )

//    Targoman::API::AAA::stuPreVoucher REST_POST(
//        getBasket,
//        (
//            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM
//        ),
//        "Fetch and make current user's pre-Voucher"
//    )

//    bool REST_POST(
//        deleteBasket,
//        (
//            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
//            Targoman::API::AAA::stuPreVoucher _lastPreVoucher
//        ),
//        "Remove all items from pre-Voucher"
//    )

    void internalCheckBasketVoucherExpirity(
        const Targoman::API::AccountModule::ORM::tblVoucher::DTO &_voucherDTO
    );

    bool REST_GET(
        checkBasketVoucherExpirity,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM
        ),
        "Check Vouchers for expirity. called by garbage collector"
    )

    bool REST_POST(
        cancelVoucher,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _voucherID
        ),
        "Cancel Voucher"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        finalizeBasket,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuPreVoucher _preVoucher,
            QString _domain,
            Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
//            qint64 _payAmount = -1, //-1: MIN(possible gateway amount, rest of voucher's remained amount)
            QString _paymentVerifyCallback = {},
            qint64 _walID = -1,
//            qint64 _walletAmount = -1, //-1: MIN(available amount in wallet, rest of voucher's remained amount)
            bool _allowChargeWallet = false
        ),
        "create a voucher based on preVoucher. "
        "Set gatewayType = COD for offline payment, url for online payment or keep empty for wallet payment,"
        "Also set walletID >0 to use specified wallet, 0 for using default wallet, <0 to discard wallet usage."
        "When callback is set to URL you must specify payment gateway"
    )

//    Targoman::API::AAA::stuVoucher REST_POST(
//        payForBasket,
//        (
//            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
//            QString _domain,
//            quint64 _voucherID,
//            NULLABLE_TYPE(Targoman::API::AccountModule::enuPaymentGatewayType::Type) _gatewayType = NULLABLE_NULL_VALUE,
//            qint64 _amount = -1, //-1: rest of voucher's remained amount
//            qint64 _walID = -1, //-1: no wallet
//            QString _paymentVerifyCallback = {}
//        ),
//        "Pay for voucher by wallet and/or offline/online payment."
//        "gatewayType=null for pay only by wallet."
//        "amount=-1 for remainig of voucher's amount or >0 for custom amount less than remaining."
//        "At least one of gatewayType or walID must be provided."
//    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOnlinePayment,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _paymentKey,
//            QString _domain,
            TAPI::JSON_t _pgResponse
        ),
        "approve payment back from payment gateway"
    )

    quint64 REST_POST(
        claimOfflinePayment,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            QString _bank,
            QString _receiptCode,
            TAPI::Date_t _receiptDate,
            quint32 _amount,
//            NULLABLE_TYPE(quint64) _voucherID = NULLABLE_NULL_VALUE,
            quint64 _walID = 0,
            QString _note = {},
            TAPI::stuFileInfo _file = {}
        ),
        "Claim offline payment by user."
        "Set vchID to null just for charging wallet, otherwise after increasing the wallet, the voucher will be paid"
    )

    bool REST_POST(
        rejectOfflinePayment,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _offlinePaymentID
        ),
        "reject offline payment claim"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOfflinePayment,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _offlinePaymentID
        ),
        "approve Voucher by offline payment"
    )

//    Targoman::API::AAA::stuVoucher EXREST_POST(
//        approveOfflinePayment_withBankInfo,
//        (
//            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
//            quint64 _vchID,
//            const QString& _bank,
//            const QString& _receiptCode,
//            TAPI::Date_t _receiptDate,
//            quint32 _amount,
//            quint64 _walID = 0,
//            const QString& _note = {}
//        ),
//        "approve Voucher by offline payment",
//        {
//            EXRESTCONFIG_HIDDEN,
//        }
//    )

    ///@TODO: create API for cancelBasketItem
    ///@TODO: create API for returnBasketItem

    quint64 REST_POST(
        addPrizeTo,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _targetUsrID,
            quint64 _amount,
            QString _desc
        ),
        "Add prize to a user by an authorized user"
    )

    quint64 REST_POST(
        addIncomeTo,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _targetUsrID,
            quint64 _amount,
            QString _desc
        ),
        "Add income to a user by an authorized user"
    )

    bool REST_POST(
        checkVoucherTTL,
        (
            APICALLBOOM_TYPE_JWT_USER_DECL &APICALLBOOM_PARAM,
            quint64 _voucherID
        ),
        "Check voucher and items for ttl"
    )

#ifdef QT_DEBUG
    QVariant REST_POST(
        fixtureGetLastForgotPasswordUUIDAndMakeAsSent,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "fixture: Get Last Forgot Password UUID And Make As Sent"
    )

    QVariant REST_POST(
        fixtureGetLastApprovalRequestCodeAndMakeAsSent,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _emailOrMobile
        ),
        "fixture: Get Last Approval Request Code And Make As Sent"
    )

    QVariant REST_POST(
        fixtureSetup,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Create sample data. give random=1 to auto generate random number"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _random = {}
        ),
        "Cleanup sample data"
    )
    bool REST_POST(
        fixtureApproveEmail,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            QString _email
        ),
        "Approve Email Address directly"
    )
    bool REST_POST(
        fixtureApproveMobile,
        (
            APICALLBOOM_TYPE_JWT_ANONYMOUSE_DECL &APICALLBOOM_PARAM,
            TAPI::Mobile_t _mobile
        ),
        "Approve Mobile directly"
    )
#endif

};

TARGOMAN_MIGRATIONS_POSTPARENT(Account, AAASchema);
TARGOMAN_ACTIONLOG_POSTPARENT(Account, AAASchema);
TARGOMAN_OBJECTSTORAGE_POSTPARENT(Account, AAASchema);
TARGOMAN_FAQ_POSTPARENT(Account, AAASchema);

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(TAPI, enuOAuthType);
//TAPI_DECLARE_METATYPE(Targoman::API::AccountModule::stuMultiJWT);

#endif // TARGOMAN_API_MODULES_ACCOUNT_AAA_H
