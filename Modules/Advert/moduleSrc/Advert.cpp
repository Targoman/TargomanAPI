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
#include "libQFieldValidator/QFieldValidator.h"
#include "Interfaces/AAA/PrivHelpers.h"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Interfaces/Common/HTTPExceptions.hpp"
//#include "Interfaces/ORM/APIQueryBuilders.h"
#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

#include "ORM/Accounting.h"
#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"

#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"
using namespace Targoman::API::Helpers;

using namespace Targoman::API::AAA;

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuBannerSize);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAccountOrdersStatus);

TAPI_REGISTER_METATYPE(
    COMPLEXITY_Complex,
    Targoman::API::AdvertModule,
    stuAdvert,
    [](const Targoman::API::AdvertModule::stuAdvert& _value) -> QVariant { return _value.toJson(); }
//    [](const Targoman::API::AdvertModule::stuAdvert& _value) -> QVariant { return _value.toVariant(); }
);

//using namespace Targoman::API::AAA;
//using namespace AdvertModule;

namespace Targoman::API::AdvertModule {

using namespace ORM;

TARGOMAN_IMPL_API_MODULE(Advert)
TARGOMAN_API_MODULE_DB_CONFIG_IMPL(Advert, AdvertSchema)
TARGOMAN_API_OBJECTSTORAGE_CONFIG_IMPL(Advert, AdvertSchema)

Advert::Advert() :
    intfAccountingBasedModule(
        AdvertDomain,
        AdvertSchema,
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
    TARGOMAN_API_IMPLEMENT_ACTIONLOG(Advert, AdvertSchema)
    TARGOMAN_API_IMPLEMENT_OBJECTSTORAGE(Advert, AdvertSchema)

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
    //TODO: complete this
    return stuServiceCreditsInfo(
                {},
                NULLABLE_NULL_VALUE,
                NULLABLE_NULL_VALUE,
                {},
                {}
                );
}

void Advert::breakCredit(quint64 _slbID) {
}

bool Advert::isUnlimited(const UsageLimits_t& _limits) const
{
}

bool Advert::isEmpty(const UsageLimits_t& _limits) const
{
}

void Advert::applyAssetAdditives(
        TAPI::JWT_t _JWT,
        INOUT stuAssetItem& _assetItem,
        const OrderAdditives_t& _orderAdditives
    )
{
//    qDebug() << "----------" << "_orderAdditives:" << _orderAdditives;

//    _assetItem.slbBasePrice *= 1.1;
};

/***************************************************************************************************/
//bool Advert::apiPOSTprocessVoucher(
//        TAPI::JWT_t _JWT,
//        Targoman::API::AAA::stuVoucherItem _voucherItem
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
//        Targoman::API::AAA::stuVoucherItem _voucherItem
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
{
}

Targoman::API::AdvertModule::stuAdvert Advert::apiGETnewText(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _location,
        Targoman::API::AdvertModule::enuAdvertOrder::Type _order,
        const QString _keywords
    )
{
}

QString Advert::apiGETretrieveURL(
        TAPI::RemoteIP_t _REMOTE_IP,
        quint64 _id,
        TAPI::IPv4_t _clientIP,
        QString _agent
    )
{
}

/****************************************************************\
|** fixture *****************************************************|
\****************************************************************/
#ifdef QT_DEBUG
QVariant Advert::apiPOSTfixtureSetup(
        TAPI::RemoteIP_t _REMOTE_IP,
        TAPI::JWT_t _JWT,
        QString _random
    )
{
    Q_UNUSED(_REMOTE_IP);

    QVariantMap Result;

    if (_random == "1")
        _random = QString("%1").arg(QRandomGenerator::global()->generate());

    if (_random.isEmpty() == false)
        Result.insert("Random", _random);

    //-- location --------------------------------------
    QString LocationUrl = FixtureHelper::MakeRandomizeName(_random, ".", "http://fixture", "com"); //.arg(SecurityHelper::UUIDtoMD5());

    QVariantMap LocationValues = {
        { tblLocations::locURL,        LocationUrl },
        { tblLocations::locPlaceCode,  "FIX" },
    };

    quint32 LocationID = CreateQuery(Locations::instance())
                         .addCol(tblLocations::locURL)
                         .addCol(tblLocations::locPlaceCode)
//                         .addCol(tblLocations::locStatus)
                         .values(LocationValues)
                         .execute(1);

    LocationValues.insert(tblLocations::locID, LocationID);
    Result.insert("Location", LocationValues);

    //-- product --------------------------------------
    QString ProductCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "product");

