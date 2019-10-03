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

class Account : public QHttp::intfRESTAPIHolder
{
    Q_OBJECT
public:
    void init();

private slots:
    QHttp::EncodedJWT_t apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP,
                          const QString& _login,
                          const QHttp::MD5_t& _pass,
                          const QString& _salt,
                          bool _rememberMe = false,
                          const QHttp::JSON_t& _sessionInfo = QHttp::JSON_t()
            );

    QHttp::EncodedJWT_t apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP,
                                 enuOAuthType::Type _type,
                                 const QString& oAuthToken,
                                 const QHttp::JSON_t& _sessionInfo = QHttp::JSON_t());

    QHttp::EncodedJWT_t apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP,
                                 const QHttp::JSON_t& _sessionInfo
                                 );

    bool apiLogout(QHttp::JWT_t _JWT);
    QHttp::MD5_t apiCreateForgotPasswordLink(const QHttp::RemoteIP_t& _REMOTE_IP,
                                             const QString& _login,
                                             enuForgotPassLinkVia::Type _via);

    bool apiChangePass(QHttp::JWT_t _JWT,
                       const QHttp::MD5_t& _oldPass,
                       const QString& _oldPassSalt,
                       const QHttp::MD5_t& _newPass
                       );

    bool apiChangePassByUUID(const QHttp::RemoteIP_t& _REMOTE_IP,
                             const QHttp::MD5_t& _uuid,
                             const QHttp::MD5_t& _newPass
                             );

private:
    Account();
    TARGOMAN_DEFINE_SINGLETON_MODULE(Account);
};

Q_DECLARE_METATYPE(enuOAuthType::Type);
Q_DECLARE_METATYPE(enuForgotPassLinkVia::Type);

#endif // TARGOMAN_API_MODULES_AAA_AAA_H
