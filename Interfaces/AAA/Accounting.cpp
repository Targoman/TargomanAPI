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

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting{

using namespace Common;
using namespace Common::Configuration;

static QMap<QString, clsRESTAPIWithAccounting*> ServiceRegistry;

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

clsRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName)
{
    Q_UNUSED(serviceAccounting);
    return ServiceRegistry.value(_serviceName);
}

stuActiveServiceAccount clsRESTAPIWithAccounting::activeAccountObject(quint32 _usrID)
{
    return this->findActiveAccount(_usrID);
}

clsRESTAPIWithAccounting::clsRESTAPIWithAccounting(const QString& _schema,
                                                   const QString& _module,
                                                   PackageRemainingCols_t _packageRemainingCols,
                                                   intfAccountSaleables* _packages,
                                                   intfAccountUserAssets* _userPackages,
                                                   intfAccountAssetUsage* _usage,
                                                   intfAccountCoupons* _discounts,
                                                   intfAccountPrizes* _prizes) :
    ORM::clsRESTAPIWithActionLogs(_schema, _module),
    AccountSaleables(_packages),
    AccountUserAssets(_userPackages),
    AccountAssetUsage(_usage),
    AccountCoupons(_discounts),
    AccountPrizes(_prizes),
    PackageRemainingCols(_packageRemainingCols)
{
    ServiceRegistry.insert(_schema, this);
    foreach(auto Col, this->PackageRemainingCols){
        if(Col.PerDay.size()) this->PackageRemainingColName.append(Col.PerDay);
        if(Col.PerWeek.size()) this->PackageRemainingColName.append(Col.PerWeek);
        if(Col.PerMonth.size()) this->PackageRemainingColName.append(Col.PerMonth);
        if(Col.Total.size()) this->PackageRemainingColName.append(Col.Total);
    }
}

clsRESTAPIWithAccounting::~clsRESTAPIWithAccounting()
{;}

