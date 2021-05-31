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
 */

#include "Accounting.h"
#include "PrivHelpers.h"
#include "Server/ServerConfigs.h"
#include "Interfaces/AAA/Authorization.h"
#include "Server/clsSimpleCrypt.h"
#include "Server/QtTypes.hpp"

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

stuActiveServiceAccount intfRESTAPIWithAccounting::activeAccountObject(quint64 _usrID)
{
    return this->findActiveAccount(_usrID);
}

intfRESTAPIWithAccounting::intfRESTAPIWithAccounting(const QString& _schema,
                                                   const QString& _module,
                                                   AssetUsageLimitsCols_t _AssetUsageLimitsCols,
                                                   intfAccountSaleables* _saleables,
                                                   intfAccountUserAssets* _userAssets,
                                                   intfAccountAssetUsage* _assetusage,
                                                   intfAccountCoupons* _discounts,
                                                   intfAccountPrizes* _prizes) :
    ORM::clsRESTAPIWithActionLogs(_schema, _module),
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
{;}

stuActiveServiceAccount intfRESTAPIWithAccounting::findActiveAccount(quint64 _usrID, const ServiceUsage_t& _requestedUsage)
{
    stuServiceAccountInfo AccountInfo = this->retrieveServiceAccountInfo(_usrID);
    if(AccountInfo.ActivePackages.isEmpty())
        return stuActiveServiceAccount();

    QDateTime Now = QDateTime::currentDateTime();

    auto effectiveStartTime = [Now](const stuAssetItem& _assetItem) -> QDateTime { return QDateTime(Now.date()).addMSecs(_assetItem.StartTime.msecsSinceStartOfDay());};
    auto effectiveEndTime  = [Now](const stuAssetItem& _assetItem) -> QDateTime{
        return _assetItem.StartTime < _assetItem.EndTime ?
                    QDateTime(Now.date()).addMSecs(_assetItem.EndTime.msecsSinceStartOfDay())
                  : _assetItem.EndDate == Now.date() ?
                        QDateTime(Now.date().addDays(1)) :
                        QDateTime(Now.date().addDays(1)).addMSecs(_assetItem.EndTime.msecsSinceStartOfDay()) ;
    };
    auto isInvalidPackage = [this, Now, effectiveStartTime, effectiveEndTime, _requestedUsage](const stuAssetItem& _package) -> bool{
        if((_package.StartDate.isValid() && _package.StartDate > Now.date())
           ||(_package.EndDate.isValid() && _package.EndDate < Now.date())
           ||(_package.StartTime.isValid() && Now < effectiveStartTime(_package))
           ||(_package.EndTime.isValid() && Now > effectiveEndTime(_package))
           ||this->isEmpty(_package.Remaining)
           )
            return false;
        if(_requestedUsage.size()){
            for(auto UsageIter = _requestedUsage.begin();
                UsageIter != _requestedUsage.end();
                UsageIter++){
                if(_package.Remaining.contains(UsageIter.key()) == false)
                    continue;
                if(this->isUnlimited(_package.Remaining) == false){
                    stuUsage Remaining = _package.Remaining.value(UsageIter.key());
                    if((Remaining.PerDay >= 0 && Remaining.PerDay - UsageIter.value() <= 0)
                       ||(Remaining.PerWeek >= 0 && Remaining.PerWeek - UsageIter.value() <= 0)
                       ||(Remaining.PerMonth >= 0 && Remaining.PerMonth - UsageIter.value() <= 0)
                       ||(Remaining.Total >= 0 && Remaining.Total - UsageIter.value() <= 0))
                        return false;
                }
            }
        }
        return true;
    };

    if(AccountInfo.PreferedPackage.isNull() == false && isInvalidPackage(*AccountInfo.PreferedPackage) == false)
        return stuActiveServiceAccount(*AccountInfo.PreferedPackage,
                                       AccountInfo.ParentID ? true : false,
                                       AccountInfo.MyLimitsOnParent,
                                       -1);

    QList<stuAssetItem> CandidatePackages;
    auto comparePackages = [this, &effectiveEndTime] (const stuAssetItem& a, stuAssetItem& b) {
        if(a.EndDate.isValid() && b.EndDate.isValid() == false) return -1;
        if(a.EndDate.isValid() == false && b.EndDate.isValid()) return 1;
        if(this->isUnlimited(a.Remaining) && this->isUnlimited(b.Remaining) == false) return -1;
        if(this->isUnlimited(a.Remaining) == false && this->isUnlimited(b.Remaining)) return 1;
        if(a.EndTime.isValid() && b.EndTime.isValid() == false) return -1;
        if(a.EndTime.isValid() == false && b.EndTime.isValid()) return 1;
        if(a.EndTime.isValid() && b.EndTime.isValid()) {
            if(effectiveEndTime(a) < effectiveEndTime(b)) return -1;
            if(effectiveEndTime(a) > effectiveEndTime(b)) return 1;
        }
        if(a.EndDate.isValid() && b.EndDate.isValid() && a.EndDate != b.EndDate)
            return b.EndDate > a.EndDate ? -1 : 1;
        return 0;
    };

    for(auto AccountIter = AccountInfo.ActivePackages.begin();
        AccountIter != AccountInfo.ActivePackages.end();
        AccountIter++){
        const stuAssetItem& Package = AccountIter.value();

        if(isInvalidPackage(Package))
            continue;

        if(CandidatePackages.isEmpty()){
            CandidatePackages.append(Package);
            continue;
        }

        bool Inserted = false;
        for(auto CandidateIter = CandidatePackages.begin();
            CandidateIter != CandidatePackages.end();
            CandidateIter++){
            if( comparePackages(Package, *CandidateIter) <0) {
                this->breakPackage(CandidateIter->PackageID);
                CandidatePackages.insert(CandidateIter, Package);
                Inserted = true;
                break;
            }
        }
        if(Inserted == false)
            CandidatePackages.append(Package);
    }

    if(CandidatePackages.isEmpty()){
        if(_requestedUsage.size())
            throw exPaymentRequired("You don't have any active account");

        return stuActiveServiceAccount();
    }

    const stuAssetItem& ActivePackage = CandidatePackages.first();
    QDateTime NextDigestTime;
    if(ActivePackage.EndDate.isValid()){
        if(ActivePackage.EndTime.isValid())
            NextDigestTime = effectiveEndTime(ActivePackage);
        else
            NextDigestTime = QDateTime(ActivePackage.EndDate.addDays(1));
    }else if (ActivePackage.EndTime.isValid())
        NextDigestTime = effectiveEndTime(ActivePackage);

    return stuActiveServiceAccount(ActivePackage,
                                   AccountInfo.ParentID ? true : false,
                                   AccountInfo.MyLimitsOnParent,
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

    QVariantMap SaleableInfo = this->AccountSaleables->selectFromTable(
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
    ).toMap();

#define SET_FIELD_FROM_VARIANMAP(_fieldName, _table) \
    TAPI::setFromVariant(AssetItem._fieldName            ,SaleableInfo.value(_table::_fieldName))

    stuAssetItem AssetItem;
    SET_FIELD_FROM_VARIANMAP(prdCode            , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdName            , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidFromDate   , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidToDate     , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidFromHour   , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdValidToHour     , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdPrivs           , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdVAT             , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdInStockCount    , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdOrderedCount    , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdReturnedCount   , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(prdStatus          , tblAccountProductsBase);
    SET_FIELD_FROM_VARIANMAP(slbID              , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbCode            , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbName            , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbPrivs           , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbBasePrice       , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbAdditives       , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbInStockCount    , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbOrderedCount    , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbReturnedCount   , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbVoucherTemplate , tblAccountSaleablesBase);
    SET_FIELD_FROM_VARIANMAP(slbStatus          , tblAccountSaleablesBase);

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
                Iter->PerDay.size() ? SaleableInfo.value(Iter->PerDay).toInt() : -1,
                Iter->PerWeek.size() ? SaleableInfo.value(Iter->PerWeek).toInt() : -1,
                Iter->PerMonth.size() ? SaleableInfo.value(Iter->PerMonth).toInt() : -1,
                Iter->Total.size() ? SaleableInfo.value(Iter->Total).toInt() : -1,
            }
        );
    AssetItem.Digested.Limits = SaleableUsageLimits;

    //---------
    this->digestPrivs(_JWT, AssetItem);

    this->applyAssetAdditives(_JWT, AssetItem, _orderAdditives);

    this->applyReferrer(_JWT, AssetItem, _referrer, _extraRefererParams);


    //TODO: modiriyate takhfif dar kenare poole jayeze :D


    //---------
    stuDiscount Discount;
    if (_discountCode.size()) {
        QVariantMap DiscountInfo = this->AccountCoupons->selectFromTable(
            {},
            QString("%1<=NOW()").arg(tblAccountCouponsBase::cpnValidFrom),
            _discountCode,
            0,
            1,
            QStringList({
                tblAccountCouponsBase::cpnID,
                tblAccountCouponsBase::cpnType,
                tblAccountCouponsBase::cpnSaleableBasedAmount,
                tblAccountCouponsBase::cpnMaxAmount,
                tblAccountCouponsBase::cpnPrimaryCount,
                tblAccountCouponsBase::cpnUsedCount,
            }).join(',')
        ).toMap();

        if (DiscountInfo.value(tblAccountCouponsBase::cpnExpiryTime).toDateTime() < QDateTime::currentDateTime())
            throw exHTTPBadRequest("Discount code has been expired");
        if (DiscountInfo.value(tblAccountCouponsBase::cpnPrimaryCount).toInt() <= DiscountInfo.value(tblAccountCouponsBase::cpnUsedCount).toInt())
            throw exHTTPBadRequest("Discount code has been finished");

        qint64 Amount = DiscountInfo.value(tblAccountCouponsBase::cpnSaleableBasedAmount).toJsonObject().value(_saleableCode).toInt(-1);
        if (Amount < 0)
            throw exHTTPBadRequest("Discount code is not valid on selected package");
        Discount.Amount = static_cast<quint32>(Amount);
        Discount.ID = DiscountInfo.value(tblAccountCouponsBase::cpnPrimaryCount).toULongLong();
        Discount.Name = _discountCode;
        Discount.Type = static_cast<TAPI::enuDiscountType::Type>(DiscountInfo.value(tblAccountCouponsBase::cpnType).toInt());
        Discount.MaxAmount = DiscountInfo.value(tblAccountCouponsBase::cpnMaxAmount).toUInt();
    }

    stuVoucherItem PreVoucherItem;
    PreVoucherItem.Service = this->ServiceName;
    ///TODO add ttl for order item
    PreVoucherItem.OrderID = this->AccountUserAssets->create(clsJWT(_JWT).usrID(), TAPI::ORMFields_t({
                                                                {tblAccountUserAssetsBase::uas_usrID, clsJWT(_JWT).usrID()},
                                                                {tblAccountUserAssetsBase::uas_slbID, Package.PackageID},
                                                            })).toULongLong();
    PreVoucherItem.Desc  = SaleableInfo.value(tblAccountSaleablesBase::Name).toString();

    ///TODO PreVoucherItem.DMInfo : json {"type":"adver", "additives":[{"color":"red"}, {"size":"m"}, ...]}
    /// used for DMLogic::applyCoupon -> match item.DMInfo by coupon rules
    /// return: amount of using coupon

    PreVoucherItem.Qty = _qty;
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

    return _lastPreVoucher;
}

