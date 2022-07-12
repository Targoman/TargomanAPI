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

namespace Targoman::API::AccountModule::Payment::Gateways {

TARGOMAN_IMPL_API_PAYMENT_GATEWAY(gtwZibal)

// [Response, TrackID, PaymentLink]
std::tuple<QString, QString, QString> gtwZibal::prepareAndRequest(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    TAPI::MD5_t _paymentKey,
    qint64 _amount,
    const QString& _callback,
    const QString& _desc
) {
//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());
    QString MerchantID = _paymentGateway.pgwMetaInfo[gtwZibal::METAINFO_KEY_MERCHANT_ID].toString();

    PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { _paymentKey, _amount, _callback, _desc });

    try {
        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwZibal::URL_GTW_REQUEST,
                                 QJsonDocument(QJsonObject({
                                                               { "merchant"    , MerchantID },
                                                               { "amount"      , _amount },
                                                               { "callbackUrl" , _callback },
                                                               { "description" , _desc },
                                                               { "orderId"     , _paymentKey }
                                                           }))
                                 );

        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { Json });

        if (Json.isObject() == false)
            throw exPayment("JsonObject expected as Zibal response");

        QJsonObject Response = Json.object();

        if (Response.contains("result") == false)
            throw exPayment("The response has no result field");

        int Result = Response.value("result").toInt();

        if (Result != 100)
            throw exPayment(QString("Error Code:%1, Message:%2")
                            .arg(Result)
                            .arg(this->errorString(Result)));

        if (Response.value("trackId").toString().isEmpty())
            throw exPayment("The trackID is empty");

        return {
            Json.toJson(QJsonDocument::Compact),
            Response.value("trackId").toString(),
            QString(URL_GTW_PAY).replace("{{track_id}}", Response.value("trackId").toString())
        };

    } catch (std::exception &e) {
        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

// [Response, refNumber]
std::tuple<QString, QString> gtwZibal::verifyAndSettle(
    const ORM::tblPaymentGateways::DTO &_paymentGateway,
    const ORM::tblOnlinePayments::DTO &_onlinePayment,
    const TAPI::JSON_t& _pgResponse,
    const QString& _domain
) {
//    TAPI::JSON_t MetaInfo = NULLABLE_GET_OR_DEFAULT(_paymentGateway.pgwMetaInfo, TAPI::JSON_t());
    QString MerchantID = _paymentGateway.pgwMetaInfo[gtwZibal::METAINFO_KEY_MERCHANT_ID].toString();

    PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { _pgResponse, _domain });

    try {
        if (_pgResponse.isObject() == false)
            throw exPayment("Invalid response from gateway.");
        QJsonObject PGResponse = _pgResponse.object();
        QString Success = PGResponse.value("success").toString();
        QString Status = PGResponse.value("status").toString();

        if (Success != "1")
            throw exPayment(QString("Payment failed. Status Code:%1, Message:%2")
                            .arg(Status)
                            .arg(this->statusString(Status.toInt()))
                            );

        QString TrackID = PGResponse.value("trackId").toString();
        QString PaymentKey = PGResponse.value("orderId").toString();

        if (PaymentKey.isEmpty() || TrackID.isEmpty())
            throw exHTTPBadRequest("Invalid response. not containing essential keys");

        QFV.md5().validate(PaymentKey, "orderID");
        QFV.asciiAlNum(false).maxLenght(32).validate(TrackID, "trackId");

        QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                 gtwZibal::URL_GTW_VERIFY,
                                 QJsonDocument(QJsonObject({
                                                               { "merchant" , MerchantID },
                                                               { "trackId"  , TrackID },
                                                           }))
                                 );

        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { Json });

        if (Json.isObject() == false)
            throw exPayment("JsonObject expected as Zibal response.");

        QJsonObject Response = Json.object();

        if (Response.contains("result") == false)
            throw exPayment("The response has no result field");

        int Result = Response.value("result").toInt();

        if ((Result != 100) && (Result != 201))
            throw exPayment(QString("Error Code:%1, Message:%2")
                            .arg(Result)
                            .arg(this->errorString(Result)));

        return {
            Json.toJson(QJsonDocument::Compact),
            Response.value("refNumber").toString()
        };

    } catch (std::exception &e) {
        PaymentLogic::log(gtwZibal::Name, __FUNCTION__, __LINE__, { e.what() });
        throw;
    }
}

QString gtwZibal::errorString(int _errCode) {
    switch (_errCode) {
        case 100 : return "Ok";
        case 102 : return "Merchant not found";
        case 103 : return "The merchant is inactive";
        case 104 : return "Invalid merchant";
        case 105 : return "Invalid amount";
        case 106 : return "Invalid callback";
        case 113 : return "The transaction amount is out of range";

        case 201 : return "Approved before";
        case 202 : return "The order was unpaid or failed";
        case 203 : return "Invalid track id";

        default  : return "Unknown";
    }
}

QString gtwZibal::statusString(int _statusCode) {
    switch (_statusCode) {
        case -1 : return "Wait for payment"; //"در انتظار پردخت";
        case -2 : return "Internal error"; //"خطای داخلی";
        case  1 : return "Payed - Approved"; //"پرداخت شده - تاییدشده";
        case  2 : return "Payed - Not approved"; //"پرداخت شده - تاییدنشده";
        case  3 : return "Cancelled by user"; //"لغوشده توسط کاربر";
        case  4 : return "Invalid card number"; //"‌شماره کارت نامعتبر می‌باشد.";
        case  5 : return "Account balance is not enough"; //"‌موجودی حساب کافی نمی‌باشد.";
        case  6 : return "Invalid password"; //"رمز واردشده اشتباه می‌باشد.";
        case  7 : return "The number of requests is too much"; //"‌تعداد درخواست‌ها بیش از حد مجاز می‌باشد.";
        case  8 : return "The number of online payments per day is more than allowed"; //"‌تعداد پرداخت اینترنتی روزانه بیش از حد مجاز می‌باشد.";
        case  9 : return "Daily internet payment is more than allowed"; //"مبلغ پرداخت اینترنتی روزانه بیش از حد مجاز می‌باشد.";
        case 10 : return "Invalid card issuer"; //"‌صادرکننده‌ی کارت نامعتبر می‌باشد.";
        case 11 : return "Switch error"; //"‌خطای سوییچ";
        case 12 : return "The card is not available"; //"کارت قابل دسترسی نمی‌باشد.";

        default : return "Unknown";
    }
}

} //namespace Targoman::API::AccountModule::Payment::Gateways
