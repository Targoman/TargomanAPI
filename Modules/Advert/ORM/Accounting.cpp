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
#include "Defs.hpp"
#include "Locations.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;
using namespace TAPI;
using namespace AAA;
using namespace AAA::Accounting;
using namespace Targoman::API::AAA::Accounting;

/******************************************************/

AccountPackages::AccountPackages() :
    intfAccountPackages(AdvertSchema,
              tblAccountPackagesBase::Name,
              { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountPackagesBase::pkgID,                 S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountPackagesBase::pkgCode,               S(TAPI::PackageCode_t), QFV,                                    QInvalid,  UPAdmin},
                {tblAccountPackagesBase::pkgType,               S(TAPI::enuPackageType::Type), QFV,                             TAPI::enuPackageType::Normal, UPAdmin},
                {tblAccountPackagesBase::pkgValidFromDate,      S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgValidToDate,        S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgValidFromTime,      S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgValidToTime,        S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackagesBase::pkg_locID,             S(quint32),             QFV.integer().minValue(1),              QInvalid,  UPAdmin},
                {tblAccountPackages::pkgShowPerDay,             S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgShowTotal,              S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksPerDay,           S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksPerMonth,         S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksTotal,            S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackagesBase::pkgPrivs,              S(JSON_t),              QFV,                                    QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QAuto,     UPAdmin},
                {tblAccountPackagesBase::pkgNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgVoucherTemplate,    S(QString),             QFV,                                    QNull,     UPAdmin},
                {tblAccountPackagesBase::pkgCreatedBy_usrID,    ORM_CREATED_BY},
                {tblAccountPackagesBase::pkgCreationDateTime,   ORM_CREATED_ON},
                {tblAccountPackagesBase::pkgUpdatedBy_usrID,    ORM_UPDATED_BY},
                {tblAccountPackagesBase::pkgStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPStatus},
              },
              { ///< Col                                    Reference Table                         ForeignCol          Rename     LeftJoin
                {tblAccountPackagesBase::pkg_locID,             R(AdvertSchema, tblLocations::Name),    tblLocations::locID},
                {tblAccountPackagesBase::pkgCreatedBy_usrID,    ORM_JOIN_CREATOR},
                {tblAccountPackagesBase::pkgUpdatedBy_usrID,    ORM_JOIN_UPDATER},
              })
{;}

/******************************************************/
AccountUsage::AccountUsage() :
    intfAccountUsage(AdvertSchema,
              tblAccountUsage::Name,
              { ///<ColName                         Type                    Validation                              Default UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUsage::usg_aupID,        S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountUsage::usgDayShow,       S(qint16),              QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountUsage::usgDayShow,       S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountUsage::usgTotalShow,     S(quint64),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountUsage::usgDayClicks,     S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountUsage::usgMonthClicks,   S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountUsage::usgTotalClicks,   S(quint64),             QFV.integer().minValue(0),              0,      UPNone,false,false},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountUsage::usg_aupID,        R(AdvertSchema,tblAccountUserPackages::Name), tblAccountUserPackages::aupID},
              })
{;}

/******************************************************/

clsAccountUserPackages::clsAccountUserPackages() :
    intfAccountUserPackages(AdvertSchema,
              tblAccountUserPackages::Name,
              { ///<ColName                                         Type                    Validation                  Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUserPackages::aupID,                     S(quint64),             QFV.integer().minValue(1),  ORM_PRIMARY_KEY},
                {tblAccountUserPackages::aup_usrID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aup_pkgID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aupPrefered,               S(bool),                QFV,                        false,     UPOwner},
                {tblAccountUserPackages::aupPurchaseRequestDateTime,S(TAPI::DateTime_t),    QFV,                        QNull,     UPNone},
                {tblAccountUserPackages::aupPaymentDateTime,        S(TAPI::DateTime_t),    QFV,                        QNull,     UPAdmin},
                {tblAccountUserPackages::aup_invID,                 S(quint64),             QFV.integer().minValue(1),  QInvalid,  UPNone},
                {tblAccountUserPackages::aupUpdatedBy_usrID,        ORM_UPDATED_BY},
                {tblAccountUserPackages::aupStatus,                 S(TAPI::enuAuditableStatus::Type), QFV,             TAPI::enuAuditableStatus::Pending, UPStatus},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountUserPackages::aup_usrID, R(AAASchema,tblUser::Name),  tblUser::usrID,    "Owner_"},
                {tblAccountUserPackages::aup_pkgID, R(AdvertSchema,tblAccountPackagesBase::Name),  tblAccountPackagesBase::pkgID},
                //Voucher is not accessible as it is in another schema
                //{tblAccountUserPackages::aup_invID,        R(AAASchema,tblVoucher::Name),  tblVoucher::vchID,    "", true},
              }
    )
{;}
/******************************************************/
AccountDiscounts::AccountDiscounts() :
    intfAccountDiscounts(AdvertSchema,
             tblAccountDiscounts::Name,
            { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
              {tblAccountDiscounts::disID,                 S(quint32),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
              {tblAccountDiscounts::disCode,               S(TAPI::DiscountCode_t),QFV,                                    QInvalid,  UPAdmin},
              {tblAccountDiscounts::disType,               S(TAPI::enuDiscountType::Type), QFV,                            TAPI::enuDiscountType::Percent, UPAdmin},
              {tblAccountDiscounts::disPackageBasedAmount, S(JSON_t),              QFV,                                    QInvalid,  UPAdmin, false, false},
              {tblAccountDiscounts::disValidFrom,          S(TAPI::DateTime_t),    QFV,                                    QAuto,     UPAdmin},
              {tblAccountDiscounts::disExpiryTime,         S(TAPI::DateTime_t),    QFV,                                    QNull,     UPAdmin},
              {tblAccountDiscounts::disPrimaryCount,       S(quint32),             QFV.integer().minValue(1),              1,         UPAdmin},
              {tblAccountDiscounts::disUsedCount,          S(quint32),             QFV.integer().minValue(0),              0,         UPNone},
              {tblAccountDiscounts::disCreatedBy_usrID,    ORM_CREATED_BY},
              {tblAccountDiscounts::disCreationDateTime,   ORM_CREATED_ON},
              {tblAccountDiscounts::disUpdatedBy_usrID,    ORM_UPDATED_BY},
              {tblAccountDiscounts::disStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPStatus},
            },
            { ///< Col                                     Reference Table                         ForeignCol          Rename     LeftJoin
              {tblAccountDiscounts::disCreatedBy_usrID,    ORM_JOIN_CREATOR},
              {tblAccountDiscounts::disUpdatedBy_usrID,    ORM_JOIN_UPDATER},
            })
{;}

}
}
}
