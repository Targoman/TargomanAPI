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

//#include "Server/clsSimpleCrypt.h"
#include "Interfaces/Helpers/SecurityHelper.h"
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

QByteArray voucherSign(const QByteArray& _data) {
   return QMessageAuthenticationCode::hash(_data, Secret.value().toUtf8(), QCryptographicHash::Sha256);
}

void checkPreVoucherSanity(stuPreVoucher _preVoucher) {
    if (_preVoucher.Items.isEmpty())
        return;

    QString Sign = _preVoucher.Sign;
    _preVoucher.Sign.clear();
    if (Sign != QString(voucherSign(QJsonDocument(_preVoucher.toJson()).toJson()).toBase64()))
        throw exHTTPBadRequest("Invalid sign found on pre-Voucher items");

    foreach (auto VoucherItem, _preVoucher.Items) {
        QString Sign = VoucherItem.Sign;
        VoucherItem.Sign.clear();

        if (Sign != QString(voucherSign(QJsonDocument(VoucherItem.toJson()).toJson()).toBase64()))
            throw exHTTPBadRequest("at least one of pre-Voucher items has invalid sign");
    }
}

intfAccountingBasedModule* serviceAccounting(const QString& _serviceName) {
    return ServiceRegistry.value(_serviceName);
}

/***************************************************************************************************\
|** intfAccountingBasedModule **********************************************************************|
\***************************************************************************************************/
intfAccountingBasedModule::intfAccountingBasedModule(
    const QString& _module,
    const QString& _schema,
    AssetUsageLimitsCols_t _AssetUsageLimitsCols,
    intfAccountProducts* _products,
    intfAccountSaleables* _saleables,
    intfAccountUserAssets* _userAssets,
    intfAccountAssetUsage* _assetUsages,
    intfAccountCoupons* _discounts,
    intfAccountPrizes* _prizes
) :
    API::intfSQLBasedWithActionLogsModule(_module, _schema),
    ServiceName(_schema),
    AccountProducts(_products),
    AccountSaleables(_saleables),
    AccountUserAssets(_userAssets),
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
    QJsonObject Privs = _APICALLBOOM.getJWTPrivsObject();

    if (Privs.contains(this->ServiceName) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->ServiceName);

    stuActiveCredit BestMatchedCredit = this->findBestMatchedCredit(_APICALLBOOM.getUserID(), _requestedUsage);

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
        if (a.prdValidToDate.isValid() && b.prdValidToDate.isValid() == false) return -1;
        if (a.prdValidToDate.isValid() == false && b.prdValidToDate.isValid()) return 1;
        if (this->isUnlimited(a.Digested.Limits) && this->isUnlimited(b.Digested.Limits) == false) return -1;
        if (this->isUnlimited(a.Digested.Limits) == false && this->isUnlimited(b.Digested.Limits)) return 1;
        if (NULLABLE_HAS_VALUE(a.prdValidToHour) && NULLABLE_IS_NULL(b.prdValidToHour)) return -1;
        if (NULLABLE_IS_NULL(a.prdValidToHour) && NULLABLE_HAS_VALUE(b.prdValidToHour)) return 1;
        if (NULLABLE_HAS_VALUE(a.prdValidToHour) && NULLABLE_HAS_VALUE(b.prdValidToHour)) {
            if (effectiveEndDateTime(a) < effectiveEndDateTime(b)) return -1;
            if (effectiveEndDateTime(a) > effectiveEndDateTime(b)) return 1;
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

stuDiscount3 intfAccountingBasedModule::applyDiscount(
    INTFAPICALLBOOM_IMPL    &APICALLBOOM_PARAM,
    INOUT stuAssetItem      &AssetItem,
    TAPI::CouponCode_t      _discountCode,
    TAPI::SaleableCode_t    _saleableCode,
    qreal                   _qty
) {
    _discountCode = _discountCode.trimmed();

    if (_discountCode.isEmpty())
        return {};

    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    QVariantMap DiscountInfo = SelectQuery(*this->AccountCoupons)
        .addCols(this->AccountCoupons->SelectableColumnNames())

        .leftJoin(SelectQuery(*this->AccountUserAssets)
                  .addCols({
                               tblAccountUserAssetsBase::Fields::uas_cpnID,
                               tblAccountUserAssetsBase::Fields::uas_vchID,
                           })
                  .addCol(enuAggregation::COUNT, tblAccountUserAssetsBase::Fields::uasID, "_discountUsedCount")
                  .where({ tblAccountUserAssetsBase::Fields::uas_usrID, enuConditionOperator::Equal, CurrentUserID })
                  .andWhere({ tblAccountUserAssetsBase::Fields::uasStatus, enuConditionOperator::In, QString("'%1','%2'")
                              .arg(QChar(enuAuditableStatus::Active)).arg(QChar(enuAuditableStatus::Banned)) })
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_cpnID)
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_vchID)
            , "tmp_cpn_count"
            , { "tmp_cpn_count", tblAccountUserAssetsBase::Fields::uas_cpnID,
                enuConditionOperator::Equal,
                tblAccountCouponsBase::Name, tblAccountCouponsBase::Fields::cpnID }
        )
        .addCol("tmp_cpn_count._discountUsedCount")

        .leftJoin(SelectQuery(*this->AccountUserAssets)
                  .addCol(tblAccountUserAssetsBase::Fields::uas_cpnID)
                  .addCol(enuAggregation::SUM, tblAccountUserAssetsBase::Fields::uasDiscountAmount, "_discountUsedAmount")
                  .where({ tblAccountUserAssetsBase::Fields::uas_usrID, enuConditionOperator::Equal, CurrentUserID })
                  .andWhere({ tblAccountUserAssetsBase::Fields::uasStatus, enuConditionOperator::In, QString("'%1','%2'")
                              .arg(QChar(enuAuditableStatus::Active)).arg(QChar(enuAuditableStatus::Banned)) })
                  .groupBy(tblAccountUserAssetsBase::Fields::uas_cpnID)
            , "tmp_cpn_amount"
            , { "tmp_cpn_amount", tblAccountUserAssetsBase::Fields::uas_cpnID,
                enuConditionOperator::Equal,
                tblAccountCouponsBase::Name, tblAccountCouponsBase::Fields::cpnID }
        )
        .addCol("tmp_cpn_amount._discountUsedAmount")

        .where({ tblAccountCouponsBase::Fields::cpnCode, enuConditionOperator::Equal, _discountCode })
        .andWhere({ tblAccountCouponsBase::Fields::cpnValidFrom, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountCouponsBase::Fields::cpnExpiryTime, enuConditionOperator::Null })
            .orCond({ tblAccountCouponsBase::Fields::cpnExpiryTime, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    if (DiscountInfo.size() == 0)
        throw exHTTPBadRequest("Discount code not found.");

    QDateTime Now = DiscountInfo.value(Targoman::API::CURRENT_TIMESTAMP).toDateTime();

    QJsonObject DiscountInfoJson = QJsonObject::fromVariantMap(DiscountInfo);
    tblAccountCouponsBase::DTO FullDiscount;
    FullDiscount.fromJson(DiscountInfoJson);

    stuDiscount3 Discount3;

    Discount3.ID     = FullDiscount.cpnID;
    Discount3.Code   = FullDiscount.cpnCode;
    Discount3.Amount = FullDiscount.cpnAmount;

    NULLABLE_TYPE(quint32) _discountUsedCount;
    TAPI::setFromVariant(_discountUsedCount, DiscountInfo.value("_discountUsedCount"));
    NULLABLE_TYPE(quint32) _discountUsedAmount;
    TAPI::setFromVariant(_discountUsedAmount, DiscountInfo.value("_discountUsedAmount"));

//        if (NULLABLE_HAS_VALUE(cpnExpiryTime) && NULLABLE_GET(cpnExpiryTime).toDateTime() < Now)
//            throw exHTTPBadRequest("Discount code has been expired");

    if (FullDiscount.cpnTotalUsedCount >= FullDiscount.cpnPrimaryCount)
        throw exHTTPBadRequest("Discount code has been finished");

    if ((NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxCount, 0) > 0)
            && (NULLABLE_GET_OR_DEFAULT(_discountUsedCount, 0) >= NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxCount, 0)))
        throw exHTTPBadRequest("Max discount usage per user has been reached");

    if (FullDiscount.cpnTotalUsedAmount >= FullDiscount.cpnTotalMaxAmount)
        throw exHTTPBadRequest("Max discount usage amount has been reached");

    if ((NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxAmount, 0) > 0)
            && (NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0) >= NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxAmount, 0)))
        throw exHTTPBadRequest("Max discount usage amount per user has been reached");

    //-- SaleableBasedMultiplier ---------------------------
    QJsonArray arr = FullDiscount.cpnSaleableBasedMultiplier.array();
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

            if ((cur.SaleableCode == _saleableCode)
                    && (NULLABLE_GET_OR_DEFAULT(cur.MinQty, 0) <= _qty)
            ) {
                if ((multiplier.Multiplier == 0)
                        || (NULLABLE_GET_OR_DEFAULT(multiplier.MinQty, 0) < MinQty))
                    multiplier = cur;
            }
        }

