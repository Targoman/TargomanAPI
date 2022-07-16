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

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AdvertModule, enuProductType);

using namespace Targoman::API::AAA;

namespace Targoman::API::AdvertModule::ORM {

/******************************************************/
AccountUnits::AccountUnits() :
    intfAccountUnits(
        AdvertSchema,
        tblAccountUnits::Private::ExtraORMFields,
        tblAccountUnits::Private::ExtraRelations,
        tblAccountUnits::Private::ExtraIndexes
) {
    AccountUnitsTranslate::instance();
}

AccountUnitsTranslate::AccountUnitsTranslate() :
    intfAccountUnitsTranslate(
        AdvertSchema,
        tblAccountUnitsTranslate::Private::ExtraORMFields,
        tblAccountUnitsTranslate::Private::ExtraRelations,
        tblAccountUnitsTranslate::Private::ExtraIndexes
) { ; }

/******************************************************/
AccountProducts::AccountProducts() :
    intfAccountProducts(
        AdvertSchema,
        tblAccountProducts::Private::ExtraORMFields,
        tblAccountProducts::Private::ExtraRelations,
        tblAccountProducts::Private::ExtraIndexes
) {
    AccountProductsTranslate::instance();
}

AccountProductsTranslate::AccountProductsTranslate() :
    intfAccountProductsTranslate(
        AdvertSchema,
        tblAccountProductsTranslate::Private::ExtraORMFields,
        tblAccountProductsTranslate::Private::ExtraRelations,
        tblAccountProductsTranslate::Private::ExtraIndexes
) { ; }

/******************************************************/
AccountSaleables::AccountSaleables() :
    intfAccountSaleables(
        AdvertSchema,
        tblAccountSaleables::Private::ExtraORMFields,
        tblAccountSaleables::Private::ExtraRelations,
        tblAccountSaleables::Private::ExtraIndexes
) {
    AccountSaleablesTranslate::instance();
}

AccountSaleablesTranslate::AccountSaleablesTranslate() :
    intfAccountSaleablesTranslate(
        AdvertSchema,
        tblAccountSaleablesTranslate::Private::ExtraORMFields,
        tblAccountSaleablesTranslate::Private::ExtraRelations,
        tblAccountSaleablesTranslate::Private::ExtraIndexes
) { ; }

/******************************************************/
AccountUserAssets::AccountUserAssets() :
    intfAccountUserAssets(
        AdvertSchema,
        tblAccountUserAsset::Private::ExtraORMFields,
        tblAccountUserAsset::Private::ExtraRelations,
        tblAccountUserAsset::Private::ExtraIndexes
) { ; }

/******************************************************/
AccountAssetUsage::AccountAssetUsage() :
    intfAccountAssetUsage(
        AdvertSchema,
        tblAccountAssetUsage::Private::ExtraORMFields,
        tblAccountAssetUsage::Private::ExtraRelations,
        tblAccountAssetUsage::Private::ExtraIndexes
) { ; }

/******************************************************/
AccountCoupons::AccountCoupons() :
    intfAccountCoupons(
        AdvertSchema,
        tblAccountCoupons::Private::ExtraORMFields,
        tblAccountCoupons::Private::ExtraRelations,
        tblAccountCoupons::Private::ExtraIndexes
) { ; }

} // namespace Targoman::API::AdvertModule::ORM
