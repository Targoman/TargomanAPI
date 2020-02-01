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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_AAA_H
#define TARGOMAN_API_MODULES_ACCOUNT_AAA_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {

class Account : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Account)

public:
    void init();

private:
    TAPI::EncodedJWT_t createJWT(const QString _login, const QJsonObject& _result, const QString& _requiredTLPs);

private slots:
    TAPI::EncodedJWT_t REST(,Login,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                   const QString& _login,
                                   const TAPI::MD5_t& _pass,
                                   const QString& _salt,
                                   const QString& _tlps = "",
                                   bool _rememberMe = false,
                                   const TAPI::JSON_t& _sessionInfo = {},
                                   const TAPI::MD5_t& _fingerprint = {}),
                           "Login user and return an encoded JWT you can provide toplevel privs as comma separated string")

    TAPI::EncodedJWT_t REST(,LoginByOAuth,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                          TAPI::enuOAuthType::Type _type,
                                          const QString& _oAuthToken,
                                          const QString& _tlps,
                                          const TAPI::JSON_t& _sessionInfo = TAPI::JSON_t(),
                                          const TAPI::MD5_t& _fingerprint = {}),
                           "Login by Open Authentication and return an encoded JWT")

    TAPI::EncodedJWT_t REST(,RefreshJWT,(const TAPI::RemoteIP_t& _REMOTE_IP, TAPI::JWT_t _JWT),
                           "Refresh JWT in order to update information or expiry time")

    quint32 REST(PUT,Signup,(const TAPI::RemoteIP_t& _REMOTE_IP,
                            const QString& _emailOrMobile,
                            const TAPI::MD5_t& _pass,
                            const QString& _role,
                            const QString& _name = "",
                            const QString& _family = "",
                            TAPI::JSON_t _specialPrivs = {},
                            qint8 _maxSessions = -1),
                "Base method for signup with email address. this method can be called just by predefined IPs")

    bool REST(,Logout,(TAPI::JWT_t _JWT),
             "Logout logged in user")

    bool REST(,CreateForgotPasswordLink,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                        const QString& _login,
                                        TAPI::enuForgotPassLinkVia::Type _via),
             "Create a forgot password request returning a UUID for the requiest")

    bool REST(,ChangePass,(TAPI::JWT_t _JWT,
                          const TAPI::MD5_t& _oldPass,
                          const QString& _oldPassSalt,
                          const TAPI::MD5_t& _newPass
                          ),
             "Changes password of the logged-in user")

    bool REST(,ChangePassByUUID,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                const TAPI::MD5_t& _uuid,
                                const TAPI::MD5_t& _newPass
                                ),
             "Changes password based on a UUID provided by ")
    bool REST(POST,ApproveEmail,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                const TAPI::MD5_t& _uuid),
             "Approves Email by provided UUID")

    bool REST(POST,ApproveMobile,(const TAPI::RemoteIP_t& _REMOTE_IP,
                                 const TAPI::Mobile_t _mobile,
                                 const quint16& _code),
             "Approves Mobile by provided mobile no and code")
    private:
    TARGOMAN_DEFINE_API_MODULE(Account)
};

}
}

Q_DECLARE_METATYPE(TAPI::enuOAuthType::Type);
Q_DECLARE_METATYPE(TAPI::enuForgotPassLinkVia::Type);

#endif // TARGOMAN_API_MODULES_ACCOUNT_AAA_H
