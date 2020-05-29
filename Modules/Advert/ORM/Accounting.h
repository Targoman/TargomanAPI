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
namespace tblAccountPackages {
TARGOMAN_CREATE_CONSTEXPR(pkgShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(pkgShowTotal);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksTotal);
}

namespace tblAccountUsage {
constexpr char Name[] = "tblAccountUsage";
TARGOMAN_CREATE_CONSTEXPR(usg_aupID);
TARGOMAN_CREATE_CONSTEXPR(usgRemainingDays);
TARGOMAN_CREATE_CONSTEXPR(usgDayShow);
TARGOMAN_CREATE_CONSTEXPR(usgTotalShow);
TARGOMAN_CREATE_CONSTEXPR(usgDayClicks);
TARGOMAN_CREATE_CONSTEXPR(usgMonthClicks);
TARGOMAN_CREATE_CONSTEXPR(usgTotalClicks);
}
#pragma GCC diagnostic pop

/******************************************************/
class AccountPackages: public Accounting::intfAccountPackages
{
    Q_OBJECT
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountPackages)
};

/******************************************************/
class AccountUsage: public Accounting::intfAccountUsage
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountUsage)
};

/******************************************************/
class clsAccountUserPackages: public Accounting::intfAccountUserPackages
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,clsAccountUserPackages)
};

/******************************************************/
class AccountDiscounts: public Accounting::intfAccountDiscounts
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountDiscounts)
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
