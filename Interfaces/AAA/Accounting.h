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

#ifndef TARGOMAN_API_AAA_ACCOUNTING_H
#define TARGOMAN_API_AAA_ACCOUNTING_H

#include "Interfaces/AAA/Accounting_Interfaces.h"

using namespace TAPI;

namespace Targoman::API::AAA::Accounting {

class intfRESTAPIWithAccounting : public ORM::clsRESTAPIWithActionLogs
{
    Q_OBJECT

protected:
    intfRESTAPIWithAccounting(
            const QString& _module,
            const QString& _schema,
            AssetUsageLimitsCols_t _AssetUsageLimitsCols,
            intfAccountProducts* _products,
            intfAccountSaleables* _saleables,
            intfAccountUserAssets* _userAssets,
            intfAccountAssetUsage* _assetUsages,
            intfAccountCoupons* _discounts = nullptr,
            intfAccountPrizes* _prizes = nullptr
            );

    virtual ~intfRESTAPIWithAccounting();

public:
    stuActiveCredit activeAccountObject(quint64 _usrID);

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID) = 0;
    virtual void breakCredit(quint64 _slbID) = 0;
    virtual bool isUnlimited(const UsageLimits_t& _limits) const = 0;
    virtual bool isEmpty(const UsageLimits_t& _limits) const = 0;

    virtual void digestPrivs(TAPI::JWT_t _JWT,
                             INOUT stuAssetItem& _assetItem)
    {
        Q_UNUSED(_JWT);
        Q_UNUSED(_assetItem)
    };
    virtual void applyAssetAdditives(TAPI::JWT_t _JWT,
                                     INOUT stuAssetItem& _assetItem,
                                     const OrderAdditives_t& _orderAdditives)
    {
        Q_UNUSED(_JWT);
        Q_UNUSED(_assetItem)
        Q_UNUSED(_orderAdditives)
    };
    virtual void applyReferrer(TAPI::JWT_t _JWT,
                               INOUT stuAssetItem& AssetItem,
                               QString _referrer,
                               TAPI::JSON_t _extraReferrerParams)
    {
        Q_UNUSED(_JWT);
        Q_UNUSED(AssetItem);
        Q_UNUSED(_referrer);
        Q_UNUSED(_extraReferrerParams);
    };

    void checkUsageIsAllowed(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage);

    virtual bool increaseDiscountUsage(const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem);
    virtual bool decreaseDiscountUsage(const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem);
    virtual bool activateUserAsset(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem);
    virtual bool removeFromUserAssets(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem);

    virtual bool preProcessVoucherItem(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };
    virtual bool processVoucherItem(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem);
    virtual bool postProcessVoucherItem(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };

    virtual bool preCancelVoucherItem(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };
    virtual bool cancelVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem,
            std::function<bool(const QVariantMap &_userAssetInfo)> _checkUserAssetLambda = nullptr);
    virtual bool postCancelVoucherItem(quint64 _userID, const Targoman::API::AAA::Accounting::stuVoucherItem &_voucherItem) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };

private:
    stuActiveCredit findBestMatchedCredit(quint64 _usrID, const ServiceUsage_t& _requestedUsage = {});

protected slots:
    bool REST(
        POST,
        processVoucherItem,
        (
            TAPI::JWT_t _JWT,
            Targoman::API::AAA::Accounting::stuVoucherItem _voucherItem
        ),
        "Process voucher item"
    )

    bool REST(
        POST,
        cancelVoucherItem,
        (
            TAPI::JWT_t _JWT,
            Targoman::API::AAA::Accounting::stuVoucherItem _voucherItem
        ),
        "Cancel voucher item"
    )

    Targoman::API::AAA::Accounting::stuPreVoucher REST(
        POST,
        addToBasket,
        (
            TAPI::JWT_t _JWT,
            TAPI::SaleableCode_t _saleableCode,
            Targoman::API::AAA::Accounting::OrderAdditives_t _orderAdditives = {},
            quint16 _qty = 1,
            TAPI::CouponCode_t _discountCode = {},
            QString _referrer = {},
            TAPI::JSON_t _extraReferrerParams = {},
            Targoman::API::AAA::Accounting::stuPreVoucher _lastPreVoucher = {}
        ),
        "add a package to basket and return updated pre-Voucher"
    )

    Targoman::API::AAA::Accounting::stuPreVoucher REST(
        POST,
        removeBasketItem,
        (
            TAPI::JWT_t _JWT,
            TAPI::MD5_t _itemUUID,
            Targoman::API::AAA::Accounting::stuPreVoucher _lastPreVoucher
        ),
        "Remove a package from basket and return updated pre-Voucher."
        "Only Pending items can be removed."
    )

//    Targoman::API::AAA::Accounting::stuPreVoucher REST(
//        POST,
//        updateBasketItem,
//        (
//            TAPI::JWT_t _JWT,
//            TAPI::MD5_t _itemUUID,
//            quint16 _new_qty,
//            Targoman::API::AAA::Accounting::stuPreVoucher _lastPreVoucher
//        ),
//        "Update a package from basket and return updated pre-Voucher."
//        "Only Pending items can be modify."
//    )

private:
    QString ServiceName;

protected:
    QScopedPointer<intfAccountProducts> AccountProducts;
    QScopedPointer<intfAccountSaleables> AccountSaleables;
    QScopedPointer<intfAccountUserAssets> AccountUserAssets;
    QScopedPointer<intfAccountAssetUsage> AccountAssetUsages;
    QScopedPointer<intfAccountCoupons> AccountCoupons;
    QScopedPointer<intfAccountPrizes> AccountPrizes;

private:
    AssetUsageLimitsCols_t AssetUsageLimitsCols;
    QStringList AssetUsageLimitsColsName;
};

extern intfRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName);

} //namespace Targoman::API::AAA::Accounting

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