//            if (multiplier.Multiplier == 0) //not found
//                throw exHTTPBadRequest("Discount code is not valid on selected package");

        if (multiplier.Multiplier > 0) { //found
            auto m = Discount3.Amount;
            Discount3.Amount = Discount3.Amount * multiplier.Multiplier;

            TargomanDebug(5) << "Discount Before Multiply(" << m << ")" << "multiplier (" << multiplier.Multiplier << ")" << "Discount After Multiply(" << Discount3.Amount << ")";
        }
    } //if (arr.size())

//        Discount.Code = _discountCode;

    TargomanDebug(5) << "1 Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";

    if (FullDiscount.cpnAmountType == enuDiscountType::Percent)
        Discount3.Amount = AssetItem.SubTotal * Discount3.Amount / 100.0;

    TargomanDebug(5) << "2 Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";

    //check cpnMaxAmount
    if (NULLABLE_HAS_VALUE(FullDiscount.cpnMaxAmount)) {
        //note: cpnMaxAmount type is opposite to cpnAmountType
        if (FullDiscount.cpnAmountType == enuDiscountType::Percent)
            Discount3.Amount = fmin(Discount3.Amount, NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnMaxAmount, 0));
        else {
            quint32 _max = ceil(AssetItem.SubTotal * NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnMaxAmount, 0) / 100.0);
            Discount3.Amount = fmin(Discount3.Amount, _max);
        }
        TargomanDebug(5) << "3 Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";
    }

    //check total - used amount
    qint32 remainDiscountAmount = FullDiscount.cpnTotalMaxAmount - FullDiscount.cpnTotalUsedAmount;
    if (remainDiscountAmount < Discount3.Amount) {
        Discount3.Amount = remainDiscountAmount;
        TargomanDebug(5) << "4 Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";
    }

    //check per user - used amount
    if (NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxAmount, 0) > 0) {
        remainDiscountAmount = NULLABLE_GET_OR_DEFAULT(FullDiscount.cpnPerUserMaxAmount, 0) - NULLABLE_GET_OR_DEFAULT(_discountUsedAmount, 0);
        if (remainDiscountAmount <= 0)
            Discount3.Amount = 0;
        else if (remainDiscountAmount < Discount3.Amount)
            Discount3.Amount = remainDiscountAmount;
        TargomanDebug(5) << "5 Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";
    }

    //----------
    Discount3.Amount = ceil(Discount3.Amount);
    TargomanDebug(5) << "Discount:" << "ID(" << Discount3.ID << ")" << "Code(" << Discount3.Code << ")" << "Amount(" << Discount3.Amount << ")";

    if (Discount3.Amount > 0) {
        AssetItem.Discount = Discount3;
        //do not decrease TotalPrice, because PENDING_VOUCHER_NAME_COUPON_DISCOUNT will increase credit
//        AssetItem.TotalPrice = AssetItem.SubTotal - Discount3.Amount;

        TargomanDebug(5, "AssetItem.TotalPrice:" << AssetItem.TotalPrice);

        ///@kambizzandi: Increase coupon statistics were moved to finalizeBasket,
        /// because the customer may be angry about not being able to use the coupon again in same voucher
//            quint64 affectedRowsCount = UpdateQuery(*this->AccountCoupons)
//                .increament(tblAccountCouponsBase::Fields::cpnTotalUsedCount, 1)
//                .increament(tblAccountCouponsBase::Fields::cpnTotalUsedAmount, Discount.Amount)
//                .where({ tblAccountCouponsBase::Fields::cpnID , enuConditionOperator::Equal, Discount.ID })
//                .execute(currentUserID);

//            if (affectedRowsCount == 0)
//               throw exHTTPInternalServerError("could not update discount usage");

//            TargomanLogInfo(5, "Discount Usages updated (+1, +" << Discount.Amount << ")");

        AssetItem.PendingVouchers.append({
            /* Name     */ PENDING_VOUCHER_NAME_COUPON_DISCOUNT,
            /* Type     */ enuVoucherType::Prize, //Credit,
            /* Amount   */ Discount3.Amount,
            /* Desc     */ Discount3.toJson(),
        });

        return Discount3;
    }

    return {};
}

