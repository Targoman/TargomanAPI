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
#include "QHttp/QRESTServer.h"
#include "QHttp/intfAPIArgManipulator.h"
#include "Helpers/AAA/AAA.hpp"
#include "Helpers/AAA/PrivHelpers.h"
#include "Helpers/AAA/GenericEnums.hpp"
#include "Configs.h"

#include "ORM/Defs.hpp"
//#include "ORM/ActionLogs.h"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"

using namespace Targoman;
using namespace Targoman::API;
using namespace Targoman::API::Advertisement;
using namespace Targoman::API::Helpers::AAA;
using namespace QHttp;

void Advert::init()
{}

stuAdvert Advert::apiGETNewBanner(const RemoteIP_t& _REMOTE_IP, const QString& _location, enuAdvertOrder::Type _order)
{
      /*clsDACResult Result = AdvertDACInstance().execQuery(
                                "",
                                "SELECT "
                                )*/
}

stuAdvert Advert::apiGETNewText(const RemoteIP_t& _REMOTE_IP, const QString& _location, enuAdvertOrder::Type _order, const QString _keywords)
{

}

QString Advert::apiGETRetrieveURL(const RemoteIP_t& _REMOTE_IP, quint64 _id, IPv4_t _clientIP, QString _agent)
{

}

Advert::Advert() :
    Helpers::ORM::clsRESTAPIWithActionLogs (AdvertDACInstance(), "MT", "Targoman"){
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAdvertType);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuAdvertOrder);
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuBannerSizes);

//    Advertisement::ActionLogs::instance().init();
    Advertisement::ActiveAds::instance().init();
    Advertisement::Bin::instance().init();
    Advertisement::Clicks::instance().init();
    Advertisement::Props::instance().init();

    this->registerMyRESTAPIs();
}
