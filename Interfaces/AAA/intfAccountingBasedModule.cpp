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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "intfAccountingBasedModule.h"
#include "PrivHelpers.h"
#include "Interfaces/AAA/Authorization.h"
#include "App/Server/ServerConfigs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
using namespace Targoman::Common::Configuration;

//#include "Server/clsSimpleCrypt.h"
#include "Interfaces/Helpers/SecurityHelper.h"
#include "Interfaces/Helpers/IteratorHelper.hpp"
using namespace Targoman::API::Helpers;

#include "Interfaces/DBM/QueryBuilders.h"
using namespace Targoman::API::DBM;

using namespace Targoman::API::Common;

namespace Targoman::API::AAA {

static QMap<QString, intfAccountingBasedModule*> ServiceRegistry;

Targoman::Common::Configuration::tmplConfigurable<QString> Secret(
    makeConfig("Secret"),
    "Secret to be used for signing voucher and prevoucher",
    "fcy^E?a*4<;auY?>^6s@");

QByteArray signWithSecret(const QByteArray& _data) {
   return QMessageAuthenticationCode::hash(_data, Secret.value().toUtf8(), QCryptographicHash::Sha256);
}
QByteArray sign(stuPreVoucher _preVoucher) {
    return signWithSecret(QJsonDocument(_preVoucher.toJson()).toJson(QJsonDocument::Compact)).toBase64();
}
QByteArray sign(stuVoucherItem _voucherItem) {
    return signWithSecret(QJsonDocument(_voucherItem.toJson()).toJson(QJsonDocument::Compact)).toBase64();
}
QByteArray sign(stuVoucherItemForTrustedAction _voucherItemForTrustedAction) {
    return signWithSecret(QJsonDocument(_voucherItemForTrustedAction.toJson()).toJson(QJsonDocument::Compact)).toBase64();
}

void checkPreVoucherSanity(stuPreVoucher _preVoucher) {
    if (_preVoucher.Items.isEmpty())
        return;

    QString Sign = _preVoucher.Sign;
    _preVoucher.Sign.clear();
    if (Sign != QString(sign(_preVoucher)))
        throw exHTTPBadRequest("Invalid sign found on pre-Voucher items");

    foreach (auto VoucherItem, _preVoucher.Items) {
        QString Sign = VoucherItem.Sign;
        VoucherItem.Sign.clear();

        if (Sign != QString(sign(VoucherItem)))
            throw exHTTPBadRequest("at least one of pre-Voucher items has invalid sign");
    }
}

Targoman::Common::Configuration::tmplConfigurable<quint64> SimpleCryptKey(
    makeConfig("SimpleCryptKey"),
    "Secret to be used for encrypting private objects",
    static_cast<quint64>(43121109170974192),
    ReturnTrueCrossValidator(),
    "",
    "", //"SECRET",
    "", //"jwt-private-key",
    /*enuConfigSource::Arg | */enuConfigSource::File);

thread_local static clsSimpleCrypt* SimpleCryptInstance = nullptr;
clsSimpleCrypt* simpleCryptInstance()
{
    if (Q_UNLIKELY(!SimpleCryptInstance)) {
        SimpleCryptInstance = new clsSimpleCrypt(SimpleCryptKey.value());
        SimpleCryptInstance->setIntegrityProtectionMode(clsSimpleCrypt::ProtectionHash);
    }

    return SimpleCryptInstance;
}

intfAccountingBasedModule* serviceAccounting(const QString& _serviceName) {
    return ServiceRegistry.value(_serviceName);
}

/***************************************************************************************************\
|** intfAccountingBasedModule **********************************************************************|
\***************************************************************************************************/
intfAccountingBasedModule::intfAccountingBasedModule(
    const QString               &_module,
    const QString               &_schema,
    AssetUsageLimitsCols_t      _AssetUsageLimitsCols,
    intfAccountUnits            *_units,
//    intfAccountUnitsI18N        *_unitsI18N,
    intfAccountProducts         *_products,
//    intfAccountProductsI18N     *_productsI18N,
    intfAccountSaleables        *_saleables,
//    intfAccountSaleablesI18N    *_saleablesI18N,
    intfAccountSaleablesFiles   *_saleablesFiles,
    intfAccountUserAssets       *_userAssets,
    intfAccountUserAssetsFiles  *_userAssetsFiles,
    intfAccountAssetUsage       *_assetUsages,
    intfAccountCoupons          *_discounts,
    intfAccountPrizes           *_prizes
) :
    API::intfSQLBasedWithActionLogsModule(_module, _schema),
    ServiceName(_schema),
    AccountUnits(_units),
//    AccountUnitsI18N(_unitsI18N),
    AccountProducts(_products),
//    AccountProductsI18N(_productsI18N),
    AccountSaleables(_saleables),
//    AccountSaleablesI18N(_saleablesI18N),
    AccountSaleablesFiles(_saleablesFiles),
    AccountUserAssets(_userAssets),
    AccountUserAssetsFiles(_userAssetsFiles),
    AccountAssetUsages(_assetUsages),
    AccountCoupons(_discounts),
    AccountPrizes(_prizes),
    AssetUsageLimitsCols(_AssetUsageLimitsCols)
{
    foreach (auto Col, this->AssetUsageLimitsCols) {
        if (Col.PerDay.size())
            this->AssetUsageLimitsColsName.append(Col.PerDay);

        if (Col.PerWeek.size())
            this->AssetUsageLimitsColsName.append(Col.PerWeek);

        if (Col.PerMonth.size())
            this->AssetUsageLimitsColsName.append(Col.PerMonth);

        if (Col.Total.size())
            this->AssetUsageLimitsColsName.append(Col.Total);
    }

    ServiceRegistry.insert(this->ServiceName, this);
}

//intfAccountingBasedModule::~intfAccountingBasedModule() { ; }

stuActiveCredit intfAccountingBasedModule::activeAccountObject(quint64 _usrID) {
    return this->findBestMatchedCredit(_usrID);
}

void intfAccountingBasedModule::checkUsageIsAllowed(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const ServiceUsage_t &_requestedUsage
) {
    QJsonObject Privs = APICALLBOOM_PARAM.getJWTPrivsObject();

    if (Privs.contains(this->ServiceName) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->ServiceName);

    stuActiveCredit BestMatchedCredit = this->findBestMatchedCredit(APICALLBOOM_PARAM.getUserID(), _requestedUsage);

    if (BestMatchedCredit.TTL == 0) ///@TODO: TTL must be checked
        throw exHTTPForbidden("[82] You don't have access to: " + this->ServiceName);

    auto checkCredit = [](auto _usageIter, stuUsage _remaining, auto _type) {
        if (NULLABLE_HAS_VALUE(_remaining.PerDay) && *_remaining.PerDay - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Day:%2>").arg(_type).arg(_usageIter.key()));

        if (NULLABLE_HAS_VALUE(_remaining.PerWeek) && *_remaining.PerWeek - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Week:%2>").arg(_type).arg(_usageIter.key()));

        if (NULLABLE_HAS_VALUE(_remaining.PerMonth) && *_remaining.PerMonth - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Month:%2>").arg(_type).arg(_usageIter.key()));

        if (NULLABLE_HAS_VALUE(_remaining.Total) && *_remaining.Total - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Total:%2>").arg(_type).arg(_usageIter.key()));
    };

    const stuAssetItem& ActiveCredit = BestMatchedCredit.Credit;

    if (BestMatchedCredit.IsFromParent) {
        for (auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++
        ) {
            if (ActiveCredit.Digested.Limits.contains(UsageIter.key()) == false)
                continue;

            if (this->isUnlimited(BestMatchedCredit.MyLimitsOnParent) == false)
                checkCredit(UsageIter, BestMatchedCredit.MyLimitsOnParent.value(UsageIter.key()), "Own");

            checkCredit(UsageIter, ActiveCredit.Digested.Limits.value(UsageIter.key()), "Parent");
        }

        return;
    }

    if (this->isUnlimited(ActiveCredit.Digested.Limits))
        return;

    for (auto UsageIter = _requestedUsage.begin();
        UsageIter != _requestedUsage.end();
        UsageIter++) {
        if (ActiveCredit.Digested.Limits.contains(UsageIter.key()) == false)
            continue;

        checkCredit(UsageIter, ActiveCredit.Digested.Limits.value(UsageIter.key()), "Self");
    }
}

stuActiveCredit intfAccountingBasedModule::findBestMatchedCredit(
    quint64 _usrID,
    const ServiceUsage_t& _requestedUsage
) {
    stuServiceCreditsInfo ServiceCreditsInfo = this->retrieveServiceCreditsInfo(_usrID);
    if (ServiceCreditsInfo.ActiveCredits.isEmpty())
        return stuActiveCredit();

    QDateTime Now = ServiceCreditsInfo.DBCurrentDateTime;

    auto effectiveStartDateTime = [Now](const stuAssetItem& _assetItem) -> QDateTime {
        return _assetItem.Product.prdValidFromHour //.isNull() == fasle
            ? QDateTime(Now.date()).addSecs(*_assetItem.Product.prdValidFromHour * 3600)
            : QDateTime(Now.date());
    };

    auto effectiveEndDateTime = [Now](const stuAssetItem& _assetItem) -> QDateTime {
        if (NULLABLE_IS_NULL(_assetItem.Product.prdValidFromHour) || NULLABLE_IS_NULL(_assetItem.Product.prdValidToHour))
            QDateTime(Now.date().addDays(1));

        return _assetItem.Product.prdValidFromHour < _assetItem.Product.prdValidToHour
            ? QDateTime(Now.date()).addSecs(*_assetItem.Product.prdValidToHour * 3600)
            : _assetItem.Product.prdValidToDate == Now.date()
                ? QDateTime(Now.date().addDays(1))
                : QDateTime(Now.date().addDays(1)).addSecs(*_assetItem.Product.prdValidToHour * 3600);
    };

    auto isInvalidPackage = [this, Now, effectiveStartDateTime, effectiveEndDateTime, _requestedUsage](const stuAssetItem& _assetItem) -> bool {
        if ((_assetItem.Product.prdValidFromDate.isValid() && _assetItem.Product.prdValidFromDate > Now.date())
               || (_assetItem.Product.prdValidToDate.isValid() && _assetItem.Product.prdValidToDate < Now.date())
               || (NULLABLE_HAS_VALUE(_assetItem.Product.prdValidFromHour) && Now < effectiveStartDateTime(_assetItem))
               || (NULLABLE_HAS_VALUE(_assetItem.Product.prdValidToHour) && Now > effectiveEndDateTime(_assetItem))
               || this->isEmpty(_assetItem.Digested.Limits)
            )
            return false;

        if (_requestedUsage.size()) {
            for (auto UsageIter = _requestedUsage.begin();
                UsageIter != _requestedUsage.end();
                UsageIter++) {
                if (_assetItem.Digested.Limits.contains(UsageIter.key()) == false)
                    continue;

                if (this->isUnlimited(_assetItem.Digested.Limits) == false) {
                    stuUsage Remaining = _assetItem.Digested.Limits.value(UsageIter.key());
                    if ((NULLABLE_HAS_VALUE(Remaining.PerDay) && *Remaining.PerDay - UsageIter.value() <= 0)
                           || (NULLABLE_HAS_VALUE(Remaining.PerWeek) && *Remaining.PerWeek - UsageIter.value() <= 0)
                           || (NULLABLE_HAS_VALUE(Remaining.PerMonth) && *Remaining.PerMonth - UsageIter.value() <= 0)
                           || (NULLABLE_HAS_VALUE(Remaining.Total) && *Remaining.Total - UsageIter.value() <= 0)
                        )
                        return false;
                }
            }
        }
        return true;
    };

    if (NULLABLE_HAS_VALUE(ServiceCreditsInfo.PreferedCredit) && isInvalidPackage(*ServiceCreditsInfo.PreferedCredit) == false)
        return stuActiveCredit(*ServiceCreditsInfo.PreferedCredit,
                               NULLABLE_HAS_VALUE(ServiceCreditsInfo.ParentID),
                               ServiceCreditsInfo.MyLimitsOnParent,
                               -1);

    auto comparePackages = [this, &effectiveEndDateTime] (const stuAssetItem& a, stuAssetItem& b) {
        if (a.Product.prdValidToDate.isValid() && b.Product.prdValidToDate.isValid() == false) return -1;
        if (a.Product.prdValidToDate.isValid() == false && b.Product.prdValidToDate.isValid()) return 1;
        if (this->isUnlimited(a.Digested.Limits) && this->isUnlimited(b.Digested.Limits) == false) return -1;
        if (this->isUnlimited(a.Digested.Limits) == false && this->isUnlimited(b.Digested.Limits)) return 1;
        if (NULLABLE_HAS_VALUE(a.Product.prdValidToHour) && NULLABLE_IS_NULL(b.Product.prdValidToHour)) return -1;
        if (NULLABLE_IS_NULL(a.Product.prdValidToHour) && NULLABLE_HAS_VALUE(b.Product.prdValidToHour)) return 1;
        if (NULLABLE_HAS_VALUE(a.Product.prdValidToHour) && NULLABLE_HAS_VALUE(b.Product.prdValidToHour)) {
            if (effectiveEndDateTime(a) < effectiveEndDateTime(b)) return -1;
            if (effectiveEndDateTime(a) > effectiveEndDateTime(b)) return 1;
        }
        if (a.Product.prdValidToDate.isValid() && b.Product.prdValidToDate.isValid() && a.Product.prdValidToDate != b.Product.prdValidToDate)
            return b.Product.prdValidToDate > a.Product.prdValidToDate ? -1 : 1;
        return 0;
    };

    QList<stuAssetItem> CandidateCredit;

    for (auto AccountIter = ServiceCreditsInfo.ActiveCredits.begin();
        AccountIter != ServiceCreditsInfo.ActiveCredits.end();
        AccountIter++) {
        const stuAssetItem& Item = AccountIter.value();

        if (isInvalidPackage(Item))
            continue;

        if (CandidateCredit.isEmpty()) {
            CandidateCredit.append(Item);
            continue;
        }

        bool Inserted = false;
        for (auto CandidateIter = CandidateCredit.begin();
            CandidateIter != CandidateCredit.end();
            CandidateIter++) {
            if (comparePackages(Item, *CandidateIter) <0) {
                this->breakCredit(CandidateIter->Saleable.slbID);
                CandidateCredit.insert(CandidateIter, Item);
                Inserted = true;
                break;
            }
        }
        if (Inserted == false)
            CandidateCredit.append(Item);
    }

    if (CandidateCredit.isEmpty()) {
        if (_requestedUsage.size())
            throw exPaymentRequired("You don't have any active account");

        return stuActiveCredit();
    }

    const stuAssetItem& ActivePackage = CandidateCredit.first();
    QDateTime NextDigestTime;
    if (ActivePackage.Product.prdValidToDate.isNull() == false) {
        if (NULLABLE_HAS_VALUE(ActivePackage.Product.prdValidToHour))
            NextDigestTime = effectiveEndDateTime(ActivePackage);
        else
            NextDigestTime = QDateTime(ActivePackage.Product.prdValidToDate.addDays(1));
    } else if (NULLABLE_HAS_VALUE(ActivePackage.Product.prdValidToHour))
        NextDigestTime = effectiveEndDateTime(ActivePackage);

    return stuActiveCredit(ActivePackage,
                           NULLABLE_HAS_VALUE(ServiceCreditsInfo.ParentID),
                           ServiceCreditsInfo.MyLimitsOnParent,
                           NextDigestTime.isValid() ? (Now.msecsTo(NextDigestTime) / 1000) : -1);
}

Targoman::API::AAA::stuBasketActionResult IMPL_REST_POST(intfAccountingBasedModule, addToBasket, (
    APICALLBOOM_TYPE_JWT_IMPL               &APICALLBOOM_PARAM,
    TAPI::SaleableCode_t                    _saleableCode,
    Targoman::API::AAA::OrderAdditives_t    _orderAdditives,
    qreal                                   _qty,
    TAPI::CouponCode_t                      _discountCode,
    QString                                 _referrer,
    TAPI::JSON_t                            _referrerParams,
    Targoman::API::AAA::stuPreVoucher       _lastPreVoucher
)) {
    /**
     1: validate preVoucher and owner
     2: find duplicates
     3: fetch SLB & PRD
     4: processItemForBasket
     5: create new user asset (+ custom user asset fields)
     6: compute preVoucherItem prices and sign
     7: compute preVoucher prices and sign
    **/

    //-- validate preVoucher and owner --------------------------------
    checkPreVoucherSanity(_lastPreVoucher);

    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();

    if (_lastPreVoucher.Items.isEmpty())
        _lastPreVoucher.UserID = CurrentUserID;
    else if (_lastPreVoucher.UserID != CurrentUserID)
        throw exHTTPBadRequest("invalid pre-Voucher owner");

    if (_qty <= 0)
        throw exHTTPBadRequest("invalid qty");

    //-- find duplicates --------------------------------
    for (QList<stuVoucherItem>::iterator it = _lastPreVoucher.Items.begin();
         it != _lastPreVoucher.Items.end();
         it++
    ) {
        if (it->Service != this->ServiceName)
            continue;

        if (it->Additives != _orderAdditives)
            continue;

        if (it->Referrer != _referrer)
            continue;

        /**
          * discount code:
          * C | old | new | result
          * ---------------------------------
          * 1 |  -  |  -  |  OK (-)
          * 2 |  -  |  x  |  OK (x)
          * 3 |  x  |  -  |  OK (x)
          * 4 |  x  |  x  |  OK (x)
          * 5 |  x  |  y  | NOK (don't update. will be added as a new item in basket)
          */

        QString NewDiscountCode = _discountCode;

        //C3,C4,C5:
        if (it->CouponDiscount.ID > 0) {
            //C3:
            if (NewDiscountCode.isEmpty())
                NewDiscountCode = it->CouponDiscount.Code;
            //C5:
            else if (it->CouponDiscount.Code != NewDiscountCode)
                continue;
        }

        QVariantMap UserAssetInfo = this->AccountUserAssets->GetSelectQuery(APICALLBOOM_PARAM)
            .addCols(this->AccountUserAssets->SelectableColumnNames())
            .addCols(this->AccountSaleables->SelectableColumnNames())
            .innerJoinWith(tblAccountUserAssetsBase::Relation::Saleable)
            .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, it->OrderID })
//            .andWhere({ tblAccountUserAssetsBase::Fields::uas_slbID, enuConditionOperator::Equal, AssetItem.Saleable.slbID })
//            .andWhere({ tblAccountUserAssetsBase::Fields::uas_usrID, enuConditionOperator::Equal, CurrentUserID })
//            .andWhere({ tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, enuConditionOperator::Equal, it->UUID })
            .one();

        tblAccountUserAssetsBase::DTO AccountUserAssetsBaseDTO;
        AccountUserAssetsBaseDTO.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));

        tblAccountSaleablesBase::DTO AccountSaleablesBaseDTO;
        AccountSaleablesBaseDTO.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));

