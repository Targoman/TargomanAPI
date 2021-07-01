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

#include "Accounting.h"
#include "PrivHelpers.h"
#include "Interfaces/AAA/Authorization.h"
#include "Server/ServerConfigs.h"
#include "Server/clsSimpleCrypt.h"

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

using namespace Common;
using namespace Common::Configuration;

static QMap<QString, intfRESTAPIWithAccounting*> ServiceRegistry;

Common::Configuration::tmplConfigurable<QString> Secret(
        makeConfig("Secret"),
        "Secret to be used for signing voucher and prevoucher",
        "fcy^E?a*4<;auY?>^6s@");

QByteArray hash(const QByteArray& _data)
{
   return QMessageAuthenticationCode::hash(_data, Accounting::Secret.value().toUtf8(), QCryptographicHash::Sha256);
}

void checkPreVoucherSanity(TAPI::stuPreVoucher _preVoucher)
{
    if(_preVoucher.Items.isEmpty())
        return;

    QString Sign = _preVoucher.Sign;
    _preVoucher.Sign.clear();
    if (Sign != QString(Accounting::hash(QJsonDocument(_preVoucher.toJson()).toJson()).toBase64()))
        throw exHTTPBadRequest("Invalid sign found on pre-Voucher items");

    foreach(auto VoucherItem, _preVoucher.Items){
        QString Sign = VoucherItem.Sign;
        VoucherItem.Sign.clear();
        if (Sign != QString(Accounting::hash(QJsonDocument(VoucherItem.toJson()).toJson()).toBase64()))
            throw exHTTPBadRequest("at least one of pre-Voucher items has invalid sign");
    }
}

intfRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName)
{
    Q_UNUSED(serviceAccounting);
    return ServiceRegistry.value(_serviceName);
}

stuActiveCredit intfRESTAPIWithAccounting::activeAccountObject(quint64 _usrID)
{
    return this->findBestMatchedCredit(_usrID);
}

intfRESTAPIWithAccounting::intfRESTAPIWithAccounting(const QString& _schema,
                                                     const QString& _module,
                                                     AssetUsageLimitsCols_t _AssetUsageLimitsCols,
                                                     intfAccountProducts* _products,
                                                     intfAccountSaleables* _saleables,
                                                     intfAccountUserAssets* _userAssets,
                                                     intfAccountAssetUsage* _assetusage,
                                                     intfAccountCoupons* _discounts,
                                                     intfAccountPrizes* _prizes) :
    ORM::clsRESTAPIWithActionLogs(_schema, _module),
    AccountProducts(_products),
    AccountSaleables(_saleables),
    AccountUserAssets(_userAssets),
    AccountAssetUsage(_assetusage),
    AccountCoupons(_discounts),
    AccountPrizes(_prizes),
    AssetUsageLimitsCols(_AssetUsageLimitsCols)
{
    ServiceRegistry.insert(_schema, this);
    foreach(auto Col, this->AssetUsageLimitsCols){
        if(Col.PerDay.size()) this->AssetUsageLimitsColsName.append(Col.PerDay);
        if(Col.PerWeek.size()) this->AssetUsageLimitsColsName.append(Col.PerWeek);
        if(Col.PerMonth.size()) this->AssetUsageLimitsColsName.append(Col.PerMonth);
        if(Col.Total.size()) this->AssetUsageLimitsColsName.append(Col.Total);
    }
}

intfRESTAPIWithAccounting::~intfRESTAPIWithAccounting()
{}

