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

#include "Advert.h"
#include "QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"

#include "ORM/Accounting.h"
#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuBannerSizes);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAccountOrdersStatus);

TAPI_REGISTER_METATYPE(
        COMPLEXITY_Complex,
        TAPI, stuAdvert,
        [](const TAPI::stuAdvert& _value) -> QVariant{return _value.toVariant();}
);

namespace Targoman {
namespace API {

using namespace Targoman;
using namespace TAPI;
using namespace AAA;
using namespace Advertisement;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert);

stuAdvert Advert::apiGETNewBanner(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order)
{
    /*clsDACResult Result = AdvertDACInstance().execQuery(
                                "",
                                "SELECT "
                                )*/
}

stuAdvert Advert::apiGETNewText(TAPI::RemoteIP_t _REMOTE_IP, QString _location, TAPI::enuAdvertOrder::Type _order, const QString _keywords)
{

}

QString Advert::apiGETRetrieveURL(TAPI::RemoteIP_t _REMOTE_IP, quint64 _id, TAPI::IPv4_t _clientIP, QString _agent)
{

}

Advert::Advert() :
    Accounting::clsRESTAPIWithAccounting(AdvertSchema,
                                         AdvertDomain,
                                         {
                                            {"show", {"pkgShowPerDay", {}, {}, "pkgShowTotal"}},
                                            {"click", {"pkgShowPerDay", {}, {}, "pkgShowTotal"}},
                                         },
                                         &Advertisement::AccountPackages::instance(),
                                         &Advertisement::clsAccountUserPackages::instance(),
                                         &Advertisement::AccountUsage::instance(),
                                         &Advertisement::AccountDiscounts::instance()
                                         )
{
    this->addSubModule(AccountPackages.data());
    this->addSubModule(AccountUserPackages.data());
    this->addSubModule(AccountUsage.data());
    this->addSubModule(AccountDiscounts.data());
    //this->addSubModule(AccountPrizes); // There is no prize in advertisement module

    this->addSubModule(&Advertisement::ActiveAds::instance());
    this->addSubModule(&Advertisement::Bin::instance());
    this->addSubModule(&Advertisement::Locations::instance());
    this->addSubModule(&Advertisement::Banners::instance());
    this->addSubModule(&Advertisement::Clicks::instance());
    this->addSubModule(&Advertisement::Props::instance());
}

/*******************************************************************************************************************/
Accounting::stuServiceAccountInfo Advert::retrieveServiceAccountInfo(quint32 _usrID)
{

}

void Advert::breakPackage(quint64 _pkgID)
{

}

bool Advert::isUnlimited(const Accounting::PackageRemaining_t& _limits) const
{

}

bool Advert::isEmpty(const Accounting::PackageRemaining_t& _limits) const
{

}

}
}