    QVariantMap ProductValues = {
        { tblAccountProductsBase::prdCode,          ProductCode },
        { tblAccountProductsBase::prdName,          FixtureHelper::MakeRandomizeName(_random, " ", "fixture product", "name") },
        { tblAccountProductsBase::prdInStockQty,    1'000 },
        { tblAccountProducts::prdType,              Targoman::API::AdvertModule::enuProductType::toStr(Targoman::API::AdvertModule::enuProductType::Advertise) },
        { tblAccountProducts::prd_locID,            LocationID },
    };

    quint32 ProductID = CreateQuery(*this->AccountProducts)
//                        .addCol(tblAccountProductsBase::prdID)
                        .addCol(tblAccountProductsBase::prdCode)
                        .addCol(tblAccountProductsBase::prdName)
//                        .addCol(tblAccountProductsBase::prdDesc)
//                        .addCol(tblAccountProductsBase::prdValidFromDate)
//                        .addCol(tblAccountProductsBase::prdValidToDate)
//                        .addCol(tblAccountProductsBase::prdValidFromHour)
//                        .addCol(tblAccountProductsBase::prdValidToHour)
//                        .addCol(tblAccountProductsBase::prdPrivs)
//                        .addCol(tblAccountProductsBase::prdVAT)
                        .addCol(tblAccountProductsBase::prdInStockQty)
//                        .addCol(tblAccountProductsBase::prdOrderedQty)
//                        .addCol(tblAccountProductsBase::prdReturnedQty)
//                        .addCol(tblAccountProductsBase::prdStatus)
                        .addCol(tblAccountProducts::prdType)
                        .addCol(tblAccountProducts::prd_locID)
//                        .addCol(tblAccountProducts::prdShowPerDay)
//                        .addCol(tblAccountProducts::prdShowTotal)
//                        .addCol(tblAccountProducts::prdClicksPerDay)
//                        .addCol(tblAccountProducts::prdClicksPerMonth)
//                        .addCol(tblAccountProducts::prdClicksTotal)
                        .values(ProductValues)
                        .execute(1);

    ProductValues.insert(tblAccountProductsBase::prdID, ProductID);
    Result.insert("Product", ProductValues);

    //-- saleable --------------------------------------
    QString SaleableCode = (_random.isEmpty() ? "0-0" : QString("%1-%1").arg(_random));

    QVariantMap SaleableValues = {
        { tblAccountSaleablesBase::slb_prdID,           ProductID },
        { tblAccountSaleablesBase::slbCode,             SaleableCode },
        { tblAccountSaleablesBase::slbName,             FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "name") },
        { tblAccountSaleablesBase::slbDesc,             FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "desc") },
        { tblAccountSaleablesBase::slbType,             TAPI::enuSaleableType::toStr(TAPI::enuSaleableType::Special) },
        { tblAccountSaleablesBase::slbBasePrice,        12'000 },
//        { tblAccountSaleablesBase::slbProductCount,     900 },
//        { tblAccountSaleablesBase::slbMaxSaleCountPerUser,  },
        { tblAccountSaleablesBase::slbInStockQty,       150 },
        { tblAccountSaleablesBase::slbVoucherTemplate,  FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "vt") },
    };

    quint32 SaleableID = CreateQuery(*this->AccountSaleables)
//                         .addCol(tblAccountSaleablesBase::slbID)
                         .addCol(tblAccountSaleablesBase::slb_prdID)
                         .addCol(tblAccountSaleablesBase::slbCode)
                         .addCol(tblAccountSaleablesBase::slbName)
                         .addCol(tblAccountSaleablesBase::slbDesc)
                         .addCol(tblAccountSaleablesBase::slbType)
//                         .addCol(tblAccountSaleablesBase::slbAvailableFromDate)
//                         .addCol(tblAccountSaleablesBase::slbAvailableToDate)
//                         .addCol(tblAccountSaleablesBase::slbPrivs)
                         .addCol(tblAccountSaleablesBase::slbBasePrice)
//                         .addCol(tblAccountSaleablesBase::slbAdditives)
//                         .addCol(tblAccountSaleablesBase::slbProductCount)
//                         .addCol(tblAccountSaleablesBase::slbMaxSaleCountPerUser)
                         .addCol(tblAccountSaleablesBase::slbInStockQty)
//                         .addCol(tblAccountSaleablesBase::slbOrderedQty)
//                         .addCol(tblAccountSaleablesBase::slbReturnedQty)
                         .addCol(tblAccountSaleablesBase::slbVoucherTemplate)