        if ((AccountSaleablesBaseDTO.slbCode != _saleableCode)
            || (AccountUserAssetsBaseDTO.uas_usrID != CurrentUserID)
            || (AccountUserAssetsBaseDTO.uasVoucherItemUUID != it->UUID)
        )
            continue;

        return this->internalUpdateBasketItem(
            APICALLBOOM_PARAM,
            _lastPreVoucher,
            *it,
            it->Qty + _qty,
            NewDiscountCode);
    } //find duplicates

    //-- fetch SLB & PRD --------------------------------
    QVariantMap SaleableInfo = this->AccountSaleables->GetSelectQuery(APICALLBOOM_PARAM)
        .addCols(this->AccountSaleables->SelectableColumnNames())
        .addCols(this->AccountProducts->SelectableColumnNames())
        .addCols(this->AssetUsageLimitsColsName)
        .addCol(DBExpression::VALUE("prdInStockQty - IFNULL(prdOrderedQty,0) + IFNULL(prdReturnedQty,0)"), "prdQtyInHand")
        .addCol(DBExpression::VALUE("slbInStockQty - IFNULL(slbOrderedQty,0) + IFNULL(slbReturnedQty,0)"), "slbQtyInHand")
        .innerJoinWith(tblAccountSaleablesBase::Relation::Product)
        .where({ tblAccountSaleablesBase::Fields::slbCode, enuConditionOperator::Equal, _saleableCode })
        .andWhere({ tblAccountSaleablesBase::Fields::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::Null })
            .orCond({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    TargomanDebug(5) << "intfAccountingBasedModule::addToBasket : SaleableInfo" << SaleableInfo;

    QJsonObject JsonSaleableInfo = QJsonObject::fromVariantMap(SaleableInfo);

    stuAssetItem AssetItem;
    AssetItem.fromJson(JsonSaleableInfo);
    AssetItem.Product.fromJson(JsonSaleableInfo);
    AssetItem.Saleable.fromJson(JsonSaleableInfo);

    //-- --------------------------------
    AssetItem.DiscountCode      = _discountCode;
    AssetItem.OrderAdditives    = _orderAdditives;
    AssetItem.Referrer          = _referrer;
    AssetItem.ReferrerParams    = _referrerParams;

    AssetItem.Qty               = _qty;
    AssetItem.UnitPrice         = AssetItem.Saleable.slbBasePrice;
    AssetItem.Discount          = 0;

    //-- --------------------------------
    UsageLimits_t SaleableUsageLimits;
    for (auto Iter = this->AssetUsageLimitsCols.begin();
        Iter != this->AssetUsageLimitsCols.end();
        Iter++
    ) {
        SaleableUsageLimits.insert(Iter.key(), {
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerDay)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerWeek)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerMonth)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, SaleableInfo.value(Iter->Total))
        });
    }
    AssetItem.Digested.Limits = SaleableUsageLimits;

    //-- --------------------------------
    this->processItemForBasket(APICALLBOOM_PARAM, AssetItem);

    //-- --------------------------------
    stuVoucherItem PreVoucherItem;

    QJsonDocument JSDPendingVouchers = QJsonDocument();
    JSDPendingVouchers.setObject(AssetItem.Private.toJson());
    PreVoucherItem.Private = simpleCryptInstance()->encryptToString(JSDPendingVouchers.toJson(QJsonDocument::Compact));

    PreVoucherItem.Service = this->ServiceName;
    //PreVoucherItem.OrderID
    PreVoucherItem.UUID = SecurityHelper::UUIDtoMD5();
    PreVoucherItem.Desc = AssetItem.Saleable.slbName;
    PreVoucherItem.Qty = AssetItem.Qty; //_qty;
    PreVoucherItem.UnitPrice = AssetItem.UnitPrice;
    PreVoucherItem.SubTotal = AssetItem.SubTotal;

    //store multiple discounts (system (multi) + coupon (one))
    PreVoucherItem.SystemDiscounts = AssetItem.SystemDiscounts;
    PreVoucherItem.CouponDiscount = AssetItem.CouponDiscount;

    PreVoucherItem.DisAmount = AssetItem.Discount;
    PreVoucherItem.AfterDiscount = AssetItem.AfterDiscount;

    PreVoucherItem.VATPercent = AssetItem.VATPercent;
    PreVoucherItem.VATAmount = AssetItem.VAT;

    PreVoucherItem.TotalPrice = AssetItem.TotalPrice;

    PreVoucherItem.Additives = AssetItem.OrderAdditives;
    PreVoucherItem.Referrer = AssetItem.Referrer;
    PreVoucherItem.ReferrerParams = AssetItem.ReferrerParams;

    ORMCreateQuery qry = this->AccountUserAssets->GetCreateQuery(APICALLBOOM_PARAM)
        .addCols({
                     tblAccountUserAssetsBase::Fields::uas_usrID,
                     tblAccountUserAssetsBase::Fields::uas_slbID,
                     tblAccountUserAssetsBase::Fields::uasQty,
//                     tblAccountUserAssetsBase::Fields::uas_vchID,
                     tblAccountUserAssetsBase::Fields::uasVoucherItemUUID,
                     tblAccountUserAssetsBase::Fields::uasVoucherItemInfo,
//                     tblAccountUserAssetsBase::Fields::uasPrefered,
//                     tblAccountUserAssetsBase::Fields::uasOrderDateTime,
//                     tblAccountUserAssetsBase::Fields::uasStatus,
                 })
    ;

    QVariantMap values;
    values = {
        { tblAccountUserAssetsBase::Fields::uas_usrID, CurrentUserID },
        { tblAccountUserAssetsBase::Fields::uas_slbID, AssetItem.Saleable.slbID },
        { tblAccountUserAssetsBase::Fields::uasQty, _qty },
//        { tblAccountUserAssetsBase::Fields::uas_vchID, ??? },
        { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, PreVoucherItem.UUID },
        { tblAccountUserAssetsBase::Fields::uasVoucherItemInfo, PreVoucherItem.toJson().toVariantMap() },
//            { tblAccountUserAssetsBase::Fields::uasPrefered, ??? },
//        { tblAccountUserAssetsBase::Fields::uasOrderDateTime, DBExpression::NOW() },
//            { tblAccountUserAssetsBase::Fields::uasStatus, },
    };

    //-- discount
    if (AssetItem.CouponDiscount.ID > 0) {
        qry.addCols({
                        tblAccountUserAssetsBase::Fields::uas_cpnID,
                        tblAccountUserAssetsBase::Fields::uasDiscountAmount,
                    })
        ;

        values.insert(tblAccountUserAssetsBase::Fields::uas_cpnID, AssetItem.CouponDiscount.ID);
        values.insert(tblAccountUserAssetsBase::Fields::uasDiscountAmount, AssetItem.Discount); //CouponDiscount.Amount);
    }

    //-- CustomUserAssetFields
    QVariantMap CustomFields = this->getCustomUserAssetFieldsForQuery(APICALLBOOM_PARAM, AssetItem);
    for (QVariantMap::const_iterator it = CustomFields.constBegin();
         it != CustomFields.constEnd();
         it++
    ) {
        qry.addCol(it.key());
        values.insert(it.key(), *it);
    }

    //--
    qry.values(values);

    //-- --------------------------------
    PreVoucherItem.OrderID = qry.execute(CurrentUserID);

    //-- --------------------------------
    PreVoucherItem.Sign = QString(sign(PreVoucherItem));

    //-- --------------------------------
    ///@TODO: PreVoucherItem.DMInfo : json {"type":"adver", "additives":[{"color":"red"}, {"size":"m"}, ...]}
    /// used for DMLogic::applyCoupon -> match item.DMInfo by coupon rules
    /// return: amount of using coupon

    //-- add to the last pre voucher --------------------------------
    _lastPreVoucher.Items.append(PreVoucherItem);
    _lastPreVoucher.Summary = _lastPreVoucher.Items.size() > 1 ?
                                  QString("%1 items").arg(_lastPreVoucher.Items.size()) :
                                  QString("%1 of %2").arg(PreVoucherItem.Qty).arg(PreVoucherItem.Desc);

    qint64 FinalPrice = _lastPreVoucher.Round
                        + _lastPreVoucher.ToPay
                        + PreVoucherItem.TotalPrice;

    if (FinalPrice < 0) {
        this->AccountUserAssets->DeleteByPks(
            APICALLBOOM_PARAM,
            /*PK*/ QString::number(PreVoucherItem.OrderID),
            {
                //this is just for make condition safe and strong:
                { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, PreVoucherItem.UUID },
            },
            false
        );

        this->AccountSaleables->callSP(APICALLBOOM_PARAM,
                                       "spSaleable_unReserve", {
                                           { "iSaleableID", AssetItem.Saleable.slbID },
                                           { "iUserID", CurrentUserID },
                                           { "iQty", PreVoucherItem.Qty },
                                       });

        throw exHTTPInternalServerError("Final amount computed negative!");
    }

    _lastPreVoucher.Round = static_cast<quint16>(FinalPrice % 1000);
    _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
