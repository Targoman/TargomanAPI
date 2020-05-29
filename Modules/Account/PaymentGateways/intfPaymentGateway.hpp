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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PG_INTFPAYMENTGATEWAY_HPP
#define TARGOMAN_API_MODULES_ACCOUNT_PG_INTFPAYMENTGATEWAY_HPP

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include "3rdParty/QtCUrl/src/QtCUrl.h"
#include "Interfaces/Common/HTTPExceptions.hpp"

using namespace qhttp;

namespace Targoman {
namespace API {
namespace AAA {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_SERVICE_UNAVAILABLE, exPayment);
#pragma clang diagnostic pop


struct stuPaymentResponse{
    int     ErrorCode;
    QString ErrorString;
    QString Result;
    QString TrackID;
    QString OrderMD5;
    QString PaymentLink;

    stuPaymentResponse() :
        ErrorCode(-1)
    {}

    stuPaymentResponse(const QString& _trackID, const QString& _paymentLink) :
        ErrorCode(0),
        TrackID(_trackID),
        PaymentLink(_paymentLink)
    {}

    stuPaymentResponse(const QString _orderMD5, const QString& _result, int _errorCode, const QString& _errStr) :
        ErrorCode(_errorCode),
        ErrorString(_errStr),
        Result(_result),
        OrderMD5(_orderMD5)
    {}
};

class intfPaymentGateway{
public:
    static void log(const QString _gw, const QString _function, quint16 _line, const QVariantList& _info){
        QFile File("output.txt"); // TODO read from config file
        if (File.open(QFile::WriteOnly | QFile::Append)) {
            QTextStream Out(&File);
            Out << QDateTime::currentDateTime().toString(Qt::ISODateWithMs) <<
                   QString("[%1:%2:%3] ").arg(_gw, _function).arg(_line)<<
                   QJsonDocument::fromVariant(_info).toJson();
        }
    }

    static QJsonDocument postJsonWithCurl(const QString _url, const QJsonDocument& _json, bool _jsonDecode = true){
        QtCUrl CUrl;
        CUrl.setTextCodec("UTF-8");

        QtCUrl::Options Opt;
        Opt[CURLOPT_URL] = _url;
        Opt[CURLOPT_HTTPHEADER] = QStringList({
                                                  "content-type: application/json",
                                                  "cache-control: no-cache"
                                              });
        Opt[CURLOPT_POST] = true;
        Opt[CURLOPT_FOLLOWLOCATION] = true;
        Opt[CURLOPT_FAILONERROR] = true;
        Opt[CURLOPT_TIMEOUT] = 60;
        Opt[CURLOPT_POSTFIELDS] = _json.toJson();

        QString CUrlResult = CUrl.exec(Opt);

        if (CUrl.lastError().code() == CURLE_OPERATION_TIMEDOUT)
            throw exPayment("Connection to <" + _url +"> timed out.");
        else if(CUrl.lastError().code() == CURLE_COULDNT_CONNECT)
            throw exPayment("Connection to <" + _url +"> failed.");
        else if(CUrl.lastError().isOk() == false)
            throw exPayment("Connection to <" + _url + "> error: " + CUrl.lastError().text());

        if(_jsonDecode){
            QJsonParseError JsonError;
            QJsonDocument Json = QJsonDocument::fromJson(CUrlResult.toUtf8(), &JsonError);
            if(JsonError.error != QJsonParseError::NoError)
                throw exPayment("Unable to parse gateway response. " + JsonError.errorString());
            return Json;
        }
        return QJsonDocument(QJsonObject({{"result" , QString(CUrlResult.toUtf8()) }}));
    }
};

}
}
}
#endif // TARGOMAN_API_MODULES_ACCOUNT_PG_INTFPAYMENTGATEWAY_HPP
