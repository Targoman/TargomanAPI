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

#include "QHttp/intfRESTAPIHolder.h"
#include "libTargomanDBM/clsDAC.h"
#include "./ORM/Defs.hpp"
#include "Helpers/ORM/clsRESTAPIWithActionLogs.h"

namespace Targoman {
namespace API {

#ifndef API
#define API(_method, _name, _sig, _doc) api##_method##_name _sig; QString signOf##_method##_name(){ return #_sig; } QString docOf##_method##_name(){ return _doc; }
#endif

struct stuAdvert{
    quint64 ID;
    QString Title;
    QString Description;
    QString PrettyURL;
};

class Advert : private Helpers::ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT
public:
    void init();
    stuAdvert API(GET,NewBanner,(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _location, enuAdvertOrder::Type _order),
                  "Get new banner based on location and order info")
    stuAdvert API(GET,NewText,(const QHttp::RemoteIP_t& _REMOTE_IP, const QString& _location, enuAdvertOrder::Type _order, const QString _keywords),
                  "Get new text advertisement")
    QString   API(GET,RetrieveURL, (const QHttp::RemoteIP_t& _REMOTE_IP, quint64 _id, QHttp::IPv4_t _clientIP, QString _agent),
                  "Retrieve URL of the specified Advertisement")
private slots:

private:
    Advert();
    TARGOMAN_DEFINE_SINGLETON_MODULE(Advert);
};

}
}

Q_DECLARE_METATYPE(Targoman::API::stuAdvert);

#endif // TARGOMAN_API_MODULES_ACCOUNT_ADVERT_H
