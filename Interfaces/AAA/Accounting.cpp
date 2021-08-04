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

//#include "Server/clsSimpleCrypt.h"
#include "Interfaces/Helpers/SecurityHelper.h"
using namespace Targoman::API::Helpers;

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

namespace Targoman::API::AAA::Accounting {

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
    if (_preVoucher.Items.isEmpty())
        return;

    QString Sign = _preVoucher.Sign;
    _preVoucher.Sign.clear();
    if (Sign != QString(Accounting::hash(QJsonDocument(_preVoucher.toJson()).toJson()).toBase64()))
        throw exHTTPBadRequest("Invalid sign found on pre-Voucher items");

    foreach (auto VoucherItem, _preVoucher.Items)
    {
        QString Sign = VoucherItem.Sign;
        VoucherItem.Sign.clear();

        if (Sign != QString(Accounting::hash(QJsonDocument(VoucherItem.toJson()).toJson()).toBase64()))
            throw exHTTPBadRequest("at least one of pre-Voucher items has invalid sign");
    }
}

/***************************************************************************************************\
|** intfRESTAPIWithAccounting **********************************************************************|
\***************************************************************************************************/
intfRESTAPIWithAccounting::intfRESTAPIWithAccounting(
        const QString& _schema,
        const QString& _module,
        AssetUsageLimitsCols_t _AssetUsageLimitsCols,
        intfAccountProducts* _products,
        intfAccountSaleables* _saleables,
        intfAccountUserAssets* _userAssets,
        intfAccountAssetUsage* _assetUsages,
        intfAccountCoupons* _discounts,
        intfAccountPrizes* _prizes
    ) :
    ORM::clsRESTAPIWithActionLogs(_schema, _module),
    ServiceName(_schema),
    AccountProducts(_products),
    AccountSaleables(_saleables),
    AccountUserAssets(_userAssets),
    AccountAssetUsages(_assetUsages),
    AccountCoupons(_discounts),
    AccountPrizes(_prizes),
    AssetUsageLimitsCols(_AssetUsageLimitsCols)
{
    ServiceRegistry.insert(this->ServiceName, this);

    foreach(auto Col, this->AssetUsageLimitsCols)
    {
        if (Col.PerDay.size())
            this->AssetUsageLimitsColsName.append(Col.PerDay);

        if (Col.PerWeek.size())
            this->AssetUsageLimitsColsName.append(Col.PerWeek);

        if (Col.PerMonth.size())
            this->AssetUsageLimitsColsName.append(Col.PerMonth);

        if (Col.Total.size())
            this->AssetUsageLimitsColsName.append(Col.Total);
    }
}

intfRESTAPIWithAccounting::~intfRESTAPIWithAccounting()
{}

intfRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName)
{
    Q_UNUSED(serviceAccounting);
    return ServiceRegistry.value(_serviceName);
}

stuActiveCredit intfRESTAPIWithAccounting::activeAccountObject(quint64 _usrID)
{
    return this->findBestMatchedCredit(_usrID);
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

    if (BestMatchedCredit.IsFromParent)
    {
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++)
        {
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
        UsageIter++)
    {
        if (ActiveCredit.Digested.Limits.contains(UsageIter.key()) == false)
            continue;

        checkCredit(UsageIter, ActiveCredit.Digested.Limits.value(UsageIter.key()), "Self");
    }
}

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

        if (_requestedUsage.size())
        {
            for (auto UsageIter = _requestedUsage.begin();
                UsageIter != _requestedUsage.end();
                UsageIter++)
            {
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
        AccountIter++)
    {
        const stuAssetItem& Item = AccountIter.value();

        if (isInvalidPackage(Item))
            continue;

        if (CandidateCredit.isEmpty())
        {
            CandidateCredit.append(Item);
            continue;
        }

        bool Inserted = false;
        for (auto CandidateIter = CandidateCredit.begin();
            CandidateIter != CandidateCredit.end();
            CandidateIter++)
        {
            if (comparePackages(Item, *CandidateIter) <0)
            {
                this->breakCredit(CandidateIter->slbID);
                CandidateCredit.insert(CandidateIter, Item);
                Inserted = true;
                break;
            }
        }
        if (Inserted == false)
            CandidateCredit.append(Item);
    }

    if (CandidateCredit.isEmpty())
    {
        if (_requestedUsage.size())
            throw exPaymentRequired("You don't have any active account");

        return stuActiveCredit();
    }

    const stuAssetItem& ActivePackage = CandidateCredit.first();
    QDateTime NextDigestTime;
    if (ActivePackage.prdValidToDate.isNull() == false)
    {
        if (NULLABLE_HAS_VALUE(ActivePackage.prdValidToHour))
            NextDigestTime = effectiveEndDateTime(ActivePackage);
        else
            NextDigestTime = QDateTime(ActivePackage.prdValidToDate.addDays(1));
    }
    else if (NULLABLE_HAS_VALUE(ActivePackage.prdValidToHour))
        NextDigestTime = effectiveEndDateTime(ActivePackage);

    return stuActiveCredit(ActivePackage,
                           NULLABLE_HAS_VALUE(ServiceCreditsInfo.ParentID),
                           ServiceCreditsInfo.MyLimitsOnParent,
                           NextDigestTime.isValid() ? (Now.msecsTo(NextDigestTime) / 1000) : -1);
}

