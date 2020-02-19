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
TARGOMAN_CREATE_CONSTEXPR(acpID);
TARGOMAN_CREATE_CONSTEXPR(acpCode);
TARGOMAN_CREATE_CONSTEXPR(acpValidFromDate);
TARGOMAN_CREATE_CONSTEXPR(acpValidToDate);
TARGOMAN_CREATE_CONSTEXPR(acpValidFromTime);
TARGOMAN_CREATE_CONSTEXPR(acpValidToTime);
TARGOMAN_CREATE_CONSTEXPR(acp_locID);
TARGOMAN_CREATE_CONSTEXPR(acpShowPerDay);
TARGOMAN_CREATE_CONSTEXPR(acpShowTotal);
TARGOMAN_CREATE_CONSTEXPR(acpClicksPerDay);
TARGOMAN_CREATE_CONSTEXPR(acpClicksPerMonth);
TARGOMAN_CREATE_CONSTEXPR(acpClicksTotal);
TARGOMAN_CREATE_CONSTEXPR(acpPrivs);
TARGOMAN_CREATE_CONSTEXPR(acpInvoiceTemplate);
TARGOMAN_CREATE_CONSTEXPR(acpCreatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(acpCreationDateTime);
TARGOMAN_CREATE_CONSTEXPR(acpUpdatedBy_usrID);
TARGOMAN_CREATE_CONSTEXPR(acpStatus);
}

namespace tblAccountUsage {
constexpr char Name[] = "tblAccountUsage";
TARGOMAN_CREATE_CONSTEXPR(acg_aupID);
TARGOMAN_CREATE_CONSTEXPR(acgDayShow);
TARGOMAN_CREATE_CONSTEXPR(acgTotalShow);
TARGOMAN_CREATE_CONSTEXPR(acgDayClicks);
TARGOMAN_CREATE_CONSTEXPR(acgMonthClicks);
TARGOMAN_CREATE_CONSTEXPR(acgTotalClicks);
}

namespace tblAccountUserPackages {
constexpr char Name[] = "tblAccountUserPackages";
TARGOMAN_CREATE_CONSTEXPR(aupID);
TARGOMAN_CREATE_CONSTEXPR(aup_usrID);
TARGOMAN_CREATE_CONSTEXPR(aup_acpID);
TARGOMAN_CREATE_CONSTEXPR(aupPurchaseRequestDateTime);
TARGOMAN_CREATE_CONSTEXPR(aupPaymentDataTime);
TARGOMAN_CREATE_CONSTEXPR(aup_invID);
TARGOMAN_CREATE_CONSTEXPR(aupStatus);
}
#pragma GCC diagnostic pop

class AccountPackages: public ORM::clsTable
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountPackages)
};

/******************************************************/
class AccountUsage: public ORM::clsTable
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountUsage)
};

/******************************************************/
class AccountUserPackages: public ORM::clsTable
{
    Q_OBJECT
private slots:
public:
    TARGOMAN_DEFINE_API_SUBMODULE(Advert,AccountUserPackages)
};

}
}
}
#endif // TARGOMAN_API_MODULES_ADVERT_ORM_ACCOUNTING_H