//    _lastPreVoucher.Type = enuPreVoucherType::Invoice;
    _lastPreVoucher.Sign.clear();
    _lastPreVoucher.Sign = QString(sign(_lastPreVoucher));

    return stuBasketActionResult(
                _lastPreVoucher,
                PreVoucherItem.UUID);
}

Targoman::API::AAA::stuBasketActionResult IMPL_REST_POST(intfAccountingBasedModule, updateBasketItem, (
    APICALLBOOM_TYPE_JWT_IMPL           &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuPreVoucher   _lastPreVoucher,
    TAPI::MD5_t                         _itemUUID,
    qreal                               _newQty,
    NULLABLE_TYPE(TAPI::CouponCode_t)   _newDiscountCode
)) {
    if (_newQty < 0)
        throw exHTTPBadRequest("invalid qty");

    for (QList<stuVoucherItem>::iterator it = _lastPreVoucher.Items.begin();
         it != _lastPreVoucher.Items.end();
         it++
    ) {
        if (it->UUID == _itemUUID) {
            return internalUpdateBasketItem(
                APICALLBOOM_PARAM,
                _lastPreVoucher,
                *it,
                _newQty,
                _newDiscountCode
            );
        }
    }

    throw exHTTPNotFound("item not found");
}

Targoman::API::AAA::stuBasketActionResult IMPL_REST_POST(intfAccountingBasedModule, removeBasketItem, (
    APICALLBOOM_TYPE_JWT_IMPL           &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuPreVoucher   _lastPreVoucher,
    TAPI::MD5_t                         _itemUUID
)) {
    return this->apiPOSTupdateBasketItem(
        APICALLBOOM_PARAM,
        _lastPreVoucher,
        _itemUUID,
        0
    );
}

