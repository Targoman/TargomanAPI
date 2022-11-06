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

#include "intfMTAccounting.h"

namespace Targoman::API::ModuleHelpers::MT::Interfaces {

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountUnitsI18NMT::intfAccountUnitsI18NMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountUnitsI18N(
        _schema,
        tblAccountUnitsI18NMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountUnitsI18NMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountUnitsI18NMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
intfAccountUnitsMT::intfAccountUnitsMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountUnits(
        _schema,
        tblAccountUnitsMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountUnitsMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountUnitsMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountProductsI18NMT::intfAccountProductsI18NMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountProductsI18N(
        _schema,
        tblAccountProductsI18NMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountProductsI18NMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountProductsI18NMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
intfAccountProductsMT::intfAccountProductsMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountProducts(
        _schema,
        tblAccountProductsMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountProductsMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountProductsMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountSaleablesI18NMT::intfAccountSaleablesI18NMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountSaleablesI18N(
        _schema,
        tblAccountSaleablesI18NMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountSaleablesI18NMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountSaleablesI18NMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
intfAccountSaleablesMT::intfAccountSaleablesMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountSaleables(
        _schema,
        tblAccountSaleablesMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountSaleablesMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountSaleablesMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
intfAccountSaleablesFilesMT::intfAccountSaleablesFilesMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<DBM::stuDBIndex>& _exclusiveIndexes
) :
    intfAccountSaleablesFiles(
        _schema,
        tblAccountSaleablesFilesMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountSaleablesFilesMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountSaleablesFilesMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
baseintfAccountUserAssetsMT::baseintfAccountUserAssetsMT(
//    const QString& _schema,
//    const QList<DBM::clsORMField>& _exclusiveCols,
//    const QList<DBM::stuRelation>& _exclusiveRelations,
//    const QList<DBM::stuDBIndex>& _exclusiveIndexes
//) :
//    baseintfAccountUserAssets(
//        _schema,
//        _exclusiveCols,
//        _exclusiveRelations,
//        _exclusiveIndexes
    )
{ ; }

/******************************************************************/
template <bool _itmplIsTokenBase>
intfAccountUserAssetsMT<_itmplIsTokenBase>::intfAccountUserAssetsMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfAccountUserAssets<_itmplIsTokenBase>(
        _schema,
        tblAccountUserAssetsMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountUserAssetsMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountUserAssetsMTBase::Private::ExtraIndexes + _exclusiveIndexes
    ),
    baseintfAccountUserAssetsMT()
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountUserAssetsFilesMT::intfAccountUserAssetsFilesMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfAccountUserAssetsFiles(
        _schema,
        tblAccountUserAssetsFilesMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountUserAssetsFilesMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountUserAssetsFilesMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
baseintfAccountAssetUsageMT::baseintfAccountAssetUsageMT(
//    const QString& _schema,
//    const QList<DBM::clsORMField>& _exclusiveCols,
//    const QList<DBM::stuRelation>& _exclusiveRelations,
//    const QList<DBM::stuDBIndex>& _exclusiveIndexes
//) :
//    baseintfAccountAssetUsage(
//        _schema,
//        _exclusiveCols,
//        _exclusiveRelations,
//        _exclusiveIndexes
    )
{ ; }

/******************************************************************/
template <bool _itmplIsTokenBase>
intfAccountAssetUsageMT<_itmplIsTokenBase>::intfAccountAssetUsageMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfAccountAssetUsage<_itmplIsTokenBase>(
        _schema,
        tblAccountAssetUsageMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountAssetUsageMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountAssetUsageMTBase::Private::ExtraIndexes + _exclusiveIndexes
    ),
    baseintfAccountAssetUsageMT()
{ ; }

//template <bool _itmplIsTokenBase>
//QStringList intfAccountAssetUsageMT<_itmplIsTokenBase>::creditFieldNames() {
//    return {
//        tblAccountAssetUsageMTBase::ExtraFields::usgUsedTotalWords,
//    };
//}

/******************************************************************/
/******************************************************************/
/******************************************************************/
intfAccountCouponsMT::intfAccountCouponsMT(
    const QString& _schema,
    const QList<DBM::clsORMField>& _exclusiveCols,
    const QList<DBM::stuRelation>& _exclusiveRelations,
    const QList<stuDBIndex>& _exclusiveIndexes
) :
    intfAccountCoupons(
        _schema,
        tblAccountCouponsMTBase::Private::ExtraORMFields + _exclusiveCols,
        tblAccountCouponsMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
        tblAccountCouponsMTBase::Private::ExtraIndexes + _exclusiveIndexes
    )
{ ; }

/******************************************************************/
/******************************************************************/
/******************************************************************/
//intfAccountPrizesMT::intfAccountPrizesMT(
//    const QString& _schema,
//    const QList<DBM::clsORMField>& _exclusiveCols,
//    const QList<DBM::stuRelation>& _exclusiveRelations,
//    const QList<stuDBIndex>& _exclusiveIndexes
//) :
//    intfAccountPrizes(
//        _schema,
//        tblAccountPrizesMTBase::Private::ExtraORMFields + _exclusiveCols,
//        tblAccountPrizesMTBase::Private::ExtraRelations(_schema) + _exclusiveRelations,
//        tblAccountPrizesMTBase::Private::ExtraIndexes + _exclusiveIndexes
//    )
//{ ; }

/******************************************************/
/******************************************************/
/******************************************************/
//template class intfAccountUnitsMT<false>;
//template class intfAccountUnitsI18NMT<false>;
//template class intfAccountProductsMT<false>;
//template class intfAccountProductsI18NMT<false>;
//template class intfAccountSaleablesMT<false>;
//template class intfAccountSaleablesI18NMT<false>;
//template class intfAccountSaleablesFilesMT<false>;
template class intfAccountUserAssetsMT<false>;
//template class intfAccountUserAssetsFilesMT<false>;
template class intfAccountAssetUsageMT<false>;
//template class intfAccountCouponsMT<false>;
//template class intfAccountPrizesMT<false>;

//template class intfAccountUnitsMT<true>;
//template class intfAccountUnitsI18NMT<true>;
//template class intfAccountProductsMT<true>;
//template class intfAccountProductsI18NMT<true>;
//template class intfAccountSaleablesMT<true>;
//template class intfAccountSaleablesI18NMT<true>;
//template class intfAccountSaleablesFilesMT<true>;
template class intfAccountUserAssetsMT<true>;
//template class intfAccountUserAssetsFilesMT<true>;
template class intfAccountAssetUsageMT<true>;
//template class intfAccountCouponsMT<true>;
//template class intfAccountPrizesMT<true>;

} //namespace Targoman::API::ModuleHelpers::MT::Interfaces
