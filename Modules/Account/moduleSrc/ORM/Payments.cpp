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
#include "Payment/PaymentLogic.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentStatus);

namespace Targoman::API::AccountModule {

void stuOnlinePayment::fromVariantMap(
    const QVariantMap& _info
) {
    QJsonObject JsonInfo = QJsonObject::fromVariantMap(_info);

    this->OnlinePayment.fromJson(JsonInfo);

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
        {///< ColName                                   Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { tblOnlinePayments::onpID,                 ORM_PRIMARYKEY_64 },
            { tblOnlinePayments::onpMD5,                S(TAPI::MD5_t),         QFV,                                QRequired,  UPAdmin },
            { tblOnlinePayments::onp_vchID,             S(quint64),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { tblOnlinePayments::onp_pgwID,             S(quint32),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { tblOnlinePayments::onpTrackNumber,        S(QString),             QFV.allwaysValid().maxLenght(50),   QNull,      UPAdmin },
            { tblOnlinePayments::onpAmount,             S(quint64),             QFV.integer().minValue(1),          QRequired,  UPAdmin },
            { tblOnlinePayments::onpTarget_walID,       S(quint64),             QFV,                                QNull,      UPNone },
            { tblOnlinePayments::onpResult,             S(QString),             QFV,                                QNull,      UPAdmin, false, false },
            { tblOnlinePayments::onpStatus,             ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { tblOnlinePayments::onpCreationDateTime,   ORM_CREATED_ON },
            { tblOnlinePayments::onpLastUpdateDateTime, ORM_UPDATED_ON },
        },
        {///<                     Col                           Reference Table                            ForeignCol         Rename     LeftJoin
            { "voucher",        { tblOnlinePayments::onp_vchID, R(AAASchema, tblVoucher::Name),            tblVoucher::vchID } },
            { "paymentGateway", { tblOnlinePayments::onp_pgwID, R(AAASchema, tblPaymentGateways::Name),    tblPaymentGateways::pgwID } },
        }
    ) { ; }

QVariant IMPL_ORMGET(OnlinePayments) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, 0, QueryLambda);
}

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/
OfflinePaymentClaims::OfflinePaymentClaims() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePaymentClaims::Name,
        {///< ColName                                           Type                        Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { tblOfflinePaymentClaims::ofpcID,                  ORM_PRIMARYKEY_64},
            { tblOfflinePaymentClaims::ofpc_vchID,              S(NULLABLE_TYPE(quint64)),  QFV.integer().minValue(1),          QNull,      UPOwner},
            { tblOfflinePaymentClaims::ofpcBank,                S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePaymentClaims::ofpcReceiptCode,         S(QString),                 QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePaymentClaims::ofpcReceiptDate,         S(TAPI::DateTime_t),        QFV,                                QRequired,  UPOwner},
            { tblOfflinePaymentClaims::ofpcAmount,              S(quint32),                 QFV,                                QRequired,  UPOwner},
            { tblOfflinePaymentClaims::ofpcTarget_walID,        S(quint64),                 QFV,                                QNull,      UPNone },
            { tblOfflinePaymentClaims::ofpcNotes,               S(QString),                 QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
            { tblOfflinePaymentClaims::ofpcStatus,              ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::New) },
            { tblOfflinePaymentClaims::ofpcCreationDateTime,    ORM_CREATED_ON},
            { tblOfflinePaymentClaims::ofpcCreatedBy_usrID,     ORM_CREATED_BY},
            { tblOfflinePaymentClaims::ofpcUpdatedBy_usrID,     ORM_UPDATED_BY},
        },
        {///< Col                                               Reference Table                 ForeignCol          Rename     LeftJoin
            { tblOfflinePaymentClaims::ofpc_vchID,              R(AAASchema, tblVoucher::Name), tblVoucher::vchID },
            ORM_RELATION_OF_CREATOR(tblOfflinePaymentClaims::ofpcCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblOfflinePaymentClaims::ofpcUpdatedBy_usrID),
        }
    ) { ; }

QVariant IMPL_ORMGET(OfflinePaymentClaims) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, 0, QueryLambda);
}

//quint32 IMPL_ORMCREATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    return this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
//}

//bool IMPL_ORMUPDATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
//    return this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
//}

//bool IMPL_ORMDELETE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));

//    return this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
//}

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
OfflinePayments::OfflinePayments() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePayments::Name,
        {///< ColName                                   Type                    Validation                          Default     UpBy   Sort  Filter Self  Virt   PK
            { tblOfflinePayments::ofpID,                ORM_PRIMARYKEY_64},
            { tblOfflinePayments::ofp_vchID,            S(quint64),             QFV.integer().minValue(1),          QRequired,  UPOwner},
            { tblOfflinePayments::ofpBank,              S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptCode,       S(QString),             QFV.allwaysValid().maxLenght(50),   QRequired,  UPOwner},
            { tblOfflinePayments::ofpReceiptDate,       S(TAPI::DateTime_t),    QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpAmount,            S(quint32),             QFV,                                QRequired,  UPOwner},
            { tblOfflinePayments::ofpTarget_walID,      S(quint64),             QFV,                                QNull,      UPNone },
            { tblOfflinePayments::ofpNotes,             S(QString),             QFV.allwaysValid().maxLenght(500),  QNull,      UPOwner},
            { tblOfflinePayments::ofpStatus,            ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentStatus, Targoman::API::AccountModule::enuPaymentStatus::Pending) },
            { tblOfflinePayments::ofpCreationDateTime,  ORM_CREATED_ON},
            { tblOfflinePayments::ofpCreatedBy_usrID,   ORM_CREATED_BY},
            { tblOfflinePayments::ofpUpdatedBy_usrID,   ORM_UPDATED_BY},
        },
        {///< Col                        Reference Table                  ForeignCol         Rename     LeftJoin
            { tblOfflinePayments::ofp_vchID,         R(AAASchema,tblVoucher::Name),   tblVoucher::vchID},
            ORM_RELATION_OF_CREATOR(tblOfflinePayments::ofpCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblOfflinePayments::ofpUpdatedBy_usrID),
        }
    ) { ; }

QVariant IMPL_ORMGET(OfflinePayments) {
    if (Authorization::hasPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, 0, QueryLambda);
}

bool IMPL_ORMUPDATE(OfflinePayments) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

/*****************************************************************/
} //namespace ORM
} //namespace Targoman::API::AccountModule
