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

#include "Payments.h"
#include "Voucher.h"
#include "Classes/PaymentLogic.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant OnlinePayments::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

OnlinePayments::OnlinePayments() :
    clsTable(AAASchema,
              tblOnlinePayments::Name,
              { ///<ColName                              Type                    Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblOnlinePayments::onpID,                S(quint64),             QFV.integer().minValue(1),          UPNone},
                {tblOnlinePayments::onpMD5,               S(TAPI::MD5_t),         QFV,                                ORM_PRIMARY_KEY},
                {tblOnlinePayments::onpCreationDateTime,  S(TAPI::DateTime_t),    QFV,                                QAuto,      UPNone},
                {tblOnlinePayments::onp_invID,            S(quint64),             QFV.integer().minValue(1),          QNull,      UPNone},
                {tblOnlinePayments::onpPaymentGateway,    S(TAPI::enuPaymentGateways::Type),QFV,                      TAPI::enuPaymentGateways::Zibal, UPNone},
                {tblOnlinePayments::onpPGTrnID,           S(QString),             QFV.allwaysValid().maxLenght(50),   QInvalid,   UPNone},
                {tblOnlinePayments::onpAmount,            S(TAPI::DateTime_t),    QFV,                                QInvalid,   UPNone},
                {tblOnlinePayments::onpLastUpdateDateTime,S(TAPI::DateTime_t),    QFV,                                QInvalid,   UPNone},
                {tblOnlinePayments::onpStatus,            S(TAPI::enuPaymentStatus::Type),QFV,                        TAPI::enuPaymentStatus::Pending, UPStatus},
                {tblOnlinePayments::onpResult,            S(QString),             QFV,                                QNull,      UPNone,false,false},
              },
              { ///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
                {tblOnlinePayments::onp_invID,   R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
              })
{
}

/**********************************************************************************************/
QVariant OfflinePayments::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool OfflinePayments::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

OfflinePayments::OfflinePayments() :
    clsTable(AAASchema,
              tblOnlinePayments::Name,
              { ///<ColName                             Type                    Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblOfflinePayments::ofpID,               S(quint64),             QFV.integer().minValue(1),          UPNone},
                {tblOfflinePayments::ofp_invID,           S(quint64),             QFV.integer().minValue(1),          QNull,      UPNone},
                {tblOfflinePayments::ofpBank,             S(TAPI::MD5_t),         QFV,                                ORM_PRIMARY_KEY},
                {tblOfflinePayments::ofpReceiptCode,      S(QString),             QFV.allwaysValid().maxLenght(50),   QInvalid,   UPNone},
                {tblOfflinePayments::ofpReceiptDate,      S(TAPI::DateTime_t),    QFV,                                QAuto,      UPNone},
                {tblOfflinePayments::ofpAmount,           S(TAPI::DateTime_t),    QFV,                                QInvalid,   UPNone},
                {tblOfflinePayments::ofpCreatedBy_usrID,  ORM_CREATED_BY},
                {tblOfflinePayments::ofpCreationDateTime, ORM_CREATED_ON},
                {tblOfflinePayments::ofpUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblOfflinePayments::ofpStatus,           S(TAPI::enuPaymentStatus::Type),QFV,                        TAPI::enuPaymentStatus::Pending, UPStatus},
              },
              { ///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
                {tblOfflinePayments::ofpCreatedBy_usrID, ORM_JOIN_CREATOR},
                {tblOfflinePayments::ofpUpdatedBy_usrID, ORM_JOIN_UPDATER},
                {tblOfflinePayments::ofp_invID,         R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
              })
{
}

}
}
}