stuActiveServiceAccount clsRESTAPIWithAccounting::findActiveAccount(quint32 _usrID, const ServiceUsage_t& _requestedUsage)
{
    stuServiceAccountInfo AccountInfo = this->retrieveServiceAccountInfo(_usrID);
    if(AccountInfo.ActivePackages.isEmpty())
        return stuActiveServiceAccount();

    QDateTime Now = QDateTime::currentDateTime();

    auto effectiveStartTime = [Now](const stuPackage& _package) -> QDateTime { return QDateTime(Now.date()).addMSecs(_package.StartTime.msecsSinceStartOfDay());};
    auto effectiveEndTime  = [Now](const stuPackage& _package) -> QDateTime{
        return _package.StartTime < _package.EndTime ?
                    QDateTime(Now.date()).addMSecs(_package.EndTime.msecsSinceStartOfDay())
                  : _package.EndDate == Now.date() ?
                        QDateTime(Now.date().addDays(1)) :
                        QDateTime(Now.date().addDays(1)).addMSecs(_package.EndTime.msecsSinceStartOfDay()) ;
    };
    auto isInvalidPackage = [this, Now, effectiveStartTime, effectiveEndTime, _requestedUsage](const stuPackage& _package) -> bool{
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

    QList<stuPackage> CandidatePackages;
    auto comparePackages = [this, &effectiveEndTime] (const stuPackage& a, stuPackage& b) {
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
        const stuPackage& Package = AccountIter.value();

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

    const stuPackage& ActivePackage = CandidatePackages.first();
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

TAPI::stuPreVoucher clsRESTAPIWithAccounting::apiPOSTaddToBasket(TAPI::JWT_t _JWT,
                                                                 TAPI::PackageCode_t _packageCode,
                                                                 ///TODO: addittive
                                                                 qint16 _qty,
                                                                 TAPI::DiscountCode_t _discountCode,
                                                                 QString _referer,
                                                                 TAPI::JSON_t _extraRefererParams,
                                                                 TAPI::stuPreVoucher _lastPreVoucher)
{
    ///TODO reserve product and discount : bikhial
    Accounting::checkPreVoucherSanity(_lastPreVoucher);
    QString ExtraFilters = QString ("( %1>=NOW() + %2<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min)")
                           .arg(tblAccountSaleablesBase::slbCanBePurchasedSince)
                           .arg(tblAccountSaleablesBase::slbNotAvailableSince);

    ///TODO tell qty to select and join userAssets to count pending saleables -> reject addToBasket
    ///TODO: fetch available addittives
    QVariantMap PackageInfo = this->AccountSaleables->selectFromTable({}, ExtraFilters, _packageCode, 0, 1,
                                                                     QStringList({
                                                                                     tblAccountSaleablesBase::slbID,
                                                                                     tblAccountSaleablesBase::slbCode,
                                                                                     tblAccountSaleablesBase::slbName,
                                                                                     tblAccountSaleablesBase::slbRemainingDays,
                                                                                     tblAccountSaleablesBase::slbPrice,
                                                                                     tblAccountSaleablesBase::slbValidFromDate,
                                                                                     tblAccountSaleablesBase::slbValidToDate,
                                                                                     tblAccountSaleablesBase::slbValidFromTime,
                                                                                     tblAccountSaleablesBase::slbValidToTime,
                                                                                     tblAccountSaleablesBase::slbPrivs,
                                                                                     tblAccountSaleablesBase::slbQuantity,
                                                                                     tblAccountSaleablesBase::slbVAT,
                                                                                     this->PackageRemainingColName.join(',')
                                                                                 }).join(',')
                                                                     ).toMap();

    PackageRemaining_t PackageOffer;
    for(auto PackageRemIter = this->PackageRemainingCols.begin();
        PackageRemIter != this->PackageRemainingCols.end();
        PackageRemIter++)
        PackageOffer.insert(PackageRemIter.key(), {
                                PackageRemIter->PerDay.size() ? PackageInfo.value(PackageRemIter->PerDay).toInt() : -1,
                                PackageRemIter->PerWeek.size() ? PackageInfo.value(PackageRemIter->PerWeek).toInt() : -1,
                                PackageRemIter->PerMonth.size() ? PackageInfo.value(PackageRemIter->PerMonth).toInt() : -1,
                                PackageRemIter->Total.size() ? PackageInfo.value(PackageRemIter->Total).toInt() : -1,
                            });

    ///TODO: apply addittives
    stuPackage Package(
                PackageInfo.value(tblAccountSaleablesBase::slbID).toUInt(),
                PackageInfo.value(tblAccountSaleablesBase::slbCode).toString(),
                PackageInfo.value(tblAccountSaleablesBase::slbRemainingDays).toInt(),
                -1,
                PackageOffer,
                PackageInfo.value(tblAccountSaleablesBase::slbValidFromDate).toDate(),
                PackageInfo.value(tblAccountSaleablesBase::slbValidToDate).toDate(),
                PackageInfo.value(tblAccountSaleablesBase::slbValidFromTime).toTime(),
                PackageInfo.value(tblAccountSaleablesBase::slbValidToTime).toTime(),
                QJsonObject::fromVariantMap(PackageInfo.value(tblAccountSaleablesBase::slbPrivs).toMap())
                );

    ///TODO: apply addittives to discounts
    stuDiscount Discount;
    if(_discountCode.size()){
        ExtraFilters = QString("%1 >= NOW()").arg(tblAccountCoupons::cpnValidFrom);
        QVariantMap DiscountInfo = this->AccountCoupons->selectFromTable({}, ExtraFilters, _discountCode, 0, 1,
                                                                           QStringList({
                                                                                           tblAccountCoupons::cpnID,
                                                                                           tblAccountCoupons::cpnCode,
                                                                                           tblAccountCoupons::cpnType,
                                                                                           tblAccountCoupons::cpnStatus,
                                                                                           tblAccountCoupons::cpnPackageBasedAmount,
                                                                                           tblAccountCoupons::cpnMaxAmount,
                                                                                           tblAccountCoupons::cpnPrimaryCount,
                                                                                           tblAccountCoupons::cpnUsedCount,
                                                                                           tblAccountCoupons::cpnValidFrom,
                                                                                       }).join(',')
                                                                           ).toMap();
        if(DiscountInfo.value(tblAccountCoupons::cpnExpiryTime).toDateTime() < QDateTime::currentDateTime())
            throw exHTTPBadRequest("Discount code has been expired");
        if(DiscountInfo.value(tblAccountCoupons::cpnPrimaryCount).toInt() <= DiscountInfo.value(tblAccountCoupons::cpnUsedCount).toInt())
            throw exHTTPBadRequest("Discount code has been finished");

        qint64 Amount = DiscountInfo.value(tblAccountCoupons::cpnPackageBasedAmount).toJsonObject().value(_packageCode).toInt(-1);
        if(Amount <0)
            throw exHTTPBadRequest("Discount code is not valid on selected package");
        Discount.Amount = static_cast<quint32>(Amount);
        Discount.ID = DiscountInfo.value(tblAccountCoupons::cpnPrimaryCount).toULongLong();
        Discount.Name = _discountCode;
        Discount.Type = static_cast<TAPI::enuDiscountType::Type>(DiscountInfo.value(tblAccountCoupons::cpnType).toInt());
        Discount.MaxAmount = DiscountInfo.value(tblAccountCoupons::cpnMaxAmount).toUInt();
    }

    if(_referer.size()){
        Q_UNUSED(_JWT)
        Q_UNUSED (_extraRefererParams)
        //TODO check for prize on referers
    }

    stuVoucherItem PreVoucherItem;
    PreVoucherItem.Service = this->ServiceName;
    ///TODO add ttl for order item
    PreVoucherItem.OrderID = this->AccountUserAssets->create(clsJWT(_JWT).usrID(), TAPI::ORMFields_t({
                                                                {tblAccountUserAssets::uas_usrID, clsJWT(_JWT).usrID()},
                                                                {tblAccountUserAssets::uas_slbID, Package.PackageID},
                                                            })).toULongLong();
    PreVoucherItem.Desc  = PackageInfo.value(tblAccountSaleablesBase::Name).toString();

    ///TODO PreVoucherItem.DMInfo : json {"type":"adver", "additives":[{"color":"red"}, {"size":"m"}, ...]}
    /// used for DMLogic::applyCoupon -> match item.DMInfo by coupon rules
    /// return: amount of using coupon

    PreVoucherItem.Qty = _qty;
    PreVoucherItem.UnitPrice = PackageInfo.value(tblAccountSaleablesBase::slbPrice).toUInt();
    PreVoucherItem.SubTotal = PreVoucherItem.UnitPrice * PreVoucherItem.Qty;
    PreVoucherItem.Discount = Discount;
    if (Discount.Amount)
        switch(Discount.Type){
        case TAPI::enuDiscountType::Free: PreVoucherItem.DisAmount = PreVoucherItem.SubTotal; break;
        case TAPI::enuDiscountType::Amount: PreVoucherItem.DisAmount = qMin(PreVoucherItem.SubTotal, Discount.Amount); break;
        case TAPI::enuDiscountType::Percent: PreVoucherItem.DisAmount = qMin(Discount.MaxAmount, static_cast<quint32>(floor(PreVoucherItem.SubTotal * Discount.Amount / 100.)));
        }
    //TODO rename Tax to VAT
    PreVoucherItem.VATPercent = PackageInfo.value(tblAccountSaleablesBase::slbVAT).toUInt();
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

void clsRESTAPIWithAccounting::hasCredit(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage)
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

    const stuPackage& Package = ActiveServiceAccount.ActivePackage;
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

