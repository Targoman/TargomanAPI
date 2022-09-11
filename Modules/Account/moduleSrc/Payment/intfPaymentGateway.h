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

#ifndef TARGOMAN_API_MODULES_ACCOUNT_PAYMENT_INTFPAYMENTGATEWAY_H
#define TARGOMAN_API_MODULES_ACCOUNT_PAYMENT_INTFPAYMENTGATEWAY_H

#include <qglobal.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QTextStream>
#include "QtCUrl.h"
#include "Interfaces/Common/HTTPExceptions.hpp"
#include "libTargomanCommon/Configuration/tmplConfigurableArray.hpp"
#include "Classes/Defs.hpp"
#include "ORM/PaymentGateways.h"
#include "ORM/Payments.h"

using namespace qhttp;

namespace Targoman::API::AccountModule {

//namespace Classes {
//class PaymentLogic;
//}

namespace Payment {

class PaymentLogic;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER_WITH_CODE (ESTATUS_SERVICE_UNAVAILABLE, exPayment);
#pragma clang diagnostic pop

} //namespace Payment

//struct stuPaymentResponse {
//    int     ErrorCode;
//    QString ErrorString;
//    QString Result;
//    QString TrackID;
//    QString PaymentKey;
//    QString PaymentLink;

//    stuPaymentResponse() :
//        ///@TODO: why -1????
////        ErrorCode(-1)
//        ErrorCode(0)
//    { ; }

//    stuPaymentResponse(const QString& _trackID, const QString& _paymentLink) :
//        ErrorCode(0),
//        TrackID(_trackID),
//        PaymentLink(_paymentLink)
//    { ; }

//    stuPaymentResponse(const QString _paymentKey, const QString& _result, int _errorCode, const QString& _errStr) :
//        ErrorCode(_errorCode),
//        ErrorString(_errStr),
//        Result(_result),
//        PaymentKey(_paymentKey)
//    { ; }
//};

namespace Payment {

///@TODO: move this to common
#define TARGOMAN_BEGIN_STATIC_CTOR(_className) \
    struct _##_className##_static_constructor { \
        _##_className##_static_constructor() {

#define TARGOMAN_END_STATIC_CTOR(_className) \
        } \
    }; \
    static inline _##_className##_static_constructor _##_className##_static_constructor_internal;

///@TODO: move this to common
#define instanceGetterPtr(_class) static _class* instancePtr() { \
    static _class* Instance = nullptr; return (Q_LIKELY(Instance) ? Instance : (Instance = new _class)); \
}

#define TARGOMAN_DEFINE_API_PAYMENT_GATEWAY(_name, _type) \
public: \
    instanceGetterPtr(_name); \
protected: \
    virtual Targoman::API::AccountModule::enuPaymentGatewayType::Type getType() { return _type; }; \
    virtual /*[Response, TrackID, PaymentLink]*/std::tuple<QString, QString, QString> prepareAndRequest( \
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, \
            const Targoman::API::AccountModule::ORM::tblPaymentGateways::DTO &_paymentGateway, \
            TAPI::MD5_t _paymentKey, \
            qint64 _amount, \
            const QString &_callback, \
            const QString &_desc \
            ); \
    virtual /*[Response, refNumber]*/std::tuple<QString, QString> verifyAndSettle( \
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM, \
            const Targoman::API::AccountModule::ORM::tblPaymentGateways::DTO &_paymentGateway, \
            const Targoman::API::AccountModule::ORM::tblOnlinePayments::DTO &_onlinePayment, \
            const TAPI::JSON_t &_pgResponse \
            ); \
    friend Targoman::API::AccountModule::Payment::PaymentLogic; \
private: \
    _name(); \
    TAPI_DISABLE_COPY(_name); \
    TARGOMAN_BEGIN_STATIC_CTOR(_name) \
        Targoman::API::AccountModule::Payment::PaymentLogic::registerGateway(_name::Name, _name::instancePtr()); \
    TARGOMAN_END_STATIC_CTOR(_name)

//PaymentLogic::registerGateway<_name>(_name::Name, _name::instancePtr());
//PaymentLogic::registerGateway<_name>(_name::Name);

#define TARGOMAN_IMPL_API_PAYMENT_GATEWAY(_name) \
    _name::_name() { ; }

/**
 * @brief The intfPaymentGateway class is base class of gateway drivers
 */