Targoman::API::AAA::stuPreVoucher IMPL_REST_POST(intfAccountingBasedModule, addToBasket, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    TAPI::SaleableCode_t _saleableCode,
    Targoman::API::AAA::OrderAdditives_t _orderAdditives,
    qreal _qty,
    TAPI::CouponCode_t _discountCode,
    QString _referrer,
    TAPI::JSON_t _extraReferrerParams,
    Targoman::API::AAA::stuPreVoucher _lastPreVoucher
)) {
    checkPreVoucherSanity(_lastPreVoucher);

    quint64 CurrentUserID = _APICALLBOOM.getUserID();

    if (_lastPreVoucher.Items.isEmpty())
        _lastPreVoucher.UserID = CurrentUserID;
    else if (_lastPreVoucher.UserID != CurrentUserID)
        throw exHTTPBadRequest("invalid pre-Voucher owner");

    QVariantMap SaleableInfo = SelectQuery(*this->AccountSaleables)
        .addCols(this->AccountSaleables->SelectableColumnNames())
        .addCols(this->AccountProducts->SelectableColumnNames())
        .addCols(this->AssetUsageLimitsColsName)
        .leftJoinWith("product")
        .where({ tblAccountSaleablesBase::Fields::slbCode, enuConditionOperator::Equal, _saleableCode })
        .andWhere({ tblAccountSaleablesBase::Fields::slbAvailableFromDate, enuConditionOperator::LessEqual, DBExpression::NOW() })
        .andWhere(clsCondition({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::Null })
            .orCond({ tblAccountSaleablesBase::Fields::slbAvailableToDate, enuConditionOperator::GreaterEqual,
                DBExpression::DATE_ADD(DBExpression::NOW(), 15, enuDBExpressionIntervalUnit::MINUTE) })
        )
        .one();

    TargomanDebug(5) << "intfAccountingBasedModule::addToBasket : SaleableInfo" << SaleableInfo;

    stuAssetItem AssetItem;
    AssetItem.fromJson(QJsonObject::fromVariantMap(SaleableInfo));

    AssetItem.NewUnitPrice = AssetItem.slbBasePrice;

    //-- check available count --------------------------------
    qreal AvailableProductQty = AssetItem.prdInStockQty - (NULLABLE_GET_OR_DEFAULT(AssetItem.prdOrderedQty, 0) - NULLABLE_GET_OR_DEFAULT(AssetItem.prdReturnedQty, 0));
    qreal AvailableSaleableQty = AssetItem.slbInStockQty - (NULLABLE_GET_OR_DEFAULT(AssetItem.slbOrderedQty, 0) - NULLABLE_GET_OR_DEFAULT(AssetItem.slbReturnedQty, 0));

    if ((AvailableSaleableQty < 0) || (AvailableProductQty < 0))
        throw exHTTPInternalServerError(QString("AvailableSaleableQty(%1) or AvailableProductQty(%2) < 0").arg(AvailableSaleableQty).arg(AvailableProductQty));

    if (AvailableSaleableQty > AvailableProductQty)
        throw exHTTPInternalServerError(QString("AvailableSaleableQty(%1) > AvailableProductQty(%2)").arg(AvailableSaleableQty).arg(AvailableProductQty));

    if (AvailableSaleableQty < _qty)
        throw exHTTPBadRequest(QString("Not enough %1 available in store. Available(%2) qty(%3)").arg(_saleableCode).arg(AvailableSaleableQty).arg(_qty));

    //-- --------------------------------
    AssetItem.SubTotal = AssetItem.NewUnitPrice * _qty;
    AssetItem.TotalPrice = AssetItem.SubTotal;
    TargomanDebug(5) << "NewUnitPrice(" << AssetItem.NewUnitPrice << ")";

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
    this->digestPrivs(_APICALLBOOM, AssetItem);

    this->applyAssetAdditives(_APICALLBOOM, AssetItem, _orderAdditives);
    TargomanDebug(5) << "after applyAssetAdditives: NewUnitPrice(" << AssetItem.NewUnitPrice << ")";

    this->applyReferrer(_APICALLBOOM, AssetItem, _referrer, _extraReferrerParams);

//    this->parsePrize(...); -> AssetItem.PendingVouchers

    //-- --------------------------------
    AssetItem.SubTotal = AssetItem.NewUnitPrice * _qty;
    AssetItem.TotalPrice = AssetItem.SubTotal;
    TargomanDebug(5) << "SubTotal(" << AssetItem.SubTotal << ")";

    //-- discount --------------------------------
    ///TODO: what if some one uses discount code and at the same time will pay by prize credit
    stuDiscount3 Discount3 = this->applyDiscount(_APICALLBOOM, AssetItem, _discountCode, _saleableCode, _qty);

    //-- reserve saleable & product ------------------------------------
    this->AccountSaleables->callSP(APICALLBOOM_PARAM,
                                   "spSaleable_Reserve", {
                                       { "iSaleableID", AssetItem.slbID },
                                       { "iUserID", CurrentUserID },
                                       { "iQty", _qty },
                                   });

    //-- new pre voucher item --------------------------------
    ///TODO: add ttl for order item

    stuVoucherItem PreVoucherItem;

    PreVoucherItem.PendingVouchers = AssetItem.PendingVouchers;
    PreVoucherItem.Service = this->ServiceName;
    //PreVoucherItem.OrderID
    PreVoucherItem.UUID = SecurityHelper::UUIDtoMD5();
    PreVoucherItem.Desc = AssetItem.slbName;
    PreVoucherItem.UnitPrice = AssetItem.NewUnitPrice;
    PreVoucherItem.Qty = _qty;
    PreVoucherItem.SubTotal = AssetItem.SubTotal;
    PreVoucherItem.Discount = Discount3;
    PreVoucherItem.DisAmount = (Discount3.ID > 0 ? Discount3.Amount : 0);
    PreVoucherItem.VATPercent = NULLABLE_GET_OR_DEFAULT(AssetItem.prdVAT, 0); // * 100;
    PreVoucherItem.VATAmount = (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount) * PreVoucherItem.VATPercent / 100;
    //PreVoucherItem.Sign

    CreateQuery qry = CreateQuery(*this->AccountUserAssets)
        .addCols({
                     tblAccountUserAssetsBase::Fields::uas_usrID,
                     tblAccountUserAssetsBase::Fields::uas_slbID,
                     tblAccountUserAssetsBase::Fields::uasQty,
//                     tblAccountUserAssetsBase::Fields::uas_vchID,
                     tblAccountUserAssetsBase::Fields::uasVoucherItemUUID,
//                     tblAccountUserAssetsBase::Fields::uasPrefered,
//                     tblAccountUserAssetsBase::Fields::uasOrderDateTime,
//                     tblAccountUserAssetsBase::Fields::uasStatus,
                 })
    ;

    QVariantMap values;
    values = {
        { tblAccountUserAssetsBase::Fields::uas_usrID, CurrentUserID },
        { tblAccountUserAssetsBase::Fields::uas_slbID, AssetItem.slbID },
        { tblAccountUserAssetsBase::Fields::uasQty, _qty },
//        { tblAccountUserAssetsBase::Fields::uas_vchID, ??? },
        { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, PreVoucherItem.UUID },
//            { tblAccountUserAssetsBase::Fields::uasPrefered, ??? },
//        { tblAccountUserAssetsBase::Fields::uasOrderDateTime, DBExpression::NOW() },
//            { tblAccountUserAssetsBase::Fields::uasStatus, },
    };

    if (Discount3.ID > 0) {
        qry.addCols({
                        tblAccountUserAssetsBase::Fields::uas_cpnID,
                        tblAccountUserAssetsBase::Fields::uasDiscountAmount,
                    })
        ;

        values.insert(tblAccountUserAssetsBase::Fields::uas_cpnID, Discount3.ID);
        values.insert(tblAccountUserAssetsBase::Fields::uasDiscountAmount, Discount3.Amount);
    }

    qry.values(values);

    //-- --------------------------------
    PreVoucherItem.OrderID = qry.execute(CurrentUserID);

    //-- --------------------------------
    PreVoucherItem.Sign = QString(voucherSign(QJsonDocument(PreVoucherItem.toJson()).toJson()).toBase64());

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

    _lastPreVoucher.Round = static_cast<quint16>(FinalPrice % 1000);
    _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
    _lastPreVoucher.Sign.clear();
    _lastPreVoucher.Sign = QString(voucherSign(QJsonDocument(_lastPreVoucher.toJson()).toJson()).toBase64());

    return _lastPreVoucher;
}

