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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "Payments.h"
#include "Voucher.h"
#include "Classes/PaymentLogic.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentStatus);

namespace Targoman::API::AAA {

using namespace ORM;

/*****************************************************************\
|* OnlinePayments ************************************************|
\*****************************************************************/
OnlinePayments::OnlinePayments() :
    clsTable(
        AAASchema,
        tblOnlinePayments::Name,
        {///< ColName                                   Type                    Validation                           Default    UpBy   Sort  Filter Self  Virt   PK
            { tblOnlinePayments::onpID,                 ORM_PRIMARY_KEY64 },
            { tblOnlinePayments::onpMD5,                S(TAPI::MD5_t),         QFV,                                 QRequired, UPNone },
            { tblOnlinePayments::onp_vchID,             S(quint64),             QFV.integer().minValue(1),           QRequired, UPNone },
//            { tblOnlinePayments::onpPaymentGateway,     S(TAPI::enuPaymentGateway::Type),QFV,                        TAPI::enuPaymentGateway::Zibal, UPNone },
            { tblOnlinePayments::onp_pgwID,             S(quint64),             QFV.integer().minValue(1),           QRequired, UPNone },
            { tblOnlinePayments::onpPGTrnID,            S(QString),             QFV.allwaysValid().maxLenght(50),    QNull,     UPNone },
            { tblOnlinePayments::onpAmount,             S(quint64),             QFV.integer().minValue(1),           QRequired, UPNone },
            { tblOnlinePayments::onpResult,             S(QString),             QFV,                                 QNull,     UPNone, false, false },
            { tblOnlinePayments::onpStatus,             ORM_STATUS_FIELD(TAPI::enuPaymentStatus, TAPI::enuPaymentStatus::Pending) },
            { tblOnlinePayments::onpCreationDateTime,   ORM_CREATED_ON },
            { tblOnlinePayments::onpLastUpdateDateTime, ORM_UPDATED_ON },
        },
        {///< Col                            Reference Table                            ForeignCol         Rename     LeftJoin
            { tblOnlinePayments::onp_vchID,  R(AAASchema, tblVoucher::Name),            tblVoucher::vchID },
            { tblOnlinePayments::onp_pgwID,  R(AAASchema, tblPaymentGateways::Name),    tblPaymentGateways::pgwID },
        }
    )
{}

QVariant OnlinePayments::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
OfflinePayments::OfflinePayments() :
    clsTable(
        AAASchema,
        tblOnlinePayments::Name,
        {///< ColName                                  Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { tblOfflinePayments::ofpID,               ORM_PRIMARY_KEY64},
            { tblOfflinePayments::ofp_vchID,           S(quint64),             QFV.integer().minValue(1),          QRequired,  UPOwner},
            { tblOfflinePayments::ofpBank,             S(TAPI::MD5_t),         QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptCode,      S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptDate,      S(TAPI::DateTime_t),    QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpAmount,           S(quint32),             QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpNotes,            S(QString),             QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
            { tblOfflinePayments::ofpStatus,           ORM_STATUS_FIELD(TAPI::enuPaymentStatus, TAPI::enuPaymentStatus::Pending) },
            { tblOfflinePayments::ofpCreationDateTime, ORM_CREATED_ON},
            { tblOfflinePayments::ofpCreatedBy_usrID,  ORM_CREATED_BY},
            { tblOfflinePayments::ofpUpdatedBy_usrID,  ORM_UPDATED_BY},
        },
        {///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
            { tblOfflinePayments::ofp_vchID,         R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
            ORM_RELATION_OF_CREATOR(tblOfflinePayments::ofpCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblOfflinePayments::ofpUpdatedBy_usrID),
        }
    )
{}

QVariant OfflinePayments::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool OfflinePayments::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::AAA
