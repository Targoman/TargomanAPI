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

#ifndef TARGOMAN_API_AAA_ACCOUNTING_H
#define TARGOMAN_API_AAA_ACCOUNTING_H

#include "Interfaces/AAA/Accounting_Interfaces.h"

namespace Targoman {
namespace API {
namespace AAA {
namespace Accounting {

class intfRESTAPIWithAccounting : public ORM::clsRESTAPIWithActionLogs {
    Q_OBJECT

public:
    stuActiveCredit activeAccountObject(quint64 _usrID);

protected:
    intfRESTAPIWithAccounting(const QString& _schema,
                             const QString& _module,
                             AssetUsageLimitsCols_t _AssetUsageLimitsCols,
                             intfAccountProducts* _products,
                             intfAccountSaleables* _saleables,
                             intfAccountUserAssets* _userAssets,
                             intfAccountAssetUsage* _assetusage,
                             intfAccountCoupons* _discounts = nullptr,
                             intfAccountPrizes* _prizes = nullptr);
    virtual ~intfRESTAPIWithAccounting();
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID) = 0;
    virtual void breakCredit(quint64 _slbID) = 0;
    virtual bool isUnlimited(const UsageLimits_t& _limits) const = 0;
    virtual bool isEmpty(const UsageLimits_t& _limits) const = 0;

    virtual void digestPrivs(TAPI::JWT_t _JWT,
                             INOUT stuAssetItem& _assetItem) {
        Q_UNUSED(_JWT);
        Q_UNUSED(_assetItem)
    };
    virtual void applyAssetAdditives(TAPI::JWT_t _JWT,
                                     INOUT stuAssetItem& _assetItem,
                                     const OrderAdditives_t& _orderAdditives) {
        Q_UNUSED(_JWT);
        Q_UNUSED(_assetItem)
        Q_UNUSED(_orderAdditives)
    };
    virtual void applyReferrer(TAPI::JWT_t _JWT,
                               INOUT stuAssetItem& AssetItem,
                               QString _referrer,
                               TAPI::JSON_t _extraRefererParams) {
        Q_UNUSED(_JWT);
        Q_UNUSED(AssetItem);
        Q_UNUSED(_referrer);
        Q_UNUSED(_extraRefererParams);
    };

    void checkUsageIsAllowed(const clsJWT& _jwt, const ServiceUsage_t& _requestedUsage);

private:
    stuActiveCredit findBestMatchedCredit(quint64 _usrID, const ServiceUsage_t& _requestedUsage = {});

private slots:
    TAPI::stuPreVoucher REST(POST, addToBasket, (TAPI::JWT_t _JWT,
                                                 TAPI::SaleableCode_t _saleableCode,
                                                 TAPI::OrderAdditives_t _orderAdditives = {},
                                                 quint16 _qty = 1,
                                                 TAPI::CouponCode_t _discountCode = {},
                                                 QString _referrer = {},
                                                 TAPI::JSON_t _extraRefererParams = {},
                                                 TAPI::stuPreVoucher _lastPreVoucher = {}),
                                "add a package to basket and return updated pre-Voucher")
    ///TODO removeFromBasket
    ///TODO updateItemInBasket

protected:
    QScopedPointer<intfAccountProducts> AccountProducts;
    QScopedPointer<intfAccountSaleables> AccountSaleables;
    QScopedPointer<intfAccountUserAssets> AccountUserAssets;
    QScopedPointer<intfAccountAssetUsage> AccountAssetUsage;
    QScopedPointer<intfAccountCoupons> AccountCoupons;
    QScopedPointer<intfAccountPrizes> AccountPrizes;

private:
    QString ServiceName;

    AssetUsageLimitsCols_t AssetUsageLimitsCols;
    QStringList AssetUsageLimitsColsName;

};

extern intfRESTAPIWithAccounting* serviceAccounting(const QString& _serviceName);

}
}
}
}

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
