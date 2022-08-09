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
#include "Interfaces/Server/ServerCommon.h"
using namespace Targoman::API::Server;
#include "Account.h"

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
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({
                                 { tblVoucher::Fields::vch_usrID, APICALLBOOM_PARAM.getUserID() }
                             }, _filters);

    auto fnTouchQuery = [](ORMSelectQuery &_query) {
        _query.innerJoin(tblVoucher::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, {}, 0, fnTouchQuery);
}

/**
  * this proxy received GET and POST and make GET call to the clients callback
  */
QVariant IMPL_REST_GET_OR_POST(OnlinePayments, paymentCallback, (
    APICALLBOOM_TYPE_NO_JWT_IMPL &APICALLBOOM_PARAM,
    QString _paymentKey
)) {
//curl -X POST 'http://localhost:10000/rest/v1/Account/OnlinePayments/paymentCallback?paymentKey=11&q1=qv1' -H 'content-type:application/json' -d '{"d1":"dv1"}'

    _paymentKey = _paymentKey.trimmed();
    if (_paymentKey.isEmpty())
        throw exTargomanBase("paymentKey is empty", ESTATUS_INTERNAL_SERVER_ERROR);

    tblOnlinePayments::DTO OnlinePaymentDTO = OnlinePayments::instance().makeSelectQuery(APICALLBOOM_PARAM)
//            .addCols(OnlinePayments::instance().selectableColumnNames())
//            .addCols(PaymentGateways::instance().selectableColumnNames())
//            .innerJoinWith("paymentGateway")
//            .addCol(tblOnlinePayments::Fields::onpCallbackUrl)
            .where({ tblOnlinePayments::Fields::onpMD5, enuConditionOperator::Equal, _paymentKey })
            .one<tblOnlinePayments::DTO>();

    //-------------------------------------------------
    QVariantMap ResponseParams;

    ResponseParams.insert("vchID", OnlinePaymentDTO.onp_vchID);

    QList<QPair<QString, QString>> RequestQueryParams = APICALLBOOM_PARAM.getRequestQueryParams();
    foreach (auto Item, RequestQueryParams) {
//        if (Item.first == "paymentKey")
//            continue;
        ResponseParams.insert(Item.first, Item.second);
    }

    QList<QPair<QString, QString>> RequestBodyParams = APICALLBOOM_PARAM.getRequestBodyParams();
    foreach (auto Item, RequestBodyParams) {
        ResponseParams.insert(Item.first, Item.second);
    }

    //-------------------------------------------------
    QString Callback = OnlinePaymentDTO.onpCallbackUrl;
    Callback = URLHelper::addParameters(Callback, ResponseParams);

    return TAPI::ResponseRedirect_t(Callback, false, qhttp::ESTATUS_SEE_OTHER).toVariant();
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

    QString HostPort = APICALLBOOM_PARAM.hostAndPort();
    QString ServerUrl = QString("https://%1%2")
                        .arg(HostPort)
                        .arg(ServerCommonConfigs::BasePathWithVersion);

    if (_callback.isEmpty())
        _callback = QString("%1Account/OnlinePayments/devTestCallbackPage?paymentKey=%2")
                    .arg(ServerUrl) //ClientConfigs::RESTServerAddress.value())
                    .arg(_paymentKey)
                    ;
    else {
        _callback = URLHelper::decode(_callback);
        _callback = URLHelper::addParameter(_callback, "paymentKey", _paymentKey);
    }

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
                <div style='white-space: nowrap;'>API Callback Url:</div><div>{API_CALLBACK_URL}</div>
                <div style='white-space: nowrap;'>UI Callback Url:</div><div>{UI_CALLBACK_URL}</div>
            </div>
        </div>
    </body>
</html>
)";

    tblOnlinePayments::DTO OnlinePaymentsDTO = OnlinePayments::instance().makeSelectQuery(APICALLBOOM_PARAM)
                                               .where({ tblOnlinePayments::Fields::onpMD5, enuConditionOperator::Equal, _paymentKey })
                                               .one<tblOnlinePayments::DTO>();

    Content = Content
              .replace("{URL_OK}", URLHelper::addParameter(_callback, "result", "ok").toLatin1())
              .replace("{URL_ERROR}", URLHelper::addParameter(_callback, "result", "error").toLatin1())
              .replace("{VOUCHER_ID}", QString::number(OnlinePaymentsDTO.onp_vchID).toLatin1())
              .replace("{WALLET_ID}", QString::number(NULLABLE_GET_OR_DEFAULT(OnlinePaymentsDTO.onpTarget_walID, 0)).toLatin1())
              .replace("{AMOUNT}", QString::number(OnlinePaymentsDTO.onpAmount).toLatin1())
              .replace("{API_CALLBACK_URL}", _callback.toLatin1())
              .replace("{UI_CALLBACK_URL}", OnlinePaymentsDTO.onpCallbackUrl.toLatin1())
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
    QVariantMap ResponseParams;
    QList<QPair<QString, QString>> RequestQueryParams = APICALLBOOM_PARAM.getRequestQueryParams();
    foreach (auto Item, RequestQueryParams) {
        if (Item.first == "paymentKey")
            continue;
        ResponseParams.insert(Item.first, Item.second);
    }
    ResponseParams.insert("result", _result);

    return RESTClientHelper::callAPI(
        RESTClientHelper::POST,
        "Account/approveOnlinePayment",
        {},
        {
            { "paymentKey",     _paymentKey },
//            { "domain",         "dev.test" },
            { "pgResponse",     ResponseParams },
        }
    );
}
#endif