TAPI::stuPreVoucher intfRESTAPIWithAccounting::apiPOSTaddToBasket(
        TAPI::JWT_t _JWT,
        TAPI::SaleableCode_t _saleableCode,
        TAPI::OrderAdditives_t _orderAdditives,
        quint16 _qty, ///TODO: use float for qty
        TAPI::CouponCode_t _discountCode,
        QString _referrer,
        TAPI::JSON_t _extraReferrerParams,
        TAPI::stuPreVoucher _lastPreVoucher
    )
{
    clsJWT JWT(_JWT);
    quint64 currentUserID = JWT.usrID();

    QVariantMap SaleableInfo = SelectQuery(*this->AccountSaleables)
        .addCols(QStringList({
            tblAccountSaleablesBase::slbID,
            tblAccountSaleablesBase::slbCode,
            //tblAccountSaleablesBase::slb_prdID,
            tblAccountSaleablesBase::slbName,
            //tblAccountSaleablesBase::slbDesc,
            //tblAccountSaleablesBase::slbType,
            tblAccountSaleablesBase::slbAvailableFromDate,
            tblAccountSaleablesBase::slbAvailableToDate,
            //tblAccountSaleablesBase::slbPrivs,
            tblAccountSaleablesBase::slbBasePrice,
            tblAccountSaleablesBase::slbAdditives,
            tblAccountSaleablesBase::slbProductCount,
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
        .andWhere({ tblAccountSaleablesBase::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountSaleablesBase::slbAvailableToDate, enuConditionOperator::Null })
            .orCond({ tblAccountSaleablesBase::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    qDebug() << "-- intfRESTAPIWithAccounting::apiPOSTaddToBasket() : SaleableInfo" << SaleableInfo;

    stuAssetItem AssetItem;
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdCode,                SaleableInfo, tblAccountProductsBase,  prdCode);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdName,                SaleableInfo, tblAccountProductsBase,  prdName);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdValidFromDate,       SaleableInfo, tblAccountProductsBase,  prdValidFromDate);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdValidToDate,         SaleableInfo, tblAccountProductsBase,  prdValidToDate);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdValidFromHour,       SaleableInfo, tblAccountProductsBase,  prdValidFromHour);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdValidToHour,         SaleableInfo, tblAccountProductsBase,  prdValidToHour);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdPrivs,               SaleableInfo, tblAccountProductsBase,  prdPrivs);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdVAT,                 SaleableInfo, tblAccountProductsBase,  prdVAT);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdInStockCount,        SaleableInfo, tblAccountProductsBase,  prdInStockCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdOrderedCount,        SaleableInfo, tblAccountProductsBase,  prdOrderedCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdReturnedCount,       SaleableInfo, tblAccountProductsBase,  prdReturnedCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.prdStatus,              SaleableInfo, tblAccountProductsBase,  prdStatus);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbID,                  SaleableInfo, tblAccountSaleablesBase, slbID);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbCode,                SaleableInfo, tblAccountSaleablesBase, slbCode);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbName,                SaleableInfo, tblAccountSaleablesBase, slbName);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbPrivs,               SaleableInfo, tblAccountSaleablesBase, slbPrivs);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbBasePrice,           SaleableInfo, tblAccountSaleablesBase, slbBasePrice);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbAdditives,           SaleableInfo, tblAccountSaleablesBase, slbAdditives);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbProductCount,        SaleableInfo, tblAccountSaleablesBase, slbProductCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbMaxSaleCountPerUser, SaleableInfo, tblAccountSaleablesBase, slbMaxSaleCountPerUser);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbInStockCount,        SaleableInfo, tblAccountSaleablesBase, slbInStockCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbOrderedCount,        SaleableInfo, tblAccountSaleablesBase, slbOrderedCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbReturnedCount,       SaleableInfo, tblAccountSaleablesBase, slbReturnedCount);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbVoucherTemplate,     SaleableInfo, tblAccountSaleablesBase, slbVoucherTemplate);
    SET_FIELD_FROM_VARIANT_MAP(AssetItem.slbStatus,              SaleableInfo, tblAccountSaleablesBase, slbStatus);

    //-- check available count --------------------------------
    qint64 AvailableProductCount = AssetItem.prdInStockCount - (NULLABLE_GET_OR_DEFAULT(AssetItem.prdOrderedCount, 0) - NULLABLE_GET_OR_DEFAULT(AssetItem.prdReturnedCount, 0));
    qint64 AvailableSaleableCount = AssetItem.slbInStockCount - (NULLABLE_GET_OR_DEFAULT(AssetItem.slbOrderedCount, 0) - NULLABLE_GET_OR_DEFAULT(AssetItem.slbReturnedCount, 0));

    if ((AvailableSaleableCount < 0) || (AvailableProductCount < 0))
        throw exHTTPInternalServerError(QString("AvailableSaleableCount(%1) or AvailableProductCount(%2) < 0").arg(AvailableSaleableCount).arg(AvailableProductCount));

    if (AvailableSaleableCount > AvailableProductCount)
        throw exHTTPInternalServerError(QString("AvailableSaleableCount(%1) > AvailableProductCount(%2)").arg(AvailableSaleableCount).arg(AvailableProductCount));

    if (AvailableSaleableCount < _qty)
        throw exHTTPBadRequest(QString("Not enough %1 available in store. Available(%2) qty(%3)").arg(_saleableCode).arg(AvailableSaleableCount).arg(_qty));

    //-- --------------------------------
    UsageLimits_t SaleableUsageLimits;
    for (auto Iter = this->AssetUsageLimitsCols.begin();
            Iter != this->AssetUsageLimitsCols.end();
            Iter++)
    {
        SaleableUsageLimits.insert(
            Iter.key(),
            {
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerDay)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerWeek)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint32, SaleableInfo.value(Iter->PerMonth)),
                NULLABLE_INSTANTIATE_FROM_QVARIANT(quint64, SaleableInfo.value(Iter->Total))
            }
        );
    }
    AssetItem.Digested.Limits = SaleableUsageLimits;

    AssetItem.SubTotal = AssetItem.slbBasePrice * _qty;
    AssetItem.TotalPrice = AssetItem.SubTotal;
    qDebug() << "slbBasePrice(" << AssetItem.slbBasePrice << ")";

    //-- --------------------------------
    this->digestPrivs(_JWT, AssetItem);

    this->applyAssetAdditives(_JWT, AssetItem, _orderAdditives);
    qDebug() << "after applyAssetAdditives: slbBasePrice(" << AssetItem.slbBasePrice << ")";

    this->applyReferrer(_JWT, AssetItem, _referrer, _extraReferrerParams);

    //-- --------------------------------
    AssetItem.SubTotal = AssetItem.slbBasePrice * _qty;
    AssetItem.TotalPrice = AssetItem.SubTotal;
    qDebug() << "SubTotal(" << AssetItem.SubTotal << ")";

    //-- discount --------------------------------
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
                Targoman::API::CURRENT_TIMESTAMP,
            }))

            .leftJoin(SelectQuery(*this->AccountUserAssets)
                      .addCol(tblAccountUserAssetsBase::uas_cpnID)
                      .addCol(tblAccountUserAssetsBase::uas_vchID)
                      .addCol(enuAggregation::COUNT, tblAccountUserAssetsBase::uasID, "_discountUsedCount")
                      .where({ tblAccountUserAssetsBase::uas_usrID, enuConditionOperator::Equal, currentUserID })
                      .groupBy(tblAccountUserAssetsBase::uas_cpnID)
                      .groupBy(tblAccountUserAssetsBase::uas_vchID)
                , "tmp_cpn_count"
                , { "tmp_cpn_count", tblAccountUserAssetsBase::uas_cpnID,
                    enuConditionOperator::Equal,
                    tblAccountCouponsBase::Name, tblAccountCouponsBase::cpnID }
            )
            .addCol("tmp_cpn_count._discountUsedCount")

            .leftJoin(SelectQuery(*this->AccountUserAssets)
                      .addCol(tblAccountUserAssetsBase::uas_cpnID)
                      .addCol(enuAggregation::SUM, tblAccountUserAssetsBase::uasDiscountAmount, "_discountUsedAmount")
                      .where({ tblAccountUserAssetsBase::uas_usrID, enuConditionOperator::Equal, currentUserID })
                      .groupBy(tblAccountUserAssetsBase::uas_cpnID)
                , "tmp_cpn_amount"
                , { "tmp_cpn_amount", tblAccountUserAssetsBase::uas_cpnID,
                    enuConditionOperator::Equal,
                    tblAccountCouponsBase::Name, tblAccountCouponsBase::cpnID }
            )
            .addCol("tmp_cpn_amount._discountUsedAmount")

            .where({ tblAccountCouponsBase::cpnCode, enuConditionOperator::Equal, _discountCode })
            .andWhere({ tblAccountCouponsBase::cpnValidFrom, enuConditionOperator::LessEqual, DBExpression::NOW() })
            .andWhere(clsCondition({ tblAccountCouponsBase::cpnExpiryTime, enuConditionOperator::Null })
                .orCond({ tblAccountCouponsBase::cpnExpiryTime, enuConditionOperator::GreaterEqual,
                    DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
            )
            .one();
//        qDebug() << "--------- DiscountInfo:" << DiscountInfo;

        if (DiscountInfo.size() == 0)
            throw exHTTPBadRequest("Discount code not found.");

        QDateTime Now = DiscountInfo.value(Targoman::API::CURRENT_TIMESTAMP).toDateTime();

        SET_FIELD_FROM_VARIANT_MAP(Discount.ID,                     DiscountInfo, tblAccountCouponsBase, cpnID);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.Code,                   DiscountInfo, tblAccountCouponsBase, cpnCode);
        quint32 cpnPrimaryCount;
        SET_FIELD_FROM_VARIANT_MAP(cpnPrimaryCount,                 DiscountInfo, tblAccountCouponsBase, cpnPrimaryCount);
        quint32 cpnTotalMaxAmount;
        SET_FIELD_FROM_VARIANT_MAP(cpnTotalMaxAmount,               DiscountInfo, tblAccountCouponsBase, cpnTotalMaxAmount);
        NULLABLE_TYPE(quint32) cpnPerUserMaxCount;
        SET_FIELD_FROM_VARIANT_MAP(cpnPerUserMaxCount,              DiscountInfo, tblAccountCouponsBase, cpnPerUserMaxCount);
        NULLABLE_TYPE(quint32) cpnPerUserMaxAmount;
        SET_FIELD_FROM_VARIANT_MAP(cpnPerUserMaxAmount,             DiscountInfo, tblAccountCouponsBase, cpnPerUserMaxAmount);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.ValidFrom,              DiscountInfo, tblAccountCouponsBase, cpnValidFrom);
        NULLABLE_TYPE(TAPI::DateTime_t) cpnExpiryTime;
        SET_FIELD_FROM_VARIANT_MAP(cpnExpiryTime,                   DiscountInfo, tblAccountCouponsBase, cpnExpiryTime);
        SET_FIELD_FROM_VARIANT_MAP(Discount.Amount,                 DiscountInfo, tblAccountCouponsBase, cpnAmount);
        TAPI::enuDiscountType::Type cpnAmountType;
        SET_FIELD_FROM_VARIANT_MAP(cpnAmountType,                   DiscountInfo, tblAccountCouponsBase, cpnAmountType);
        NULLABLE_TYPE(quint32) cpnMaxAmount;
