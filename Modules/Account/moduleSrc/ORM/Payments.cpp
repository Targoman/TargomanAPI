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
#include "Interfaces/Helpers/URLHelper.h"
#include "Interfaces/Helpers/RESTClientHelper.h"
using namespace Targoman::API::Helpers;

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

    auto fnTouchQuery = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, fnTouchQuery);
}

#ifdef QT_DEBUG
QVariant IMPL_REST_GET_OR_POST(OnlinePayments, devTestPayPage, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _paymentKey,
    Q_DECL_UNUSED QString _trackID,
    QString _callback
)) {
/*

curl -v -H 'accept: application/json' -X 'GET' 'http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestPayPage?paymentKey=ppp&trackID=aaa&callback=http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage'

http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestPayPage?paymentKey=ppp&trackID=aaa&callback=http://127.0.0.1:10000/rest/v1/Account/OnlinePayments/devTestCallbackPage

*/

    if (_callback.isEmpty())
        _callback = QString("%1/Account/OnlinePayments/devTestCallbackPage?paymentKey=%2")
                    .arg("https://newapi.targoman.ir/rest/v1") //ClientConfigs::RESTServerAddress.value())
                    .arg(_paymentKey)
                    ;
    else
        _callback = URLHelper::addParameter(_callback, "paymentKey", _paymentKey);

    QByteArray Content = R"(
<html>
    <head>
        <title>Payment Page</title>
    </head>
    <body>
        <p>&nbsp;</p>
        <h1 style='text-align:center; margin:0;'>DevTest</h1><br>
        <h2 style='text-align:center; margin:0;'>Payment Page</h2><br>
        <hr>
        <div style='display: grid; justify-items: center; grid-template-columns: 1fr 1fr; align-items: start;'>
            <div style='width:90%'>
                <p style='text-align:center'>GET:</p>
                <div style='display: grid; justify-items: center; grid-template-columns: 1fr 1fr;'>
                    <div>
                        <a href='{URL_OK}'>[OK]</a>
                    </div>
                    <div>
                        <a href='{URL_ERROR}'>[ERROR]</a>
                    </div>
                </div>
                <hr>
                <p style='text-align:center'>POST:</p>
                <div style='display: grid; justify-items: center; grid-template-columns: 1fr 1fr;'>
                    <div>
                        <form action='{URL_OK}' method='post'>
                            <input type='submit' value='[OK]'>
                        </form>
                    </div>
                    <div>
                        <form action='{URL_ERROR}' method='post'>
                            <input type='submit' value='[ERROR]'>
                        </form>
                    </div>
                </div>
                <hr>
            </div>
            <div style='display: grid; grid-template-columns: auto 100%; grid-column-gap: 10px;'>
                <div style='white-space: nowrap;'>Voucher ID:</div><div>{VOUCHER_ID}</div>
                <div style='white-space: nowrap;'>Wallet ID:</div><div>{WALLET_ID}</div>
                <div style='white-space: nowrap;'>Amount:</div><div>{AMOUNT}</div>
                <div style='white-space: nowrap;'>Callback Url:</div><div>{CALLBACK_URL}</div>
            </div>
        </div>
    </body>
</html>
)";

    tblOnlinePayments::DTO OnlinePaymentsDTO = SelectQuery(OnlinePayments::instance())
                                               .where({ tblOnlinePayments::Fields::onpMD5, enuConditionOperator::Equal, _paymentKey })
                                               .one<tblOnlinePayments::DTO>();

    Content = Content
              .replace("{URL_OK}", URLHelper::addParameter(_callback, "result", "ok").toLatin1())
              .replace("{URL_ERROR}", URLHelper::addParameter(_callback, "result", "error").toLatin1())
              .replace("{VOUCHER_ID}", QString::number(OnlinePaymentsDTO.onp_vchID).toLatin1())
              .replace("{WALLET_ID}", QString::number(NULLABLE_GET_OR_DEFAULT(OnlinePaymentsDTO.onpTarget_walID, 0)).toLatin1())
              .replace("{AMOUNT}", QString::number(OnlinePaymentsDTO.onpAmount).toLatin1())
              .replace("{CALLBACK_URL}", _callback.toLatin1())
    ;

    return TAPI::RawData_t(Content,
                           "text/html; charset=utf-8"
                           ).toVariant();

//    return TAPI::ResponseRedirect_t(_callback, false).toVariant();
}

QVariant IMPL_REST_GET_OR_POST(OnlinePayments, devTestCallbackPage, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _paymentKey,
    QString _result
)) {
    return RESTClientHelper::callAPI(
        RESTClientHelper::POST,
        "Account/approveOnlinePayment",
        {},
        {
            { "paymentKey",     _paymentKey },
            { "domain",         "dev.test" },
            { "pgResponse",     QVariantMap({
                  { "result",   _result },
            }) },
        }
    );
}
#endif

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

    auto fnTouchQuery = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, fnTouchQuery);
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

    auto fnTouchQuery = [](SelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, fnTouchQuery);
}

bool IMPL_ORMUPDATE(OfflinePayments) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

/*****************************************************************/
} //namespace ORM
} //namespace Targoman::API::AccountModule