stuActiveCredit intfRESTAPIWithAccounting::findBestMatchedCredit(quint64 _usrID, const ServiceUsage_t& _requestedUsage)
{
    stuServiceCreditsInfo ServiceCreditsInfo = this->retrieveServiceCreditsInfo(_usrID);
    if (ServiceCreditsInfo.ActiveCredits.isEmpty())
        return stuActiveCredit();

    QDateTime Now = ServiceCreditsInfo.DBCurrentDateTime;

    auto effectiveStartDateTime = [Now](const stuAssetItem& _assetItem) -> QDateTime {
        return _assetItem.prdValidFromHour //.isNull() == fasle
            ? QDateTime(Now.date()).addSecs(*_assetItem.prdValidFromHour * 3600)
            : QDateTime(Now.date());
    };

    auto effectiveEndDateTime = [Now](const stuAssetItem& _assetItem) -> QDateTime {
        if (NULLABLE_IS_NULL(_assetItem.prdValidFromHour) || NULLABLE_IS_NULL(_assetItem.prdValidToHour))
            QDateTime(Now.date().addDays(1));

        return _assetItem.prdValidFromHour < _assetItem.prdValidToHour
            ? QDateTime(Now.date()).addSecs(*_assetItem.prdValidToHour * 3600)
            : _assetItem.prdValidToDate == Now.date()
                ? QDateTime(Now.date().addDays(1))
                : QDateTime(Now.date().addDays(1)).addSecs(*_assetItem.prdValidToHour * 3600);
    };

    auto isInvalidPackage = [this, Now, effectiveStartDateTime, effectiveEndDateTime, _requestedUsage](const stuAssetItem& _assetItem) -> bool {
        if ((_assetItem.prdValidFromDate.isValid() && _assetItem.prdValidFromDate > Now.date())
               || (_assetItem.prdValidToDate.isValid() && _assetItem.prdValidToDate < Now.date())
               || (NULLABLE_HAS_VALUE(_assetItem.prdValidFromHour) && Now < effectiveStartDateTime(_assetItem))
               || (NULLABLE_HAS_VALUE(_assetItem.prdValidToHour) && Now > effectiveEndDateTime(_assetItem))
               || this->isEmpty(_assetItem.Digested.Limits)
            )
            return false;

        if (_requestedUsage.size()) {
            for (auto UsageIter = _requestedUsage.begin();
                UsageIter != _requestedUsage.end();
                UsageIter++) {
                if (_assetItem.Digested.Limits.contains(UsageIter.key()) == false)
                    continue;
                if (this->isUnlimited(_assetItem.Digested.Limits) == false){
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
        if (a.prdValidToDate.isValid() && b.prdValidToDate.isValid() == false) return -1;
        if (a.prdValidToDate.isValid() == false && b.prdValidToDate.isValid()) return 1;
        if (this->isUnlimited(a.Digested.Limits) && this->isUnlimited(b.Digested.Limits) == false) return -1;
        if (this->isUnlimited(a.Digested.Limits) == false && this->isUnlimited(b.Digested.Limits)) return 1;
        if (NULLABLE_HAS_VALUE(a.prdValidToHour) && NULLABLE_IS_NULL(b.prdValidToHour)) return -1;
        if (NULLABLE_IS_NULL(a.prdValidToHour) && NULLABLE_HAS_VALUE(b.prdValidToHour)) return 1;
        if (NULLABLE_HAS_VALUE(a.prdValidToHour) && NULLABLE_HAS_VALUE(b.prdValidToHour)) {
            if(effectiveEndDateTime(a) < effectiveEndDateTime(b)) return -1;
            if(effectiveEndDateTime(a) > effectiveEndDateTime(b)) return 1;
        }
        if (a.prdValidToDate.isValid() && b.prdValidToDate.isValid() && a.prdValidToDate != b.prdValidToDate)
            return b.prdValidToDate > a.prdValidToDate ? -1 : 1;
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
                this->breakCredit(CandidateIter->slbID);
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
    if (ActivePackage.prdValidToDate.isNull() == false) {
        if (NULLABLE_HAS_VALUE(ActivePackage.prdValidToHour))
            NextDigestTime = effectiveEndDateTime(ActivePackage);
        else
            NextDigestTime = QDateTime(ActivePackage.prdValidToDate.addDays(1));
    } else if (NULLABLE_HAS_VALUE(ActivePackage.prdValidToHour))
        NextDigestTime = effectiveEndDateTime(ActivePackage);

    return stuActiveCredit(ActivePackage,
                           NULLABLE_HAS_VALUE(ServiceCreditsInfo.ParentID),
                           ServiceCreditsInfo.MyLimitsOnParent,
                           NextDigestTime.isValid() ? (Now.msecsTo(NextDigestTime) / 1000) : -1);
}

TAPI::stuPreVoucher intfRESTAPIWithAccounting::apiPOSTaddToBasket(TAPI::JWT_t _JWT,
                                                                  TAPI::SaleableCode_t _saleableCode,
                                                                  TAPI::OrderAdditives_t _orderAdditives,
                                                                  quint16 _qty, ///TODO: use float for qty
                                                                  TAPI::CouponCode_t _discountCode,
                                                                  QString _referrer,
                                                                  TAPI::JSON_t _extraRefererParams,
                                                                  TAPI::stuPreVoucher _lastPreVoucher)
{
    Accounting::checkPreVoucherSanity(_lastPreVoucher);

    QVariantMap SaleableInfo = SelectQuery(*this->AccountSaleables)
        .addCols(QStringList({
            tblAccountSaleablesBase::slbID,
            tblAccountSaleablesBase::slbCode,
            //tblAccountSaleablesBase::slb_prdID,
            tblAccountSaleablesBase::slbName,
            //tblAccountSaleablesBase::slbDesc,
            //tblAccountSaleablesBase::slbType,
            //tblAccountSaleablesBase::slbCanBePurchasedSince,
            //tblAccountSaleablesBase::slbNotAvailableSince,
            //tblAccountSaleablesBase::slbPrivs,
            tblAccountSaleablesBase::slbBasePrice,
            tblAccountSaleablesBase::slbAdditives,
            tblAccountSaleablesBase::slbInStockCount,
            tblAccountSaleablesBase::slbOrderedCount,
            tblAccountSaleablesBase::slbReturnedCount,
            tblAccountSaleablesBase::slbVoucherTemplate,
            tblAccountSaleablesBase::slbStatus,
            //tblAccountSaleablesBase::slbCreatedBy_usrID,
            //tblAccountSaleablesBase::slbCreationDateTime,
            //tblAccountSaleablesBase::slbUpdatedBy_usrID,

            //tblAccountProductsBase::prdID,
            tblAccountProductsBase::prdCode,
            tblAccountProductsBase::prdName,
            //tblAccountProductsBase::prdDesc,
            tblAccountProductsBase::prdValidFromDate,
            tblAccountProductsBase::prdValidToDate,
            tblAccountProductsBase::prdValidFromHour,
            tblAccountProductsBase::prdValidToHour,
            //tblAccountProductsBase::prdPrivs,
            tblAccountProductsBase::prdVAT,
            tblAccountProductsBase::prdInStockCount,
            tblAccountProductsBase::prdOrderedCount,
            tblAccountProductsBase::prdReturnedCount,
            tblAccountProductsBase::prdStatus,
            //tblAccountProductsBase::prdCreatedBy_usrID,
            //tblAccountProductsBase::prdCreationDateTime,
            //tblAccountProductsBase::prdUpdatedBy_usrID,
        }))
        .addCols(this->AssetUsageLimitsColsName)
        .leftJoinWith("product")
        .where({ tblAccountSaleablesBase::slbCode, enuConditionOperator::Equal, _saleableCode })
        .andWhere({ tblAccountSaleablesBase::slbCanBePurchasedSince, enuConditionOperator::GreaterEqual, DBExpression::NOW() })
        .andWhere({ tblAccountSaleablesBase::slbNotAvailableSince, enuConditionOperator::Less, DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        .one();

    /*QVariantMap OLD_SaleableInfo = this->AccountSaleables->selectFromTable(
        {},
        QString ("( %1>=NOW() + %2<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min) )").arg(
            tblAccountSaleablesBase::slbCanBePurchasedSince).arg(
            tblAccountSaleablesBase::slbNotAvailableSince),
        _saleableCode,
        0,
        1,
        QStringList({
            //tblAccountProductsBase::prdID,
            tblAccountProductsBase::prdCode,
            tblAccountProductsBase::prdName,
            //tblAccountProductsBase::prdDesc,
            tblAccountProductsBase::prdValidFromDate,
            tblAccountProductsBase::prdValidToDate,
            tblAccountProductsBase::prdValidFromHour,
            tblAccountProductsBase::prdValidToHour,
            //tblAccountProductsBase::prdPrivs,
            tblAccountProductsBase::prdVAT,
            tblAccountProductsBase::prdInStockCount,
            tblAccountProductsBase::prdOrderedCount,
            tblAccountProductsBase::prdReturnedCount,
            tblAccountProductsBase::prdStatus,
            //tblAccountProductsBase::prdCreatedBy_usrID,
            //tblAccountProductsBase::prdCreationDateTime,
            //tblAccountProductsBase::prdUpdatedBy_usrID,

            tblAccountSaleablesBase::slbID,
            tblAccountSaleablesBase::slbCode,
            //tblAccountSaleablesBase::slb_prdID,
            tblAccountSaleablesBase::slbName,
            //tblAccountSaleablesBase::slbDesc,
            //tblAccountSaleablesBase::slbType,
            //tblAccountSaleablesBase::slbCanBePurchasedSince,
            //tblAccountSaleablesBase::slbNotAvailableSince,
            //tblAccountSaleablesBase::slbPrivs,
            tblAccountSaleablesBase::slbBasePrice,
            tblAccountSaleablesBase::slbAdditives,
            tblAccountSaleablesBase::slbInStockCount,
            tblAccountSaleablesBase::slbOrderedCount,
            tblAccountSaleablesBase::slbReturnedCount,
            tblAccountSaleablesBase::slbVoucherTemplate,
            tblAccountSaleablesBase::slbStatus,
            //tblAccountSaleablesBase::slbCreatedBy_usrID,
            //tblAccountSaleablesBase::slbCreationDateTime,
            //tblAccountSaleablesBase::slbUpdatedBy_usrID,

            this->AssetUsageLimitsColsName.join(',')
        }).join(',')
    ).toMap();*/

#define SET_FIELD_FROM_VARIANMAP(_fieldName, _table) \
    TAPI::setFromVariant(AssetItem._fieldName, SaleableInfo.value(_table::_fieldName))

    stuAssetItem AssetItem;
    SET_FIELD_FROM_VARIANMAP(prdCode,                tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdName,                tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidFromDate,       tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidToDate,         tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidFromHour,       tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidToHour,         tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdPrivs,               tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdVAT,                 tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdInStockCount,        tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdOrderedCount,        tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdReturnedCount,       tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdStatus,              tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(slbID,                  tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbCode,                tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbName,                tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbPrivs,               tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbBasePrice,           tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbAdditives,           tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbProductCount,        tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbMaxSaleCountPerUser, tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbInStockCount,        tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbOrderedCount,        tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbReturnedCount,       tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbVoucherTemplate,     tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbStatus,              tblAccountSaleablesBase);

    //check available count
    qint64 AvailableProductCount = AssetItem.prdInStockCount - (AssetItem.prdOrderedCount - AssetItem.prdReturnedCount);
    qint64 AvailableSaleableCount = AssetItem.slbInStockCount - (AssetItem.slbOrderedCount - AssetItem.slbReturnedCount);
    if ((AvailableSaleableCount < 0) || (AvailableProductCount < 0))
        throw exHTTPInternalServerError("AvailableSaleableCount or AvailableProductCount < 0");
    if (AvailableSaleableCount > AvailableProductCount)
        throw exHTTPInternalServerError("AvailableSaleableCount > AvailableProductCount");
    if (AvailableSaleableCount < _qty)
        throw exHTTPBadRequest(QString("Not enough %1 available in store.").arg(_saleableCode));

    //---------
    UsageLimits_t SaleableUsageLimits;
    for (auto Iter = this->AssetUsageLimitsCols.begin();
        Iter != this->AssetUsageLimitsCols.end();
        Iter++)
        SaleableUsageLimits.insert(
            Iter.key(),
            {
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerDay)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerWeek)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerMonth)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, SaleableInfo.value(Iter->Total))
            }
        );
    AssetItem.Digested.Limits = SaleableUsageLimits;

    //---------
    this->digestPrivs(_JWT, AssetItem);

    this->applyAssetAdditives(_JWT, AssetItem, _orderAdditives);

    this->applyReferrer(_JWT, AssetItem, _referrer, _extraRefererParams);

    //---------
    ///TODO: what if some one uses discount code and at the same time will pay by prize credit
    stuDiscount Discount;
    if (_discountCode.size()) {
        QVariantMap DiscountInfo = SelectQuery(*this->AccountCoupons)
            .addCols(QStringList({
                tblAccountCouponsBase::cpnID,
                //tblAccountCouponsBase::cpnCode,
                tblAccountCouponsBase::cpnPrimaryCount,
                tblAccountCouponsBase::cpnTotalMaxAmount,
                tblAccountCouponsBase::cpnPerUserMaxCount,
                tblAccountCouponsBase::cpnPerUserMaxAmount,
                //tblAccountCouponsBase::cpnValidFrom,
                tblAccountCouponsBase::cpnExpiryTime,
                tblAccountCouponsBase::cpnAmount,
                tblAccountCouponsBase::cpnAmountType,
                tblAccountCouponsBase::cpnMaxAmount,
                tblAccountCouponsBase::cpnSaleableBasedMultiplier,
                tblAccountCouponsBase::cpnTotalUsedCount,
                tblAccountCouponsBase::cpnTotalUsedAmount,
                tblAccountCouponsBase::cpnStatus,
                //tblAccountCouponsBase::cpnCreatedBy_usrID,
                //tblAccountCouponsBase::cpnCreationDateTime,
                //tblAccountCouponsBase::cpnUpdatedBy_usrID,
                Targoman::API::CURRENT_DATETIME,
            }))
        ///TODO: join with userAssets to count user discount usage per voucher
        .where({ tblAccountCouponsBase::cpnID, enuConditionOperator::Equal, _discountCode })
        .andWhere({ tblAccountCouponsBase::cpnValidFrom, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .one();

        if (DiscountInfo.size() == 0)
            throw exHTTPBadRequest("Discount code not found.");

        QDateTime Now = DiscountInfo.value(Targoman::API::CURRENT_DATETIME).toDateTime();

        if (DiscountInfo.value(tblAccountCouponsBase::cpnExpiryTime).toDateTime() < Now)
            throw exHTTPBadRequest("Discount code has been expired");

        if (DiscountInfo.value(tblAccountCouponsBase::cpnPrimaryCount).toInt() <= DiscountInfo.value(tblAccountCouponsBase::cpnTotalUsedCount).toInt())
            throw exHTTPBadRequest("Discount code has been finished");

        qint64 Amount = DiscountInfo.value(tblAccountCouponsBase::cpnSaleableBasedMultiplier).toJsonObject().value(_saleableCode).toInt(-1);
        if (Amount < 0)
            throw exHTTPBadRequest("Discount code is not valid on selected package");
        Discount.Amount = static_cast<quint32>(Amount);
        Discount.ID = DiscountInfo.value(tblAccountCouponsBase::cpnPrimaryCount).toULongLong();
        Discount.Name = _discountCode;
        Discount.Type = static_cast<TAPI::enuDiscountType::Type>(DiscountInfo.value(tblAccountCouponsBase::cpnAmountType).toInt());
        Discount.MaxAmount = DiscountInfo.value(tblAccountCouponsBase::cpnMaxAmount).toUInt();
    }

    stuVoucherItem PreVoucherItem;
    PreVoucherItem.Service = this->ServiceName;
    ///TODO add ttl for order item
    /** TODO COMPLETE
     * PreVoucherItem.OrderID = this->AccountUserAssets->create(clsJWT(_JWT).usrID(), TAPI::ORMFields_t({
                                                                {tblAccountUserAssetsBase::uas_usrID, clsJWT(_JWT).usrID()},
                                                                {tblAccountUserAssetsBase::uas_slbID, Package.PackageID},
                                                            })).toULongLong();
                                                            */
    PreVoucherItem.Desc  = SaleableInfo.value(tblAccountSaleablesBase::Name).toString();

    ///TODO PreVoucherItem.DMInfo : json {"type":"adver", "additives":[{"color":"red"}, {"size":"m"}, ...]}
    /// used for DMLogic::applyCoupon -> match item.DMInfo by coupon rules
    /// return: amount of using coupon

/*    PreVoucherItem.Qty = _qty;
    PreVoucherItem.UnitPrice = SaleableInfo.value(tblAccountSaleablesBase::slbPrice).toUInt();
    PreVoucherItem.SubTotal = PreVoucherItem.UnitPrice * PreVoucherItem.Qty;
    PreVoucherItem.Discount = Discount;
    if (Discount.Amount)
        switch(Discount.Type){
        case TAPI::enuDiscountType::Free: PreVoucherItem.DisAmount = PreVoucherItem.SubTotal; break;
        case TAPI::enuDiscountType::Amount: PreVoucherItem.DisAmount = qMin(PreVoucherItem.SubTotal, Discount.Amount); break;
        case TAPI::enuDiscountType::Percent: PreVoucherItem.DisAmount = qMin(Discount.MaxAmount, static_cast<quint32>(floor(PreVoucherItem.SubTotal * Discount.Amount / 100.)));
        }
    //TODO rename Tax to VAT
    PreVoucherItem.VATPercent = SaleableInfo.value(tblAccountSaleablesBase::slbVAT).toUInt();
    PreVoucherItem.VATAmount = ((PreVoucherItem.SubTotal - PreVoucherItem.DisAmount) * PreVoucherItem.VATPercent / 100);
    PreVoucherItem.Sign = QString(Accounting::hash(QJsonDocument(PreVoucherItem.toJson()).toJson()).toBase64());

    _lastPreVoucher.Items.append(PreVoucherItem);
    _lastPreVoucher.Summary = _lastPreVoucher.Items.size() > 1 ?
                                  QString("%1 items").arg(_lastPreVoucher.Items.size()) :
                                  QString("%1 of %2").arg(PreVoucherItem.Qty).arg(PreVoucherItem.Desc);
    qint64 FinalPrice = PreVoucherItem.SubTotal - PreVoucherItem.DisAmount + PreVoucherItem.VATAmount;
    if(FinalPrice<0) throw exHTTPInternalServerError("Final amount computed negative!");
    _lastPreVoucher.Round = static_cast<quint16>((FinalPrice / 100.));
    _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
    _lastPreVoucher.Sign.clear();
    _lastPreVoucher.Sign =  QString(Accounting::hash(QJsonDocument(_lastPreVoucher.toJson()).toJson()).toBase64());
    */

    return _lastPreVoucher;
}

void intfRESTAPIWithAccounting::checkUsageIsAllowed(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage)
{
    QJsonObject Privs = _jwt.privsObject();
    if (Privs.contains(this->ServiceName) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->ServiceName);

    stuActiveCredit BestMatchedCredit = this->findBestMatchedCredit(clsJWT(_jwt).usrID(), _requestedUsage);
    if (BestMatchedCredit.TTL == 0) ///TODO: TTL must be checked
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
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++) {

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

}
}
}
}

