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
constexpr char Name[] = "tblAccountPackages";
TARGOMAN_CREATE_CONSTEXPR(pkgID);
TARGOMAN_CREATE_CONSTEXPR(pkgCode);
TARGOMAN_CREATE_CONSTEXPR(pkgType);
TARGOMAN_CREATE_CONSTEXPR(pkgValidFromDate);
TARGOMAN_CREATE_CONSTEXPR(pkgValidToDate);
TARGOMAN_CREATE_CONSTEXPR(pkgValidFromTime);
TARGOMAN_CREATE_CONSTEXPR(pkgValidToTime);
TARGOMAN_CREATE_CONSTEXPR(pkg_locID);
TARGOMAN_CREATE_CONSTEXPR(pkgShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(pkgShowTotal);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(pkgClicksTotal);
TARGOMAN_CREATE_CONSTEXPR(pkgPrivs);
TARGOMAN_CREATE_CONSTEXPR(pkgCanBePurchasedSince);
TARGOMAN_CREATE_CONSTEXPR(pkgNotAvailableSince);
TARGOMAN_CREATE_CONSTEXPR(pkgInvoiceTemplate);
TARGOMAN_CREATE_CONSTEXPR(pkgCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pkgCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(pkgUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(pkgStatus);
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

namespace tblAccountUserPackages {
constexpr char Name[] = "tblAccountUserPackages";
TARGOMAN_CREATE_CONSTEXPR(aupID);
TARGOMAN_CREATE_CONSTEXPR(aup_usrID);
TARGOMAN_CREATE_CONSTEXPR(aup_pkgID);
TARGOMAN_CREATE_CONSTEXPR(aupPrefered);
TARGOMAN_CREATE_CONSTEXPR(aupPurchaseRequestDateTime);
TARGOMAN_CREATE_CONSTEXPR(aupPaymentDataTime);
TARGOMAN_CREATE_CONSTEXPR(aup_invID);
TARGOMAN_CREATE_CONSTEXPR(aupUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(aupStatus);
}

namespace tblAccountDiscounts {
constexpr char Name[] = "tblAccountDiscounts";
TARGOMAN_CREATE_CONSTEXPR(disID);
TARGOMAN_CREATE_CONSTEXPR(disCode);
TARGOMAN_CREATE_CONSTEXPR(disType);
TARGOMAN_CREATE_CONSTEXPR(disPackageBasedAmount);
TARGOMAN_CREATE_CONSTEXPR(disValidFrom);
TARGOMAN_CREATE_CONSTEXPR(disExpiryTime);
TARGOMAN_CREATE_CONSTEXPR(disPrimaryCount);
TARGOMAN_CREATE_CONSTEXPR(disUsedCount);
TARGOMAN_CREATE_CONSTEXPR(disCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(disCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(disUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(disStatus);
}

#pragma GCC diagnostic pop

class AccountPackages: public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Available Packages")
    bool ORMDELETE("Delete a package")
    bool ORMUPDATE("Update a package info by priviledged user")
    quint32 ORMCREATE("Create a new package by priviledged user")

public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountPackages)
};

/******************************************************/
class AccountUsage: public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get User Usage on each package")

public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountUsage)
};

/******************************************************/
class AccountUserPackages: public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get User Packages")
    bool REST(UPDATE, disablePackage, (TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH),
              "Mark a user package banned by priviledged user")
    bool REST(UPDATE, setAsPrefered, (TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH),
              "Mark a user package as prefered")
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountUserPackages)
};

/******************************************************/
class AccountDiscounts: public ORM::clsTable
{
    Q_OBJECT
private slots:
    QVariant ORMGET("Get Active Discounts")
    bool ORMDELETE("Delete a Discount")
    bool ORMUPDATE("Update a Discount info by priviledged user")
    quint32 ORMCREATE("Create a new Discount by priviledged user")

public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountDiscounts)
};

/******************************************************/
//There is no Prize in the advertisement module
/*class AccountPrizes: public ORM::clsTable
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountPrizes)
};
*/
}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
