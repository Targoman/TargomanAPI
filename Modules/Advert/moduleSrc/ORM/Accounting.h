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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
#define TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H

#include "Interfaces/ORM/clsTable.h"
#include "Interfaces/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

namespace tblAccountProducts {
TARGOMAN_CREATE_CONSTEXPR(prd_locID);
TARGOMAN_CREATE_CONSTEXPR(prdShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(prdShowTotal);
TARGOMAN_CREATE_CONSTEXPR(prdClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(prdClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(prdClicksTotal);
}

namespace tblAccountSaleables {
TARGOMAN_CREATE_CONSTEXPR(slbShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(slbShowTotal);
TARGOMAN_CREATE_CONSTEXPR(slbClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(slbClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(slbClicksTotal);
}

namespace AccountAsset {
TARGOMAN_CREATE_CONSTEXPR(Days);
TARGOMAN_CREATE_CONSTEXPR(DayShow);
TARGOMAN_CREATE_CONSTEXPR(TotalShow);
TARGOMAN_CREATE_CONSTEXPR(DayClicks);
TARGOMAN_CREATE_CONSTEXPR(MonthClicks);
TARGOMAN_CREATE_CONSTEXPR(TotalClicks);
}

///<  ColName                                                    Type        Validation                  Default  UpBy    Sort   Filter Self  Virt   PK
#define ADVERT_DEFINE_ASSET_FIELDS(_prefix) \
    { QString("%1%2").arg(_prefix, AccountAsset::Days),          S(qint16),  QFV.integer().minValue(-1), 0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::DayShow),       S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::TotalShow),     S(quint64), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::DayClicks),     S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::MonthClicks),   S(quint32), QFV.integer().minValue(0),  0,       UPNone, false, false }, \
    { QString("%1%2").arg(_prefix, AccountAsset::TotalClicks),   S(quint64), QFV.integer().minValue(0),  0,       UPNone, false, false } \

namespace tblAccountUserAsset {
//Extra fields goes here
}

#pragma GCC diagnostic pop

/******************************************************/
class AccountProducts: public Accounting::intfAccountProducts
{
    Q_OBJECT
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountProducts)
};

/******************************************************/
class AccountSaleables: public Accounting::intfAccountSaleables
{
    Q_OBJECT
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountSaleables)
};

/******************************************************/
class AccountUserAssets: public Accounting::intfAccountUserAssets
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountUserAssets)
};

/******************************************************/
class AccountAssetUsage: public Accounting::intfAccountAssetUsage
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountAssetUsage)
};

/******************************************************/
class AccountCoupons: public Accounting::intfAccountCoupons
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert, AccountCoupons)
};

/******************************************************/
//There is no Prize in the advertisement module
/*class clsAccountPrizes: public Accounting::intfAccountPrizes
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,clsAccountPrizes)
};
*/
}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