class intfPaymentGateway
{
    friend Targoman::API::AccountModule::Payment::PaymentLogic;

protected:
    virtual Targoman::API::AccountModule::enuPaymentGatewayType::Type getType() = 0;
//    virtual TAPI::enuPaymentGatewayDriver::Type getGateway() = 0;

    /*[Response, TrackID, PaymentLink]*/
    virtual std::tuple<QString, QString, QString> prepareAndRequest(
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
            const ORM::tblPaymentGateways::DTO &_paymentGateway,
            TAPI::MD5_t _paymentKey,
            qint64 _amount,
            const QString &_callback,
            const QString &_desc
            ) = 0;

    /*[Response, refNumber]*/
    virtual std::tuple<QString, QString> verifyAndSettle(
            INTFAPICALLBOOM_DECL &APICALLBOOM_PARAM,
            const ORM::tblPaymentGateways::DTO &_paymentGateway,
            Q_DECL_UNUSED const ORM::tblOnlinePayments::DTO &_onlinePaymentDTO,
            const TAPI::JSON_t &_pgResponse
//            const QString &_domain
            ) = 0;

    virtual QString errorString(int _errCode) { return QString("unknown error %1").arg(_errCode); }

    static QJsonDocument postJsonWithCurl(const QString _url, const QJsonDocument& _json, bool _jsonDecode = true) {
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

        Opt[CURLOPT_SSL_VERIFYPEER] = 0;
        Opt[CURLOPT_SSL_VERIFYHOST] = 0;

        Opt[CURLOPT_POSTFIELDS] = _json.toJson(QJsonDocument::Compact);

        QString CUrlResult = CUrl.exec(Opt);

        if (CUrl.lastError().code() == CURLE_OPERATION_TIMEDOUT)
            throw exPayment("Connection to <" + _url +"> timed out.");
        else if (CUrl.lastError().code() == CURLE_COULDNT_CONNECT)
            throw exPayment("Connection to <" + _url +"> failed.");
        else if (CUrl.lastError().isOk() == false)
            throw exPayment("Connection to <" + _url + "> error: " + CUrl.lastError().text());

        if (_jsonDecode) {
            QJsonParseError JsonError;
            QJsonDocument Json = QJsonDocument::fromJson(CUrlResult.toUtf8(), &JsonError);

            if (JsonError.error != QJsonParseError::NoError)
                throw exPayment("Unable to parse gateway response. " + JsonError.errorString());

            return Json;
        }

        return QJsonDocument(QJsonObject({
                                             { "result", QString(CUrlResult.toUtf8()) }
                                         })
                             );
    }

    /*
    struct stuGateway {
        stuGateway(const QString& _basePath) :
            CallBackDomain(_basePath + "CallBackdomain", "Domain used for callback"),
            Gateway(_basePath + "Gateway", "Gateway to be used for this callback"),
            CustomerID(_basePath + "Gateway", "Customer ID on the gateway"),
            PrivateToken(_basePath + "Gateway", "PrivateToken for gateway if required") { ; }

        Targoman::Common::Configuration::tmplConfigurable<QString>    CallBackDomain;
        Targoman::Common::Configuration::tmplConfigurable<TAPI::enuPaymentGateway::Type> Gateway;
        Targoman::Common::Configuration::tmplConfigurable<QString>    CustomerID;
        Targoman::Common::Configuration::tmplConfigurable<QString>    PrivateToken;
    };

    static Targoman::Common::Configuration::tmplConfigurableArray<stuGateway> GatewayEndPoints;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t> TransactionLogFile;

    static QString merchantID(const QString _callBack, TAPI::enuPaymentGateway::Type _gateway) {
        QString CallBackDomain = _callBack;
        CallBackDomain = CallBackDomain.replace("https://","").replace("http://", "");

        for (size_t i=0; i<intfPaymentGateway::GatewayEndPoints.size(); ++i) {
            if (intfPaymentGateway::GatewayEndPoints.at(i).Gateway.value() == _gateway &&
                    CallBackDomain.startsWith(intfPaymentGateway::GatewayEndPoints.at(i).CallBackDomain.value())
                )
                return intfPaymentGateway::GatewayEndPoints.at(i).CustomerID.value();
        }
        throw exPayment(QString("callback unrecognized for %1: %2").arg(TAPI::enuPaymentGateway::toStr(_gateway), _callBack));
    }
    */
};

} //namespace Payment
} //namespace Targoman::API::AccountModule

#endif // TARGOMAN_API_MODULES_ACCOUNT_PAYMENT_INTFPAYMENTGATEWAY_H
