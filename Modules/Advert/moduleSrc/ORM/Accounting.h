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
namespace tblAccountSaleables {
TARGOMAN_CREATE_CONSTEXPR(slbShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(slbShowTotal);
TARGOMAN_CREATE_CONSTEXPR(slbClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(slbClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(slbClicksTotal);
}

namespace tblAccountAssetUsage {
constexpr char Name[] = "tblAccountAssetUsage";
TARGOMAN_CREATE_CONSTEXPR(usg_uasID);
TARGOMAN_CREATE_CONSTEXPR(usgRemainingDays);
TARGOMAN_CREATE_CONSTEXPR(usgDayShow);
TARGOMAN_CREATE_CONSTEXPR(usgTotalShow);
TARGOMAN_CREATE_CONSTEXPR(usgDayClicks);
TARGOMAN_CREATE_CONSTEXPR(usgMonthClicks);
TARGOMAN_CREATE_CONSTEXPR(usgTotalClicks);
}
#pragma GCC diagnostic pop

/******************************************************/
class AccountSaleables: public Accounting::intfAccountSaleables
{
    Q_OBJECT
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountSaleables)
};

/******************************************************/
class AccountAssetUsage: public Accounting::intfAccountAssetUsage
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountAssetUsage)
};

/******************************************************/
class clsAccountUserAssets: public Accounting::intfAccountUserAssets
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,clsAccountUserAssets)
};

/******************************************************/
class AccountCoupons: public Accounting::intfAccountCoupons
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountCoupons)
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