/**
  * called by:
  *     addToBasket
  *     updateBasketItem
  *     removeBasketItem
  */
Targoman::API::AAA::stuBasketActionResult intfAccountingBasedModule::internalUpdateBasketItem(
    INTFAPICALLBOOM_IMPL                &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuPreVoucher   &_lastPreVoucher,
    stuVoucherItem                      &_voucherItem,
    qreal                               _newQty,
    NULLABLE_TYPE(TAPI::CouponCode_t)   _newDiscountCode
) {
    /**
      1: check prev and new coupon code


        check available instock (minus _voucherItem.qty)




    **/

    //no change?
    if ((_newQty == _voucherItem.Qty)
            && (NULLABLE_IS_NULL(_newDiscountCode)
                || (NULLABLE_VALUE(_newDiscountCode) == _voucherItem.CouponDiscount.Code)
            )
        )
        return stuBasketActionResult(
                    _lastPreVoucher,
                    _voucherItem.UUID);

    //-- validate preVoucher and owner --------------------------------
    checkPreVoucherSanity(_lastPreVoucher);

    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();

    if (_lastPreVoucher.Items.isEmpty())
        throw exHTTPBadRequest("Pre-Voucher is empty");

    if (_lastPreVoucher.UserID != CurrentUserID)
        throw exHTTPBadRequest("invalid pre-Voucher owner");

    //--  --------------------------------
    bool Found = false;
    int VoucherItemIndex;
    for (VoucherItemIndex = 0; VoucherItemIndex < _lastPreVoucher.Items.length(); ++VoucherItemIndex) {
        if (_lastPreVoucher.Items.at(VoucherItemIndex).UUID == _voucherItem.UUID) {
            Found = true;
            break;
        }
    }
    if (Found == false)
        throw exHTTPBadRequest("Item not found in pre-Voucher");

    //-- fetch SLB & PRD --------------------------------
    QVariantMap UserAssetInfo = this->AccountSaleables->GetSelectQuery(APICALLBOOM_PARAM)
        .addCols(this->AccountSaleables->SelectableColumnNames())
        .addCols(this->AccountProducts->SelectableColumnNames())
        .addCols(this->AssetUsageLimitsColsName)
        .addCol(DBExpression::VALUE("prdInStockQty - IFNULL(prdOrderedQty,0) + IFNULL(prdReturnedQty,0)"), "prdQtyInHand")
        .addCol(DBExpression::VALUE("slbInStockQty - IFNULL(slbOrderedQty,0) + IFNULL(slbReturnedQty,0)"), "slbQtyInHand")
        .innerJoinWith(tblAccountSaleablesBase::Relation::Product)

        .innerJoinWith(tblAccountSaleablesBase::Relation::UserAsset)
        .addCols(this->AccountUserAssets->SelectableColumnNames())
        .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })

        .andWhere({ tblAccountSaleablesBase::Fields::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::Null })
            .orCond({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    TargomanDebug(5) << "intfAccountingBasedModule::internalUpdateBasketItem : UserAssetInfo" << UserAssetInfo;

    tblAccountUserAssetsBase::DTO AccountUserAssetsBaseDTO;
    AccountUserAssetsBaseDTO.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));

    stuAssetItem AssetItem;
    AssetItem.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));
    AssetItem.Product.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));
    AssetItem.Saleable.fromJson(QJsonObject::fromVariantMap(UserAssetInfo));

    //--  --------------------------------
    QString StrPrivate = simpleCryptInstance()->decryptToString(_voucherItem.Private);
    AssetItem.Private.fromJson(QJsonDocument().fromJson(StrPrivate.toLatin1()).object());

    //--  --------------------------------
    AssetItem.DiscountCode      = NULLABLE_GET_OR_DEFAULT(_newDiscountCode, _voucherItem.CouponDiscount.Code);
    AssetItem.OrderAdditives    = _voucherItem.Additives;
    AssetItem.Referrer          = _voucherItem.Referrer;
    AssetItem.ReferrerParams    = _voucherItem.ReferrerParams;

    AssetItem.Qty               = _newQty;
    AssetItem.UnitPrice         = AssetItem.Saleable.slbBasePrice;
    AssetItem.Discount          = _voucherItem.DisAmount;

    //-- --------------------------------
    UsageLimits_t SaleableUsageLimits;
    for (auto Iter = this->AssetUsageLimitsCols.begin();
        Iter != this->AssetUsageLimitsCols.end();
        Iter++
    ) {
        SaleableUsageLimits.insert(Iter.key(), {
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerDay)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerWeek)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, UserAssetInfo.value(Iter->PerMonth)),
            NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, UserAssetInfo.value(Iter->Total))
        });
    }
    AssetItem.Digested.Limits = SaleableUsageLimits;

    //-- --------------------------------
    this->processItemForBasket(APICALLBOOM_PARAM, AssetItem, &_voucherItem);

    //-- --------------------------------
    qint64 FinalPrice = _lastPreVoucher.ToPay + _lastPreVoucher.Round;
    FinalPrice -= _voucherItem.TotalPrice;
    if (_newQty == 0) { //remove
        this->AccountUserAssets->DeleteByPks(
            APICALLBOOM_PARAM,
            /*PK*/ QString::number(_voucherItem.OrderID),
            {
                //this is just for make condition safe and strong:
                { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, _voucherItem.UUID },
            },
            false
        );

        //moved to processItemForBasket
//        this->AccountSaleables->callSP(APICALLBOOM_PARAM,
//                                       "spSaleable_unReserve", {
//                                           { "iSaleableID", AssetItem.Saleable.slbID },
//                                           { "iUserID", CurrentUserID },
//                                           { "iQty", _voucherItem.Qty },
//                                       });

        _lastPreVoucher.Items.removeAt(VoucherItemIndex);
    } else { //update
        FinalPrice += AssetItem.TotalPrice;

        QJsonDocument JSDPendingVouchers = QJsonDocument();
        JSDPendingVouchers.setObject(AssetItem.Private.toJson());
        _voucherItem.Private = simpleCryptInstance()->encryptToString(JSDPendingVouchers.toJson(QJsonDocument::Compact));

        _voucherItem.Service = this->ServiceName;
//        _voucherItem.UUID = _voucherItem.UUID;
        _voucherItem.Desc = AssetItem.Saleable.slbName;
        _voucherItem.Qty = AssetItem.Qty;
        _voucherItem.UnitPrice = AssetItem.UnitPrice;
        _voucherItem.SubTotal = AssetItem.SubTotal;

        //store multiple discounts (system (multi) + coupon (one))
        _voucherItem.SystemDiscounts = AssetItem.SystemDiscounts;
        _voucherItem.CouponDiscount = AssetItem.CouponDiscount;

        _voucherItem.DisAmount = AssetItem.Discount;
        _voucherItem.AfterDiscount = AssetItem.AfterDiscount;

        _voucherItem.VATPercent = AssetItem.VATPercent;
        _voucherItem.VATAmount = AssetItem.VAT;

        _voucherItem.TotalPrice = AssetItem.TotalPrice;

        _voucherItem.Additives = AssetItem.OrderAdditives;
        _voucherItem.Referrer = AssetItem.Referrer;
        _voucherItem.ReferrerParams = AssetItem.ReferrerParams;

        ORMUpdateQuery qry = this->AccountUserAssets->GetUpdateQuery(APICALLBOOM_PARAM)
                          .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })
                          .set(tblAccountUserAssetsBase::Fields::uasVoucherItemInfo, _voucherItem.toJson().toVariantMap())
                          .set(tblAccountUserAssetsBase::Fields::uasQty, _newQty)
                          .set(tblAccountUserAssetsBase::Fields::uasDiscountAmount, AssetItem.Discount)
                          ;

        //-- discount
        if (AssetItem.CouponDiscount.ID > 0)
            qry.set(tblAccountUserAssetsBase::Fields::uas_cpnID, AssetItem.CouponDiscount.ID);
        else
            qry.setNull(tblAccountUserAssetsBase::Fields::uas_cpnID);

        //-- CustomUserAssetFields
        QVariantMap CustomFields = this->getCustomUserAssetFieldsForQuery(APICALLBOOM_PARAM, AssetItem, &_voucherItem);
        for (QVariantMap::const_iterator it = CustomFields.constBegin();
             it != CustomFields.constEnd();
             it++
        ) {
            qry.set(it.key(), *it);
        }

        //-- --------------------------------
        qry.execute(CurrentUserID);

        //-- --------------------------------
        _voucherItem.Sign.clear();
        _voucherItem.Sign = QString(sign(_voucherItem));

        _lastPreVoucher.Items.replace(VoucherItemIndex, _voucherItem);
    }

    if (_lastPreVoucher.Items.isEmpty()) {
        _lastPreVoucher.Summary = "";
    } else if (_lastPreVoucher.Items.size() > 1)
        _lastPreVoucher.Summary = QString("%1 items").arg(_lastPreVoucher.Items.size());
    else {
        auto item = _lastPreVoucher.Items.first();
        _lastPreVoucher.Summary = QString("%1 of %2").arg(item.Qty).arg(item.Desc);
    }

    _lastPreVoucher.Round = static_cast<quint16>(FinalPrice % 1000);
    _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
