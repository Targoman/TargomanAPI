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

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Payments.h"
#include "Classes/Defs.hpp"
#include "Classes/PaymentLogic.h"

namespace TAPI {
struct stuMultiJWT {
    TAPI::EncodedJWT_t Login;
    TAPI::EncodedJWT_t Session;
};
}

namespace Targoman::API {

namespace AAA {
class Voucher;
}

class Account : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Account);
    TARGOMAN_DEFINE_API_MODULE(Account);

    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> InvalidPasswordsFile;

public:
    stuDBInfo requiredDB() const {return stuDBInfo(AAASchema);}
    virtual QJsonObject todayPrivs(quint64 _usrID) final {Q_UNUSED(_usrID) return {}; }

public:
    static TAPI::stuVoucher processVoucher(quint64 _voucherID);

private:
    TAPI::EncodedJWT_t createJWT(const QString _login, const stuActiveAccount& _activeAccount, const QString& _services = {});
    TAPI::EncodedJWT_t createLoginJWT(bool _remember, const QString& _login, const QString &_ssid, const QString& _services);

private slots:
    /*****************************************************************\
    |* User **********************************************************|
    \*****************************************************************/
    TAPI::stuMultiJWT REST(,Login,(TAPI::RemoteIP_t _REMOTE_IP,
                                    QString _login,
                                    TAPI::MD5_t _pass,
                                    QString _salt,
                                    TAPI::CommaSeparatedStringList_t _services = {},
                                    bool _rememberMe = false,
                                    TAPI::JSON_t _sessionInfo = {},
                                    TAPI::MD5_t _fingerprint = {}),
                            "Login user and return an encoded JWT if services are provided (as coma separated list) then user must have access to specified services")

    TAPI::stuMultiJWT REST(,LoginByOAuth,(TAPI::RemoteIP_t _REMOTE_IP,
                                           TAPI::enuOAuthType::Type _type,
                                           QString _oAuthToken,
                                           TAPI::CommaSeparatedStringList_t _services,
                                           TAPI::JSON_t _sessionInfo = TAPI::JSON_t(),
                                           TAPI::MD5_t _fingerprint = {}),
                            "Login by Open Authentication and return an encoded JWT")

    TAPI::stuMultiJWT REST(,RefreshJWT,(TAPI::RemoteIP_t _REMOTE_IP, TAPI::JWT_t _loginJWT, QString _services = {}),
                            "Refresh JWT in order to update information or expiry time. Provide services in order to create service specific JWT")

    QVariantMap REST(PUT,Signup,( TAPI::RemoteIP_t _REMOTE_IP,
                                  QString _emailOrMobile,
                                  TAPI::MD5_t _pass,
                                  QString _role = "BaseUser",
                                  QString _name = "",
                                  QString _family = "",
                                  TAPI::JSON_t _specialPrivs = {},
                                  qint8 _maxSessions = -1),
                     "Base method for signup with email address. this method can be called just by predefined IPs")

    bool REST(,Logout,(TAPI::JWT_t _JWT),
              "Logout logged in user")

    QString REST(,CreateForgotPasswordLink,(TAPI::RemoteIP_t _REMOTE_IP,
                                            QString _login),
                 "Create a forgot password request returning a UUID for the requiest")

    bool REST(,ChangePass,(TAPI::JWT_t _JWT,
                           TAPI::MD5_t _oldPass,
                           QString     _oldPassSalt,
                           TAPI::MD5_t _newPass
                           ),
              "Changes password of the logged-in user")

    bool REST(,ChangePassByUUID,(TAPI::RemoteIP_t _REMOTE_IP,
                                 TAPI::MD5_t _uuid,
                                 TAPI::MD5_t _newPass
                                 ),
              "Changes password based on a UUID provided by ")
    bool REST(POST,ApproveEmail,(TAPI::RemoteIP_t _REMOTE_IP,
                                 TAPI::MD5_t _uuid),
              "Approves Email by provided UUID")

    bool REST(POST,ApproveMobile,(TAPI::RemoteIP_t _REMOTE_IP,
                                  TAPI::Mobile_t _mobile,
                                  quint32 _code),
              "Approves Mobile by provided mobile no and code")

    /*****************************************************************\
    |* Voucher & Payments ********************************************|
    \*****************************************************************/
    TAPI::stuVoucher REST(
        POST,
        finalizeBasket,
        (
            TAPI::JWT_t _JWT,
            TAPI::stuPreVoucher _preVoucher,
            TAPI::enuPaymentGatewayType::Type _gatewayType,
            QString _domain,
            qint64 _walletID = -1,
            QString _paymentVerifyCallback = {}
        ),
        "create a voucher based on preVoucher. "
        "Set gatewayType = COD for offline payment, url for online payment or keep empty for wallet payment,"
        "Also set walletID >0 to use specified wallet, 0 for using default wallet, <0 to discard wallet usage."
        "When callback is set to URL you must specify payment gateway"
    )

    TAPI::stuVoucher REST(
        POST,
        approveOnlinePayment,
        (
//            TAPI::enuPaymentGatewayType::Type _gatewayType,
            const QString _paymentMD5,
            const QString _domain,
            TAPI::JSON_t _pgResponse
        ),
        "approve payment back from payment gateway"
    )

    TAPI::stuVoucher REST(
        POST,
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

    bool REST(
        POST,
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

    bool REST(
        POST,
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
};

} //namespace Targoman::API

TAPI_DECLARE_METATYPE_ENUM(TAPI::enuOAuthType);
TAPI_DECLARE_METATYPE(TAPI::stuMultiJWT);

#endif // TARGOMAN_API_MODULES_ACCOUNT_AAA_H