void intfRESTAPIWithAccounting::hasCredit(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage)
{
    QJsonObject Privs = _jwt.privsObject();
    if(Privs.contains(this->ServiceName) == false)
        throw exHTTPForbidden("[81] You don't have access to: " + this->ServiceName);

    stuActiveServiceAccount ActiveServiceAccount = this->findActiveAccount(clsJWT(_jwt).usrID(), _requestedUsage);
    if(ActiveServiceAccount.TTL == 0)
        throw exHTTPForbidden("[82] You don't have access to: " + this->ServiceName);

    auto checkCredit = [](auto _usageIter, auto _remaining, auto _type) {
        if(_remaining.PerDay >= 0 && _remaining.PerDay - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Day:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.PerWeek >= 0 && _remaining.PerWeek - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Week:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.PerMonth >= 0 && _remaining.PerMonth - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Month:%2>").arg(_type).arg(_usageIter.key()));
        if(_remaining.Total >= 0 && _remaining.Total - _usageIter.value() <= 0)
            throw exPaymentRequired(QString("You have not enough credit: <%1:Total:%2>").arg(_type).arg(_usageIter.key()));
    };

    const stuAssetItem& Package = ActiveServiceAccount.ActivePackage;
    stuUsage CurrUsage;
    if(ActiveServiceAccount.IsFromParent){
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++){
            if(Package.Remaining.contains(UsageIter.key()) == false)
                continue;
            if(this->isUnlimited(ActiveServiceAccount.MyLimitsOnParent) == false)
                checkCredit(UsageIter, ActiveServiceAccount.MyLimitsOnParent.value(UsageIter.key()), "Own");

            checkCredit(UsageIter, Package.Remaining.value(UsageIter.key()), "Parent");
        }
        return;
    }else if(this->isUnlimited(Package.Remaining))
        return;
    else{
        for(auto UsageIter = _requestedUsage.begin();
            UsageIter != _requestedUsage.end();
            UsageIter++){
            if(Package.Remaining.contains(UsageIter.key()) == false)
                continue;
            checkCredit(UsageIter, Package.Remaining.value(UsageIter.key()), "Self");
        }
    }
}

}
}
}
}