//    _lastPreVoucher.Type = enuPreVoucherType::Invoice;
    _lastPreVoucher.Sign.clear();
    _lastPreVoucher.Sign = QString(sign(_lastPreVoucher));

    return stuBasketActionResult(
                _lastPreVoucher,
                _voucherItem.UUID);
}

/**
  * called by:
  *     internalUpdateBasketItem:
  *        addToBasket
  *        updateBasketItem
  *        removeBasketItem
  */
void intfAccountingBasedModule::processItemForBasket(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    /**
     1: check available SLB & PRD in stock qty

     check buy 2, take 3

     2: apply additives and compute unit price
     3: apply referrer -> prize
     4: apply system discount
     5: apply coupon discount
     6: digest privs
     7: reserve SLB
    **/

    //-- --------------------------------
    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();

    //-- --------------------------------
    if ((_oldVoucherItem == nullptr) && (_assetItem.Qty == 0))
        throw exHTTPBadRequest("qty is zero and old item not specified.");

    qreal DeltaQty = _assetItem.Qty;
    if (_oldVoucherItem != nullptr)
        DeltaQty -= _oldVoucherItem->Qty;

    //-- check available count --------------------------------
    if (DeltaQty > 0) {
        if ((_assetItem.slbQtyInHand < 0) || (_assetItem.prdQtyInHand < 0))
            throw exHTTPInternalServerError(QString("Available Saleable Qty(%1) or Available Product Qty(%2) < 0")
                                            .arg(_assetItem.slbQtyInHand).arg(_assetItem.prdQtyInHand));

        if (_assetItem.slbQtyInHand > _assetItem.prdQtyInHand)
            throw exHTTPInternalServerError(QString("Available Saleable Qty(%1) > Available Product Qty(%2)")
                                            .arg(_assetItem.slbQtyInHand).arg(_assetItem.prdQtyInHand));

        if (_assetItem.slbQtyInHand < DeltaQty)
            throw exHTTPBadRequest(QString("Not enough %1 available in store. Available Qty(%2) Requested Qty(%3)")
                                   .arg(_assetItem.Saleable.slbCode).arg(_assetItem.slbQtyInHand).arg(DeltaQty));
    }

    //-- --------------------------------
    auto fnComputeTotalPrice = [&_assetItem](const QString &_label) {
        _assetItem.SubTotal = _assetItem.UnitPrice * _assetItem.Qty;
        _assetItem.AfterDiscount = _assetItem.SubTotal - _assetItem.Discount;

        _assetItem.VATPercent = NULLABLE_GET_OR_DEFAULT(_assetItem.Product.prdVAT, 0); // * 100;
        _assetItem.VAT = _assetItem.AfterDiscount * _assetItem.VATPercent / 100.0;

        _assetItem.TotalPrice = _assetItem.AfterDiscount - _assetItem.VAT;

        TargomanDebug(5) << "[" << _label << "]:" << endl
                         << "    Qty:           (" << _assetItem.Qty << ")" << endl
                         << "    UnitPrice:     (" << _assetItem.UnitPrice << ")" << endl
                         << "    SubTotal:      (" << _assetItem.SubTotal << ")" << endl
                         << "    Discount:      (" << _assetItem.Discount << ")" << endl
                         << "    AfterDiscount: (" << _assetItem.AfterDiscount << ")" << endl
                         << "    VAT:           (" << _assetItem.VAT << ")" << endl
                         << "    TotalPrice:    (" << _assetItem.TotalPrice << ")" << endl
                        ;
    };

    //-- --------------------------------
    fnComputeTotalPrice("start");

    //-- check buy 2, take 3 --------------------------------
    /**
     * example:
     *      2 of slb A -> 1 will be added as prize (sub item with 100% discount)
     */
    ///@TODO: 2 ta bekhar 3 ta bebar:

    //-- --------------------------------
    this->computeAdditives(APICALLBOOM_PARAM, _assetItem, _oldVoucherItem);
    fnComputeTotalPrice("after computeAdditives");

    //-- --------------------------------
    this->computeReferrer(APICALLBOOM_PARAM, _assetItem, _oldVoucherItem);
    fnComputeTotalPrice("after computeReferrer");

    //-- --------------------------------
//    this->parsePrize(...); -> AssetItem.PendingVouchers

    //-- discount --------------------------------
    ///@TODO: what if some one uses discount code and at the same time will pay by prize credit

    this->computeSystemDiscount(APICALLBOOM_PARAM, _assetItem, {}, _oldVoucherItem);
    fnComputeTotalPrice("after computeSystemDiscount");

    this->computeCouponDiscount(APICALLBOOM_PARAM, _assetItem, _oldVoucherItem);
    fnComputeTotalPrice("after applyCouponBasedDiscount");

    //-- --------------------------------
    this->digestPrivs(APICALLBOOM_PARAM, _assetItem, _oldVoucherItem);

    //-- reserve and un-reserve saleable and product ------------------------------------
    ///@TODO: call spSaleable_unReserve by cron

    if (DeltaQty > 0) {
        this->AccountSaleables->callSP(APICALLBOOM_PARAM,
                                       "spSaleable_Reserve", {
                                           { "iSaleableID", _assetItem.Saleable.slbID },
                                           { "iUserID", CurrentUserID },
                                           { "iQty", DeltaQty },
                                       });
    } else if (DeltaQty < 0) {
        this->AccountSaleables->callSP(APICALLBOOM_PARAM,
                                       "spSaleable_unReserve", {
                                           { "iSaleableID", _assetItem.Saleable.slbID },
                                           { "iUserID", CurrentUserID },
                                           { "iQty", abs(DeltaQty) },
                                       });
    }

    //-- new pre voucher item --------------------------------
    ///@TODO: add ttl for order item

    fnComputeTotalPrice("finish");
}

