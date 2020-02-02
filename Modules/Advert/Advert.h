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

#ifndef TARGOMAN_API_MODULES_ADVERT_ADVERT_H
#define TARGOMAN_API_MODULES_ADVERT_ADVERT_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/ORM/clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "ORM/Defs.hpp"

namespace Targoman {
namespace API {

struct stuAdvert{
    quint64 ID;
    QString Title;
    QString Description;
    QString PrettyURL;
};

class Advert : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID INTFAPIMODULE_IID)
    Q_INTERFACES(Targoman::API::intfAPIModule)
    TARGOMAN_API_MODULE_DB_CONFIGS(Advert)
private slots:
        stuAdvert REST(GET,NewBanner,(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _location, TAPI::enuAdvertOrder::Type _order),
                      "Get new banner based on location and order info")
        stuAdvert REST(GET,NewText,(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _location, TAPI::enuAdvertOrder::Type _order, const QString _keywords),
                      "Get new text advertisement")
        QString   REST(GET,RetrieveURL, (const TAPI::RemoteIP_t& _REMOTE_IP, quint64 _id, TAPI::IPv4_t _clientIP, QString _agent),
                      "Retrieve URL of the specified Advertisement")

private:
    TARGOMAN_DEFINE_API_MODULE(Advert)
};

}
}

Q_DECLARE_METATYPE(Targoman::API::stuAdvert);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ADVERT_H
