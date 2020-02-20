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

#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Accounting.h"
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
    ORM::clsRESTAPIWithActionLogs(AdvertSchema, AdvertDomain),
    Accounting::intfAccounting(this->moduleBaseName()){

    this->addSubModule(new Advertisement::AccountPackages);
    this->addSubModule(new Advertisement::AccountUsage);
    this->addSubModule(new Advertisement::AccountUserPackages);
    this->addSubModule(new Advertisement::AccountDiscounts);
    //this->addSubModule(new Advertisement::AccountPrizes); // There is no prize in advertisement module

    this->addSubModule(new Advertisement::ActiveAds);
    this->addSubModule(new Advertisement::Bin);
    this->addSubModule(new Advertisement::Locations);
    this->addSubModule(new Advertisement::Banners);
    this->addSubModule(new Advertisement::Clicks);
    this->addSubModule(new Advertisement::Props);
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
/*******************************************************************************************************************/
TAPI::stuPreInvoice Advert::apiPOSTpreInvoice(TAPI::PackageCode_t _pkg, TAPI::DiscountCode_t _discountCode, QString _referer, QJsonObject _extraRefererParams){

}

TAPI::stuInvoice Advert::apiPOSTcreateInvoice(TAPI::PackageCode_t _pkg, TAPI::URL_t _callBack, TAPI::DiscountCode_t _discountCode, QString _referer, QJsonObject _extraRefererParams){
}

TAPI::stuInvoice Advert::apiPOSTapproveInvoice(TAPI::MD5_t _invCode, TAPI::JSON_t _bankInfo){
}


}
}