void intfAccountingBasedModule::digestPrivs(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    ///@TODO: What should I do here?
}

void intfAccountingBasedModule::computeSystemDiscount(
    INTFAPICALLBOOM_IMPL            &APICALLBOOM_PARAM,
    INOUT stuAssetItem              &_assetItem,
    const stuPendingSystemDiscount  &_pendingSystemDiscount /*= {}*/,
    const stuVoucherItem            *_oldVoucherItem /*= nullptr*/
) {
    if (_pendingSystemDiscount.Amount > 0) {
        if (_pendingSystemDiscount.Key.isEmpty())
            throw exHTTPBadRequest("Pending System Discount Key is empty.");

        //revert same key system discount
        if ((_oldVoucherItem != nullptr)
            && _oldVoucherItem->SystemDiscounts.contains(_pendingSystemDiscount.Key)
        ) {
            stuSystemDiscount OldSystemDiscount = _oldVoucherItem->SystemDiscounts[_pendingSystemDiscount.Key];
            _assetItem.Discount -= OldSystemDiscount.Amount;

            if (_assetItem.Qty == 0) {
                return;
            }
        };

        stuSystemDiscount SystemDiscount;

        SystemDiscount.Info.insert("desc", _pendingSystemDiscount.Desc);

        if (_pendingSystemDiscount.AmountType == enuDiscountType::Percent) {
            SystemDiscount.Info.insert("amount", QString::number(_pendingSystemDiscount.Amount) + "%");

            SystemDiscount.Amount = _assetItem.SubTotal * _pendingSystemDiscount.Amount / 100.0;

            //Amount is %, Max is $
            if (_pendingSystemDiscount.Max > 0)
                SystemDiscount.Amount = qMin(SystemDiscount.Amount, _pendingSystemDiscount.Max);

        } else {
            SystemDiscount.Info.insert("amount", _pendingSystemDiscount.Amount);

            SystemDiscount.Amount = _pendingSystemDiscount.Amount;

            //Amount is $, Max is %
            if (_pendingSystemDiscount.Max > 0) {
                qreal Max = _assetItem.SubTotal * _pendingSystemDiscount.Max / 100.0;
                SystemDiscount.Amount = qMin(SystemDiscount.Amount, Max);
            }
        }

        if (SystemDiscount.Amount != _pendingSystemDiscount.Amount)
            SystemDiscount.Info.insert("applied-amount", SystemDiscount.Amount);

        //
        _assetItem.SystemDiscounts[_pendingSystemDiscount.Key] = SystemDiscount;
//            _assetItem.SystemDiscounts.insert(_pendingSystemDiscount.Key, SystemDiscount);
        _assetItem.Discount += SystemDiscount.Amount;

        return;
    }

    ///@TODO: tblAccountSystemDiscounts and all its behaviors must be implemented
}

