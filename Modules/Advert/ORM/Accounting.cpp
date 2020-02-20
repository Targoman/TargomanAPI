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
using namespace TAPI;

/******************************************************/
QVariant AccountPackages::apiGET(GET_METHOD_ARGS_IMPL)
{
    constexpr quint16 CACHE_TIME  = 15 * 60;
    QString ExtraFilters;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        ExtraFilters = QString ("( %1>=NOW() | %2<DATE_ADD(NOW(),INTERVAL$SPACE$15$SPACE$Min)")
                       .arg(tblAccountPackages::pkgCanBePurchasedSince)
                       .arg(tblAccountPackages::pkgNotAvailableSince);

    return this->selectFromTable({}, ExtraFilters, GET_METHOD_CALL_ARGS, CACHE_TIME);
}

bool AccountPackages::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool AccountPackages::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 AccountPackages::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

AccountPackages::AccountPackages() :
    clsTable(AdvertSchema,
              tblAccountPackages::Name,
              { ///<ColName                                 Type                    Validation                              Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountPackages::pkgID,                 S(quint64),             QFV.integer().minValue(1),              ORM_PRIMARY_KEY},
                {tblAccountPackages::pkgCode,               S(TAPI::PackageCode_t), QFV,                                    QInvalid,  UPAdmin},
                {tblAccountPackages::pkgType,               S(TAPI::enuPackageType::Type), QFV,                             TAPI::enuPackageType::Normal, UPAdmin},
                {tblAccountPackages::pkgValidFromDate,      S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::pkgValidToDate,        S(TAPI::Date_t),        QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::pkgValidFromTime,      S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackages::pkgValidToTime,        S(quint8),              QFV.integer().minValue(0).maxValue(23), QNull,     UPAdmin},
                {tblAccountPackages::pkg_locID,             S(quint32),             QFV.integer().minValue(1),              QInvalid,  UPAdmin},
                {tblAccountPackages::pkgShowPerDay,         S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgShowTotal,          S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksPerDay,       S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksPerMonth,     S(quint32),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgClicksTotal,        S(quint64),             QFV.integer().minValue(-1),             -1,        UPAdmin},
                {tblAccountPackages::pkgPrivs,              S(JSON_t),              QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::pkgNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QAuto,     UPAdmin},
                {tblAccountPackages::pkgNotAvailableSince,  S(TAPI::DateTime_t),    QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::pkgInvoiceTemplate,    S(QString),             QFV,                                    QNull,     UPAdmin},
                {tblAccountPackages::pkgCreatedBy_usrID,    ORM_CREATED_BY},
                {tblAccountPackages::pkgCreationDateTime,   ORM_CREATED_ON},
                {tblAccountPackages::pkgUpdatedBy_usrID,    ORM_UPDATED_BY},
                {tblAccountPackages::pkgStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPAdmin},
              },
              { ///< Col                                    Reference Table                         ForeignCol          Rename     LeftJoin
                {tblAccountPackages::pkg_locID,             R(AdvertSchema, tblLocations::Name),    tblLocations::locID},
                {tblAccountPackages::pkgCreatedBy_usrID,    ORM_JOIN_CREATOR},
                {tblAccountPackages::pkgUpdatedBy_usrID,    ORM_JOIN_UPDATER},
              })
{;}

/******************************************************/
QVariant AccountUsage::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAccountUserPackages::aup_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

AccountUsage::AccountUsage() :
    clsTable(AdvertSchema,
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
QVariant AccountUserPackages::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAccountUserPackages::aup_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool AccountUserPackages::apiUPDATEsetAsPrefered(TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH){
    bool Ok;
    quint64 UserPackageID = _EXTRAPATH.toUInt(&Ok);
    if(!Ok || !UserPackageID )
        throw exHTTPBadRequest("Invalid UserPackageID provided");

    this->callSP("sp_UPDATE_setAsPrefered", {
                     {"iUserID", clsJWT(_JWT).usrID()},
                     {"iAUPID",  UserPackageID},
                 });
    return false;
}

bool AccountUserPackages::apiUPDATEdisablePackage(TAPI::JWT_t _JWT, TAPI::ExtraPath_t _EXTRAPATH){
    bool Ok;
    quint64 UserPackageID = _EXTRAPATH.toUInt(&Ok);
    if(!Ok || !UserPackageID )
        throw exHTTPBadRequest("Invalid UserPackageID provided");
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(clsJWT(_JWT).usrID(), {
                            {tblAccountUserPackages::aupID, UserPackageID}
                        }, {
                            {tblAccountUserPackages::aupStatus, enuAuditableStatus::Banned},
                        });
}

AccountUserPackages::AccountUserPackages() :
    clsTable(AdvertSchema,
              tblAccountUserPackages::Name,
              { ///<ColName                                         Type                    Validation                  Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAccountUserPackages::aupID,                     S(quint64),             QFV.integer().minValue(1),  ORM_PRIMARY_KEY},
                {tblAccountUserPackages::aup_usrID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aup_pkgID,                 S(quint32),             QFV.integer().minValue(1),  QInvalid,  UPAdmin},
                {tblAccountUserPackages::aupPrefered,               S(bool),                QFV,                        false,     UPOwner},
                {tblAccountUserPackages::aupPurchaseRequestDateTime,S(TAPI::DateTime_t),    QFV,                        QNull,     UPNone},
                {tblAccountUserPackages::aupPaymentDataTime,        S(TAPI::DateTime_t),    QFV,                        QNull,     UPAdmin},
                {tblAccountUserPackages::aup_invID,                 S(quint64),             QFV.integer().minValue(1),  QInvalid,  UPNone},
                {tblAccountUserPackages::aupUpdatedBy_usrID,        ORM_UPDATED_BY},
                {tblAccountUserPackages::aupStatus,                 S(TAPI::enuAuditableStatus::Type), QFV,             TAPI::enuAuditableStatus::Pending, UPNone},
              },
              { ///< Col                            Reference Table                               ForeignCol                     Rename     LeftJoin
                {tblAccountUserPackages::aup_usrID, R(AAASchema,tblUser::Name),  tblUser::usrID,    "Owner_"},
                {tblAccountUserPackages::aup_pkgID, R(AdvertSchema,tblAccountPackages::Name),  tblAccountPackages::pkgID},
                //Invoice is not accessible as it is in another schema
                //{tblAccountUserPackages::aup_invID,        R(AAASchema,tblInvoice::Name),  tblInvoice::invID,    "", true},
              }
    )
{;}
/******************************************************/
QVariant AccountDiscounts::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool AccountDiscounts::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool AccountDiscounts::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 AccountDiscounts::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

AccountDiscounts::AccountDiscounts() :
    clsTable(AdvertSchema,
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
              {tblAccountDiscounts::disStatus,             S(TAPI::enuGenericStatus::Type), QFV,                           TAPI::enuGenericStatus::Active, UPAdmin},
            },
            { ///< Col                                     Reference Table                         ForeignCol          Rename     LeftJoin
              {tblAccountDiscounts::disCreatedBy_usrID,    ORM_JOIN_CREATOR},
              {tblAccountDiscounts::disUpdatedBy_usrID,    ORM_JOIN_UPDATER},
            })
{;}

}
}
}