Targoman::API::AAA::stuPreVoucher IMPL_REST_POST(intfAccountingBasedModule, removeBasketItem, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
//    quint64 _orderID, //it is uasID
    TAPI::MD5_t _itemUUID,
    Targoman::API::AAA::stuPreVoucher _lastPreVoucher
)) {
    checkPreVoucherSanity(_lastPreVoucher);

    if (_lastPreVoucher.Items.isEmpty())
        throw exHTTPInternalServerError("pre-voucher items is empty.");

    quint64 CurrentUserID = _APICALLBOOM.getUserID();
    if (_lastPreVoucher.UserID != CurrentUserID)
        throw exHTTPBadRequest("invalid pre-Voucher owner");

    bool Found = false;
    qint64 FinalPrice = 0;

    auto iter = _lastPreVoucher.Items.begin();
    while (iter != _lastPreVoucher.Items.end()) {
        stuVoucherItem PreVoucherItem = *iter;

        if (PreVoucherItem.UUID == _itemUUID) {
            Found = true;

            //delete voucher item
            if (this->cancelVoucherItem(_APICALLBOOM,
                                        _APICALLBOOM.getUserID(),
                                        PreVoucherItem,
                                        [](const QVariantMap &_userAssetInfo) -> bool {
                                            TAPI::enuAuditableStatus::Type UserAssetStatus = TAPI::enuAuditableStatus::toEnum(_userAssetInfo.value(tblAccountUserAssetsBase::Fields::uasStatus, TAPI::enuAuditableStatus::Pending).toString());
                                            return (UserAssetStatus == TAPI::enuAuditableStatus::Pending);
                                        }) == false) //not pending
                throw exHTTPInternalServerError("only Pending items can be removed from pre-voucher.");

            //remove item
            iter = _lastPreVoucher.Items.erase(iter);

            //preventing ++iter
            continue; //continue for computing final price
        } else
            FinalPrice += (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount + PreVoucherItem.VATAmount);

        ++iter;
    }

    if (Found) {
        if (_lastPreVoucher.Items.isEmpty())
            _lastPreVoucher.Summary = "";
        else if (_lastPreVoucher.Items.size() > 1)
            _lastPreVoucher.Summary = QString("%1 items").arg(_lastPreVoucher.Items.size());
        else {
            auto item = _lastPreVoucher.Items.first();
            _lastPreVoucher.Summary = QString("%1 of %2").arg(item.Qty).arg(item.Desc);
        }

        _lastPreVoucher.Round = static_cast<quint16>(FinalPrice % 1000);
        _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
        _lastPreVoucher.Sign.clear();
        _lastPreVoucher.Sign = QString(voucherSign(QJsonDocument(_lastPreVoucher.toJson()).toJson()).toBase64());

        return _lastPreVoucher;
    }

    throw exHTTPInternalServerError("item not found in pre-voucher items.");
}