//        TAPI::setFromVariant(cpnMaxAmount, DiscountInfo.value("cpnMaxAmount"));
        SET_FIELD_FROM_VARIANT_MAP(cpnMaxAmount,                    DiscountInfo, tblAccountCouponsBase, cpnMaxAmount);
        TAPI::JSON_t cpnSaleableBasedMultiplier;
        SET_FIELD_FROM_VARIANT_MAP(cpnSaleableBasedMultiplier,      DiscountInfo, tblAccountCouponsBase, cpnSaleableBasedMultiplier);
        quint32 cpnTotalUsedCount;
        SET_FIELD_FROM_VARIANT_MAP(cpnTotalUsedCount,               DiscountInfo, tblAccountCouponsBase, cpnTotalUsedCount);
        quint32 cpnTotalUsedAmount;
        SET_FIELD_FROM_VARIANT_MAP(cpnTotalUsedAmount,              DiscountInfo, tblAccountCouponsBase, cpnTotalUsedAmount);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.Status,                 DiscountInfo, tblAccountCouponsBase, cpnStatus);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.CreatedBy_usrID,        DiscountInfo, tblAccountCouponsBase, cpnCreatedBy_usrID);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.CreationDateTime,       DiscountInfo, tblAccountCouponsBase, cpnCreationDateTime);
