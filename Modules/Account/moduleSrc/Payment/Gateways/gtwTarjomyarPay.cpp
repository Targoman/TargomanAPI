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

#include "gtwTarjomyarPay.h"

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwTarjomyarPay)

// [Response, TrackID, PaymentLink]
std::tuple<QString, QString, QString> gtwTarjomyarPay::prepareAndRequest(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    TAPI::MD5_t _paymentKey,
    qint64 _amount,
    const QString& _callback,
    const QString& _desc
) {
//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());
    QString Token = _paymentGateway.pgwMetaInfo[gtwTarjomyarPay::METAINFO_KEY_TOKEN].toString();

    PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { _paymentKey, _amount, _callback, _desc });

    try {
        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwTarjomyarPay::URL_GTW_REQUEST,
                                 QJsonDocument(QJsonObject({
                                                               { "token"    , Token },
                                                               { "amount"   , _amount },
                                                               { "cb"       , _callback },
                                                               { "desc"     , _desc },
                                                           }))
                                 );

        PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { Json });

        if (Json.isObject() == false)
            throw exPayment("JsonObject expected as TarjomyarPay response");

        QJsonObject Response = Json.object();
        /*
        {
            "ordID":"b5a73777f32bcb814062c6f96f8a1d9d",
            "url":"https:\/\/tarjomyar.ir\/paymentGW.html
                    ?ref=b5a73777f32bcb814062c6f96f8a1d9d
                    &cb=https%3A%2F%2Ffreepaper.me%2FpaymentResponse.html"
        }
        */

        if (Response.contains("ordID") == false)
            throw exPayment("The ordID is missing");
        if (Response.value("ordID").toString().isEmpty())
            throw exPayment("The ordID is empty");

        if (Response.contains("url") == false)
            throw exPayment("The url is missing");
        if (Response.value("url").toString().isEmpty())
            throw exPayment("The url is empty");

        return {
            Json.toJson(QJsonDocument::Compact),
            Response.value("ordID").toString(),
            Response.value("url").toString()
        };

    } catch (std::exception &e) {
        PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

// [Response, refNumber]
std::tuple<QString, QString> gtwTarjomyarPay::verifyAndSettle(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    Q_DECL_UNUSED const ORM::tblOnlinePayments::DTO &_onlinePaymentDTO,
    const TAPI::JSON_t& _pgResponse
//    const QString& _domain
) {
    QString Token = _paymentGateway.pgwMetaInfo[gtwTarjomyarPay::METAINFO_KEY_TOKEN].toString();

    PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { _pgResponse/*, _domain*/ });

    try {
        if (_pgResponse.isObject() == false)
            throw exPayment("Invalid response from gateway.");

        QJsonObject PGResponse = _pgResponse.object();

        //error
        if (PGResponse.contains("err")) {
            QString Error = PGResponse.value("err").toString();
            throw exPayment(QString("Error:%1")
                            .arg(Error));
        }

        //ok
        if (PGResponse.contains("approval") == false)
            throw exPayment("The approval is missing");
        if (PGResponse.value("approval").toString().isEmpty())
            throw exPayment("The approval is empty");

        QString Approval = PGResponse.value("approval").toString();

        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwTarjomyarPay::URL_GTW_VERIFY,
                                 QJsonDocument(QJsonObject({
                                                               { "token", Token },
                                                               { "ordID", Approval },
                                                           }))
                                 );

        PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { Json });

        if (Json.isObject() == false)
            throw exPayment("JsonObject expected as TarjomyarPay response.");

        QJsonObject Response = Json.object();

        //
        QString RespStatus;
        if (Response.contains("status"))
            RespStatus = Response.value("status").toString();
        else if (Response.contains("bitStatus"))
            RespStatus = Response.value("bitStatus").toString();
        else
            throw exPayment("The response has no status field");

//        if (RespStatus == "A")
//            throw exPayment("This payment verified before");

        //
        QString RespAmount;
        if (Response.contains("amount"))
            RespAmount = Response.value("amount").toString();
        else if (Response.contains("bitAmount"))
            RespAmount = Response.value("bitAmount").toString();
        else
            throw exPayment("The response has no amount field");

        //
        return {
            Json.toJson(QJsonDocument::Compact),
            Approval
        };

    } catch (std::exception &e) {
        PaymentLogic::log(gtwTarjomyarPay::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
