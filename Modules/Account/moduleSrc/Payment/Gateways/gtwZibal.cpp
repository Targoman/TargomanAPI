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

#include "gtwZibal.h"

//using namespace Targoman::API::AccountModule::Classes;

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwZibal)

stuPaymentResponse gtwZibal::request(
    const stuPaymentGateway& _paymentGateway,
    TAPI::MD5_t _orderMD5,
    qint64 _amount,
    const QString& _callback,
    const QString& _desc
) {
    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());
    QString MerchantID = MetaInfo[gtwZibal::METAINFO_KEY_MERCHANT_ID].toString();

    PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { _orderMD5, _amount, _callback, _desc });

    try {
        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwZibal::URL_GTW_REQUEST,
                                 QJsonDocument(QJsonObject({
                                                               { "merchant"    , MerchantID },
                                                               { "amount"      , _amount },
                                                               { "callbackUrl" , _callback },
                                                               { "description" , _desc },
                                                               { "orderId"     , _orderMD5 }
                                                           }))
                                 );

        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { Json });

        if (Json.isObject() == false)
            throw exPayment("JsonObject expected as Zibal response");

        QJsonObject Response = Json.object();

        if (Response.contains("result") == false
            || Response.value("result").toInt() != 100
            || Response.value("trackId").toString().isEmpty()
        )
            return stuPaymentResponse(
                _orderMD5,
                Json.toJson(),
                Response.value("result").toInt(),
                errorString(Response.value("result").toInt())
            );

        return stuPaymentResponse(
            Response.value("trackId").toString(),
            QString(URL_GTW_PAY).replace("{{track_id}}", Response.value("trackId").toString())
        );

    } catch (std::exception &e) {
        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

stuPaymentResponse gtwZibal::verify(
    const stuPaymentGateway& _paymentGateway,
    const TAPI::JSON_t& _pgResponse,
    const QString& _domain
) {
    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());
    QString MerchantID = MetaInfo[gtwZibal::METAINFO_KEY_MERCHANT_ID].toString();

    PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { _pgResponse, _domain });

    try {
        QString OrderMD5 = _pgResponse.object().value("orderId").toString();
        QString TrackID = _pgResponse.object().value("trackId").toString();

        if (OrderMD5.isEmpty() || TrackID.isEmpty())
            throw exHTTPBadRequest("Invalid response. not containing essential keys");

        QFV.md5().validate(OrderMD5, "orderID");
        QFV.asciiAlNum(false).maxLenght(32).validate(TrackID, "trackId");

        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwZibal::URL_GTW_VERIFY,
                                 QJsonDocument(QJsonObject({
                                                               { "merchant" , MerchantID },
                                                               { "trackId"  , TrackID },
                                                           }))
                                 );

        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, {Json});

        if (Json.isObject() == false)
            return stuPaymentResponse(
                OrderMD5,
                Json.toJson(),
                -1,
                "JsonObject expected as Zibal response"
            );

        QJsonObject Response = Json.object();

        if (Response.contains("result") == false
           || ((Response.value("result").toInt() != 100)
               && (Response.value("result").toInt() != 201))
        )
            return stuPaymentResponse(
                OrderMD5,
                Json.toJson(),
                Response.value("result").toInt(),
                errorString(Response.value("result").toInt())
            );

        return {};
    } catch (std::exception &e) {
        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

QString gtwZibal::errorString(int _errCode) {
    switch (_errCode) {
        case 100 : return "Ok";
        case 102 : return "Invalid API Key";
        case 103 : return "Erroneous API Key";
        case 104 : return "Erroneous API Key";
        case 201 : return "Ok";
        case 105 : return "Invalid Amount";
        case 106 : return "Invalid CallBack";
        default  : return "UNKNOWN";
    }
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
