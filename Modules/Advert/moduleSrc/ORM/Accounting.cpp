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
#include "../Advert.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuProductType);

using namespace Targoman::API::AAA;

namespace Targoman::API::AdvertModule::ORM {

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountUnits)

AccountUnits::AccountUnits() :
    intfAccountUnits(
//        false,
        AdvertSchema,
        tblAccountUnits::Private::ExtraORMFields,
        tblAccountUnits::Private::ExtraRelations,
        tblAccountUnits::Private::ExtraIndexes
) {
    AccountUnitsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountUnitsI18N)

AccountUnitsI18N::AccountUnitsI18N() :
    intfAccountUnitsI18N(
//        false,
        AdvertSchema,
        tblAccountUnitsI18N::Private::ExtraORMFields,
        tblAccountUnitsI18N::Private::ExtraRelations,
        tblAccountUnitsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountProducts)

AccountProducts::AccountProducts() :
    intfAccountProducts(
//        false,
        AdvertSchema,
        tblAccountProducts::Private::ExtraORMFields,
        tblAccountProducts::Private::ExtraRelations,
        tblAccountProducts::Private::ExtraIndexes
) {
    AccountProductsI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountProductsI18N)

AccountProductsI18N::AccountProductsI18N() :
    intfAccountProductsI18N(
//        false,
        AdvertSchema,
        tblAccountProductsI18N::Private::ExtraORMFields,
        tblAccountProductsI18N::Private::ExtraRelations,
        tblAccountProductsI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountSaleables)

AccountSaleables::AccountSaleables() :
    intfAccountSaleables(
//        false,
        AdvertSchema,
        tblAccountSaleables::Private::ExtraORMFields,
        tblAccountSaleables::Private::ExtraRelations,
        tblAccountSaleables::Private::ExtraIndexes
) {
    AccountSaleablesI18N::instance();
}

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountSaleablesI18N)

AccountSaleablesI18N::AccountSaleablesI18N() :
    intfAccountSaleablesI18N(
//        false,
        AdvertSchema,
        tblAccountSaleablesI18N::Private::ExtraORMFields,
        tblAccountSaleablesI18N::Private::ExtraRelations,
        tblAccountSaleablesI18N::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountSaleablesFiles)

AccountSaleablesFiles::AccountSaleablesFiles() :
    intfAccountSaleablesFiles(
//        false,
        AdvertSchema,
        tblAccountSaleablesFiles::Private::ExtraORMFields,
        tblAccountSaleablesFiles::Private::ExtraRelations,
        tblAccountSaleablesFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountUserAssets)

AccountUserAssets::AccountUserAssets() :
    intfAccountUserAssets(
//        false,
        AdvertSchema,
        tblAccountUserAssets::Private::ExtraORMFields,
        tblAccountUserAssets::Private::ExtraRelations,
        tblAccountUserAssets::Private::ExtraIndexes
) { ; }

/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountUserAssetsFiles)

AccountUserAssetsFiles::AccountUserAssetsFiles() :
    intfAccountUserAssetsFiles(
//        false,
        AdvertSchema,
        tblAccountUserAssetsFiles::Private::ExtraORMFields,
        tblAccountUserAssetsFiles::Private::ExtraRelations,
        tblAccountUserAssetsFiles::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountAssetUsage)

AccountAssetUsage::AccountAssetUsage() :
    intfAccountAssetUsage(
//        false,
        AdvertSchema,
        tblAccountAssetUsage::Private::ExtraORMFields,
        tblAccountAssetUsage::Private::ExtraRelations,
        tblAccountAssetUsage::Private::ExtraIndexes
) { ; }

/******************************************************/
/******************************************************/
/******************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Advert, AccountCoupons)

AccountCoupons::AccountCoupons() :
    intfAccountCoupons(
//        false,
        AdvertSchema,
        tblAccountCoupons::Private::ExtraORMFields,
        tblAccountCoupons::Private::ExtraRelations,
        tblAccountCoupons::Private::ExtraIndexes
) { ; }

} // namespace Targoman::API::AdvertModule::ORM
