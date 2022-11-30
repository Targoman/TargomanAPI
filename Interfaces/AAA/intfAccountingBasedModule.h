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
#include "Interfaces/API/intfSQLBasedModule.h"

using namespace TAPI;

namespace Targoman::API::AAA {

using namespace API;

/******************************************************/
class baseintfAccountingBasedModule : public intfSQLBasedModule
{
    Q_OBJECT

public:
    baseintfAccountingBasedModule(
        const QString &_module,
        const QString &_schema,
        AssetUsageLimitsCols_t              _AssetUsageLimitsCols,
        intfAccountUnits                    *_units,
        intfAccountProducts                 *_products,
        intfAccountSaleables                *_saleables,
        intfAccountSaleablesFiles           *_saleablesFiles,
        baseintfAccountUserAssets           *_userAssets,
        intfAccountUserAssetsFiles          *_userAssetsFiles,
        baseintfAccountAssetUsage           *_assetUsage,
        intfAccountCoupons                  *_discounts = nullptr,
        intfAccountPrizes                   *_prizes = nullptr
    );

public:
    virtual bool IsTokenBase() = 0;

//    virtual stuActiveCredit activeAccountObject(
//        INTFAPICALLCONTEXT_DECL &_apiCallContext,
//        quint64 _actorID);

protected:
    virtual stuServiceCreditsInfo retrieveServiceCreditsInfo(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        quint64 _actorID,
        const ServiceUsage_t &_requestedUsage = {},
        const QString &_action = {}
    ) = 0;

    virtual void breakCredit(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const stuAssetItem &_assetItem,
        const QString &_action = {}
    ) = 0;

    virtual bool isUnlimited(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const UsageLimits_t &_limits
    ) const = 0;

    virtual bool isEmpty(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const UsageLimits_t &_limits
    ) const = 0;

    stuActiveCredit findBestMatchedCredit(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        quint64 _actorID,
        const ServiceUsage_t &_requestedUsage = {},
        const QString &_action = {});

public:
    stuActiveCredit checkUsageIsAllowed(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const ServiceUsage_t &_requestedUsage,
        const QString &_action = {}
    );

