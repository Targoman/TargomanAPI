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

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

PaymentGateways::PaymentGateways() :
    clsTable(
        AAASchema,
        tblPaymentGateways::Name,
        {///< ColName                                   Type                                    Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { tblPaymentGateways::pgwID,                ORM_PRIMARY_KEY32 },
            { tblPaymentGateways::pgwName,              S(QString),                             QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { tblPaymentGateways::pgwUUID,              S(TAPI::MD5_t),                         QFV,                                QAuto,      UPAdmin },
            { tblPaymentGateways::pgwType,              S(TAPI::enuPaymentGatewayType::Type),   QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwDriver,            S(TAPI::enuPaymentGatewayDriver::Type), QFV,                                QRequired,  UPAdmin, false, false },
            { tblPaymentGateways::pgwCredentials,       S(TAPI::JSON_t),                        QFV,                                QRequired,  UPAdmin, false, false },
            { tblPaymentGateways::pgwStatus,            ORM_STATUS_FIELD(TAPI::enuPaymentGatewayStatus, TAPI::enuPaymentGatewayStatus::Active) },
//            { ORM_INVALIDATED_AT_FIELD },
            { tblPaymentGateways::pgwCreationDateTime,  ORM_CREATED_ON },
            { tblPaymentGateways::pgwCreatedBy_usrID,   ORM_CREATED_BY },
            { tblPaymentGateways::pgwUpdatedBy_usrID,   ORM_UPDATED_BY },
        }
        ,{///<Col                        Reference Table              ForeignCol       Rename     LeftJoin
            ORM_RELATION_OF_CREATOR(tblPaymentGateways::pgwCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblPaymentGateways::pgwUpdatedBy_usrID),
        }
//        ,{
//            { {
//                tblPaymentGateways::pgwName,
//                ORM_INVALIDATED_AT_FIELD_NAME,
//              }, enuDBIndex::Unique },
//        }
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

}
}
}
