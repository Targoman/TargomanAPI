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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PG_ZIBA_HPP
#define TARGOMAN_API_MODULES_ACCOUNT_PG_ZIBA_HPP

#include "QFieldValidator.h"
#include "Interfaces/Common/GenericTypes.h"
#include "intfPaymentGateway.hpp"
#include "libTargomanCommon/Configuration/ConfigManager.h"

namespace Targoman {
namespace API {
namespace AAA {

class Zibal
{
private:
    static QString errorString(int _errCode){
        switch(_errCode){
        case  100 : return "Ok";
        case  102 : return "InvalidAPIKey";
        case  103 : return "ErroneousAPIKey";
        case  104 : return "ErroneousAPIKey";
        case  201 : return "Ok";
        case  105 : return "InvalidAmount";
        case  106 : return "InvalidCallBack";
        default   : return "UNKNOWN";
        }
    }
public:
    static stuPaymentResponse request(TAPI::MD5_t _orderMD5, qint64 _amount, const QString& _callback, const QString& _desc){
        intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {_orderMD5, _amount, _callback, _desc});
        try{
            QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                     "https://gateway.zibal.ir/v1/request",
                                     QJsonDocument(QJsonObject({
                                                                   {"merchant"    , intfPaymentGateway::merchantID(_callback, TAPI::enuPaymentGateway::Zibal)},
                                                                   {"amount"      , _amount},
                                                                   {"callbackUrl" , _callback},
                                                                   {"description" , _desc},
                                                                   {"orderId"     , _orderMD5}
                                                               }))
                                     );
            intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {Json});
            if(Json.isObject() == false)
                throw exPayment("JsonObject expected as Zibal response");
            QJsonObject Response = Json.object();
            if(Response.contains("result") == false
               || Response.value("result").toInt() != 100
               || Response.value("trackId").toString().isEmpty())
                return {_orderMD5, Json.toJson(), Response.value("result").toInt(), errorString(Response.value("result").toInt())};

            return { Response.value("trackId").toString(),  "https://gateway.zibal.ir/start/"+Response.value("trackId").toString()+"/direct" };

        }catch(std::exception &e){
            intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {e.what()});
            throw;
        }
    }

    static stuPaymentResponse verify(const QJsonObject& _pgResponse, const QString& _domain){
        intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {_pgResponse, _domain});
        try {
            QString OrderMD5 = _pgResponse.value("orderId").toString();
            QString TrackID = _pgResponse.value("trackId").toString();

            if(OrderMD5.isEmpty() || TrackID.isEmpty())
                throw exHTTPBadRequest("Invalid response not containing essential keys");

            QFV.md5().validate(OrderMD5, "orderID");
            QFV.asciiAlNum(false).maxLenght(32).validate(TrackID, "trackId");

            QJsonDocument Json = intfPaymentGateway::postJsonWithCurl(
                                     "https://gateway.zibal.ir/v1/verify",
                                     QJsonDocument(QJsonObject({
                                                                   {"merchant" , intfPaymentGateway::merchantID(_domain, TAPI::enuPaymentGateway::Zibal)},
                                                                   {"trackId"  , TrackID},
                                                               }))
                                     );
            intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {Json});
            if(Json.isObject() == false)
                return {OrderMD5, Json.toJson(), -1, "JsonObject expected as Zibal response"};

            QJsonObject Response = Json.object();
            if(Response.contains("result") == false
               || (Response.value("result").toInt() != 100 && Response.value("result").toInt() != 201))
                return { OrderMD5, Json.toJson(), Response.value("result").toInt(), errorString(Response.value("result").toInt())};

            return {};
        } catch (std::exception &e) {
            intfPaymentGateway::log("Zibal", __FUNCTION__, __LINE__, {e.what()});
            throw;
        }
    }
};

}
}
}

#endif // TARGOMAN_API_MODULES_ACCOUNT_PG_ZIBA_HPP
