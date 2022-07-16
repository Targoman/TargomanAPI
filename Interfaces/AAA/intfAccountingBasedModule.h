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
        const QString           &_module,
        const QString           &_schema,
        AssetUsageLimitsCols_t  _AssetUsageLimitsCols,
        intfAccountUnits        *_units,
    //    intfAccountUnitsTranslate* _unitsTranslate,
        intfAccountProducts     *_products,
    //    intfAccountProductsTranslate* _productsTranslate,
        intfAccountSaleables    *_saleables,
    //    intfAccountSaleablesTranslate* _saleablesTranslate,
        intfAccountUserAssets   *_userAssets,
        intfAccountAssetUsage   *_assetUsages,
        intfAccountCoupons      *_discounts = nullptr,
        intfAccountPrizes       *_prizes = nullptr
    );

//    virtual ~intfAccountingBasedModule();

public:
    virtual stuActiveCredit activeAccountObject(quint64 _usrID);

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(quint64 _usrID) = 0;
    virtual void breakCredit(quint64 _slbID) = 0;
    virtual bool isUnlimited(const UsageLimits_t& _limits) const = 0;
    virtual bool isEmpty(const UsageLimits_t& _limits) const = 0;

    void checkUsageIsAllowed(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const ServiceUsage_t& _requestedUsage
    );

    stuActiveCredit findBestMatchedCredit(quint64 _usrID, const ServiceUsage_t& _requestedUsage = {});

    //-- used by addToBasket: ----------------------------------------
    virtual void processItemForBasket(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual void digestPrivs(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual void computeAdditives(
                      INTFAPICALLBOOM_IMPL  &APICALLBOOM_PARAM,
        Q_DECL_UNUSED INOUT stuAssetItem    &_assetItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { ; }

    virtual void computeReferrer(
                      INTFAPICALLBOOM_IMPL  &APICALLBOOM_PARAM,
        Q_DECL_UNUSED INOUT stuAssetItem    &_assetItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { ; }

    virtual void computeSystemDiscount(
        INTFAPICALLBOOM_DECL            &APICALLBOOM_PARAM,
        INOUT stuAssetItem              &_assetItem,
        const stuPendingSystemDiscount  &_pendingSystemDiscount = {},
        const stuVoucherItem            *_oldVoucherItem = nullptr
    );

    virtual void computeCouponDiscount(
        INTFAPICALLBOOM_DECL    &APICALLBOOM_PARAM,
        INOUT stuAssetItem      &_assetItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual QVariantMap getCustomUserAssetFieldsForQuery(
                      INTFAPICALLBOOM_IMPL  &APICALLBOOM_PARAM,
        Q_DECL_UNUSED INOUT stuAssetItem    &_assetItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { return {}; }

    //-- used by processVoucherItem and cancelVoucherItem: ----------------------------------------
    virtual bool increaseDiscountUsage(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );
    virtual bool decreaseDiscountUsage(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );
    virtual bool activateUserAsset(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        quint64 _voucherID
    );
    virtual bool removeFromUserAssets(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );

    //-- processVoucherItem: ----------------------------------------
    virtual bool preProcessVoucherItem(
                      INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        Q_DECL_UNUSED quint64 _voucherID
    ) {
        return true;
    };
    virtual bool processVoucherItem(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _userID,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        quint64 _voucherID
    );
    virtual bool postProcessVoucherItem(
                      INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        Q_DECL_UNUSED quint64 _voucherID
    ) {
        return true;
    };

    //-- cancelVoucherItem: ----------------------------------------
    virtual bool preCancelVoucherItem(
                      INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem
    ) {
        return true;
    };
    virtual bool cancelVoucherItem(
        INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
        quint64 _userID,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        std::function<bool(const QVariantMap &_userAssetInfo)> _fnCheckUserAsset = nullptr
    );
    virtual bool postCancelVoucherItem(
                      INTFAPICALLBOOM_IMPL &APICALLBOOM_PARAM,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem
    ) {
        return true;
    };

protected slots:
    Targoman::API::AAA::stuBasketActionResult REST_POST(
        addToBasket,
        (
            APICALLBOOM_TYPE_JWT_DECL               &APICALLBOOM_PARAM,
            TAPI::SaleableCode_t                    _saleableCode,
            Targoman::API::AAA::OrderAdditives_t    _orderAdditives = {},
            qreal                                   _qty = 1,
            TAPI::CouponCode_t                      _discountCode = {},
            QString                                 _referrer = {},
            TAPI::JSON_t                            _referrerParams = {},
            Targoman::API::AAA::stuPreVoucher       _lastPreVoucher = {}
//            TAPI::MD5_t                             _parentItemUUID = {}
        ),
        "add an item to the basket and return updated pre-Voucher"
    )

    Targoman::API::AAA::stuBasketActionResult REST_POST(
        updateBasketItem,
        (
            APICALLBOOM_TYPE_JWT_DECL           &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuPreVoucher   _lastPreVoucher,
            TAPI::MD5_t                         _itemUUID,
            qreal                               _newQty,
            NULLABLE_TYPE(TAPI::CouponCode_t)   _newDiscountCode = NULLABLE_NULL_VALUE
        ),
        "Update a basket item and return updated pre-Voucher."
    )

    Targoman::API::AAA::stuBasketActionResult REST_POST(
        removeBasketItem,
        (
            APICALLBOOM_TYPE_JWT_DECL           &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuPreVoucher   _lastPreVoucher,
            TAPI::MD5_t                         _itemUUID
        ),
        "Remove an item from basket and return updated pre-Voucher."
        "Only Pending items can be removed."
    )

protected:
    Targoman::API::AAA::stuBasketActionResult internalUpdateBasketItem(
        INTFAPICALLBOOM_DECL                &APICALLBOOM_PARAM,
        Targoman::API::AAA::stuPreVoucher   &_lastPreVoucher,
        stuVoucherItem                      &_voucherItem,
        qreal                               _newQty,
        NULLABLE_TYPE(TAPI::CouponCode_t)   _newDiscountCode = NULLABLE_NULL_VALUE
    );

protected slots:
    bool REST_POST(
        processVoucherItem,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuVoucherItemForTrustedAction _data
        ),
        "Process voucher item"
    )

    bool REST_POST(
        cancelVoucherItem,
        (
            APICALLBOOM_TYPE_NO_JWT_DECL &APICALLBOOM_PARAM,
            Targoman::API::AAA::stuVoucherItemForTrustedAction _data
        ),
        "Cancel voucher item"
    )

protected:
    QString ServiceName;

    QScopedPointer<intfAccountUnits>        AccountUnits;
//    QScopedPointer<intfAccountUnitsTranslate> AccountUnitsTranslate;
    QScopedPointer<intfAccountProducts>     AccountProducts;
//    QScopedPointer<intfAccountProductsTranslate> AccountProductsTranslate;
    QScopedPointer<intfAccountSaleables>    AccountSaleables;
//    QScopedPointer<intfAccountSaleablesTranslate> AccountSaleablesTranslate;
    QScopedPointer<intfAccountUserAssets>   AccountUserAssets;
    QScopedPointer<intfAccountAssetUsage>   AccountAssetUsages;
    QScopedPointer<intfAccountCoupons>      AccountCoupons;
    QScopedPointer<intfAccountPrizes>       AccountPrizes;

    AssetUsageLimitsCols_t AssetUsageLimitsCols;
    QStringList AssetUsageLimitsColsName;
};

extern intfAccountingBasedModule* serviceAccounting(const QString& _serviceName);

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
