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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */


#ifndef TARGOMAN_API_AAA_AUTHENTICATION_H
#define TARGOMAN_API_AAA_AUTHENTICATION_H

#include "AAADefs.hpp"

namespace Targoman {
namespace API {

TARGOMAN_DEFINE_ENUM (enuOAuthType,
                       Google   = 'G',
                       Yahoo    = 'Y',
                       Github   = 'H',
                       Linkedin = 'L'
                       );
namespace AAA {
namespace Authentication{

struct stuOAuthInfo{
    QString Type;
    QString Name;
    QString Family;
    QString Email;
    QString Photo;
    QString ID;
};

extern QJsonObject login(const QString& _ip,
                         const QString& _login,
                         const QString& _pass,
                         const QString& _salt,
                         bool _rememberMe,
                         const QStringList& _requiredTLPs,
                         const QJsonObject& _info,
                         const QString& _fingerPrint);
extern QJsonObject updatePrivs(const QString& _ip, const QString& _ssid, const QString& _requiredTLPs);

extern stuOAuthInfo retrieveGoogleUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveLinkedinUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveYahooUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveGitHubUserInfo(const QString& _authToken);

}
}
}
}
#endif // TARGOMAN_API_AAA_AUTHENTICATION_H
