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
#include "../Account.h"
#include "Payment/PaymentLogic.h"
#include "Interfaces/Helpers/URLHelper.h"
#include "Interfaces/Helpers/LanguageHelper.h"

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
|* PaymentGatewayTypesI18N ***************************************|
\*****************************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, PaymentGatewayTypesI18N)

PaymentGatewayTypesI18N::PaymentGatewayTypesI18N() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGatewayTypesI18N::Name,
        tblPaymentGatewayTypesI18N::Private::ORMFields,
        tblPaymentGatewayTypesI18N::Private::Relations,
        tblPaymentGatewayTypesI18N::Private::Indexes
) { ; }

/*****************************************************************\
|* PaymentGatewayTypes *******************************************|
\*****************************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, PaymentGatewayTypes)

PaymentGatewayTypes::PaymentGatewayTypes() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGatewayTypes::Name,
        tblPaymentGatewayTypes::Private::ORMFields,
        tblPaymentGatewayTypes::Private::Relations,
        tblPaymentGatewayTypes::Private::Indexes
) {
    PaymentGatewayTypesI18N::instance();
}

ORMSelectQuery PaymentGatewayTypes::makeSelectQuery(INTFAPICALLCONTEXT_IMPL &_apiCallContext, const QString &_alias, Q_DECL_UNUSED bool _translate, Q_DECL_UNUSED bool _isRoot) {
    PaymentGatewayTypesI18N::instance().prepareFiltersList();

    ORMSelectQuery Query = intfSQLBasedModule::makeSelectQuery(_apiCallContext, _alias, _translate);

    if (_translate) {
        Query
            .removeCol(tblPaymentGatewayTypes::Fields::pgtName)
            .leftJoin(tblPaymentGatewayTypesI18N::Name)
            .addCol(DBExpression::VALUE(QString("COALESCE("
                                                + LanguageHelper::getI18NClauseForCoalesce(_apiCallContext, tblPaymentGatewayTypesI18N::Name, "pgtName") + ","
                                                "%1.pgtName)")
                                        .arg(_alias.isEmpty() ? tblPaymentGatewayTypes::Name : _alias)
                                        ),
                    tblPaymentGatewayTypes::Fields::pgtName
                    )
        ;
    } else {
        Query
            .leftJoin(tblPaymentGatewayTypesI18N::Name, tblPaymentGatewayTypesI18N::Name)
            .addCol(R(tblPaymentGatewayTypesI18N::Name, tblPaymentGatewayTypesI18N::Fields::i18nData), tblPaymentGatewayTypes::Fields::pgtI18NData)
        ;
    }

    return Query;
}

QVariant IMPL_ORMGET_USER(PaymentGatewayTypes) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint32 IMPL_ORMCREATE_USER(PaymentGatewayTypes) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PUT, this->moduleBaseName()));
    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE_USER(PaymentGatewayTypes) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE_USER(PaymentGatewayTypes) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

/*****************************************************************\
|* PaymentGateways ***********************************************|
\*****************************************************************/
TARGOMAN_API_SUBMODULE_IMPLEMENT(Account, PaymentGateways)

PaymentGateways::PaymentGateways() :
    intfSQLBasedModule(
        AAASchema,
        tblPaymentGateways::Name,
        tblPaymentGateways::Private::ORMFields,
        tblPaymentGateways::Private::Relations,
        tblPaymentGateways::Private::Indexes
) { ; }

QVariant IMPL_ORMGET_USER(PaymentGateways) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_GET, this->moduleBaseName()));
    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint32 IMPL_ORMCREATE_USER(PaymentGateways) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    if (_createInfo.contains(tblPaymentGateways::Fields::pgwAllowedDomainName))
        _createInfo[tblPaymentGateways::Fields::pgwAllowedDomainName] = URLHelper::domain(_createInfo[tblPaymentGateways::Fields::pgwAllowedDomainName].toString());

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE_USER(PaymentGateways) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE_USER(PaymentGateways) {
    Authorization::checkPriv(_apiCallContext, this->privOn(EHTTP_DELETE, this->moduleBaseName()));
    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
}

QVariantList IMPL_REST_GET_OR_POST(PaymentGateways, availableGatewayTypes, (
    APICALLCONTEXT_TYPE_JWT_USER_IMPL &_apiCallContext,
    quint32 _amount,
    QString _domain
)) {
    _domain = URLHelper::domain(_domain, true);

    if (_amount == 0)
        throw exHTTPBadRequest("amount is zero");

    return PaymentLogic::findAvailableGatewayTypes(
                _apiCallContext,
                _amount,
                _domain
                );
}

} //namespace ORM
} //namespace Targoman::API::AccountModule