void intfAccountingBasedModule::computeCouponDiscount(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &_assetItem,
    const stuVoucherItem    *_oldVoucherItem /*= nullptr*/
) {
    quint64 CurrentUserID = APICALLBOOM_PARAM.getUserID();

    /**
      * discount code:
      *  C  | old | new | qty | result
      * ------------------------------------------
    = * 1   |  -  |  -  |     | nothing
    = * 2   |  -  |  x  |     | compute (x)
    = * 3   |  x  |  -  |     | remove (x)
    = * 4.1 |  x  |  x  | ==  | nothing
    = * 4.2 |  x  |  x  | !=  | re-compute (x)
    = * 5   |  x  |  y  |     | remove (x) + compute (y)
      */

    _assetItem.DiscountCode = _assetItem.DiscountCode.trimmed();

    //C1:
    if (((_oldVoucherItem == nullptr) || (_oldVoucherItem->CouponDiscount.Code.isEmpty()))
            && _assetItem.DiscountCode.isEmpty())
        return;

    //C4.1: qty not changed
    if ((_oldVoucherItem != nullptr)
            && (_oldVoucherItem->CouponDiscount.Code.isEmpty() == false)
            && (_assetItem.DiscountCode.isEmpty() == false)
            && (_assetItem.DiscountCode == _oldVoucherItem->CouponDiscount.Code)
            && (_assetItem.Qty == _oldVoucherItem->Qty))
        return;

    //C3, 4.2, 5: remove
    if ((_oldVoucherItem != nullptr)
            && (_oldVoucherItem->CouponDiscount.Code.isEmpty() == false)
//            && (_assetItem.DiscountCode != _oldVoucherItem->CouponDiscount.Code)
    ) {
        _assetItem.Discount -= _oldVoucherItem->CouponDiscount.Amount;
        _assetItem.CouponDiscount = {};

        //C3:
        if (_assetItem.DiscountCode.isEmpty())
            return;
    }

    //C2, 4.2, 5:
    if (_assetItem.Qty == 0)
        return;

    clsCondition OmmitOldCondition;

    if ((_oldVoucherItem != nullptr)
            && (_oldVoucherItem->CouponDiscount.Code.isEmpty() == false)
    ) {
        OmmitOldCondition.setCond({ tblAccountUserAssetsBase::Fields::uasID,
                                    enuConditionOperator::NotEqual,
                                    _oldVoucherItem->OrderID });
    }

    QVariantMap DiscountInfo = this->AccountCoupons->GetSelectQuery(APICALLBOOM_PARAM)
        .addCols(this->AccountCoupons->SelectableColumnNames())

        .nestedLeftJoin(this->AccountUserAssets->GetSelectQuery(APICALLBOOM_PARAM, "", false)
                  .addCols({
                               tblAccountUserAssetsBase::Fields::uas_cpnID,
                               tblAccountUserAssetsBase::Fields::uas_vchID,
                           })
                  .addCol(enuAggregation::COUNT, tblAccountUserAssetsBase::Fields::uasID, "_discountUsedCount")
                  .where({ tblAccountUserAssetsBase::Fields::uas_usrID, enuConditionOperator::Equal, CurrentUserID })
                  .andWhere({ tblAccountUserAssetsBase::Fields::uasStatus, enuConditionOperator::In, QString("'%1','%2'")
                              .arg(QChar(enuAuditableStatus::Active)).arg(QChar(enuAuditableStatus::Banned)) })
                  .andWhere(OmmitOldCondition)
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_cpnID)
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_vchID)
            , "tmp_cpn_count"
            , { "tmp_cpn_count", tblAccountUserAssetsBase::Fields::uas_cpnID,
                enuConditionOperator::Equal,
                tblAccountCouponsBase::Name, tblAccountCouponsBase::Fields::cpnID }
        )
        .addCol("tmp_cpn_count._discountUsedCount")

        .nestedLeftJoin(this->AccountUserAssets->GetSelectQuery(APICALLBOOM_PARAM, "", false)
                  .addCol(tblAccountUserAssetsBase::Fields::uas_cpnID)
                  .addCol(enuAggregation::SUM, tblAccountUserAssetsBase::Fields::uasDiscountAmount, "_discountUsedAmount")
                  .where({ tblAccountUserAssetsBase::Fields::uas_usrID, enuConditionOperator::Equal, CurrentUserID })
                  .andWhere({ tblAccountUserAssetsBase::Fields::uasStatus, enuConditionOperator::In, QString("'%1','%2'")
                              .arg(QChar(enuAuditableStatus::Active)).arg(QChar(enuAuditableStatus::Banned)) })
                  .andWhere(OmmitOldCondition)
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_cpnID)
            , "tmp_cpn_amount"
            , { "tmp_cpn_amount", tblAccountUserAssetsBase::Fields::uas_cpnID,
                enuConditionOperator::Equal,
                tblAccountCouponsBase::Name, tblAccountCouponsBase::Fields::cpnID }
        )
        .addCol("tmp_cpn_amount._discountUsedAmount")

        .where({ tblAccountCouponsBase::Fields::cpnCode, enuConditionOperator::Equal, _assetItem.DiscountCode })
        .andWhere({ tblAccountCouponsBase::Fields::cpnValidFrom, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountCouponsBase::Fields::cpnExpiryTime, enuConditionOperator::Null })
            .orCond({ tblAccountCouponsBase::Fields::cpnExpiryTime, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    if (DiscountInfo.size() == 0)
        throw exHTTPBadRequest("Discount code not found.");

    tblAccountCouponsBase::DTO DiscountDTO;
    DiscountDTO.fromJson(QJsonObject::fromVariantMap(DiscountInfo));

    QDateTime Now = DiscountInfo.value(Targoman::API::CURRENT_TIMESTAMP).toDateTime();

    stuCouponDiscount Discount;
    Discount.ID     = DiscountDTO.cpnID;
    Discount.Code   = DiscountDTO.cpnCode;
    Discount.Amount = DiscountDTO.cpnAmount;

    NULLABLE_TYPE(quint32) _discountUsedCount;
    TAPI::setFromVariant(_discountUsedCount, DiscountInfo.value("_discountUsedCount"));
    NULLABLE_TYPE(quint32) _discountUsedAmount;
    TAPI::setFromVariant(_discountUsedAmount, DiscountInfo.value("_discountUsedAmount"));

//        if (NULLABLE_HAS_VALUE(cpnExpiryTime) && NULLABLE_GET(cpnExpiryTime).toDateTime() < Now)
//            throw exHTTPBadRequest("Discount code has been expired");

    if (DiscountDTO.cpnTotalUsedCount >= DiscountDTO.cpnPrimaryCount)
        throw exHTTPBadRequest("Discount code has been finished");

    if ((NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxCount, 0) > 0)
            && (NULLABLE_GET_OR_DEFAULT(_discountUsedCount, 0) >= NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxCount, 0)))
        throw exHTTPBadRequest("Max discount usage per user has been reached");

    if (DiscountDTO.cpnTotalUsedAmount >= DiscountDTO.cpnTotalMaxAmount)
        throw exHTTPBadRequest("Max discount usage amount has been reached");

    if ((NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxAmount, 0) > 0)
            && (NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0) >= NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxAmount, 0)))
        throw exHTTPBadRequest("Max discount usage amount per user has been reached");

    //-- SaleableBasedMultiplier ---------------------------
    QJsonArray arr = DiscountDTO.cpnSaleableBasedMultiplier.array();
    if (arr.size()) {
        stuDiscountSaleableBasedMultiplier multiplier;

        for (QJsonArray::const_iterator itr = arr.constBegin();
            itr != arr.constEnd();
            itr++
        ) {
            auto elm = *itr;

            stuDiscountSaleableBasedMultiplier cur;
            cur.fromJson(elm.toObject());

            qreal MinQty = NULLABLE_GET_OR_DEFAULT(cur.MinQty, -1);

            if ((cur.SaleableCode == _assetItem.Saleable.slbCode)
                    && (NULLABLE_GET_OR_DEFAULT(cur.MinQty, 0) <= _assetItem.Qty)
            ) {
                if ((multiplier.Multiplier == 0)
                        || (NULLABLE_GET_OR_DEFAULT(multiplier.MinQty, 0) < MinQty))
                    multiplier = cur;
            }
        }

//            if (multiplier.Multiplier == 0) //not found
//                throw exHTTPBadRequest("Discount code is not valid on selected package");

        if (multiplier.Multiplier > 0) { //found
            auto m = Discount.Amount;
            Discount.Amount = Discount.Amount * multiplier.Multiplier;

            TargomanDebug(5) << "Discount Before Multiply(" << m << ")" << "multiplier (" << multiplier.Multiplier << ")" << "Discount After Multiply(" << Discount.Amount << ")";
        }
    } //if (arr.size())

//        Discount.Code = _discountCode;

    TargomanDebug(5) << "1 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

    if (DiscountDTO.cpnAmountType == enuDiscountType::Percent)
        Discount.Amount = _assetItem.SubTotal * Discount.Amount / 100.0;

    TargomanDebug(5) << "2 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

    //check cpnMaxAmount
    if (NULLABLE_HAS_VALUE(DiscountDTO.cpnMaxAmount)) {
        //note: cpnMaxAmount type is opposite to cpnAmountType
        if (DiscountDTO.cpnAmountType == enuDiscountType::Percent)
            Discount.Amount = fmin(Discount.Amount, NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnMaxAmount, 0));
        else {
            quint32 _max = /*ceil*/(_assetItem.SubTotal * NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnMaxAmount, 0) / 100.0);
            Discount.Amount = fmin(Discount.Amount, _max);
        }
        TargomanDebug(5) << "3 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
    }

    //check total - used amount
    qint32 remainDiscountAmount = DiscountDTO.cpnTotalMaxAmount - DiscountDTO.cpnTotalUsedAmount;
    if (remainDiscountAmount < Discount.Amount) {
        Discount.Amount = remainDiscountAmount;
        TargomanDebug(5) << "4 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
    }

    //check per user - used amount
    if (NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxAmount, 0) > 0) {
        remainDiscountAmount = NULLABLE_GET_OR_DEFAULT(DiscountDTO.cpnPerUserMaxAmount, 0) - NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0);
        if (remainDiscountAmount <= 0)
            Discount.Amount = 0;
        else if (remainDiscountAmount < Discount.Amount)
            Discount.Amount = remainDiscountAmount;
        TargomanDebug(5) << "5 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
    }

    //----------
//    Discount.Amount = ceil(Discount.Amount);
    TargomanDebug(5) << "Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

    if (Discount.Amount > 0) {
        _assetItem.CouponDiscount = Discount;
        _assetItem.Discount += Discount.Amount;

        //@kambizzandi: Increase coupon statistics were moved to finalizeBasket,
        // because the customer may be angry about not being able to use the coupon again in same voucher
    }
}

