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

#include "PaymentGateways.h"
#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGatewayType);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGatewayDriver);
TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGatewayStatus);

namespace Targoman::API::AAA {

using namespace ORM;

PaymentGateways::PaymentGateways() :
    clsTable(
        AAASchema,
        tblPaymentGateways::Name,
        {///< ColName                                       Type                                                Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { tblPaymentGateways::pgwID,                    ORM_PRIMARY_KEY32 },
            { tblPaymentGateways::pgwName,                  S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { tblPaymentGateways::pgwType,                  S(TAPI::enuPaymentGatewayType::Type),               QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwDriver,                S(TAPI::enuPaymentGatewayDriver::Type),             QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwMetaInfo,              S(TAPI::JSON_t),                                    QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwMinPayAmount,          S(quint32),                                         QFV.minValue(1),                    QRequired,  UPAdmin },
            { tblPaymentGateways::pgwMaxPayAmount,          S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwTransactionFee,        S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwTransactionFeeType,    S(TAPI::enuPaymentGatewayTransactionFeeType::Type), QFV,                                TAPI::enuPaymentGatewayTransactionFeeType::Percent, UPAdmin },
            { tblPaymentGateways::pgwLastPaymentDateTime,   S(NULLABLE_TYPE(TAPI::DateTime_t)),                 QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwSumTodayPaidAmount,    S(quint64),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumRequestCount,       S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumRequestAmount,      S(quint64),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumFailedCount,        S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumOkCount,            S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumPaidAmount,         S(quint64),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwStatus,                ORM_STATUS_FIELD(TAPI::enuPaymentGatewayStatus, TAPI::enuPaymentGatewayStatus::Active) },
//            { ORM_INVALIDATED_AT_FIELD },
            { tblPaymentGateways::pgwCreationDateTime,      ORM_CREATED_ON },
            { tblPaymentGateways::pgwCreatedBy_usrID,       ORM_CREATED_BY },
            { tblPaymentGateways::pgwUpdatedBy_usrID,       ORM_UPDATED_BY },
        },
        {///<Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblPaymentGateways::pgwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblPaymentGateways::pgwUpdatedBy_usrID),
        }
    )
{}

QVariant PaymentGateways::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);
}

quint32 PaymentGateways::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool PaymentGateways::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool PaymentGateways::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return Targoman::API::Query::DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

} //namespace Targoman::API::AAA
