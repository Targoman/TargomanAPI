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

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

using namespace Targoman::API::AAA::Accounting;

#include "ORM/Accounting.h"
#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuBannerSizes);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuAccountOrdersStatus);

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    TAPI, stuAdvert,
    [](const TAPI::stuAdvert& _value) -> QVariant{ return _value.toJson(); }
//    [](const TAPI::stuAdvert& _value) -> QVariant{ return _value.toVariant(); }
);

namespace Targoman {
namespace API {

using namespace AAA;
using namespace Advertisement;

TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert);

quint32 Advert::apiCREATEProduct(
        TAPI::JWT_t _JWT,
        QString _productCode,
        QString _productName,
        quint32 _locationID
    )
{
    CreateQuery query = CreateQuery(*this->AccountProducts)
//        .options_ignore()
        .addCol(tblAccountProductsBase::prdCode)
        .addCol(tblAccountProductsBase::prdName)
//        .addCol(tblAccountProductsBase::prdDesc)
        .addCol(tblAccountProductsBase::prdValidFromDate)
//        .addCol(tblAccountProductsBase::prdValidToDate)
//        .addCol(tblAccountProductsBase::prdValidFromHour)
//        .addCol(tblAccountProductsBase::prdValidToHour)
//        .addCol(tblAccountProductsBase::prdPrivs)
//        .addCol(tblAccountProductsBase::prdVAT)
        .addCol(tblAccountProductsBase::prdInStockCount)
//        .addCol(tblAccountProductsBase::prdOrderedCount)
//        .addCol(tblAccountProductsBase::prdReturnedCount)
//        .addCol(tblAccountProductsBase::prdStatus)
        .addCol(tblAccountProducts::prd_locID)
//        .addCol(tblAccountProducts::prdShowPerDay)
//        .addCol(tblAccountProducts::prdShowTotal)
//        .addCol(tblAccountProducts::prdClicksPerDay)
//        .addCol(tblAccountProducts::prdClicksPerMonth)
//        .addCol(tblAccountProducts::prdClicksTotal)
        .values(QVariantMap({
            { tblAccountProductsBase::prdCode,          _productCode },
            { tblAccountProductsBase::prdName,          _productName },
//            { tblAccountProductsBase::prdDesc,          DBExpression::NIL() },
            { tblAccountProductsBase::prdValidFromDate, DBExpression::CURDATE() },
//            { tblAccountProductsBase::prdValidToDate,   DBExpression::NIL() },
//            { tblAccountProductsBase::prdValidFromHour, DBExpression::NIL() },
//            { tblAccountProductsBase::prdValidToHour,   DBExpression::NIL() },
//            { tblAccountProductsBase::prdPrivs,         DBExpression::NIL() },
//            { tblAccountProductsBase::prdVAT,           DBExpression::NIL() },
            { tblAccountProductsBase::prdInStockCount,  444 },
//            { tblAccountProductsBase::prdOrderedCount,  DBExpression::NIL() },
//            { tblAccountProductsBase::prdReturnedCount, DBExpression::NIL() },
//            { tblAccountProductsBase::prdStatus,        DBExpression::NIL() },
            { tblAccountProducts::prd_locID,            _locationID },
//            { tblAccountProducts::prdShowPerDay,        DBExpression::NIL() },
//            { tblAccountProducts::prdShowTotal,         DBExpression::NIL() },
//            { tblAccountProducts::prdClicksPerDay,      DBExpression::NIL() },
//            { tblAccountProducts::prdClicksPerMonth,    DBExpression::NIL() },
//            { tblAccountProducts::prdClicksTotal,       DBExpression::NIL() },
        }))
    ;

    clsJWT JWT(_JWT);
    quint64 insertedID = query.execute(JWT.usrID());
    return insertedID;
}

quint32 Advert::apiCREATESaleable(
        TAPI::JWT_t _JWT
    )
{
    return 456;
}

TAPI::stuAdvert Advert::apiGETNewBanner(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _location,
        TAPI::enuAdvertOrder::Type _order
    )
{
    /*clsDACResult Result = AdvertDACInstance().execQuery(
                                "",
                                "SELECT "
                                )*/
}

TAPI::stuAdvert Advert::apiGETNewText(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _location,
        TAPI::enuAdvertOrder::Type _order,
        const QString _keywords
    )
{

}

QString Advert::apiGETRetrieveURL(
        TAPI::RemoteIP_t _REMOTE_IP,
        quint64 _id,
        TAPI::IPv4_t _clientIP,
        QString _agent
    )
{

}

Advert::Advert() :
    Accounting::intfRESTAPIWithAccounting(AdvertSchema,
                                         AdvertDomain,
                                         {
                                            {"show", {"slbShowPerDay", {}, {}, "slbShowTotal"}},
                                            {"click", {"slbShowPerDay", {}, {}, "slbShowTotal"}},
                                         },
                                         &Advertisement::AccountProducts::instance(),
                                         &Advertisement::AccountSaleables::instance(),
                                         &Advertisement::AccountUserAssets::instance(),
                                         &Advertisement::AccountAssetUsage::instance(),
                                         &Advertisement::AccountCoupons::instance()
                                         )
{
    this->addSubModule(AccountProducts.data());
    this->addSubModule(AccountSaleables.data());
    this->addSubModule(AccountUserAssets.data());
    this->addSubModule(AccountAssetUsage.data());
    this->addSubModule(AccountCoupons.data());
    //this->addSubModule(AccountPrizes); // There is no prize in advertisement module

    this->addSubModule(&Advertisement::ActiveAds::instance());
    this->addSubModule(&Advertisement::Bin::instance());
    this->addSubModule(&Advertisement::Locations::instance());
    this->addSubModule(&Advertisement::Banners::instance());
    this->addSubModule(&Advertisement::Clicks::instance());
    this->addSubModule(&Advertisement::Props::instance());
}

/*******************************************************************************************************************/
Accounting::stuServiceCreditsInfo Advert::retrieveServiceCreditsInfo(quint64 _usrID)
{

}

void Advert::breakCredit(quint64 _slbID)
{

}

bool Advert::isUnlimited(const Accounting::UsageLimits_t& _limits) const
{

}

bool Advert::isEmpty(const Accounting::UsageLimits_t& _limits) const
{

}

}
}
