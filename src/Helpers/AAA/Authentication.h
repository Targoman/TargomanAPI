/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_HELPERS_AAA_AUTH_H
#define TARGOMAN_API_HELPERS_AAA_AUTH_H

#include "AAADefs.hpp"

namespace Targoman {
namespace API {

TARGOMAN_DEFINE_ENHANCED_ENUM (enuOAuthType,
                               Google,
                               Yahoo,
                               Github,
                               Linkedin
                               );
namespace Helpers {
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

extern QJsonObject login(const QString _ip, const QString& _login, const QString& _pass, const QString& _salt, bool _rememberMe, const QStringList& _requiredTLPs, const QJsonObject& _info);
extern QJsonObject updatePrivs(const QString& _ssid, const QString& _requiredTLPs);

extern stuOAuthInfo retrieveGoogleUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveLinkedinUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveYahooUserInfo(const QString& _authToken);
extern stuOAuthInfo retrieveGitHubUserInfo(const QString& _authToken);

};

}
}
}
}
#endif // AUTH_H
