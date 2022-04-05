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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_AAA_AUTHENTICATION_H
#define TARGOMAN_API_AAA_AUTHENTICATION_H

#include "Interfaces/Common/GenericTypes.h"
#include "Interfaces/AAA/AAADefs.hpp"
#include "Interfaces/AAA/PrivHelpers.h"

namespace TAPI {
TARGOMAN_DEFINE_ENUM(enuOAuthType,
                     Google   = 'G',
                     Yahoo    = 'Y',
                     Github   = 'H',
                     Linkedin = 'L'
                     );
}

namespace Targoman::API::AAA::Authentication {

struct stuOAuthInfo {
    QString Type;
    QString Name;
    QString Family;
    QString Email;
    QString Photo;
    QString ID;
};

extern Targoman::API::AAA::stuActiveAccount login(
    const QString &_ip,
    const QString &_login,
    const QString &_pass,
    const QString &_salt,
    const QStringList &_requiredServices,
    bool _rememberMe,
    const QJsonObject &_info,
    const QString &_fingerPrint
);

//extern Targoman::API::AAA::stuActiveAccount updatePrivs(const QString &_ip, const QString &_ssid, const QString &_requiredServices);
extern QString renewJWT(
    INOUT TAPI::JWT_t &_JWTPayload,
//    const QString &_jwt,
    const QString &_ip
);

extern stuOAuthInfo retrieveGoogleUserInfo(const QString &_authToken);
extern stuOAuthInfo retrieveLinkedinUserInfo(const QString &_authToken);
extern stuOAuthInfo retrieveYahooUserInfo(const QString &_authToken);
extern stuOAuthInfo retrieveGitHubUserInfo(const QString &_authToken);

} //namespace Targoman::API::AAA::Authentication

#endif // TARGOMAN_API_AAA_AUTHENTICATION_H
