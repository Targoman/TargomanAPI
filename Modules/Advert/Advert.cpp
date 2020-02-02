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

#include "Advert.h"
#include "QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/AAA/GenericEnums.hpp"

#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"

using namespace Targoman;
using namespace Targoman::API;
using namespace Targoman::API::Advertisement;
using namespace AAA;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert);

stuAdvert Advert::apiGETNewBanner(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _location, TAPI::enuAdvertOrder::Type _order)
{
      /*clsDACResult Result = AdvertDACInstance().execQuery(
                                "",
                                "SELECT "
                                )*/
}

stuAdvert Advert::apiGETNewText(const TAPI::RemoteIP_t& _REMOTE_IP, const QString& _location, TAPI::enuAdvertOrder::Type _order, const QString _keywords)
{

}

QString Advert::apiGETRetrieveURL(const TAPI::RemoteIP_t& _REMOTE_IP, quint64 _id, TAPI::IPv4_t _clientIP, QString _agent)
{

}

Advert::Advert() :
    ORM::clsRESTAPIWithActionLogs("Advert", "Advert"){
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuAdvertType);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuAdvertOrder);
    TAPI_REGISTER_TARGOMAN_ENUM(TAPI::enuBannerSizes);

    this->addSubModule(new Advertisement::ActiveAds);
    this->addSubModule(new Advertisement::Bin);
    this->addSubModule(new Advertisement::Banners);
    this->addSubModule(new Advertisement::Clicks);
    this->addSubModule(new Advertisement::Props);
}