/******************************************************************\
|** process and cancel voucher item *******************************|
\******************************************************************/
bool intfAccountingBasedModule::increaseDiscountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem
) {
    if (_voucherItem.SystemDiscounts.count()) {
        foreach (auto SystemDiscount, _voucherItem.SystemDiscounts) {
            ///@TODO: inc SystemDiscount usage
        }
    }

    if (_voucherItem.CouponDiscount.ID > 0) {
        clsDACResult Result = this->AccountCoupons->callSP(APICALLBOOM_PARAM,
                                                           "spCoupon_IncreaseStats", {
                                                               { "iDiscountID", _voucherItem.CouponDiscount.ID },
                                                               { "iTotalUsedCount", 1 },
                                                               { "iTotalUsedAmount", _voucherItem.CouponDiscount.Amount },
                                                           });
    }

    return true;
}

bool intfAccountingBasedModule::decreaseDiscountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem
) {
    if (_voucherItem.SystemDiscounts.count()) {
        foreach (auto SystemDiscount, _voucherItem.SystemDiscounts) {
            ///@TODO: dec SystemDiscount usage
        }
    }

    if (_voucherItem.CouponDiscount.ID > 0) {
        clsDACResult Result = this->AccountCoupons->callSP(APICALLBOOM_PARAM,
                                                           "spCoupon_DecreaseStats", {
                                                               { "iDiscountID", _voucherItem.CouponDiscount.ID },
                                                               { "iTotalUsedCount", 1 },
                                                               { "iTotalUsedAmount", _voucherItem.CouponDiscount.Amount },
                                                           });
    }

    return true;
}

bool intfAccountingBasedModule::activateUserAsset(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem,
    quint64 _voucherID
) {
    return this->AccountUserAssets->Update(
                        APICALLBOOM_PARAM,
                        /*PK*/ QString::number(_voucherItem.OrderID),
                        TAPI::ORMFields_t({
                            { tblAccountUserAssetsBase::Fields::uas_vchID, _voucherID },
                            { tblAccountUserAssetsBase::Fields::uasStatus, TAPI::enuAuditableStatus::Active },
                        }),
                        {
                            { tblAccountUserAssetsBase::Fields::uasID, _voucherItem.OrderID },
                            { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, _voucherItem.UUID }, //this is just for make condition strong
                        });
}

bool intfAccountingBasedModule::removeFromUserAssets(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem
) {
    return this->AccountUserAssets->DeleteByPks(
        APICALLBOOM_PARAM,
        /*PK*/ QString::number(_voucherItem.OrderID),
        {
            //this is just for make condition safe and strong:
            { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, _voucherItem.UUID },
        },
        false
    );
}

bool intfAccountingBasedModule::processVoucherItem(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _userID,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem,
    quint64 _voucherID
) {
    if (!this->preProcessVoucherItem(APICALLBOOM_PARAM, _userID, _voucherItem, _voucherID))
        return false;

    if (this->activateUserAsset(APICALLBOOM_PARAM, _voucherItem, _voucherID) == false)
        return false;

    this->increaseDiscountUsage(APICALLBOOM_PARAM, _voucherItem);


    ///@TODO: add prize on item if possible


    this->postProcessVoucherItem(APICALLBOOM_PARAM, _userID, _voucherItem, _voucherID);

    return true;
}

bool intfAccountingBasedModule::cancelVoucherItem(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _userID,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem,
    std::function<bool(const QVariantMap &_userAssetInfo)> _fnCheckUserAsset
) {
    if (!this->preCancelVoucherItem(APICALLBOOM_PARAM, _userID, _voucherItem))
        return false;

    QVariantMap UserAssetInfo = this->AccountUserAssets->GetSelectQuery(APICALLBOOM_PARAM)
                                .addCols({
                                             tblAccountUserAssetsBase::Fields::uasID,
                                             tblAccountUserAssetsBase::Fields::uas_slbID,
                                             tblAccountUserAssetsBase::Fields::uasStatus,
                                         })
                                .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })
                                .one();

    if ((_fnCheckUserAsset != nullptr) && (_fnCheckUserAsset(UserAssetInfo) == false))
        return false;

    enuAuditableStatus::Type UserAssetStatus =
        enuAuditableStatus::toEnum(UserAssetInfo.value(tblAccountUserAssetsBase::Fields::uasStatus, enuAuditableStatus::Pending).toString());

    if (UserAssetStatus == TAPI::enuAuditableStatus::Active) {
        this->decreaseDiscountUsage(APICALLBOOM_PARAM, _voucherItem);

        ///@TODO: delete prize on item if previously added in processVoucherItem
    }

    quint32 SaleableID = UserAssetInfo.value(tblAccountUserAssetsBase::Fields::uas_slbID).toUInt();

    //-- un-reserve saleable & product ------------------------------------
    this->AccountSaleables->callSP(APICALLBOOM_PARAM,
                                   "spSaleable_unReserve", {
                                       { "iSaleableID", SaleableID },
                                       { "iUserID", APICALLBOOM_PARAM.getUserID() },
                                       { "iQty", _voucherItem.Qty },
                                   });

    //-- un-reserve saleable ------------------------------------
//    UpdateQuery(*this->AccountSaleables)
//        .innerJoinWith("userAsset") //tblAccountUserAssetsBase::Name, { tblAccountUserAssetsBase::Fields::uas_slbID, enuConditionOperator::Equal, tblAccountSaleablesBase::Fields::slbID })
//        .increament(tblAccountSaleablesBase::Fields::slbReturnedQty, _voucherItem.Qty)
//        .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })
//        .andWhere({ tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, enuConditionOperator::Equal, _voucherItem.UUID }) //this is just for make condition strong
//        .execute(_userID);

    //-- un-reserve product -------------------------------------
//    UpdateQuery(*this->AccountProducts)
//        .innerJoinWith("saleable") //tblAccountSaleablesBase::Name, { tblAccountSaleablesBase::Fields::slb_prdID, enuConditionOperator::Equal, tblAccountProductsBase::Fields::prdID })
//        .innerJoin(tblAccountUserAssetsBase::Name, { tblAccountUserAssetsBase::Name, tblAccountUserAssetsBase::Fields::uas_slbID, enuConditionOperator::Equal, tblAccountSaleablesBase::Name, tblAccountSaleablesBase::Fields::slbID })
//        .increament(tblAccountProductsBase::Fields::prdReturnedQty, _voucherItem.Qty)
//        .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })
//        .andWhere({ tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, enuConditionOperator::Equal, _voucherItem.UUID }) //this is just for make condition strong
//        .execute(_userID);

    //-----------------------------------------------------------
    this->removeFromUserAssets(APICALLBOOM_PARAM, _voucherItem);

    this->postCancelVoucherItem(APICALLBOOM_PARAM, _userID, _voucherItem);

    return true;
}

void checkVoucherItemForTrustedActionSanity(stuVoucherItemForTrustedAction &_data) {
    if (_data.UserID <= 0)
        throw exHTTPBadRequest("Invalid user id");

    if (_data.VoucherID <= 0)
        throw exHTTPBadRequest("Invalid voucher id");

    QString Sign = _data.Sign;
    _data.Sign.clear();
    if (Sign != QString(sign(_data)))
        throw exHTTPBadRequest("Invalid sign");

    //--------------------
    Sign = _data.VoucherItem.Sign;
    _data.VoucherItem.Sign.clear();

    if (Sign != QString(sign(_data.VoucherItem)))
        throw exHTTPBadRequest("Invalid voucher item sign");
}

bool IMPL_REST_POST(intfAccountingBasedModule, processVoucherItem, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuVoucherItemForTrustedAction _data
)) {
    checkVoucherItemForTrustedActionSanity(_data);

    return this->processVoucherItem(APICALLBOOM_PARAM, _data.UserID, _data.VoucherItem, _data.VoucherID);
}

bool IMPL_REST_POST(intfAccountingBasedModule, cancelVoucherItem, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    Targoman::API::AAA::stuVoucherItemForTrustedAction _data
)) {
    checkVoucherItemForTrustedActionSanity(_data);

    return this->cancelVoucherItem(APICALLBOOM_PARAM, _data.UserID, _data.VoucherItem);
}

} //namespace Targoman::API::AAA
