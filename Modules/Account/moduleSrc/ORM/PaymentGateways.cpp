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
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTypeStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayStatus);
TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentGatewayTransactionFeeType);

namespace Targoman::API::AccountModule {

namespace ORM {

/*****************************************************************\
|* PaymentGatewayTypes *******************************************|
\*****************************************************************/
PaymentGatewayTypes::PaymentGatewayTypes() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGatewayTypes::Name,
        tblPaymentGatewayTypes::Private::ORMFields,
        tblPaymentGatewayTypes::Private::Relations,
        tblPaymentGatewayTypes::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(PaymentGatewayTypes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(PaymentGatewayTypes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(PaymentGatewayTypes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(PaymentGatewayTypes) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/*****************************************************************\
|* PaymentGateways ***********************************************|
\*****************************************************************/
PaymentGateways::PaymentGateways() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGateways::Name,
        tblPaymentGateways::Private::ORMFields,
        tblPaymentGateways::Private::Relations,
        tblPaymentGateways::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

quint32 IMPL_ORMCREATE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    if (_createInfo.contains(tblPaymentGateways::Fields::pgwAllowedDomainName))
        _createInfo[tblPaymentGateways::Fields::pgwAllowedDomainName] = URLHelper::domain(_createInfo[tblPaymentGateways::Fields::pgwAllowedDomainName].toString());

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMDELETE(PaymentGateways) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
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
