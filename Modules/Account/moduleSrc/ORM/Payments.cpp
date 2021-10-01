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
//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentStatus);

namespace Targoman::API::AccountModule {

void stuOnlinePayment::fromVariantMap(const QVariantMap& _info)
{
    SET_FIELD_FROM_VARIANT_MAP(this->onpID,      _info, ORM::tblOnlinePayments, onpID);
    SET_FIELD_FROM_VARIANT_MAP(this->onpMD5,     _info, ORM::tblOnlinePayments, onpMD5);
    SET_FIELD_FROM_VARIANT_MAP(this->onp_vchID,  _info, ORM::tblOnlinePayments, onp_vchID);
    SET_FIELD_FROM_VARIANT_MAP(this->onp_pgwID,  _info, ORM::tblOnlinePayments, onp_pgwID);
    SET_FIELD_FROM_VARIANT_MAP(this->onpPGTrnID, _info, ORM::tblOnlinePayments, onpPGTrnID);
    SET_FIELD_FROM_VARIANT_MAP(this->onpAmount,  _info, ORM::tblOnlinePayments, onpAmount);
    SET_FIELD_FROM_VARIANT_MAP(this->onpResult,  _info, ORM::tblOnlinePayments, onpResult);
    SET_FIELD_FROM_VARIANT_MAP(this->onpStatus,  _info, ORM::tblOnlinePayments, onpStatus);

    this->PaymentGateway.fromVariantMap(_info);
}

namespace ORM {

/*****************************************************************\
|* OnlinePayments ************************************************|
\*****************************************************************/
OnlinePayments::OnlinePayments() :
    intfSQLBasedModule(
        AAASchema,
        tblOnlinePayments::Name,
        {///< ColName                                   Type                    Validation                           Default    UpBy   Sort  Filter Self  Virt   PK
            { tblOnlinePayments::onpID,                 ORM_PRIMARYKEY_64 },
            { tblOnlinePayments::onpMD5,                S(TAPI::MD5_t),         QFV,                                 QRequired, UPAdmin },
            { tblOnlinePayments::onp_vchID,             S(quint64),             QFV.integer().minValue(1),           QRequired, UPAdmin },
            { tblOnlinePayments::onp_pgwID,             S(quint32),             QFV.integer().minValue(1),           QRequired, UPAdmin },
            { tblOnlinePayments::onpPGTrnID,            S(QString),             QFV.allwaysValid().maxLenght(50),    QNull,     UPAdmin },
            { tblOnlinePayments::onpAmount,             S(quint64),             QFV.integer().minValue(1),           QRequired, UPAdmin },
            { tblOnlinePayments::onpResult,             S(QString),             QFV,                                 QNull,     UPAdmin, false, false },
            { tblOnlinePayments::onpStatus,             ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending) },
            { tblOnlinePayments::onpCreationDateTime,   ORM_CREATED_ON },
            { tblOnlinePayments::onpLastUpdateDateTime, ORM_UPDATED_ON },
        },
        {///<                     Col                           Reference Table                            ForeignCol         Rename     LeftJoin
            { "voucher",        { tblOnlinePayments::onp_vchID, R(AAASchema, tblVoucher::Name),            tblVoucher::vchID } },
            { "paymentGateway", { tblOnlinePayments::onp_pgwID, R(AAASchema, tblPaymentGateways::Name),    tblPaymentGateways::pgwID } },
        }
    )
{}

QVariant OnlinePayments::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
OfflinePayments::OfflinePayments() :
    intfSQLBasedModule(
        AAASchema,
        tblOnlinePayments::Name,
        {///< ColName                                  Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { tblOfflinePayments::ofpID,               ORM_PRIMARYKEY_64},
            { tblOfflinePayments::ofp_vchID,           S(quint64),             QFV.integer().minValue(1),          QRequired,  UPOwner},
            { tblOfflinePayments::ofpBank,             S(TAPI::MD5_t),         QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptCode,      S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptDate,      S(TAPI::DateTime_t),    QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpAmount,           S(quint32),             QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpNotes,            S(QString),             QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
            { tblOfflinePayments::ofpStatus,           ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending) },
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

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool OfflinePayments::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace ORM
} //namespace Targoman::API::AccountModule
