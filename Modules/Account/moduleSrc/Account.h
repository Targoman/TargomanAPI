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

struct stuMultiJWT {
    TAPI::EncodedJWT_t Login;
    TAPI::EncodedJWT_t Session;
};

//namespace AAA {
//class Voucher;
//}

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
    static Targoman::API::AAA::stuVoucher processVoucher(TAPI::JWT_t _JWT, quint64 _voucherID);
    static void tryCancelVoucher(TAPI::JWT_t _JWT, quint64 _voucherID, bool _setAsError = false);

private:
    TAPI::EncodedJWT_t createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services);
    TAPI::EncodedJWT_t createJWT(const QString _login, const stuActiveAccount& _activeAccount, const QString& _services = {});

private slots:
    /*****************************************************************\
    |* User **********************************************************|
    \*****************************************************************/
    QString REST_GET_OR_POST(
        normalizePhoneNumber,
        (
            QString _phone,
            QString _country
        ),
        "Normalize and format phone number as E164 standard"
    )

    QVariantMap REST_PUT(
        signup,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            QString _emailOrMobile,
            TAPI::MD5_t _pass,
            QString _role = "BaseUser",
            QString _name = "",
            QString _family = "",
            TAPI::JSON_t _specialPrivs = {},
            qint8 _maxSessions = -1
        ),
        "Base method for signup with email or mobile. this method can be called just by predefined IPs"
    )

    /*
    QVariantMap REST_PUT(
        signupByMobileOnly,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            TAPI::Mobile_t _mobile,
            QString _role = "BaseUser",
            QString _name = "",
            QString _family = "",
            TAPI::JSON_t _specialPrivs = {},
            qint8 _maxSessions = -1
        ),
        "Base method for signup with mobile only. this method can be called just by predefined IPs"
    )
    */
//    "If verifyCode is empty, a new random code is generated and sent to the user via SMS."
//    "After the user submits this code, signupByMobile must be called again with verifyCode."

    Targoman::API::AccountModule::stuMultiJWT REST_POST(
        approveEmail,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
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

    Targoman::API::AccountModule::stuMultiJWT REST_POST(
        approveMobile,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
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

    Targoman::API::AccountModule::stuMultiJWT REST_GET_OR_POST(
        login,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
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
            TAPI::RemoteIP_t _REMOTE_IP,
            TAPI::Mobile_t _mobile,
            bool _signupIfNotExists = false,
            QString _signupRole = "BaseUser"
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
            TAPI::RemoteIP_t _REMOTE_IP,
            QString _emailOrMobile
        ),
        "Recreate (if expired) approval code and resend last valid code to the email or mobile."
    )

//    bool REST_PUT(
//        requestMobileVerifyCode,
//        (
//            TAPI::RemoteIP_t _REMOTE_IP,
//            TAPI::Mobile_t _mobile
//        ),
//        "Send verification code for provided mobile."
//    )

//    Targoman::API::AccountModule::stuMultiJWT REST_PUT(
//        verifyLoginByMobileCode,
//        (
//            TAPI::RemoteIP_t _REMOTE_IP,
//            TAPI::Mobile_t _mobile,
//            quint32 _code,
//            TAPI::CommaSeparatedStringList_t _services = {},
//            bool _rememberMe = false,
//            TAPI::JSON_t _sessionInfo = {},
//            TAPI::MD5_t _fingerprint = {}
//        ),
//        "check verification code for provided mobile."
//    )

    Targoman::API::AccountModule::stuMultiJWT REST_GET_OR_POST(
        loginByOAuth,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            TAPI::enuOAuthType::Type _type,
            QString _oAuthToken,
            TAPI::CommaSeparatedStringList_t _services,
            TAPI::JSON_t _sessionInfo = TAPI::JSON_t(),
            TAPI::MD5_t _fingerprint = {}
        ),
        "Login by Open Authentication and return an encoded JWT"
    )

    Targoman::API::AccountModule::stuMultiJWT REST_GET_OR_POST(
        refreshJWT,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            TAPI::JWT_t _loginJWT,
            QString _services = {}
        ),
        "Refresh JWT in order to update information or expiry time. Provide services in order to create service specific JWT"
    )

    bool REST_GET_OR_POST(
        logout,
        (
            TAPI::JWT_t _JWT
        ),
        "Logout logged in user"
    )

    QString REST_GET_OR_POST(
        createForgotPasswordLink,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            QString _login
        ),
        "Create a forgot password request returning a UUID for the requiest"
    )

    bool REST_GET_OR_POST(
        changePass,
        (
            TAPI::JWT_t _JWT,
            TAPI::MD5_t _oldPass,
            QString     _oldPassSalt,
            TAPI::MD5_t _newPass
        ),
        "Changes password of the logged-in user"
    )

    bool REST_GET_OR_POST(
        changePassByUUID,
        (
            TAPI::RemoteIP_t _REMOTE_IP,
            TAPI::MD5_t _uuid,
            TAPI::MD5_t _newPass
        ),
        "Changes password based on a UUID provided by "
    )

    /*****************************************************************\
    |* Voucher & Payments ********************************************|
    \*****************************************************************/
    Targoman::API::AAA::stuVoucher REST_POST(
        finalizeBasket,
        (
            TAPI::JWT_t _JWT,
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
            TAPI::JWT_t _JWT,
//            Targoman::API::AccountModule::enuPaymentGatewayType::Type _gatewayType,
            const QString _paymentMD5,
            const QString _domain,
            TAPI::JSON_t _pgResponse
        ),
        "approve payment back from payment gateway"
    )

    Targoman::API::AAA::stuVoucher REST_POST(
        approveOfflinePayment,
        (
            TAPI::JWT_t _JWT,
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
            TAPI::JWT_t _JWT,
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
            TAPI::JWT_t _JWT,
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
            TAPI::JWT_t _JWT,
            quint64 _voucherID
        ),
        "Check voucher and items for ttl"
    )

#ifdef QT_DEBUG
//protected:
//    virtual QVariant fixtureSetup(TAPI::RemoteIP_t _REMOTE_IP);
//    virtual QVariant fixtureCleanup(TAPI::RemoteIP_t _REMOTE_IP);

protected slots:
    QVariant REST_POST(
        fixtureSetup,
        (
                TAPI::RemoteIP_t _REMOTE_IP
        ),
        "Create sample data"
    )

    QVariant REST_POST(
        fixtureCleanup,
        (
                TAPI::RemoteIP_t _REMOTE_IP
        ),
        "Cleanup sample data"
    )
    bool REST_POST(
        fixtureApproveEmail,
        (
                TAPI::RemoteIP_t _REMOTE_IP,
                QString _email
        ),
        "Approve Email Address directly"
    )
    bool REST_POST(
        fixtureApproveMobile,
        (
                TAPI::RemoteIP_t _REMOTE_IP,
                TAPI::Mobile_t _mobile
        ),
        "Approve Mobile directly"
    )
#endif

};

TARGOMAN_ACTIONLOG_POSTPARENT(Account, AAASchema);

} //namespace Targoman::API::AccountModule

TAPI_DECLARE_METATYPE_ENUM(TAPI, enuOAuthType);
TAPI_DECLARE_METATYPE(Targoman::API::AccountModule::stuMultiJWT);

#endif // TARGOMAN_API_MODULES_ACCOUNT_AAA_H
