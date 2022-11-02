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
#include "Interfaces/Helpers/SecurityHelper.h"
#include "libTargomanDBM/clsDAC.h"
#include "ORM/Accounting.h"
#include "ORM/Defs.hpp"
#include "ORM/ActiveAds.h"
#include "ORM/Bin.h"
#include "ORM/Clicks.h"
#include "ORM/Props.h"
#include "ORM/Locations.h"
#include "Interfaces/Helpers/RESTClientHelper.h"
#include "Interfaces/Helpers/FixtureHelper.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertType);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAdvertOrder);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuBannerSize);
//TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuAccountOrdersStatus);

//    COMPLEXITY_Complex,
TAPI_REGISTER_METATYPE_TYPE_STRUCT(
    Targoman::API::AdvertModule,
    stuAdvert
);

namespace Targoman::API::AdvertModule {

using namespace ORM;
using namespace Helpers;
using namespace DBManager;
using namespace AAA;

TARGOMAN_API_MODULE_IMPLEMENT(Advert)
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_DB_CONFIG(Advert, AdvertSchema)
//---------------------------------------------------------
TARGOMAN_API_MODULE_IMPLEMENT_MIGRATIONS(Advert, AdvertSchema)
TARGOMAN_API_MODULE_IMPLEMENT_CONFIGURATIONS(Advert, AdvertSchema)
TARGOMAN_API_MODULE_IMPLEMENT_I18N(Advert, AdvertSchema)
TARGOMAN_API_MODULE_IMPLEMENT_ACTIONLOG(Advert, AdvertSchema)
TARGOMAN_API_MODULE_IMPLEMENT_OBJECTSTORAGE(Advert, AdvertSchema)
TARGOMAN_API_MODULE_IMPLEMENT_FAQ(Advert, AdvertSchema)

Advert::Advert() :
    intfAccountingBasedModule(
        AdvertDomain,
        AdvertSchema,
        {
            { "show", {
                /* asset-day   */ tblAccountUserAssets::ExtraFields::uasDayShow,
                /* usage-day   */ tblAccountAssetUsage::ExtraFields::usgDayShow,
                /* asset-week  */ {},
                /* usage-week  */ {},
                /* asset-month */ {},
                /* usage-month */ {},
                /* asset-total */ tblAccountUserAssets::ExtraFields::uasTotalShow,
                /* usage-total */ tblAccountAssetUsage::ExtraFields::usgTotalShow
            }},
            { "click", {
                /* asset-day   */ tblAccountUserAssets::ExtraFields::uasDayClicks,
                /* usage-day   */ tblAccountAssetUsage::ExtraFields::usgDayClicks,
                /* asset-week  */ {},
                /* usage-week  */ {},
                /* asset-month */ tblAccountUserAssets::ExtraFields::uasMonthClicks,
                /* usage-month */ tblAccountAssetUsage::ExtraFields::usgMonthClicks,
                /* asset-total */ tblAccountUserAssets::ExtraFields::uasTotalClicks,
                /* usage-total */ tblAccountAssetUsage::ExtraFields::usgTotalClicks
            }},
        },
        &AccountUnits::instance(),
        &AccountProducts::instance(),
        &AccountSaleables::instance(),
        &AccountSaleablesFiles::instance(),
        &AccountUserAssets::instance(),
        &AccountUserAssetsFiles::instance(),
        &AccountAssetUsage::instance(),
        &AccountAssetUsageHistory::instance(),
        &AccountCoupons::instance()
) {
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_MIGRATIONS(Advert, AdvertSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_CONFIGURATIONS(Advert, AdvertSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_I18N(Advert, AdvertSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_ACTIONLOG(Advert, AdvertSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_OBJECTSTORAGE(Advert, AdvertSchema)
    TARGOMAN_API_MODULE_IMPLEMENT_CTOR_FAQ(Advert, AdvertSchema)

    this->addSubModule(AccountUnits.data());
    this->addSubModule(AccountProducts.data());
    this->addSubModule(AccountSaleables.data());
    this->addSubModule(AccountSaleablesFiles.data());
    this->addSubModule(AccountUserAssets.data());
    this->addSubModule(AccountUserAssetsFiles.data());
    this->addSubModule(AccountAssetUsage.data());
    this->addSubModule(AccountAssetUsageHistory.data());
    this->addSubModule(AccountCoupons.data());
    //this->addSubModule(AccountPrizes); // There is no prize in advertisement module

    this->addSubModule(&ActiveAds::instance());
    this->addSubModule(&Bin::instance());
    this->addSubModule(&Locations::instance());
    this->addSubModule(&Banners::instance());
    this->addSubModule(&Clicks::instance());
    this->addSubModule(&Props::instance());
}

// accounting
/*********************************************************************/
stuServiceCreditsInfo Advert::retrieveServiceCreditsInfo(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _actorID,
    const ServiceUsage_t &_requestedUsage,
    const QString &_action
) {
    ///@TODO: complete this
    return stuServiceCreditsInfo(
        /* activeCredits     */ {},
        /* preferedCredit    */ NULLABLE_NULL_VALUE,
        /* parentID          */ NULLABLE_NULL_VALUE,
        /* myLimitsOnParent  */ {},
        /* dbCurrentDateTime */ {}
    );
}

void Advert::breakCredit(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const stuAssetItem &_assetItem,
    const QString &_action
) {
}

bool Advert::isUnlimited(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const UsageLimits_t &_limits
) const {
}

bool Advert::isEmpty(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const UsageLimits_t &_limits
) const {
}

void Advert::saveAccountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    stuActiveCredit &_activeCredit,
    const ServiceUsage_t &_requestedUsage,
    const QString &_action
) {
    if (_activeCredit.Credit.UserAsset.uasID == 0)
        return;


















}

// basket
/*********************************************************************/
void Advert::computeAdditives(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuBasketItem     &_basketItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this
//    qDebug() << "----------" << "_orderAdditives:" << _orderAdditives;
//    AssetItem.UnitPrice *= 1.1;
};

void Advert::computeReferrer(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuBasketItem     &_basketItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this

#ifdef QT_DEBUG
    ///::: SAMPLE CODE :::

    //1: add, modify or remove credit voucher for fp.com
    if (_oldVoucherItem != nullptr) {
        if (_oldVoucherItem->Qty != _basketItem.Qty) {
            //remove old
            QList<stuPendingVoucher>::iterator it = _basketItem.Private.PendingVouchers.begin();
            while (it != _basketItem.Private.PendingVouchers.end()) {
                if ((it->Name == PENDING_VOUCHER_NAME_REFERRER_PRIZE)
                    && (it->Info.contains("referrer"))
                ) {
                    it = _basketItem.Private.PendingVouchers.erase(it);
                    continue;
                }

                ++it;
            }
        }
    }

    if (_basketItem.Qty > 0) {
        _basketItem.Private.PendingVouchers.append({
            /* Name     */ PENDING_VOUCHER_NAME_REFERRER_PRIZE,
            /* Type     */ enuVoucherType::Prize, //Credit,
            /* Amount   */ 2000 * _basketItem.Qty,
            /* Info     */ {
                               { "referrer", _basketItem.Referrer },
                           },
        });
    }

    //2: add, modify or remove system discount
    this->computeSystemDiscount(APICALLBOOM_PARAM, _basketItem, {
                                  QString("referrer_%1").arg("fp.com"),
                                  "5% off by fp.com",
                                  5,
                                  enuDiscountType::Percent,
                                  10'000
                              }, _oldVoucherItem);

    //3: inc translate words max limit (30'000 -> 35'000)
//    int IncAmount = 5'000;
//    if (_basketItem.AdditionalInfo.contains("plus-max-words"))
//        _basketItem.AdditionalInfo["plus-max-words"] = IncAmount + _basketItem.AdditionalInfo["plus-max-words"].toInt();
//    else
//        _basketItem.AdditionalInfo.insert("plus-max-words", IncAmount);

    //4: inc days (30 -> 40)
    int IncDays = 10;
    if (_basketItem.AdditionalInfo.contains(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS))
        _basketItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS] = IncDays + _basketItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS].toInt();
    else
        _basketItem.AdditionalInfo.insert(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS, IncDays);
#endif
};

QVariantMap Advert::getCustomUserAssetFieldsForQuery(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuBasketItem     &_basketItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: [very important] complete this

    QVariantMap Result;

    if (_basketItem.AdditionalInfo.contains(ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS))
        Result.insert(tblAccountUserAssets::ExtraFields::uasDays, _basketItem.AdditionalInfo[ASSET_ITEM_ADDITIONAL_INTO_KEY_PLUS_MAX_DAYS].toInt());

    return Result;
}

/***************************************************************************************************/
//bool IMPL_REST_POST(Advert, processVoucher, (
//        APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
//        Targoman::API::AAA::stuVoucherItem _voucherItem
//    ))
//{
//    clsJWT JWT(_JWT);
//    quint64 currentUserID = JWT.usrID();

//    this->increaseDiscountUsage(_voucherItem);
//    this->activateUserAsset(currentUserID, _voucherItem);

//    return true;
//}

//bool IMPL_REST_POST(Advert, cancelVoucher, (
//        APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
//        Targoman::API::AAA::stuVoucherItem _voucherItem
//    ))
//{
//    clsJWT JWT(_JWT);
//    quint64 currentUserID = JWT.usrID();

//    this->decreaseDiscountUsage(_voucherItem);
//    this->removeFromUserAssets(currentUserID, _voucherItem);

//    return true;
//}

Targoman::API::AdvertModule::stuAdvert IMPL_REST_GET(Advert, newBanner, (
    APICALLBOOM_TYPE_JWT_ANONYMOUSE_IMPL &APICALLBOOM_PARAM,
    QString _location,
    Targoman::API::AdvertModule::enuAdvertOrder::Type _order
)) {
}

Targoman::API::AdvertModule::stuAdvert IMPL_REST_GET(Advert, newText, (
    APICALLBOOM_TYPE_JWT_ANONYMOUSE_IMPL &APICALLBOOM_PARAM,
    QString _location,
    Targoman::API::AdvertModule::enuAdvertOrder::Type _order,
    const QString _keywords
)) {
}

QString IMPL_REST_GET(Advert, retrieveURL, (
    APICALLBOOM_TYPE_JWT_ANONYMOUSE_IMPL &APICALLBOOM_PARAM,
    quint64 _id,
    TAPI::IPv4_t _clientIP,
    QString _agent
)) {
}

/****************************************************************\
|** fixture *****************************************************|
\****************************************************************/
#ifdef QT_DEBUG
QVariant IMPL_REST_POST(Advert, fixtureSetup, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
    QVariantMap Result;

    if (_random == "1")
        _random = QString::number(QRandomGenerator::global()->generate());

    if (_random.isEmpty() == false)
        Result.insert("Random", _random);

    //-- location --------------------------------------
    QString LocationUrl = FixtureHelper::MakeRandomizeName(_random, ".", "http://fixture", "com"); //.arg(SecurityHelper::UUIDtoMD5());

    QVariantMap LocationValues = {
        { tblLocations::Fields::locURL,        LocationUrl },
        { tblLocations::Fields::locPlaceCode,  "FIX" },
    };

    quint32 LocationID = Locations::instance().makeCreateQuery(APICALLBOOM_PARAM)
                         .addCols({
                                      tblLocations::Fields::locURL,
                                      tblLocations::Fields::locPlaceCode,
//                                      tblLocations::Fields::locStatus,
                                  })
                         .values(LocationValues)
                         .execute(1);

    LocationValues.insert(tblLocations::Fields::locID, LocationID);
    Result.insert("Location", LocationValues);

    //-- product --------------------------------------
    QString ProductCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "product");

    TAPI::ORMFields_t ProductValues = TAPI::ORMFields_t({
        { tblAccountProductsBase::Fields::prdCode,              ProductCode },
        { tblAccountProductsBase::Fields::prdName,              FixtureHelper::MakeRandomizeName(_random, " ", "fixture product", "name") },
        { tblAccountProductsBase::Fields::prdI18NData,          QVariantMap({
            { tblAccountProductsBase::Fields::prdName, QVariantMap({
                { "fa", FixtureHelper::MakeRandomizeName(_random, " ", "آگهی شماره") },
            }) },
        }) },
        { tblAccountProductsBase::Fields::prdInStockQty,        1'000 },
        { tblAccountProductsBase::Fields::prd_untID,            1 },
        { tblAccountProducts::ExtraFields::prdType,           Targoman::API::AdvertModule::enuProductType::toStr(Targoman::API::AdvertModule::enuProductType::Advertise) },
        { tblAccountProducts::ExtraFields::prd_locID,         LocationID },
    });

    quint32 ProductID = this->AccountProducts->Create(APICALLBOOM_PARAM, ProductValues);

    ProductValues.insert(tblAccountProductsBase::Fields::prdID, ProductID);
    Result.insert("Product", ProductValues);

    //-- saleable --------------------------------------
    QString SaleableCode = (_random.isEmpty() ? "0-0" : QString("%1-%1").arg(_random));

    TAPI::ORMFields_t SaleableValues = TAPI::ORMFields_t({
        { tblAccountSaleablesBase::Fields::slb_prdID,           ProductID },
        { tblAccountSaleablesBase::Fields::slbCode,             SaleableCode },
        { tblAccountSaleablesBase::Fields::slbName,             FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "name") },
        { tblAccountSaleablesBase::Fields::slbI18NData,         QVariantMap({
            { tblAccountSaleablesBase::Fields::slbName, QVariantMap({
                { "fa", FixtureHelper::MakeRandomizeName(_random, " ", "طرح فروش آگهی شماره") },
            }) },
        }) },
        { tblAccountSaleablesBase::Fields::slbDesc,             FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "desc") },
        { tblAccountSaleablesBase::Fields::slbType,             TAPI::enuSaleableType::toStr(TAPI::enuSaleableType::Special) },
        { tblAccountSaleablesBase::Fields::slbBasePrice,        12'000 },
//        { tblAccountSaleablesBase::Fields::slbProductCount,     900 },
//        { tblAccountSaleablesBase::Fields::slbMaxSaleCountPerUser,  },
        { tblAccountSaleablesBase::Fields::slbInStockQty,       150 },
        { tblAccountSaleablesBase::Fields::slbVoucherTemplate,  FixtureHelper::MakeRandomizeName(_random, " ", "fixture saleable", "vt") },
    });

    quint32 SaleableID = this->AccountSaleables->Create(APICALLBOOM_PARAM, SaleableValues);

    SaleableValues.insert(tblAccountSaleablesBase::Fields::slbID, SaleableID);
    Result.insert("Saleable", SaleableValues);

    //-- coupon --------------------------------------
    QString CouponCode = FixtureHelper::MakeRandomizeName(_random, ".", "fixture", "cpn");

    QVariantMap CouponValues = {
        { tblAccountCouponsBase::Fields::cpnCode, CouponCode },
        { tblAccountCouponsBase::Fields::cpnPrimaryCount, 100 },
        { tblAccountCouponsBase::Fields::cpnTotalMaxAmount, 100'000'000 },
        { tblAccountCouponsBase::Fields::cpnPerUserMaxCount, 2 },
        { tblAccountCouponsBase::Fields::cpnPerUserMaxAmount, 10'000'000 },
        { tblAccountCouponsBase::Fields::cpnValidFrom, "2020/1/1 1:2:3" },
//        { tblAccountCouponsBase::Fields::cpnExpiryTime,  },
        { tblAccountCouponsBase::Fields::cpnAmount, 10 },
        { tblAccountCouponsBase::Fields::cpnAmountType, Targoman::API::AAA::enuDiscountType::toStr(Targoman::API::AAA::enuDiscountType::Percent) },
//        { tblAccountCouponsBase::Fields::cpnMaxAmount,  },
        { tblAccountCouponsBase::Fields::cpnSaleableBasedMultiplier,
          QVariantList({
              QVariantMap({ { "saleableCode", SaleableCode }, { "multiplier", 1.5 }, { "minQty", 0 } }),
              QVariantMap({ { "saleableCode", SaleableCode }, { "multiplier", 1.8 }, { "minQty", 5 } }),
              QVariantMap({ { "saleableCode", "other" },      { "multiplier", 2.0 }                  }),
          })
        },
//        { tblAccountCouponsBase::Fields::cpnTotalUsedCount,  },
//        { tblAccountCouponsBase::Fields::cpnTotalUsedAmount,  },
//        { tblAccountCouponsBase::Fields::cpnStatus,  },
    };

    quint32 CouponID = this->AccountCoupons->makeCreateQuery(APICALLBOOM_PARAM)
                       .addCols({
//                                    tblAccountCouponsBase::Fields::cpnID,
                                    tblAccountCouponsBase::Fields::cpnCode,
                                    tblAccountCouponsBase::Fields::cpnPrimaryCount,
                                    tblAccountCouponsBase::Fields::cpnTotalMaxAmount,
                                    tblAccountCouponsBase::Fields::cpnPerUserMaxCount,
                                    tblAccountCouponsBase::Fields::cpnPerUserMaxAmount,
                                    tblAccountCouponsBase::Fields::cpnValidFrom,
//                                    tblAccountCouponsBase::Fields::cpnExpiryTime,
                                    tblAccountCouponsBase::Fields::cpnAmount,
                                    tblAccountCouponsBase::Fields::cpnAmountType,
//                                    tblAccountCouponsBase::Fields::cpnMaxAmount,
                                    tblAccountCouponsBase::Fields::cpnSaleableBasedMultiplier,
//                                    tblAccountCouponsBase::Fields::cpnTotalUsedCount,
//                                    tblAccountCouponsBase::Fields::cpnTotalUsedAmount,
//                                    tblAccountCouponsBase::Fields::cpnStatus,
                                })
                       .values(CouponValues)
                       .execute(1);

    CouponValues.insert(tblAccountCouponsBase::Fields::cpnID, CouponID);
    Result.insert("Coupon", CouponValues);

    //----------------------------------------
    stuPreVoucher LastPreVoucher;
    stuVoucher Voucher;
    stuVoucher ApproveOnlinePaymentVoucher;

    //-- add to basket --------------------------------------
    stuBasketActionResult BasketActionResult = this->apiPOSTaddToBasket(
        APICALLBOOM_PARAM,
        /* saleableCode     */ SaleableCode,
        /* orderAdditives   */ { { "adtv1", "1 1 1" }, { "adtv2", "222" } },
        /* qty              */ 1,
        /* discountCode     */ CouponCode,
        /* referrer         */ "",
        /* referrerParams   */ {},
//        /* apiToken         */ {},
        /* lastPreVoucher   */ LastPreVoucher
    );
    LastPreVoucher = BasketActionResult.PreVoucher;
    Result.insert("LastPreVoucher", LastPreVoucher.toJson());

    //-- finalize basket --------------------------------------
    QVariant res = RESTClientHelper::callAPI(
        APICALLBOOM_PARAM,
        RESTClientHelper::POST,
        "Account/finalizeBasket",
        {},
        {
            { "preVoucher",             LastPreVoucher.toJson().toVariantMap() },
            { "gatewayType",            "_DeveloperTest" },
            { "domain",                 "dev.test" },
//            { "walID",               9988 },
            { "paymentVerifyCallback",  "http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage" },
        }
    );
    Voucher.fromJson(res.toJsonObject());
    Result.insert("Voucher", Voucher.toJson());

    //-- approve online payment --------------------------------------
    if (Voucher.PaymentKey.isEmpty() == false) {
        QVariant res = RESTClientHelper::callAPI(
            APICALLBOOM_PARAM,
            RESTClientHelper::POST,
            "Account/approveOnlinePayment",
            {},
            {
                { "paymentKey",     Voucher.PaymentKey },
//                { "domain",         "dev.test" },
                { "pgResponse",     QVariantMap({
                    { "result",     "ok" },
                }) },
            }
        );
        ApproveOnlinePaymentVoucher.fromJson(res.toJsonObject());
        Result.insert("ApproveOnlinePaymentVoucher", ApproveOnlinePaymentVoucher.toJson());
    }

    //----------------------------------------
    return Result;
}

//bool IMPL_REST_POST(Advert, fixtureSetupVoucher, (
//        APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM
//    ))
//{
//}

QVariant IMPL_REST_POST(Advert, fixtureCleanup, (
    APICALLBOOM_TYPE_JWT_USER_IMPL &APICALLBOOM_PARAM,
    QString _random
)) {
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
        clsDACResult DACResult = this->AccountCoupons->execQuery(APICALLBOOM_PARAM,
                                                                 QueryString, {
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
        clsDACResult DACResult = this->AccountSaleables->execQuery(APICALLBOOM_PARAM,
                                                                   QueryString, {
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
        clsDACResult DACResult = this->AccountProducts->execQuery(APICALLBOOM_PARAM,
                                                                  QueryString, {
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
        clsDACResult DACResult = Locations::instance().execQuery(APICALLBOOM_PARAM,
                                                                 QueryString, {
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
