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

#include "Translation.h"
#include "Accounting.h"

QHttp::JWT_t Accounting::apiLogin(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _login, const QString& _pass, const QString& _salt, const QJsonObject& _sessionInfo)
{

}

QHttp::JWT_t Accounting::apiLoginByOAuth(const QHttp::RemoteIP_t& _REMOTE_IP, enuOAuthType::Type _type, const QString& _AuthToken, const QJsonObject& _sessionInfo)
{

}

QHttp::JWT_t Accounting::apiLoginAsGuest(const QHttp::RemoteIP_t& _REMOTE_IP, const QJsonObject& _sessionInfo)
{

}

bool Accounting::apiLogout(QHttp::JWT_t)
{

}

bool Accounting::apiChangePass(QHttp::JWT_t, const QString& _oldPass, const QString& _oldPassSalt, const QString& _newPass)
{

}

bool Accounting::apiChangePassByUUID(const QString& _uuid, const QString& _newPass)
{

}

QHttp::stuTable Accounting::apiGETLastSessions(QHttp::JWT_t, quint16 _fromID, quint16 _maxItems)
{

}

QHttp::stuTable Accounting::apiGETLastSessions(QHttp::JWT_t)
{

}

Accounting::Accounting()
{

}
