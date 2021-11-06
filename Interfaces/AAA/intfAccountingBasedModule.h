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
#include "Interfaces/API/intfSQLBasedWithActionLogsModule.h"

using namespace TAPI;

namespace Targoman::API::AAA {

class intfAccountingBasedModule : public API::intfSQLBasedWithActionLogsModule
{
    Q_OBJECT

protected:
    intfAccountingBasedModule(
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

//    virtual ~intfAccountingBasedModule();

public:
    virtual stuActiveCredit activeAccountObject(quint64 _usrID);

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID) = 0;
    virtual void breakCredit(quint64 _slbID) = 0;
    virtual bool isUnlimited(const UsageLimits_t& _limits) const = 0;
    virtual bool isEmpty(const UsageLimits_t& _limits) const = 0;

    void checkUsageIsAllowed(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage);

    virtual bool increaseDiscountUsage(const Targoman::API::AAA::stuVoucherItem &_voucherItem);
    virtual bool decreaseDiscountUsage(const Targoman::API::AAA::stuVoucherItem &_voucherItem);
    virtual bool activateUserAsset(quint64 _userID, const Targoman::API::AAA::stuVoucherItem &_voucherItem, quint64 _voucherID);
    virtual bool removeFromUserAssets(quint64 _userID, const Targoman::API::AAA::stuVoucherItem &_voucherItem);

    virtual bool preProcessVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem,
            quint64 _voucherID
        ) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); Q_UNUSED(_voucherID); return true; };
    virtual bool processVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem,
            quint64 _voucherID
        );
    virtual bool postProcessVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem,
            quint64 _voucherID
        ) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); Q_UNUSED(_voucherID); return true; };

    virtual bool preCancelVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem
        ) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };
    virtual bool cancelVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem,
            std::function<bool(const QVariantMap &_userAssetInfo)> _checkUserAssetLambda = nullptr
        );
    virtual bool postCancelVoucherItem(
            quint64 _userID,
            const Targoman::API::AAA::stuVoucherItem &_voucherItem
        ) { Q_UNUSED(_userID); Q_UNUSED(_voucherItem); return true; };

private:
    stuActiveCredit findBestMatchedCredit(quint64 _usrID, const ServiceUsage_t& _requestedUsage = {});

protected slots:
    Targoman::API::AAA::stuPreVoucher REST_POST(
        addToBasket,
        (
            TAPI::JWT_t _JWT,
            TAPI::SaleableCode_t _saleableCode,
            Targoman::API::AAA::OrderAdditives_t _orderAdditives = {},
            qreal _qty = 1,
            TAPI::CouponCode_t _discountCode = {},
            QString _referrer = {},
            TAPI::JSON_t _extraReferrerParams = {},
            Targoman::API::AAA::stuPreVoucher _lastPreVoucher = {}
        ),
        "add a package to basket and return updated pre-Voucher"
    )

    Targoman::API::AAA::stuPreVoucher REST_POST(
        removeBasketItem,
        (
            TAPI::JWT_t _JWT,
            TAPI::MD5_t _itemUUID,
            Targoman::API::AAA::stuPreVoucher _lastPreVoucher
        ),
        "Remove a package from basket and return updated pre-Voucher."
        "Only Pending items can be removed."
    )

//    Targoman::API::AAA::stuPreVoucher REST_POST(
//        updateBasketItem,
//        (
//            TAPI::JWT_t _JWT,
//            TAPI::MD5_t _itemUUID,
//            quint16 _new_qty,
//            Targoman::API::AAA::stuPreVoucher _lastPreVoucher
//        ),
//        "Update a package from basket and return updated pre-Voucher."
//        "Only Pending items can be modify."
//    )

    bool REST_POST(
        processVoucherItem,
        (
            TAPI::JWT_t _JWT,
            Targoman::API::AAA::stuVoucherItem _voucherItem,
            quint64 _voucherID
        ),
        "Process voucher item"
    )

    bool REST_POST(
        cancelVoucherItem,
        (
            TAPI::JWT_t _JWT,
            Targoman::API::AAA::stuVoucherItem _voucherItem
        ),
        "Cancel voucher item"
    )

protected:
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

protected:
    QString ServiceName;

protected:
    QScopedPointer<intfAccountProducts> AccountProducts;
    QScopedPointer<intfAccountSaleables> AccountSaleables;
    QScopedPointer<intfAccountUserAssets> AccountUserAssets;
    QScopedPointer<intfAccountAssetUsage> AccountAssetUsages;
    QScopedPointer<intfAccountCoupons> AccountCoupons;
    QScopedPointer<intfAccountPrizes> AccountPrizes;

protected:
    AssetUsageLimitsCols_t AssetUsageLimitsCols;
    QStringList AssetUsageLimitsColsName;
};

extern intfAccountingBasedModule* serviceAccounting(const QString& _serviceName);

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