    virtual void saveAccountUsage(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        stuActiveCredit &_activeCredit,
        const ServiceUsage_t &_requestedUsage,
        const QString &_action = {}
    ) = 0;

protected:
    //-- used by addToBasket: ----------------------------------------
    virtual void processItemForBasket(
        INTFAPICALLCONTEXT_DECL    &_apiCallContext,
        INOUT stuBasketItem     &_basketItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual void digestPrivs(
        INTFAPICALLCONTEXT_DECL    &_apiCallContext,
        INOUT stuAssetItem      &_assetItem
//        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual void computeAdditives(
                      INTFAPICALLCONTEXT_IMPL  &_apiCallContext,
        Q_DECL_UNUSED INOUT stuBasketItem   &_basketItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { ; }

    virtual void computeReferrer(
                      INTFAPICALLCONTEXT_IMPL  &_apiCallContext,
        Q_DECL_UNUSED INOUT stuBasketItem   &_basketItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { ; }

    virtual void computeSystemDiscount(
        INTFAPICALLCONTEXT_DECL            &_apiCallContext,
        INOUT stuBasketItem             &_basketItem,
        const stuPendingSystemDiscount  &_pendingSystemDiscount = {},
        const stuVoucherItem            *_oldVoucherItem = nullptr
    );

    virtual void computeCouponDiscount(
        INTFAPICALLCONTEXT_DECL    &_apiCallContext,
        INOUT stuBasketItem     &_basketItem,
        const stuVoucherItem    *_oldVoucherItem = nullptr
    );

    virtual QVariantMap getCustomUserAssetFieldsForQuery(
                      INTFAPICALLCONTEXT_IMPL  &_apiCallContext,
        Q_DECL_UNUSED INOUT stuBasketItem   &_basketItem,
        Q_DECL_UNUSED const stuVoucherItem  *_oldVoucherItem = nullptr
    ) { return {}; }

    //-- used by processVoucherItem and cancelVoucherItem: ----------------------------------------
    virtual bool increaseDiscountUsage(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );
    virtual bool decreaseDiscountUsage(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );
    virtual bool activateUserAsset(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        quint64 _voucherID
    );
    virtual bool removeFromUserAssets(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem
    );

    //-- processVoucherItem: ----------------------------------------
    virtual bool preProcessVoucherItem(
                      INTFAPICALLCONTEXT_IMPL &_apiCallContext,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        Q_DECL_UNUSED quint64 _voucherID
    ) {
        return true;
    };
    virtual bool processVoucherItem(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        quint64 _userID,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        quint64 _voucherID
    );
    virtual bool postProcessVoucherItem(
                      INTFAPICALLCONTEXT_IMPL &_apiCallContext,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        Q_DECL_UNUSED quint64 _voucherID
    ) {
        return true;
    };

    //-- cancelVoucherItem: ----------------------------------------
    virtual bool preCancelVoucherItem(
                      INTFAPICALLCONTEXT_IMPL &_apiCallContext,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem
    ) {
        return true;
    };
    virtual bool cancelVoucherItem(
        INTFAPICALLCONTEXT_DECL &_apiCallContext,
        quint64 _userID,
        const Targoman::API::AAA::stuVoucherItem &_voucherItem,
        std::function<bool(const QVariantMap &_userAssetInfo)> _fnCheckUserAsset = nullptr
    );
    virtual bool postCancelVoucherItem(
                      INTFAPICALLCONTEXT_IMPL &_apiCallContext,
        Q_DECL_UNUSED quint64 _userID,
        Q_DECL_UNUSED const Targoman::API::AAA::stuVoucherItem &_voucherItem
    ) {
        return true;
    };


protected:
    Targoman::API::AAA::stuBasketActionResult internalAddToBasket(
        APICALLCONTEXT_TYPE_JWT_USER_DECL          &_apiCallContext,
        stuBasketItem                           &_basketItem,
        TAPI::SaleableCode_t                    &_saleableCode,
        Targoman::API::AAA::OrderAdditives_t    &_orderAdditives,
        qreal                                   &_qty,
        TAPI::CouponCode_t                      &_discountCode,
        QString                                 &_referrer,
        TAPI::JSON_t                            &_referrerParams,
//        QString                                 &_apiToken,
        NULLABLE_TYPE(quint64)                  _apiTokenID,
        Targoman::API::AAA::stuPreVoucher       &_lastPreVoucher
    );

    Targoman::API::AAA::stuBasketActionResult internalUpdateBasketItem(
        INTFAPICALLCONTEXT_DECL                &_apiCallContext,
        Targoman::API::AAA::stuPreVoucher   &_lastPreVoucher,
        stuVoucherItem                      &_voucherItem,
        qreal                               _newQty,
        NULLABLE_TYPE(TAPI::CouponCode_t)   _newDiscountCode = NULLABLE_NULL_VALUE
    );

protected slots:
    Targoman::API::AAA::stuBasketActionResult REST_POST(
        updateBasketItem,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL      &_apiCallContext,
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
            APICALLCONTEXT_TYPE_JWT_USER_DECL      &_apiCallContext,
            Targoman::API::AAA::stuPreVoucher   _lastPreVoucher,
            TAPI::MD5_t                         _itemUUID
        ),
        "Remove an item from basket and return updated pre-Voucher."
        "Only Pending items can be removed."
    )

protected slots:
    bool REST_POST(
        processVoucherItem,
        (
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext,
            Targoman::API::AAA::stuVoucherItemForTrustedAction _data
        ),
        "Process voucher item"
    )

    bool REST_POST(
        cancelVoucherItem,
        (
            APICALLCONTEXT_TYPE_JWT_ANONYMOUSE_DECL &_apiCallContext,
            Targoman::API::AAA::stuVoucherItemForTrustedAction _data
        ),
        "Cancel voucher item"
    )

public:
    intfAccountUnits*                   accountUnits()              { return this->AccountUnits.data(); }
    intfAccountProducts*                accountProducts()           { return this->AccountProducts.data(); }
    intfAccountSaleables*               accountSaleables()          { return this->AccountSaleables.data(); }
    intfAccountSaleablesFiles*          accountSaleablesFiles()     { return this->AccountSaleablesFiles.data(); }
    baseintfAccountUserAssets*          accountUserAssets()         { return this->AccountUserAssets.data(); }
    intfAccountUserAssetsFiles*         accountUserAssetsFiles()    { return this->AccountUserAssetsFiles.data(); }
    baseintfAccountAssetUsage*          accountAssetUsage()        { return this->AccountAssetUsage.data(); }
    intfAccountCoupons*                 accountCoupons()            { return this->AccountCoupons.data(); }
    intfAccountPrizes*                  accountPrizes()             { return this->AccountPrizes.data(); }

protected:
    QString ServiceName;

    QScopedPointer<intfAccountUnits>                    AccountUnits;
    QScopedPointer<intfAccountProducts>                 AccountProducts;
    QScopedPointer<intfAccountSaleables>                AccountSaleables;
    QScopedPointer<intfAccountSaleablesFiles>           AccountSaleablesFiles;
    QScopedPointer<baseintfAccountUserAssets>           AccountUserAssets;
    QScopedPointer<intfAccountUserAssetsFiles>          AccountUserAssetsFiles;
    QScopedPointer<baseintfAccountAssetUsage>           AccountAssetUsage;
    QScopedPointer<intfAccountCoupons>                  AccountCoupons;
    QScopedPointer<intfAccountPrizes>                   AccountPrizes;

    AssetUsageLimitsCols_t AssetUsageLimitsCols;
//    QStringList AssetUsageLimitsColsName;
};

/******************************************************/
class baseintfAccountingBasedModule_USER : public baseintfAccountingBasedModule
{
    Q_OBJECT

protected:
    baseintfAccountingBasedModule_USER(
        const QString                       &_module,
        const QString                       &_schema,
        AssetUsageLimitsCols_t              _AssetUsageLimitsCols,
        intfAccountUnits                    *_units,
        intfAccountProducts                 *_products,
        intfAccountSaleables                *_saleables,
        intfAccountSaleablesFiles           *_saleablesFiles,
        intfAccountUserAssets<false>        *_userAssets,
        intfAccountUserAssetsFiles          *_userAssetsFiles,
        intfAccountAssetUsage<false>        *_assetUsage,
        intfAccountCoupons                  *_discounts = nullptr,
        intfAccountPrizes                   *_prizes = nullptr
    );

protected:
    inline bool IsTokenBase() {
        return false;
    }

protected slots:
    Targoman::API::AAA::stuBasketActionResult REST_POST(
        addToBasket,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL          &_apiCallContext,
            TAPI::SaleableCode_t                    _saleableCode,
            Targoman::API::AAA::OrderAdditives_t    _orderAdditives = {},
            qreal                                   _qty = 1,
            TAPI::CouponCode_t                      _discountCode = {},
            QString                                 _referrer = {},
            TAPI::JSON_t                            _referrerParams = {},
            Targoman::API::AAA::stuPreVoucher       _lastPreVoucher = {}
        ),
        "add an item to the basket and return updated pre-Voucher"
    )
};

/******************************************************/
class baseintfAccountingBasedModule_API : public baseintfAccountingBasedModule
{
    Q_OBJECT

protected:
    baseintfAccountingBasedModule_API(
        const QString                       &_module,
        const QString                       &_schema,
        AssetUsageLimitsCols_t              _AssetUsageLimitsCols,
        intfAccountUnits                    *_units,
        intfAccountProducts                 *_products,
        intfAccountSaleables                *_saleables,
        intfAccountSaleablesFiles           *_saleablesFiles,
        intfAccountUserAssets<true>         *_userAssets,
        intfAccountUserAssetsFiles          *_userAssetsFiles,
        intfAccountAssetUsage<true>         *_assetUsage,
        intfAccountCoupons                  *_discounts = nullptr,
        intfAccountPrizes                   *_prizes = nullptr
    );

protected:
    inline bool IsTokenBase() {
        return true;
    }

protected slots:
    Targoman::API::AAA::stuBasketActionResult REST_POST(
        addToBasket,
        (
            APICALLCONTEXT_TYPE_JWT_USER_DECL          &_apiCallContext,
            QString                                 _apiToken,
            TAPI::SaleableCode_t                    _saleableCode,
            Targoman::API::AAA::OrderAdditives_t    _orderAdditives = {},
            qreal                                   _qty = 1,
            TAPI::CouponCode_t                      _discountCode = {},
            QString                                 _referrer = {},
            TAPI::JSON_t                            _referrerParams = {},
            Targoman::API::AAA::stuPreVoucher       _lastPreVoucher = {}
        ),
        "add an item to the basket and return updated pre-Voucher"
    )
};

/******************************************************/
template <bool _itmplIsTokenBase>
class intfAccountingBasedModule : public std::conditional<_itmplIsTokenBase, baseintfAccountingBasedModule_API, baseintfAccountingBasedModule_USER>::type
{
protected:
    intfAccountingBasedModule(
        const QString                                   &_module,
        const QString                                   &_schema,
        AssetUsageLimitsCols_t                          _AssetUsageLimitsCols,
        intfAccountUnits                                *_units,
        intfAccountProducts                             *_products,
        intfAccountSaleables                            *_saleables,
        intfAccountSaleablesFiles                       *_saleablesFiles,
        intfAccountUserAssets<_itmplIsTokenBase>        *_userAssets,
        intfAccountUserAssetsFiles                      *_userAssetsFiles,
        intfAccountAssetUsage<_itmplIsTokenBase>        *_assetUsage,
        intfAccountCoupons                              *_discounts = nullptr,
        intfAccountPrizes                               *_prizes = nullptr
    );
};

extern baseintfAccountingBasedModule* serviceAccounting(const QString& _serviceName);

} //namespace Targoman::API::AAA

#endif // TARGOMAN_API_AAA_ACCOUNTING_H
