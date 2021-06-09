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

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant OnlinePayments::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);

    SelectQuery query = SelectQuery(this);
    APPLY_GET_METHOD_CALL_ARGS_TO_QUERY(query)
    return query.one();
}

OnlinePayments::OnlinePayments() :
    clsTable(AAASchema,
              tblOnlinePayments::Name,
              { ///<ColName                              Type                    Validation                          Default    UpBy   Sort  Filter Self  Virt   PK
                {tblOnlinePayments::onpID,                ORM_PRIMARY_KEY64},
                {tblOnlinePayments::onpMD5,               S(TAPI::MD5_t),         QFV,                                QRequired,  UPNone},
                {tblOnlinePayments::onpCreationDateTime,  ORM_CREATED_ON},
                {tblOnlinePayments::onp_vchID,            S(quint64),             QFV.integer().minValue(1),          QRequired,  UPNone},
                {tblOnlinePayments::onpPaymentGateway,    S(TAPI::enuPaymentGateway::Type),QFV,                       TAPI::enuPaymentGateway::Zibal, UPNone},
                {tblOnlinePayments::onpPGTrnID,           S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,   UPNone},
                {tblOnlinePayments::onpAmount,            S(TAPI::DateTime_t),    QFV,                                QRequired,   UPNone},
                {tblOnlinePayments::onpLastUpdateDateTime,S(TAPI::DateTime_t),    QFV,                                QAuto,       UPNone},
                {tblOnlinePayments::onpStatus,            S(TAPI::enuPaymentStatus::Type),QFV,                        TAPI::enuPaymentStatus::Pending, UPStatus},
                {tblOnlinePayments::onpResult,            S(QString),             QFV,                                QNull,      UPNone,false,false},
              },
              { ///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
                {tblOnlinePayments::onp_vchID,  R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
              })
{
}

/**********************************************************************************************/
QVariant OfflinePayments::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);

    SelectQuery query = SelectQuery(this);
    APPLY_GET_METHOD_CALL_ARGS_TO_QUERY(query)
    return query.one();
}

bool OfflinePayments::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

OfflinePayments::OfflinePayments() :
    clsTable(AAASchema,
              tblOnlinePayments::Name,
              { ///<ColName                               Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
                {tblOfflinePayments::ofpID,               ORM_PRIMARY_KEY64},
                {tblOfflinePayments::ofp_vchID,           S(quint64),             QFV.integer().minValue(1),          QRequired,  UPOwner},
                {tblOfflinePayments::ofpBank,             S(TAPI::MD5_t),         QFV,                                QRequired,  UPOwner},
                {tblOfflinePayments::ofpReceiptCode,      S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
                {tblOfflinePayments::ofpReceiptDate,      S(TAPI::DateTime_t),    QFV,                                QRequired,  UPOwner},
                {tblOfflinePayments::ofpAmount,           S(quint32),             QFV,                                QRequired,  UPOwner},
                {tblOfflinePayments::ofpNotes,            S(QString),             QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
                {tblOfflinePayments::ofpCreatedBy_usrID,  ORM_CREATED_BY},
                {tblOfflinePayments::ofpCreationDateTime, ORM_CREATED_ON},
                {tblOfflinePayments::ofpUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblOfflinePayments::ofpStatus,           S(TAPI::enuPaymentStatus::Type),QFV,                        TAPI::enuPaymentStatus::Pending, UPStatus},
              },
              { ///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
                {tblOfflinePayments::ofpCreatedBy_usrID, ORM_JOIN_CREATOR},
                {tblOfflinePayments::ofpUpdatedBy_usrID, ORM_JOIN_UPDATER},
                {tblOfflinePayments::ofp_vchID,         R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
              })
{
}

}
}
}