/*
Targoman::API::AAA::stuPreVoucher IMPL_REST_POST(intfAccountingBasedModule, updateBasketItem, (
        APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
        TAPI::MD5_t _itemUUID,
        quint16 _new_qty, ///TODO: float
        Targoman::API::AAA::stuPreVoucher _lastPreVoucher
    )) {
    checkPreVoucherSanity(_lastPreVoucher);

    if (_lastPreVoucher.Items.isEmpty())
        throw exHTTPInternalServerError("pre-voucher items is empty.");

    if (_new_qty == 0)
        throw exHTTPInternalServerError("Qty must be greater than zero.");

    clsJWT JWT(_JWT);
    quint64 currentUserID = JWT.usrID();

    bool Found = false;
    qint64 FinalPrice = 0;

    auto iter = _lastPreVoucher.Items.begin();
    while (iter != _lastPreVoucher.Items.end()) {
        stuVoucherItem &PreVoucherItem = *iter;

        if (PreVoucherItem.UUID == _itemUUID) {
            Found = true;

            if (_new_qty == PreVoucherItem.Qty)
                throw exHTTPInternalServerError("Current Qty and new Qty can not be equal.");

            //check uasStatus
            QVariantMap UserAssetInfo = SelectQuery(*this->AccountUserAssets)
                                        .addCols({
                                                    tblAccountUserAssetsBase::Fields::uasID,
                                                    tblAccountUserAssetsBase::Fields::uas_slbID,
                                                    tblAccountUserAssetsBase::Fields::uasStatus,
                                                 })
                                        .where({ tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, enuConditionOperator::Equal, PreVoucherItem.UUID })
                                        .one();
            TAPI::enuAuditableStatus::Type UserAssetStatus = TAPI::enuAuditableStatus::toEnum(UserAssetInfo.value(tblAccountUserAssetsBase::Fields::uasStatus, TAPI::enuAuditableStatus::Pending).toString());
            if (UserAssetStatus != TAPI::enuAuditableStatus::Pending)
                throw exHTTPInternalServerError("only Pending items of pre-voucher can be modify.");

            //
            if (_new_qty < PreVoucherItem.Qty) {

            } else { //_new_qty > PreVoucherItem.Qty

            }

            //update voucher item in _lastPreVoucher.Items
            ///TODO: re-apply additives
            PreVoucherItem.Qty = _new_qty;

            PreVoucherItem.SubTotal = AssetItem.SubTotal;
            PreVoucherItem.Discount = Discount;
            PreVoucherItem.DisAmount = (Discount.ID > 0 ? Discount.Amount : 0);
            PreVoucherItem.VATPercent = NULLABLE_GET_OR_DEFAULT(AssetItem.prdVAT, 0); // * 100;
            PreVoucherItem.VATAmount = (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount) * PreVoucherItem.VATPercent / 100;



            //update voucher item in Module/UsetAssets
            UpdateQuery(*this->AccountUserAssets)
                    .set(tblAccountUserAssetsBase::
                        tblAccountUserAssetsBase::Fields::uasID)
                                        .addCols({
                                                    tblAccountUserAssetsBase::Fields::uas_slbID,
                                                    tblAccountUserAssetsBase::Fields::uasStatus,
                                                 })
                                        .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, PreVoucherItem.OrderID })
                                        .one();

            if (this->cancelVoucherItem(currentUserID, PreVoucherItem, [](const QVariantMap &_userAssetInfo) -> bool
                {
                    TAPI::enuAuditableStatus::Type UserAssetStatus = TAPI::enuAuditableStatus::toEnum(_userAssetInfo.value(tblAccountUserAssetsBase::Fields::uasStatus, TAPI::enuAuditableStatus::Pending).toString());

                    if (UserAssetStatus != TAPI::enuAuditableStatus::Pending)
                        return false;

                    return true;
                }) == false) //not pending
            {
                throw exHTTPInternalServerError("only Pending items can be removed from pre-voucher.");
            }
        }

        //compute FinalPrice for all items
        FinalPrice += (PreVoucherItem.SubTotal - PreVoucherItem.DisAmount + PreVoucherItem.VATAmount);

        ++iter;
    }

    if (Found) {
        if (_lastPreVoucher.Items.size() > 1)
            _lastPreVoucher.Summary = QString("%1 items").arg(_lastPreVoucher.Items.size());
        else {
            auto item = _lastPreVoucher.Items.first();
            _lastPreVoucher.Summary = QString("%1 of %2").arg(item.Qty).arg(item.Desc);
        }

        _lastPreVoucher.Round = static_cast<quint16>(FinalPrice % 1000);
        _lastPreVoucher.ToPay = static_cast<quint32>(FinalPrice) - _lastPreVoucher.Round;
        _lastPreVoucher.Sign.clear();
        _lastPreVoucher.Sign = QString(Accounting::voucherSign(QJsonDocument(_lastPreVoucher.toJson()).toJson()).toBase64());

        return _lastPreVoucher;
    }

    throw exHTTPInternalServerError("item not found in pre-voucher items.");
}
*/

