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
#include "Payment/PaymentLogic.h"
#include "Interfaces/Helpers/URLHelper.h"

using namespace Targoman::API::AccountModule::Payment;
using namespace Targoman::API::Helpers;

//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGateway);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayType);
//TAPI_REGISTER_TARGOMAN_ENUM(TAPI, enuPaymentGatewayDriver);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

namespace Targoman::API::AccountModule {

namespace ORM {

PaymentGateways::PaymentGateways() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGateways::Name,
        {///< ColName                                       Type                                                Validation                          Default     UpBy     Sort   Filter Self  Virt   PK
            { tblPaymentGateways::pgwID,                    ORM_PRIMARYKEY_32 },
            { tblPaymentGateways::pgwName,                  S(QString),                                         QFV.unicodeAlNum().maxLenght(64),   QRequired,  UPAdmin },
            { tblPaymentGateways::pgwType,                  S(Targoman::API::AccountModule::enuPaymentGatewayType::Type),               QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwDriver,                S(QString),                                         QFV,                                QRequired,  UPAdmin },
            { tblPaymentGateways::pgwMetaInfo,              S(NULLABLE_TYPE(TAPI::JSON_t)),                     QFV,                                QNull,      UPAdmin },
            { tblPaymentGateways::pgw_curID,                S(quint32),                                         QFV,                                QRequired,  UPAdmin },
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
    ) { ; }

QVariant IMPL_ORMGET(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

    if (_createInfo.contains(tblPaymentGateways::pgwAllowedDomainName))
        _createInfo[tblPaymentGateways::pgwAllowedDomainName] = URLHelper::domain(_createInfo[tblPaymentGateways::pgwAllowedDomainName].toString());

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

QVariantList IMPL_REST_GET_OR_POST(PaymentGateways, availableGatewayTypes, (
    APICALLBOOM_TYPE_JWT_IMPL &APICALLBOOM_PARAM,
    quint32 _amount,
    QString _domain
)) {
    if (_amount == 0)
        throw exHTTPBadRequest("amount is zero");

    return PaymentLogic::findAvailableGatewayTypes(
                APICALLBOOM_PARAM,
                _amount,
                _domain
                );
}

} //namespace ORM
} //namespace Targoman::API::AccountModule