//                         .addCol(tblAccountSaleablesBase::slbStatus)
//                         .addCol(tblAccountSaleablesBase::slbCreatedBy_usrID)
//                         .addCol(tblAccountSaleablesBase::slbCreationDateTime)
//                         .addCol(tblAccountSaleablesBase::slbUpdatedBy_usrID)
//                         .addCol(tblAccountSaleables::slbShowPerDay)
//                         .addCol(tblAccountSaleables::slbShowTotal)
//                         .addCol(tblAccountSaleables::slbClicksPerDay)
//                         .addCol(tblAccountSaleables::slbClicksPerMonth)
//                         .addCol(tblAccountSaleables::slbClicksTotal)
                         .values(SaleableValues)
                         .execute(1);

     SaleableValues.insert(tblAccountSaleablesBase::slbID, SaleableID);
     Result.insert("Saleable", SaleableValues);

    //-- coupon --------------------------------------
    QString CouponCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "cpn");

    QVariantMap CouponValues = {
        { tblAccountCouponsBase::cpnCode, CouponCode },
        { tblAccountCouponsBase::cpnPrimaryCount, 100 },
        { tblAccountCouponsBase::cpnTotalMaxAmount, 100'000'000 },
        { tblAccountCouponsBase::cpnPerUserMaxCount, 2 },
        { tblAccountCouponsBase::cpnPerUserMaxAmount, 10'000'000 },
        { tblAccountCouponsBase::cpnValidFrom, "2020/1/1 1:2:3" },
//        { tblAccountCouponsBase::cpnExpiryTime,  },
        { tblAccountCouponsBase::cpnAmount, 10 },
        { tblAccountCouponsBase::cpnAmountType, Targoman::API::AAA::enuDiscountType::toStr(Targoman::API::AAA::enuDiscountType::Percent) },
//        { tblAccountCouponsBase::cpnMaxAmount,  },
        { tblAccountCouponsBase::cpnSaleableBasedMultiplier,
          QVariantList({
              QVariantMap({ { "saleableCode", SaleableCode }, { "multiplier", 1.5 }, { "minQty", 0 } }),
              QVariantMap({ { "saleableCode", SaleableCode }, { "multiplier", 1.8 }, { "minQty", 5 } }),
              QVariantMap({ { "saleableCode", "other" },      { "multiplier", 2.0 }                  }),
          })
        },
//        { tblAccountCouponsBase::cpnTotalUsedCount,  },
//        { tblAccountCouponsBase::cpnTotalUsedAmount,  },
//        { tblAccountCouponsBase::cpnStatus,  },
    };

    quint32 CouponID = CreateQuery(*this->AccountCoupons)
//                       .addCol(tblAccountCouponsBase::cpnID)
                       .addCol(tblAccountCouponsBase::cpnCode)
                       .addCol(tblAccountCouponsBase::cpnPrimaryCount)
                       .addCol(tblAccountCouponsBase::cpnTotalMaxAmount)
                       .addCol(tblAccountCouponsBase::cpnPerUserMaxCount)
                       .addCol(tblAccountCouponsBase::cpnPerUserMaxAmount)
                       .addCol(tblAccountCouponsBase::cpnValidFrom)
//                       .addCol(tblAccountCouponsBase::cpnExpiryTime)
                       .addCol(tblAccountCouponsBase::cpnAmount)
                       .addCol(tblAccountCouponsBase::cpnAmountType)
//                       .addCol(tblAccountCouponsBase::cpnMaxAmount)
                       .addCol(tblAccountCouponsBase::cpnSaleableBasedMultiplier)
