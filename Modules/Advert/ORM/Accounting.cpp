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
using namespace AAA;
using namespace AAA::Accounting;
using namespace Targoman::API::AAA::Accounting;

/******************************************************/

AccountSaleables::AccountSaleables() :
    intfAccountSaleables(AdvertSchema,
              tblAccountSaleablesBase::Name,
              { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountSaleablesBase::slbID,                 S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountSaleablesBase::slbCode,               S(TAPI::PackageCode_t), QFV,                                    QRequired,  UPAdmin},
                {tblAccountSaleablesBase::slbType,               S(TAPI::enuPackageType::Type), QFV,                             TAPI::enuPackageType::Normal, UPAdmin},
                {tblAccountSaleablesBase::slbValidFromDate,      S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbValidToDate,        S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbValidFromTime,      S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbValidToTime,        S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountSaleablesBase::slb_locID,             S(quint32),             QFV.integer().minValue(1),              QRequired, UPAdmin},
                {tblAccountSaleables::slbShowPerDay,             S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountSaleables::slbShowTotal,              S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountSaleables::slbClicksPerDay,           S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountSaleables::slbClicksPerMonth,         S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountSaleables::slbClicksTotal,            S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountSaleablesBase::slbPrivs,              S(TAPI::JSON_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QAuto,     UPAdmin},
                {tblAccountSaleablesBase::slbNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbVoucherTemplate,    S(QString),             QFV,                                    QNull,     UPAdmin},
                {tblAccountSaleablesBase::slbCreatedBy_usrID,    ORM_CREATED_BY},
                {tblAccountSaleablesBase::slbCreationDateTime,   ORM_CREATED_ON},
                {tblAccountSaleablesBase::slbUpdatedBy_usrID,    ORM_UPDATED_BY},
                {tblAccountSaleablesBase::slbStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPStatus},
              },
              { ///< Col                                    Reference Table                         ForeignCol          Rename     LeftJoin
                {tblAccountSaleablesBase::slb_locID,             R(AdvertSchema, tblLocations::Name),    tblLocations::locID},
                {tblAccountSaleablesBase::slbCreatedBy_usrID,    ORM_JOIN_CREATOR},
                {tblAccountSaleablesBase::slbUpdatedBy_usrID,    ORM_JOIN_UPDATER},
              })
{;}

/******************************************************/
AccountAssetUsage::AccountAssetUsage() :
    intfAccountAssetUsage(AdvertSchema,
              tblAccountAssetUsage::Name,
              { ///<ColName                         Type                    Validation                              Default UpBy   Sort  Filter Self  Virt   PK
                {tblAccountAssetUsage::usg_uasID,        S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountAssetUsage::usgDayShow,       S(qint16),              QFV.integer().minValue(-1),             0,      UPNone,false,false},
                {tblAccountAssetUsage::usgDayShow,       S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountAssetUsage::usgTotalShow,     S(quint64),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountAssetUsage::usgDayClicks,     S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountAssetUsage::usgMonthClicks,   S(quint32),             QFV.integer().minValue(0),              0,      UPNone,false,false},
                {tblAccountAssetUsage::usgTotalClicks,   S(quint64),             QFV.integer().minValue(0),              0,      UPNone,false,false},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountAssetUsage::usg_uasID,        R(AdvertSchema,tblAccountUserAssets::Name), tblAccountUserAssets::uasID},
              })
{;}

/******************************************************/

clsAccountUserAssets::clsAccountUserAssets() :
    intfAccountUserAssets(AdvertSchema,
              tblAccountUserAssets::Name,
              { ///<ColName                                         Type                    Validation                  Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUserAssets::uasID,                     S(quint64),             QFV.integer().minValue(1),  ORM_PRIMARY_KEY},
                {tblAccountUserAssets::uas_usrID,                 S(quint32),             QFV.integer().minValue(1),  QRequired,  UPAdmin},
                {tblAccountUserAssets::uas_slbID,                 S(quint32),             QFV.integer().minValue(1),  QRequired,  UPAdmin},
                {tblAccountUserAssets::uasPrefered,               S(bool),                QFV,                        false,      UPOwner},
                {tblAccountUserAssets::uasPurchaseRequestDateTime,S(TAPI::DateTime_t),    QFV,                        QNull,      UPNone},
                {tblAccountUserAssets::uasPaymentDateTime,        S(TAPI::DateTime_t),    QFV,                        QNull,      UPAdmin},
                {tblAccountUserAssets::uas_vchID,                 S(quint64),             QFV.integer().minValue(1),  QInvalid,   UPNone},
                {tblAccountUserAssets::uasUpdatedBy_usrID,        ORM_UPDATED_BY},
                {tblAccountUserAssets::uasStatus,                 S(TAPI::enuAuditableStatus::Type), QFV,             TAPI::enuAuditableStatus::Pending, UPStatus},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountUserAssets::uas_usrID, R(AAASchema,tblUser::Name),  tblUser::usrID,    "Owner_"},
                {tblAccountUserAssets::uas_slbID, R(AdvertSchema,tblAccountSaleablesBase::Name),  tblAccountSaleablesBase::slbID},
                //Voucher is not accessible as it is in another schema
                //{tblAccountUserAssets::uas_vchID,        R(AAASchema,tblVoucher::Name),  tblVoucher::vchID,    "", true},
              }
    )
{;}
/******************************************************/
AccountCoupons::AccountCoupons() :
    intfAccountCoupons(AdvertSchema,
             tblAccountCoupons::Name,
            { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
              {tblAccountCoupons::cpnID,                 S(quint32),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
              {tblAccountCoupons::cpnCode,               S(TAPI::DiscountCode_t),QFV,                                    QRequired, UPAdmin},
              {tblAccountCoupons::cpnType,               S(TAPI::enuDiscountType::Type), QFV,                            TAPI::enuDiscountType::Percent, UPAdmin},
              {tblAccountCoupons::cpnPackageBasedAmount, S(TAPI::JSON_t),        QFV,                                    QRequired, UPAdmin, false, false},
              {tblAccountCoupons::cpnValidFrom,          S(TAPI::DateTime_t),    QFV,                                    QRequired, UPAdmin},
              {tblAccountCoupons::cpnExpiryTime,         S(TAPI::DateTime_t),    QFV,                                    QNull,     UPAdmin},
              {tblAccountCoupons::cpnPrimaryCount,       S(quint32),             QFV.integer().minValue(1),              1,         UPAdmin},
              {tblAccountCoupons::cpnUsedCount,          S(quint32),             QFV.integer().minValue(0),              0,         UPNone},
              {tblAccountCoupons::cpnCreatedBy_usrID,    ORM_CREATED_BY},
              {tblAccountCoupons::cpnCreationDateTime,   ORM_CREATED_ON},
              {tblAccountCoupons::cpnUpdatedBy_usrID,    ORM_UPDATED_BY},
              {tblAccountCoupons::cpnStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPStatus},
            },
            { ///< Col                                     Reference Table                         ForeignCol          Rename     LeftJoin
              {tblAccountCoupons::cpnCreatedBy_usrID,    ORM_JOIN_CREATOR},
              {tblAccountCoupons::cpnUpdatedBy_usrID,    ORM_JOIN_UPDATER},
            })
{;}

}
}
}
