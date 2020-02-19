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

#include "Accounting.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/Common/GenericEnums.hpp"
#include "Defs.hpp"
#include "Locations.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

/******************************************************/
AccountPackages::AccountPackages() :
    clsTable(AdvertSchema,
              tblAccountPackages::Name,
              { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountPackages::acpID,                 S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountPackages::acpCode,               S(quint32),             QFV.integer().minValue(1),              QInvalid,  UPNone},
                {tblAccountPackages::acpValidFromDate,      S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::acpValidToDate,        S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::acpValidFromTime,      S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackages::acpValidToTime,        S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackages::acp_locID,             S(quint32),             QFV.integer().minValue(1),              QInvalid,  UPAdmin},
                {tblAccountPackages::acpShowPerDay,         S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::acpShowTotal,          S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::acpClicksPerDay,       S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::acpClicksPerMonth,     S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::acpClicksTotal,        S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::acpPrivs,              S(JSON_t),              QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::acpInvoiceTemplate,    S(QString),             QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::acpCreatedBy_usrID,    S(quint32),             QFV.integer().minValue(1),              QNull,     UPNone},
                {tblAccountPackages::acpCreationDateTime,   S(QHttp::DateTime_t),   QFV,                                    QNull,     UPNone},
                {tblAccountPackages::acpUpdatedBy_usrID,    S(quint32),             QFV.integer().minValue(1),              QNull,     UPNone},
                {tblAccountPackages::acpStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPAdmin},
              },
              { ///< Col                                    Reference Table                         ForeignCol          Rename     LeftJoin
                {tblAccountPackages::acp_locID,             R(AdvertSchema, tblLocations::Name),    tblLocations::locID},
                {tblAccountPackages::acpCreatedBy_usrID,    R(AAASchema,tblUser::Name),             tblUser::usrID,    "Creator_", true},
                {tblAccountPackages::acpUpdatedBy_usrID,    R(AAASchema,tblUser::Name),             tblUser::usrID,    "Updater_", true},
              })
{;}

/******************************************************/
AccountUsage::AccountUsage() :
    clsTable(AdvertSchema,
              tblAccountUsage::Name,
              { ///<ColName                         Type                    Validation                              Default UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUsage::acg_aupID,        S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountUsage::acgDayShow,       S(quint32),             QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountUsage::acgTotalShow,     S(quint64),             QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountUsage::acgDayClicks,     S(quint32),             QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountUsage::acgMonthClicks,   S(quint32),             QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountUsage::acgTotalClicks,   S(quint64),             QFV.integer().minValue(-1),             0,      UPNone,false,false},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountUsage::acg_aupID,        R(AdvertSchema,tblAccountUserPackages::Name), tblAccountUserPackages::aupID},
              })
{;}

/******************************************************/
AccountUserPackages::AccountUserPackages() :
    clsTable(AdvertSchema,
              tblAccountUserPackages::Name,
              { ///<ColName                                         Type                    Validation                  Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUserPackages::aupID,                     S(quint64),             QFV.integer().minValue(1),  ORM_PRIMARY_KEY},
                {tblAccountUserPackages::aup_usrID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aup_acpID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aupPurchaseRequestDateTime,S(QHttp::DateTime_t),   QFV,                        QNull,     UPNone},
                {tblAccountUserPackages::aupPaymentDataTime,        S(QHttp::DateTime_t),   QFV,                        QNull,     UPAdmin},
                {tblAccountUserPackages::aup_invID,                 S(quint64),             QFV.integer().minValue(1),  QInvalid,  UPNone},
                {tblAccountUserPackages::aupStatus,                 S(TAPI::enuAuditableStatus::Type), QFV,             TAPI::enuAuditableStatus::Pending, UPNone},
              },
              {
                {tblAccountUserPackages::aup_usrID,        R(AAASchema,tblUser::Name),  tblUser::usrID,    "Owner_"},
                {tblAccountUserPackages::aup_acpID,        R(AdvertSchema,tblAccountPackages::Name),  tblAccountPackages::acpID},
             //Invoice is not accessible as it is in another schema
//{tblAccountUserPackages::aup_invID,        R(AAASchema,tblInvoice::Name),  tblInvoice::invID,    "", true},
              }
    )
{;}

}
}
}
