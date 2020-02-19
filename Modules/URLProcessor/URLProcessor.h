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

#ifndef TARGOMAN_API_MODULES_URLProcessor_URLProcessor_H
#define TARGOMAN_API_MODULES_URLProcessor_URLProcessor_H

#include "Interfaces/Common/intfAPIModule.h"

namespace Targoman {
namespace API {

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

class URLProcessor : public intfAPIModule
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_DEFINE_API_MODULE(URLProcessor);

public:
    virtual QJsonObject todayPrivs(quint32 _usrID) final { Q_UNUSED(_usrID) return {};}

private slots:
    QString API(GET, canonicalize, (const QString _url, bool _removeWWW = true),
                "normalizes url in a common format to be canonical")

    QString API(GET, convertHexCodes, (const QString _url, bool _convertAll = false),
                "helper method which converts URL encoded strings to normal strings")

    QVariantMap API(UPDATE, test, (TAPI::JWT_t _JWT,
                            NULLABLE(TAPI::enuGenericStatus::Type) _sex = {},
                            NULLABLE(QString) _name = {},
                            NULLABLE(QString) _family = {},
                            NULLABLE(TAPI::ISO639_2_t) _lang = {},
                            NULLABLE(TAPI::Email_t) _email = {},
                            NULLABLE(TAPI::Mobile_t) _mobile = {},
                            NULLABLE(TAPI::MD5_t) _pass = {},
                            NULLABLE(QString) _salt = {}), "")
};

}
}

#endif // TARGOMAN_API_MODULES_URLProcessor_URLProcessor_H