/*****************************************************************\
|* OfflinePaymentClaims ******************************************|
\*****************************************************************/

/*

OfflinePaymentClaims::OfflinePaymentClaims() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePaymentClaims::Name,
        tblOfflinePaymentClaims::Private::ORMFields,
        tblOfflinePaymentClaims::Private::Relations,
        tblOfflinePaymentClaims::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(OfflinePaymentClaims) {
//    UploadFiles::instance().prepareFiltersList();
//    UploadGateways::instance().prepareFiltersList();
    UploadQueue::instance().prepareFiltersList();

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblVoucher::Fields::vch_usrID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    auto fnTouchQuery = [this, &APICALLBOOM_PARAM](ORMSelectQuery &_query) {
        _query
                .addCols(this->selectableColumnNames())
                .leftJoin(tblVoucher::Name)
                .addCols(Voucher::instance().selectableColumnNames())
                ;

        ObjectStorageManager::applyGetFileUrlInQuery(
                    APICALLBOOM_PARAM,
                    _query,
                    UploadFiles::instance(),
                    UploadGateways::instance(),
                    UploadQueue::instance(),
                    tblOfflinePaymentClaims::Name,
                    tblOfflinePaymentClaims::Fields::ofpcReceiptImage_uflID
                    );
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, {}, 0, fnTouchQuery);
}

*/

//quint32 IMPL_ORMCREATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

//    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
//}

//bool IMPL_ORMUPDATE(OfflinePaymentClaims) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
//    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
//}

//bool IMPL_ORMDELETE(OfflinePaymentClaims) {
//    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

//    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES);
//}

/*****************************************************************/
/*
OfflinePaymentClaimsAttachments::OfflinePaymentClaimsAttachments() :
    intfSQLBasedModule(
        AAASchema,
        tblOfflinePaymentClaimsAttachments::Name,
        tblOfflinePaymentClaimsAttachments::Private::ORMFields,
        tblOfflinePaymentClaimsAttachments::Private::Relations,
        tblOfflinePaymentClaimsAttachments::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(OfflinePaymentClaimsAttachments) {
    UploadQueue::instance().prepareFiltersList();

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
                .setCond({ tblOfflinePaymentClaims::Fields::ofpcCreatedBy_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getUserID() })
                .orCond({ tblOfflinePaymentClaimsAttachments::Fields::ofpcatCreatedBy_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getUserID() })
                ;

    auto fnTouchQuery = [this, &APICALLBOOM_PARAM](ORMSelectQuery &_query) {
        _query
            .addCols(this->selectableColumnNames())
            .innerJoinWith("OfflinePaymentClaims")
            .addCols(OfflinePaymentClaims::instance().selectableColumnNames())
        ;

        ObjectStorageManager::applyGetFileUrlInQuery(
                    APICALLBOOM_PARAM,
                    _query,
                    UploadFiles::instance(),
                    UploadGateways::instance(),
                    UploadQueue::instance(),
                    tblOfflinePaymentClaimsAttachments::Name,
                    tblOfflinePaymentClaimsAttachments::Fields::ofpcat_uplID
                    );

    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, ExtraFilters, 0, fnTouchQuery);
}
*/

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
    UploadQueue::instance().prepareFiltersList();

    clsCondition ExtraFilters = {};
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        ExtraFilters
                .setCond({ tblVoucher::Fields::vch_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getUserID() })
                .orCond(clsCondition({ tblVoucher::Fields::vch_usrID, enuConditionOperator::Null })
                        .andCond({ tblOfflinePayments::Fields::ofpCreatedBy_usrID, enuConditionOperator::Equal, APICALLBOOM_PARAM.getUserID() }))
                ;

//    this->setSelfFilters({{tblVoucher::Fields::vch_usrID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    auto fnTouchQuery = [this, &APICALLBOOM_PARAM](ORMSelectQuery &_query) {
        _query
                .addCols(this->selectableColumnNames())
                .leftJoin(tblVoucher::Name)
                .addCols(Voucher::instance().selectableColumnNames())
                ;

        ObjectStorageManager::applyGetFileUrlInQuery(
                    APICALLBOOM_PARAM,
                    _query,
                    UploadFiles::instance(),
                    UploadGateways::instance(),
                    UploadQueue::instance(),
                    tblOfflinePayments::Name,
                    tblOfflinePayments::Fields::ofpReceiptImage_uflID
                    );
    };

    return this->Select(GET_METHOD_ARGS_CALL_VALUES, ExtraFilters, 0, fnTouchQuery);
}

bool IMPL_ORMUPDATE(OfflinePayments) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));
    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

/*****************************************************************/

} //namespace ORM
} //namespace Targoman::API::AccountModule
