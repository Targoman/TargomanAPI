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

#ifndef TARGOMAN_API_MODULES_AAA_AAA_H
#define TARGOMAN_API_MODULES_AAA_AAA_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"

TARGOMAN_DEFINE_ENHANCED_ENUM (enuOAuthType,
                               Google,
                               Yahoo,
                               Github,
                               Linkedin
                               );

TARGOMAN_DEFINE_ENHANCED_ENUM (enuForgotPassLinkVia,
                               Web,
                               Mobile,
                               App
                               );
#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

class Account : public QHttp::intfRESTAPIHolder
{
    Q_OBJECT
public:
    void init();

private slots:
    QHttp::EncodedJWT_t API(,Login,(const QHttp::RemoteIP_t& _REMOTE_IP,
                                    const QString& _login,
                                    const QHttp::MD5_t& _pass,
                                    const QString& _salt,
                                    bool _rememberMe = false,
                                    const QHttp::JSON_t& _sessionInfo = QHttp::JSON_t()),
                            "Login user and return an encoded JWT")

    QHttp::EncodedJWT_t API(,LoginByOAuth,(const QHttp::RemoteIP_t& _REMOTE_IP,
                                           enuOAuthType::Type _type,
                                           const QString& oAuthToken,
                                           const QHttp::JSON_t& _sessionInfo = QHttp::JSON_t()),
                            "Login by Open Authentication and return an encoded JWT")

    QHttp::EncodedJWT_t API(,LoginAsGuest,(const QHttp::RemoteIP_t& _REMOTE_IP,const QHttp::JSON_t& _sessionInfo),
                            "Login user as guest and return an encoded JWT")

    bool API(,Logout,(QHttp::JWT_t _JWT),
             "Logout logged in user")

    QHttp::MD5_t API(,CreateForgotPasswordLink,(const QHttp::RemoteIP_t& _REMOTE_IP,
                                                const QString& _login,
                                                enuForgotPassLinkVia::Type _via),
                     "Create a forgot password request returning a UUID for the requiest")

    bool API(,ChangePass,(QHttp::JWT_t _JWT,
                          const QHttp::MD5_t& _oldPass,
                          const QString& _oldPassSalt,
                          const QHttp::MD5_t& _newPass
                          ),
             "Changes password of the logged-in user")

    bool API(,ChangePassByUUID,(const QHttp::RemoteIP_t& _REMOTE_IP,
                                const QHttp::MD5_t& _uuid,
                                const QHttp::MD5_t& _newPass
                                ),
             "Changes password based on a UUID provided by ")

    private:
        Account();
    TARGOMAN_DEFINE_SINGLETON_MODULE(Account);
};

Q_DECLARE_METATYPE(enuOAuthType::Type);
Q_DECLARE_METATYPE(enuForgotPassLinkVia::Type);

#endif // TARGOMAN_API_MODULES_AAA_AAA_H
