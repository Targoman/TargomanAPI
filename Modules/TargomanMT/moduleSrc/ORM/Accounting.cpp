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
#include "../TargomanMT.h"

using namespace Targoman::API::AAA;

namespace Targoman::API::TargomanMTModule::ORM {

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountUnits)

AccountUnits::AccountUnits() :
    intfAccountUnits(
//        false,
        TargomanMTSchema,
        tblAccountUnits::Private::ExtraORMFields,
        tblAccountUnits::Private::ExtraRelations,
        tblAccountUnits::Private::ExtraIndexes
) {
    AccountUnitsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountUnitsI18N)

AccountUnitsI18N::AccountUnitsI18N() :
    intfAccountUnitsI18N(
//        false,
        TargomanMTSchema,
        tblAccountUnitsI18N::Private::ExtraORMFields,
        tblAccountUnitsI18N::Private::ExtraRelations,
        tblAccountUnitsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountProducts)

AccountProducts::AccountProducts() :
    intfAccountProducts(
//        false,
        TargomanMTSchema,
        tblAccountProducts::Private::ExtraORMFields,
        tblAccountProducts::Private::ExtraRelations,
        tblAccountProducts::Private::ExtraIndexes
) {
    AccountProductsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountProductsI18N)

AccountProductsI18N::AccountProductsI18N() :
    intfAccountProductsI18N(
//        false,
        TargomanMTSchema,
        tblAccountProductsI18N::Private::ExtraORMFields,
        tblAccountProductsI18N::Private::ExtraRelations,
        tblAccountProductsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountSaleables)

AccountSaleables::AccountSaleables() :
    intfAccountSaleables(
//        false,
        TargomanMTSchema,
        tblAccountSaleables::Private::ExtraORMFields,
        tblAccountSaleables::Private::ExtraRelations,
        tblAccountSaleables::Private::ExtraIndexes
) {
    AccountSaleablesI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountSaleablesI18N)

AccountSaleablesI18N::AccountSaleablesI18N() :
    intfAccountSaleablesI18N(
//        false,
        TargomanMTSchema,
        tblAccountSaleablesI18N::Private::ExtraORMFields,
        tblAccountSaleablesI18N::Private::ExtraRelations,
        tblAccountSaleablesI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountSaleablesFiles)

AccountSaleablesFiles::AccountSaleablesFiles() :
    intfAccountSaleablesFiles(
//        false,
        TargomanMTSchema,
        tblAccountSaleablesFiles::Private::ExtraORMFields,
        tblAccountSaleablesFiles::Private::ExtraRelations,
        tblAccountSaleablesFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountUserAssets)

AccountUserAssets::AccountUserAssets() :
    intfAccountUserAssets(
//        false,
        TargomanMTSchema,
        tblAccountUserAssets::Private::ExtraORMFields,
        tblAccountUserAssets::Private::ExtraRelations,
        tblAccountUserAssets::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountUserAssetsFiles)

AccountUserAssetsFiles::AccountUserAssetsFiles() :
    intfAccountUserAssetsFiles(
//        false,
        TargomanMTSchema,
        tblAccountUserAssetsFiles::Private::ExtraORMFields,
        tblAccountUserAssetsFiles::Private::ExtraRelations,
        tblAccountUserAssetsFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountAssetUsage)

AccountAssetUsage::AccountAssetUsage() :
    intfAccountAssetUsage(
//        false,
        TargomanMTSchema,
        tblAccountAssetUsage::Private::ExtraORMFields,
        tblAccountAssetUsage::Private::ExtraRelations,
        tblAccountAssetUsage::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(TargomanMT, AccountCoupons)

AccountCoupons::AccountCoupons() :
    intfAccountCoupons(
//        false,
        TargomanMTSchema,
        tblAccountCoupons::Private::ExtraORMFields,
        tblAccountCoupons::Private::ExtraRelations,
        tblAccountCoupons::Private::ExtraIndexes
) { ; }

} // namespace Targoman::API::TargomanMTModule::ORM