//                       .addCol(tblAccountCouponsBase::cpnTotalUsedCount)
//                       .addCol(tblAccountCouponsBase::cpnTotalUsedAmount)
//                       .addCol(tblAccountCouponsBase::cpnStatus)
                       .values(CouponValues)
                       .execute(1);

    CouponValues.insert(tblAccountCouponsBase::cpnID, CouponID);
    Result.insert("Coupon", CouponValues);

    //----------------------------------------
    stuPreVoucher LastPreVoucher;
    stuVoucher Voucher;
    stuVoucher ApproveOnlinePaymentVoucher;

    //-- add to basket --------------------------------------
    LastPreVoucher = this->apiPOSTaddToBasket(
        _JWT,
        /* saleableCode        */ SaleableCode,
        /* orderAdditives      */ { { "adtv1", "1 1 1" }, { "adtv2", "222" } },
        /* qty                 */ 1,
        /* discountCode        */ CouponCode,
        /* referrer            */ "",
        /* extraReferrerParams */ {},
        /* lastPreVoucher      */ LastPreVoucher
    );
    Result.insert("LastPreVoucher", LastPreVoucher.toJson());

    //-- finalize basket --------------------------------------
    QVariant res = RESTClientHelper::callAPI(
        _JWT,
        RESTClientHelper::POST,
        "Account/finalizeBasket",
        {},
        {
            { "preVoucher",             LastPreVoucher.toJson().toVariantMap() },
            { "gatewayType",            "_DeveloperTest" },
            { "domain",                 "dev.test" },
            { "walletID",               9988 },
            { "paymentVerifyCallback",  "http://www.a.com" },
        }
    );
    Voucher.fromJson(res.toJsonObject());
    Result.insert("Voucher", Voucher.toJson());

    //-- approve online payment --------------------------------------
    if (Voucher.PaymentMD5.isEmpty() == false) {
        QVariant res = RESTClientHelper::callAPI(
            _JWT,
            RESTClientHelper::POST,
            "Account/approveOnlinePayment",
            {},
            {
                { "paymentMD5",     Voucher.PaymentMD5 },
                { "domain",         "this.is.domain" },
                { "pgResponse",     QVariantMap({
                      { "resp_1", 1 },
                      { "resp_2", 2 },
                      { "resp_3", 3 },
                  }) },
            }
        );
        ApproveOnlinePaymentVoucher.fromJson(res.toJsonObject());
        Result.insert("ApproveOnlinePaymentVoucher", ApproveOnlinePaymentVoucher.toJson());
    }

    //----------------------------------------
    return Result;
}

//bool Advert::apiPOSTfixtureSetupVoucher(
//        TAPI::RemoteIP_t _REMOTE_IP,
//        TAPI::JWT_t _JWT
//    )
//{
//}

QVariant Advert::apiPOSTfixtureCleanup(
        TAPI::RemoteIP_t _REMOTE_IP,
        QString _random
    )
{
    Q_UNUSED(_REMOTE_IP);

    QVariantMap Result;

    //online payment
    //voucher

    try {
        QString CouponCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "cpn");
        QString QueryString = R"(
            DELETE c
              FROM tblAccountCoupons c
             WHERE c.cpnCode=?
        ;)";
        clsDACResult DACResult = this->AccountCoupons->execQuery(QueryString, {
                                                                     CouponCode
                                                                 });
        Result.insert("tblCoupon", QVariantMap({
                                                   { "items", CouponCode },
                                                   { "numRowsAffected", DACResult.numRowsAffected() },
                                               }));
    } catch (...) { ; }

    try {
        QString SaleableCode = (_random.isEmpty() ? "0-0" : QString("%1-%1").arg(_random));
        QString QueryString = R"(
            DELETE s
              FROM tblAccountSaleables s
             WHERE s.slbCode=?
        ;)";
        clsDACResult DACResult = this->AccountSaleables->execQuery(QueryString, {
                                                                       SaleableCode
                                                                   });
        Result.insert("tblSaleable", QVariantMap({
                                                  { "items", SaleableCode },
                                                  { "numRowsAffected", DACResult.numRowsAffected() },
                                              }));
    } catch (...) { ; }

    try {
        QString ProductCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "product");
        QString QueryString = R"(
            DELETE p
              FROM tblAccountProducts p
             WHERE p.prdCode=?
        ;)";
        clsDACResult DACResult = this->AccountProducts->execQuery(QueryString, {
                                                                      ProductCode
                                                                  });
        Result.insert("tblProduct", QVariantMap({
                                                 { "items", ProductCode },
                                                 { "numRowsAffected", DACResult.numRowsAffected() },
                                             }));
    } catch (...) { ; }

    try {
        QString LocationUrl = FixtureHelper::MakeRandomizeName(_random, ".", "http://fixture", "com");
        QString QueryString = R"(
            DELETE l
              FROM tblLocations l
             WHERE l.locURL=?
        ;)";
        clsDACResult DACResult = Locations::instance().execQuery(QueryString, {
                                                                     LocationUrl
                                                                 });
        Result.insert("tblLocation", QVariantMap({
                                                  { "items", LocationUrl },
                                                  { "numRowsAffected", DACResult.numRowsAffected() },
                                              }));
    } catch (...) { ; }

    return Result;
}
#endif

} //namespace Targoman::API::AdvertModule
