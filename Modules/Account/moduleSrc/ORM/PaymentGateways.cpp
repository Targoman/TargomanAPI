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
#include "Interfaces/Helpers/URLHelper.h"
using namespace Targoman::API::Helpers;

//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGateway);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayType);
//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGatewayDriver);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

namespace Targoman::API::AccountModule {

void stuPaymentGateway::fromVariantMap(const QVariantMap& _info)
{
    SET_FIELD_FROM_VARIANT_MAP(this->pgwID,                  _info, ORM::tblPaymentGateways, pgwID);
    SET_FIELD_FROM_VARIANT_MAP(this->pgwName,                _info, ORM::tblPaymentGateways, pgwName);
    SET_FIELD_FROM_VARIANT_MAP(this->pgwType,                _info, ORM::tblPaymentGateways, pgwType);
    SET_FIELD_FROM_VARIANT_MAP(this->pgwDriver,              _info, ORM::tblPaymentGateways, pgwDriver);
    SET_FIELD_FROM_VARIANT_MAP(this->pgwMetaInfo,            _info, ORM::tblPaymentGateways, pgwMetaInfo);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwTransactionFeeValue, _info, ORM::tblPaymentGateways, pgwTransactionFeeValue);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwTransactionFeeType,  _info, ORM::tblPaymentGateways, pgwTransactionFeeType);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMinRequestAmount,    _info, ORM::tblPaymentGateways, pgwMinRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMaxRequestAmount,    _info, ORM::tblPaymentGateways, pgwMaxRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwMaxPerDayAmount,     _info, ORM::tblPaymentGateways, pgwMaxPerDayAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwLastPaymentDateTime, _info, ORM::tblPaymentGateways, pgwLastPaymentDateTime);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumTodayPaidAmount,  _info, ORM::tblPaymentGateways, pgwSumTodayPaidAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumRequestCount,     _info, ORM::tblPaymentGateways, pgwSumRequestCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumRequestAmount,    _info, ORM::tblPaymentGateways, pgwSumRequestAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumFailedCount,      _info, ORM::tblPaymentGateways, pgwSumFailedCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumOkCount,          _info, ORM::tblPaymentGateways, pgwSumOkCount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwSumPaidAmount,       _info, ORM::tblPaymentGateways, pgwSumPaidAmount);
//        SET_FIELD_FROM_VARIANT_MAP(this->pgwStatus,              _info, ORM::tblPaymentGateways, pgwStatus);
}

namespace ORM {

PaymentGateways::PaymentGateways() :
    clsTable(
        AAASchema,
        tblPaymentGateways::Name,
        {///< ColName                                       Type                                                Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { tblPaymentGateways::pgwID,                    ORM_PRIMARYKEY_32 },
            { tblPaymentGateways::pgwName,                  S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { tblPaymentGateways::pgwType,                  S(Targoman::API::AccountModule::enuPaymentGatewayType::Type),               QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwDriver,                S(QString),                                         QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwMetaInfo,              S(NULLABLE_TYPE(TAPI::JSON_t)),                     QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwAllowedDomainName,     S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            //------------------
            { tblPaymentGateways::pgwTransactionFeeValue,   S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwTransactionFeeType,    S(Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Type), QFV, Targoman::API::AccountModule::enuPaymentGatewayTransactionFeeType::Currency, UPAdmin },
            //------------------
            { tblPaymentGateways::pgwMinRequestAmount,      S(quint32),                                         QFV.minValue(1),                    1,          UPAdmin },
            { tblPaymentGateways::pgwMaxRequestAmount,      S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwMaxPerDayAmount,       S(NULLABLE_TYPE(quint32)),                          QFV,                                QNull,      UPAdmin },
            //------------------
            { tblPaymentGateways::pgwLastPaymentDateTime,   S(NULLABLE_TYPE(TAPI::DateTime_t)),                 QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgwSumTodayPaidAmount,    S(quint64),                                         QFV,                                0,          UPAdmin },
            //------------------
            { tblPaymentGateways::pgwSumRequestCount,       S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumRequestAmount,      S(quint64),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumFailedCount,        S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumOkCount,            S(quint32),                                         QFV,                                0,          UPAdmin },
            { tblPaymentGateways::pgwSumPaidAmount,         S(quint64),                                         QFV,                                0,          UPAdmin },
            //------------------
            { tblPaymentGateways::pgwStatus,                ORM_STATUS_FIELD(Targoman::API::AccountModule::enuPaymentGatewayStatus, Targoman::API::AccountModule::enuPaymentGatewayStatus::Active) },
            { tblPaymentGateways::pgwCreationDateTime,      ORM_CREATED_ON },
            { tblPaymentGateways::pgwCreatedBy_usrID,       ORM_CREATED_BY },
            { tblPaymentGateways::pgwUpdatedBy_usrID,       ORM_UPDATED_BY },
        },
        {///< Col                        Reference Table              ForeignCol       Rename     LeftJoin
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

    if (_createInfo.contains(tblPaymentGateways::pgwAllowedDomainName))
        _createInfo[tblPaymentGateways::pgwAllowedDomainName] = URLHelper::domain(_createInfo[tblPaymentGateways::pgwAllowedDomainName].toString());

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

} //namespace ORM
} //namespace Targoman::API::AccountModule
