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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "Accounting.h"
#include "../MTShop.h"

using namespace Targoman::API::AAA;

namespace Targoman::API::MTShopModule::ORM {

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountUnits)

AccountUnits::AccountUnits() :
    intfAccountUnits(
        MTShopSchema,
        tblAccountUnits::Private::ExtraORMFields,
        tblAccountUnits::Private::ExtraRelations,
        tblAccountUnits::Private::ExtraIndexes
) {
    AccountUnitsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountUnitsI18N)

AccountUnitsI18N::AccountUnitsI18N() :
    intfAccountUnitsI18N(
        MTShopSchema,
        tblAccountUnitsI18N::Private::ExtraORMFields,
        tblAccountUnitsI18N::Private::ExtraRelations,
        tblAccountUnitsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountProducts)

AccountProducts::AccountProducts() :
    intfAccountProducts(
        MTShopSchema,
        tblAccountProducts::Private::ExtraORMFields,
        tblAccountProducts::Private::ExtraRelations,
        tblAccountProducts::Private::ExtraIndexes
) {
    AccountProductsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountProductsI18N)

AccountProductsI18N::AccountProductsI18N() :
    intfAccountProductsI18N(
        MTShopSchema,
        tblAccountProductsI18N::Private::ExtraORMFields,
        tblAccountProductsI18N::Private::ExtraRelations,
        tblAccountProductsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountSaleables)

AccountSaleables::AccountSaleables() :
    intfAccountSaleables(
        MTShopSchema,
        tblAccountSaleables::Private::ExtraORMFields,
        tblAccountSaleables::Private::ExtraRelations,
        tblAccountSaleables::Private::ExtraIndexes
) {
    AccountSaleablesI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountSaleablesI18N)

AccountSaleablesI18N::AccountSaleablesI18N() :
    intfAccountSaleablesI18N(
        MTShopSchema,
        tblAccountSaleablesI18N::Private::ExtraORMFields,
        tblAccountSaleablesI18N::Private::ExtraRelations,
        tblAccountSaleablesI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountSaleablesFiles)

AccountSaleablesFiles::AccountSaleablesFiles() :
    intfAccountSaleablesFiles(
        MTShopSchema,
        tblAccountSaleablesFiles::Private::ExtraORMFields,
        tblAccountSaleablesFiles::Private::ExtraRelations,
        tblAccountSaleablesFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountUserAssets)

AccountUserAssets::AccountUserAssets() :
    intfAccountUserAssets(
        MTShopSchema,
        tblAccountUserAssets::Private::ExtraORMFields,
        tblAccountUserAssets::Private::ExtraRelations,
        tblAccountUserAssets::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountUserAssetsFiles)

AccountUserAssetsFiles::AccountUserAssetsFiles() :
    intfAccountUserAssetsFiles(
        MTShopSchema,
        tblAccountUserAssetsFiles::Private::ExtraORMFields,
        tblAccountUserAssetsFiles::Private::ExtraRelations,
        tblAccountUserAssetsFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountAssetUsage)

AccountAssetUsage::AccountAssetUsage() :
    intfAccountAssetUsage(
        MTShopSchema,
        tblAccountAssetUsage::Private::ExtraORMFields,
        tblAccountAssetUsage::Private::ExtraRelations,
        tblAccountAssetUsage::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(MTShop, AccountCoupons)

AccountCoupons::AccountCoupons() :
    intfAccountCoupons(
        MTShopSchema,
        tblAccountCoupons::Private::ExtraORMFields,
        tblAccountCoupons::Private::ExtraRelations,
        tblAccountCoupons::Private::ExtraIndexes
) { ; }

} // namespace Targoman::API::MTShopModule::ORM