/******************************************************************\
|** procerss and cancel voucher item ******************************|
\******************************************************************/
bool intfAccountingBasedModule::increaseDiscountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem
) {
    if (_voucherItem.DisAmount > 0) {
        clsDACResult Result = this->AccountCoupons->callSP(APICALLBOOM_PARAM,
                                                           "spCoupon_IncreaseStats", {
                                                               { "iDiscountID", _voucherItem.Discount.ID },
                                                               { "iTotalUsedCount", 1 },
                                                               { "iTotalUsedAmount", _voucherItem.DisAmount },
                                                           });
    }

    return true;
}

bool intfAccountingBasedModule::decreaseDiscountUsage(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem
) {
    if (_voucherItem.DisAmount > 0) {
        clsDACResult Result = this->AccountCoupons->callSP(APICALLBOOM_PARAM,
                                                           "spCoupon_DecreaseStats", {
                                                               { "iDiscountID", _voucherItem.Discount.ID },
                                                               { "iTotalUsedCount", 1 },
                                                               { "iTotalUsedAmount", _voucherItem.DisAmount },
                                                           });
    }

    return true;
}

bool intfAccountingBasedModule::activateUserAsset(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem,
    quint64 _voucherID
) {
    return this->Update(*this->AccountUserAssets,
                        APICALLBOOM_PARAM,
                        /*PK*/ QString("%1").arg(_voucherItem.OrderID),
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
    return this->DeleteByPks(
        *this->AccountUserAssets,
        APICALLBOOM_PARAM,
        /*PK*/ QString("%1").arg(_voucherItem.OrderID),
        {
            { tblAccountUserAssetsBase::Fields::uasVoucherItemUUID, _voucherItem.UUID }, //this is just for make condition strong
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
    if (!this->preProcessVoucherItem(APICALLBOOM_PARAM, _voucherItem, _voucherID))
        return false;

    if (this->activateUserAsset(APICALLBOOM_PARAM, _voucherItem, _voucherID) == false)
        return false;

    this->increaseDiscountUsage(APICALLBOOM_PARAM, _voucherItem);

    this->postProcessVoucherItem(APICALLBOOM_PARAM, _voucherItem, _voucherID);

    return true;
}

bool intfAccountingBasedModule::cancelVoucherItem(
    INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
    quint64 _userID,
    const Targoman::API::AAA::stuVoucherItem &_voucherItem,
    std::function<bool(const QVariantMap &_userAssetInfo)> _checkUserAssetLambda
) {
    if (!this->preCancelVoucherItem(APICALLBOOM_PARAM, _voucherItem))
        return false;

    QVariantMap UserAssetInfo = SelectQuery(*this->AccountUserAssets)
                                .addCols({
                                             tblAccountUserAssetsBase::Fields::uasID,
                                             tblAccountUserAssetsBase::Fields::uas_slbID,
                                             tblAccountUserAssetsBase::Fields::uasStatus,
                                         })
                                .where({ tblAccountUserAssetsBase::Fields::uasID, enuConditionOperator::Equal, _voucherItem.OrderID })
                                .one();

    if ((_checkUserAssetLambda != nullptr) && (_checkUserAssetLambda(UserAssetInfo) == false))
        return false;

    TAPI::enuAuditableStatus::Type UserAssetStatus = TAPI::enuAuditableStatus::toEnum(UserAssetInfo.value(tblAccountUserAssetsBase::Fields::uasStatus, TAPI::enuAuditableStatus::Pending).toString());

    if (UserAssetStatus == TAPI::enuAuditableStatus::Active)
        this->decreaseDiscountUsage(APICALLBOOM_PARAM, _voucherItem);

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

    this->postCancelVoucherItem(APICALLBOOM_PARAM, _voucherItem);

    return true;
}

void checkVoucherItemForTrustedActionSanity(stuVoucherItemForTrustedAction &_data) {
    if (_data.UserID <= 0)
        throw exHTTPBadRequest("Invalid user id");

    if (_data.VoucherID <= 0)
        throw exHTTPBadRequest("Invalid voucher id");

    QString Sign = _data.Sign;
    _data.Sign.clear();
    if (Sign != QString(voucherSign(QJsonDocument(_data.toJson()).toJson()).toBase64()))
        throw exHTTPBadRequest("Invalid sign");

    //--------------------
    Sign = _data.VoucherItem.Sign;
    _data.VoucherItem.Sign.clear();

    if (Sign != QString(voucherSign(QJsonDocument(_data.VoucherItem.toJson()).toJson()).toBase64()))
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