//        SET_FIELD_FROM_VARIANT_MAP(Discount.UpdatedBy_usrID,        DiscountInfo, tblAccountCouponsBase, cpnUpdatedBy_usrID);

        NULLABLE_TYPE(quint32) _discountUsedCount;
        TAPI::setFromVariant(_discountUsedCount, DiscountInfo.value("_discountUsedCount"));
        NULLABLE_TYPE(quint32) _discountUsedAmount;
        TAPI::setFromVariant(_discountUsedAmount, DiscountInfo.value("_discountUsedAmount"));

//        if (NULLABLE_HAS_VALUE(cpnExpiryTime) && NULLABLE_GET(cpnExpiryTime).toDateTime() < Now)
//            throw exHTTPBadRequest("Discount code has been expired");

        if (cpnTotalUsedCount >= cpnPrimaryCount)
            throw exHTTPBadRequest("Discount code has been finished");

        if ((NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxCount, 0) > 0)
                && (NULLABLE_GET_OR_DEFAULT(_discountUsedCount, 0) >= NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxCount, 0)))
            throw exHTTPBadRequest("Discount usage per user has been reached");

        if (cpnTotalUsedAmount >= cpnTotalMaxAmount)
            throw exHTTPBadRequest("Discount usage amount has been reached");

        if ((NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxAmount, 0) > 0)
                && (NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0) >= NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxAmount, 0)))
            throw exHTTPBadRequest("Discount usage amount per user has been reached");

