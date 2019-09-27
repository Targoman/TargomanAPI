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

#ifndef TARGOMAN_APPS_MODULES_ACCOUNTING_H
#define TARGOMAN_APPS_MODULES_ACCOUNTING_H

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"

TARGOMAN_DEFINE_ENHANCED_ENUM (enuOAuthType,
                               Google,
                               Yahoo,
                               Github,
                               Linkedin
                               );

class Accounting : public QHttp::intfRESTAPIHolder
{
    Q_OBJECT
public:
    void init();

private slots:
    QHttp::JWT_t apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP,
                          const QString& _login,
                          const QString& _pass,
                          const QString& _salt,
                          const QJsonObject& _sessionInfo = QJsonObject()
            );

    QHttp::JWT_t apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP,
                                 enuOAuthType::Type _type,
                                 const QString& oAuthToken,
                                 const QJsonObject& _sessionInfo = QJsonObject());

    QHttp::JWT_t apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP,
                                 const QJsonObject& _sessionInfo
                                 );

    bool apiLogout(QHttp::JWT_t);
    bool apiCreateForgotPasswordLink(const QHttp::RemoteIP_t& _REMOTE_IP,
                                     const QString& _login);
    bool apiChangePass(QHttp::JWT_t,
                       const QString& _oldPass,
                       const QString& _oldPassSalt,
                       const QString& _newPass
                       );

    bool apiChangePassByUUID(const QString& _uuid,
                             const QString& _newPass
                             );

    QHttp::stuTable apiGETLastSessions(QHttp::JWT_t, quint16 _fromID = 0, quint16 _maxItems = 100);


    QVariantMap apiTest(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _token, const QString& _arg);

private:
    Accounting();
    TARGOMAN_DEFINE_SINGLETON_MODULE(Accounting);

private:
    QScopedPointer<Targoman::DBManager::clsDAC> DAC;
};

#endif // ACCOUNTING_H
