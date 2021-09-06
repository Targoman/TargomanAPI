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

#include "Advert.h"
#include "QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "Interfaces/ORM/APIQueryBuilders.h"
#include "ORM/Accounting.h"
#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"

using namespace Targoman::API::AAA::Accounting;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuBannerSize);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAccountOrdersStatus);

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    Targoman::API::AdvertModule,
    stuAdvert,
    [](const Targoman::API::AdvertModule::stuAdvert& _value) -> QVariant{ return _value.toJson(); }
//    [](const Targoman::API::AdvertModule::stuAdvert& _value) -> QVariant{ return _value.toVariant(); }
);

namespace Targoman::API {

using namespace AAA;
using namespace AdvertModule;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert, AdvertSchema);

Advert::Advert() :
    intfRESTAPIWithAccounting(
        AdvertSchema,
        AdvertDomain,
        {
            //           day                week   month                total
            { "show",  { "slbShowPerDay",   {},    {},                  "slbShowTotal" } },
            { "click", { "slbClicksPerDay", {},    "slbClicksPerMonth", "slbClicksTotal" } },
        },
        &AccountProducts::instance(),
        &AccountSaleables::instance(),
        &AccountUserAssets::instance(),
        &AccountAssetUsage::instance(),
        &AccountCoupons::instance()
    )
{
    this->addSubModule(AccountProducts.data());
    this->addSubModule(AccountSaleables.data());
    this->addSubModule(AccountUserAssets.data());
    this->addSubModule(AccountAssetUsages.data());
    this->addSubModule(AccountCoupons.data());
    //this->addSubModule(AccountPrizes); // There is no prize in advertisement module

    this->addSubModule(&ActiveAds::instance());
    this->addSubModule(&Bin::instance());
    this->addSubModule(&Locations::instance());
    this->addSubModule(&Banners::instance());
    this->addSubModule(&Clicks::instance());
    this->addSubModule(&Props::instance());
}

stuServiceCreditsInfo Advert::retrieveServiceCreditsInfo(quint64 _usrID)
{
}

void Advert::breakCredit(quint64 _slbID)
{
}

bool Advert::isUnlimited(const UsageLimits_t& _limits) const
{
}

bool Advert::isEmpty(const UsageLimits_t& _limits) const
{
}

void Advert::applyAssetAdditives(TAPI::JWT_t _JWT,
                                 INOUT stuAssetItem& _assetItem,
                                 const OrderAdditives_t& _orderAdditives)
{
//    qDebug() << "----------" << "_orderAdditives:" << _orderAdditives;

//    _assetItem.slbBasePrice *= 1.1;
};

/***************************************************************************************************/
//bool Advert::apiPOSTprocessVoucher(
//        TAPI::JWT_t _JWT,
//        Targoman::API::AAA::Accounting::stuVoucherItem _voucherItem
//    )
//{
//    clsJWT JWT(_JWT);
//    quint64 currentUserID = JWT.usrID();

//    this->increaseDiscountUsage(_voucherItem);
//    this->activateUserAsset(currentUserID, _voucherItem);

//    return true;
//}

//bool Advert::apiPOSTcancelVoucher(
//        TAPI::JWT_t _JWT,
//        Targoman::API::AAA::Accounting::stuVoucherItem _voucherItem
//    )
//{
//    clsJWT JWT(_JWT);
//    quint64 currentUserID = JWT.usrID();

//    this->decreaseDiscountUsage(_voucherItem);
//    this->removeFromUserAssets(currentUserID, _voucherItem);

//    return true;
//}

Targoman::API::AdvertModule::stuAdvert Advert::apiGETnewBanner(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _location,
        Targoman::API::AdvertModule::enuAdvertOrder::Type _order
    )
{}

Targoman::API::AdvertModule::stuAdvert Advert::apiGETnewText(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _location,
        Targoman::API::AdvertModule::enuAdvertOrder::Type _order,
        const QString _keywords
    )
{}

QString Advert::apiGETretrieveURL(
        TAPI::RemoteIP_t _REMOTE_IP,
        quint64 _id,
        TAPI::IPv4_t _clientIP,
        QString _agent
    )
{}

} //namespace Targoman::API
