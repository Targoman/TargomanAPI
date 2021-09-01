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

#ifndef TARGOMAN_API_RESTCLIENTHELPER_H
#define TARGOMAN_API_RESTCLIENTHELPER_H

#include <QString>
#include <QVariant>
#include "QtCUrl.h"
#include "Interfaces/Common/GenericTypes.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman::API::Helpers {

struct ClientConfigs
{
    static inline QString makeConfig(const QString& _name) { return "/Client/" + _name; }
    static Common::Configuration::tmplConfigurable<QString> RESTServerAddress;
};

class RESTClientHelper
{
public:
    enum enuHTTPMethod {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
    };

    static QVariant callAPI(
        TAPI::JWT_t _JWT,
        RESTClientHelper::enuHTTPMethod _method,
        const QString& _api,
        const QVariantMap& _urlArgs = {},
        const QVariantMap& _postFields = {},
        QString _aPIURL = {}
    );

    static QVariant callAPI(
        QString _encodedJWT,
        RESTClientHelper::enuHTTPMethod _method,
        const QString& _api,
        const QVariantMap& _urlArgs = {},
        const QVariantMap& _postFields = {},
        QString _aPIURL = {}
    );

};

} //namespace Targoman::API::Helpers

#endif // TARGOMAN_API_RESTCLIENTHELPER_H
