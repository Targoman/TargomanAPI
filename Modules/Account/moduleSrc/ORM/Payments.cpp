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
#include "Payment/PaymentLogic.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuPaymentStatus);

namespace Targoman::API::AccountModule {

void stuOnlinePayment::fromVariantMap(
    const QVariantMap& _info
) {
    QJsonObject JsonInfo = QJsonObject::fromVariantMap(_info);

    this->OnlinePayment.fromJson(JsonInfo);
    this->PaymentGateway.fromJson(JsonInfo);
}

namespace ORM {

/*****************************************************************\
|* OnlinePayments ************************************************|
\*****************************************************************/
OnlinePayments::OnlinePayments() :
    intfSQLBasedModule(
        AAASchema,
        tblOnlinePayments::Name,
        tblOnlinePayments::Private::ORMFields,
        tblOnlinePayments::Private::Relations,
        tblOnlinePayments::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(OnlinePayments) {
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, QueryLambda);
}

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/
OfflinePaymentClaims::OfflinePaymentClaims() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePaymentClaims::Name,
        tblOfflinePaymentClaims::Private::ORMFields,
        tblOfflinePaymentClaims::Private::Relations,
        tblOfflinePaymentClaims::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(OfflinePaymentClaims) {
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, QueryLambda);
}

//quint32 IMPL_ORMCREATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
//}

//bool IMPL_ORMUPDATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
//    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
//}

//bool IMPL_ORMDELETE(OfflinePaymentClaims) {
//    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

//    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM);
//}

/*****************************************************************\
|* OfflinePayments ***********************************************|
\*****************************************************************/
OfflinePayments::OfflinePayments() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePayments::Name,
        tblOfflinePayments::Private::ORMFields,
        tblOfflinePayments::Private::Relations,
        tblOfflinePayments::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(OfflinePayments) {
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, QueryLambda);
}

bool IMPL_ORMUPDATE(OfflinePayments) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/*****************************************************************/
} //namespace ORM
} //namespace Targoman::API::AccountModule