//        qDebug() << "-- DiscountInfo" << DiscountInfo;
//        qDebug() << "--" << DiscountInfo.value(tblAccountCouponsBase::cpnSaleableBasedMultiplier);
//        qDebug() << "-- cpnSaleableBasedMultiplier" << cpnSaleableBasedMultiplier;
//        qDebug() << "--" << cpnSaleableBasedMultiplier.object();
//        qDebug() << "-- _saleableCode" << _saleableCode;
//        qDebug() << "--" << cpnSaleableBasedMultiplier.object().value(_saleableCode);
//        qDebug() << "--" << cpnSaleableBasedMultiplier.object().value(_saleableCode).toString().toInt();
//        qDebug() << "--" << cpnSaleableBasedMultiplier.object().value(_saleableCode).toInt(-1);

        QJsonArray arr = cpnSaleableBasedMultiplier.array();
//        qDebug() << "arr" << arr;
        if (arr.size())
        {
//            qDebug() << "AAAAAAAAAAAAAAA 2" << arr;
            TAPI::stuDiscountSaleableBasedMultiplier multiplier;

            for (QJsonArray::const_iterator itr = arr.constBegin();
                itr != arr.constEnd();
                itr++)
            {
                auto elm = *itr;

//                qDebug() << "elm" << elm << "elm.toObject()=" << elm.toObject();

                TAPI::stuDiscountSaleableBasedMultiplier cur;
                cur.fromJson(elm.toObject());

                qint32 MinCount = NULLABLE_GET_OR_DEFAULT(cur.MinCount, -1);
//                qDebug() << "********" << cur.SaleableCode << MinCount << cur.Multiplier;

                if ((cur.SaleableCode == _saleableCode)
                        && (NULLABLE_GET_OR_DEFAULT(cur.MinCount, 0) <= _qty)
                    )
                {
                    if ((multiplier.Multiplier == 0)
                            || (NULLABLE_GET_OR_DEFAULT(multiplier.MinCount, 0) < MinCount))
                        multiplier = cur;
                }
            }

//            if (multiplier.Multiplier == 0) //not found
//                throw exHTTPBadRequest("Discount code is not valid on selected package");

            if (multiplier.Multiplier > 0) //found
            {
                auto m = Discount.Amount;
                Discount.Amount = Discount.Amount * multiplier.Multiplier;
                qDebug() << "Discount Before Multiply(" << m << ")" << "multiplier (" << multiplier.Multiplier << ")" << "Discount After Multiply(" << Discount.Amount << ")";
            }
        }

        Discount.Code = _discountCode;

        qDebug() << "1 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

        if (cpnAmountType == TAPI::enuDiscountType::Percent)
            Discount.Amount = AssetItem.SubTotal * Discount.Amount / 100.0;

        qDebug() << "2 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

        //check cpnMaxAmount
        if (NULLABLE_HAS_VALUE(cpnMaxAmount))
        {
            //note: cpnMaxAmount type is reverse of cpnAmountType
            if (cpnAmountType == TAPI::enuDiscountType::Percent)
                Discount.Amount = fmin(Discount.Amount, NULLABLE_GET_OR_DEFAULT(cpnMaxAmount, 0));
            else {
                quint32 _max = ceil(AssetItem.SubTotal * NULLABLE_GET_OR_DEFAULT(cpnMaxAmount, 0) / 100.0);
                Discount.Amount = fmin(Discount.Amount, _max);
            }
            qDebug() << "3 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
        }

        //check total
        qint32 remainDiscountAmount = cpnTotalMaxAmount - cpnTotalUsedAmount;
        if (remainDiscountAmount < Discount.Amount)
        {
            Discount.Amount = remainDiscountAmount;
            qDebug() << "4 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
        }

        //check per user
        if (NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxAmount, 0) > 0) {
            remainDiscountAmount = NULLABLE_GET_OR_DEFAULT(cpnPerUserMaxAmount, 0) - NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0);
            if (remainDiscountAmount <= 0)
                Discount.Amount = 0;
            else if (remainDiscountAmount < Discount.Amount)
                Discount.Amount = remainDiscountAmount;
            qDebug() << "5 Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";
        }

        //----------
        Discount.Amount = ceil(Discount.Amount);
        qDebug() << "Discount:" << "ID(" << Discount.ID << ")" << "Code(" << Discount.Code << ")" << "Amount(" << Discount.Amount << ")";

        if (Discount.Amount > 0)
        {
            AssetItem.Discount = Discount;
            AssetItem.TotalPrice = AssetItem.SubTotal - Discount.Amount;
            qDebug() << "AssetItem.TotalPrice:" << AssetItem.TotalPrice;

            ///TODO: why cpnTotalUsedCount is readonly?
//            quint64 affectedRowsCount = UpdateQuery(*this->AccountCoupons)
//                .increament(tblAccountCouponsBase::cpnTotalUsedCount, 1)
//                .increament(tblAccountCouponsBase::cpnTotalUsedAmount, Discount.Amount)
//                .where({ tblAccountCouponsBase::cpnID , enuConditionOperator::Equal, Discount.ID })
//                .execute(currentUserID);

//            if (affectedRowsCount == 0)
//               throw exHTTPInternalServerError("could not update discount usage");

//            TargomanLogInfo(5, "Discount Usages updated (+1, +" << Discount.Amount << ")");
        }
    } //if discount

    //-- new pre voucher item --------------------------------
    ///TODO: add ttl for order item

    stuVoucherItem PreVoucherItem;
    PreVoucherItem.Service = this->ServiceName;
    //PreVoucherItem.OrderID
    PreVoucherItem.Desc = AssetItem.slbName;
    PreVoucherItem.UnitPrice = AssetItem.slbBasePrice;
    PreVoucherItem.Qty = _qty;
    PreVoucherItem.SubTotal = AssetItem.SubTotal;
    PreVoucherItem.Discount = Discount;
    PreVoucherItem.DisAmount = (Discount.ID > 0 ? Discount.Amount : 0);
    PreVoucherItem.VATPercent = NULLABLE_GET_OR_DEFAULT(AssetItem.prdVAT, 0); // * 100;
    PreVoucherItem.VATAmount = (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount) * PreVoucherItem.VATPercent / 100;
    PreVoucherItem.UUID = SecurityHelper::UUIDtoMD5();
    //PreVoucherItem.Sign

    CreateQuery qry = CreateQuery(*this->AccountUserAssets)
        .addCol(tblAccountUserAssetsBase::uas_usrID)
        .addCol(tblAccountUserAssetsBase::uas_slbID)
//        .addCol(tblAccountUserAssetsBase::uas_vchID)
        .addCol(tblAccountUserAssetsBase::uasVoucherItemUUID)
//        .addCol(tblAccountUserAssetsBase::uasPrefered)
//        .addCol(tblAccountUserAssetsBase::uasOrderDateTime)
//        .addCol(tblAccountUserAssetsBase::uasStatus)
    ;

    QVariantMap values;
    values = {
        { tblAccountUserAssetsBase::uas_usrID, currentUserID },
        { tblAccountUserAssetsBase::uas_slbID, AssetItem.slbID },
//        { tblAccountUserAssetsBase::uas_vchID, ??? },
        { tblAccountUserAssetsBase::uasVoucherItemUUID, PreVoucherItem.UUID },
//            { tblAccountUserAssetsBase::uasPrefered, ??? },
//        { tblAccountUserAssetsBase::uasOrderDateTime, DBExpression::NOW() },
//            { tblAccountUserAssetsBase::uasStatus, },
    };

    if (Discount.ID > 0)
    {
        qry
            .addCol(tblAccountUserAssetsBase::uas_cpnID)
            .addCol(tblAccountUserAssetsBase::uasDiscountAmount)
        ;

        values.insert(tblAccountUserAssetsBase::uas_cpnID, Discount.ID);
        values.insert(tblAccountUserAssetsBase::uasDiscountAmount, Discount.Amount);
    }

    qry.values(values);

    PreVoucherItem.OrderID = qry.execute(currentUserID);
    PreVoucherItem.Sign = QString(Accounting::hash(QJsonDocument(PreVoucherItem.toJson()).toJson()).toBase64());

    //-- --------------------------------
    ///TODO: PreVoucherItem.DMInfo : json {"type":"adver", "additives":[{"color":"red"}, {"size":"m"}, ...]}
    /// used for DMLogic::applyCoupon -> match item.DMInfo by coupon rules
    /// return: amount of using coupon

    //-- add to the last pre voucher --------------------------------
    _lastPreVoucher.Items.append(PreVoucherItem);
    _lastPreVoucher.Summary = _lastPreVoucher.Items.size() > 1 ?
                                  QString("%1 items").arg(_lastPreVoucher.Items.size()) :
                                  QString("%1 of %2").arg(PreVoucherItem.Qty).arg(PreVoucherItem.Desc);
    qint64 FinalPrice = _lastPreVoucher.Round + _lastPreVoucher.ToPay
                        + (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount + PreVoucherItem.VATAmount);
    if (FinalPrice < 0)
        throw exHTTPInternalServerError("Final amount computed negative!");
    _lastPreVoucher.Round = static_cast<quint16>((FinalPrice / 100.));
    _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
    _lastPreVoucher.Sign.clear();
    _lastPreVoucher.Sign = QString(Accounting::hash(QJsonDocument(_lastPreVoucher.toJson()).toJson()).toBase64());

    return _lastPreVoucher;
}

} //namespace Targoman::API::AAA::Accounting
