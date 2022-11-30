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

#include "RESTClientHelper.h"
#include <QUrlQuery>
#include <QJsonDocument>
#include <QDebug>
#include "Interfaces/AAA/PrivHelpers.h"
//#include "Interfaces/AAA/clsJWT.hpp"
#include "Interfaces/Server/QJWT.h"

using namespace Targoman::Common::Configuration;
using namespace Targoman::API::AAA;
using namespace Targoman::API::Server;

namespace Targoman::API::Helpers {

//using namespace Common;

tmplConfigurable<QString> ClientConfigs::RESTServerAddress(
    ClientConfigs::makeConfig("RESTServerAddress"),
    "REST Server Address",
    "http://127.0.0.1:10000/rest/v1",
    ReturnTrueCrossValidator(),
    "",
    "",
    "rest-server-address",
    enuConfigSource::Arg | enuConfigSource::File);

//static const char* HTTPMethodString[] = {
//    "GET",
//    "POST",
//    "PUT",
//    "PATCH",
//    "DELETE"
//};

QVariant RESTClientHelper::callAPI(
    RESTClientHelper::enuHTTPMethod _method,
    const QString &_api,
    const QVariantMap &_urlArgs,
    const QVariantMap &_postOrFormFields,
    const QVariantMap &_formFiles,
    QString _aPIURL,
    const QStringList &_requestHeaders,
    QVariantMap *_outResponseHeaders
) {
    QString temp;

    QVariant Result = RESTClientHelper::callAPI(
        temp,
        _method,
        _api,
        _urlArgs,
        _postOrFormFields,
        _formFiles,
        _aPIURL,
        _requestHeaders,
        _outResponseHeaders
    );

    return Result;
}

QVariant RESTClientHelper::callAPI(
    INTFAPICALLCONTEXT_IMPL &_apiCallContext,
    RESTClientHelper::enuHTTPMethod _method,
    const QString &_api,
    const QVariantMap &_urlArgs,
    const QVariantMap &_postOrFormFields,
    const QVariantMap &_formFiles,
    QString _aPIURL,
    const QStringList &_requestHeaders,
    QVariantMap *_outResponseHeaders
) {
    TAPI::JWT_t JWT = _apiCallContext.getJWT();

    QString OldEncodedJWT = JWT["encodedJWT"].toString();
    QVariantMap ResponseHeaders;

    QVariant Result = RESTClientHelper::callAPI(
        JWT,
        _method,
        _api,
        _urlArgs,
        _postOrFormFields,
        _formFiles,
        _aPIURL,
        _requestHeaders,
        &ResponseHeaders
    );

#ifdef TARGOMAN_API_ENABLE_JWT_DASTRESHTE
    if (OldEncodedJWT != JWT["encodedJWT"].toString())
    {
        _apiCallContext.addResponseHeader("x-auth-new-token", JWT["encodedJWT"].toString());
        _apiCallContext.setJWT(JWT);
    }
#endif

    if (_outResponseHeaders != nullptr)
        *_outResponseHeaders = ResponseHeaders;

    return Result;
}

QVariant RESTClientHelper::callAPI(
    INOUT TAPI::JWT_t &_JWT,
    RESTClientHelper::enuHTTPMethod _method,
    const QString &_api,
    const QVariantMap &_urlArgs,
    const QVariantMap &_postOrFormFields,
    const QVariantMap &_formFiles,
    QString _aPIURL,
    const QStringList &_requestHeaders,
    QVariantMap *_outResponseHeaders
) {
    QString EncodedJWT = _JWT["encodedJWT"].toString();

    QVariantMap ResponseHeaders;

    QVariant Result = RESTClientHelper::callAPI(
        EncodedJWT,
        _method,
        _api,
        _urlArgs,
        _postOrFormFields,
        _formFiles,
        _aPIURL,
        _requestHeaders,
        &ResponseHeaders
    );

    if (_outResponseHeaders != nullptr)
        *_outResponseHeaders = ResponseHeaders;

#ifdef TARGOMAN_API_ENABLE_JWT_DASTRESHTE
    if (EncodedJWT != _JWT["encodedJWT"].toString()) {
        QJWT::extractAndDecryptPayload(EncodedJWT, _JWT);
        _JWT["encodedJWT"] = EncodedJWT;
    }
#endif

    return Result;
}

QVariant RESTClientHelper::callAPI(
    INOUT QString &_encodedJWT,
    RESTClientHelper::enuHTTPMethod _method,
    const QString &_api,
    const QVariantMap &_urlArgs,
    const QVariantMap &_postOrFormFields,
    const QVariantMap &_formFiles,
    QString _aPIURL,
    const QStringList &_requestHeaders,
    QVariantMap *_outResponseHeaders
) {
    if (_aPIURL.isEmpty())
        _aPIURL = ClientConfigs::RESTServerAddress.value();

    QtCUrl CUrl;
    CUrl.setTextCodec("UTF-8");

    auto makeURL = [_aPIURL, _api, _urlArgs]() {
        QUrlQuery URLQuery;

        auto DocArgs = QJsonDocument::fromVariant(_urlArgs).object();
        for (auto it = DocArgs.begin(); it != DocArgs.end(); it++) {

            QString Key = it.key();
            auto Value = it.value();

            QString ValueToString = "";

            if (Value.isObject()) {
                ///@TODO: complete this
//                auto o = Value.toObject();
                ValueToString = Value.toVariant().toString();
            } else if (Value.isArray()) {
                auto a = Value.toArray();
                foreach (auto v, a) {
                    if (ValueToString.isEmpty() == false)
                        ValueToString += ",";

//                    if (v.isString()) ValueToString += "'";
                    ValueToString += v.toVariant().toString();
//                    if (v.isString()) ValueToString += "'";
                }
            } else
                ValueToString = Value.toVariant().toString();

            URLQuery.addQueryItem(Key, ValueToString);
        }

//        for (auto it = _urlArgs.begin(); it != _urlArgs.end(); ++it) {
//            URLQuery.addQueryItem(it.key(), it.value().toString());
//        }

        QUrl URL(_aPIURL + "/" + _api);
        URL.setQuery(URLQuery);
        QString URLStr = URL.toString().replace(" ", "%20").replace("+","%2B");

//            qDebug() << "Request" << HTTPMethodString[_method] << URLStr;

        return URLStr;
    };

    bool HasForm = false;
    if (_formFiles.isEmpty() == false) {
        HasForm = true;
        for (auto iter=_formFiles.begin(); iter!=_formFiles.end(); ++iter)
            CUrl.mime_addFile(iter.key(), iter.value().toString());
    }

    QtCUrl::Options Opt;
    Opt[CURLOPT_URL] = makeURL();
    Opt[CURLOPT_FAILONERROR] = false;
#ifdef QT_DEBUG
    Opt[CURLOPT_TIMEOUT] = 120;
#else
    Opt[CURLOPT_TIMEOUT] = 5;
#endif
    Opt[CURLOPT_CONNECTTIMEOUT] = 5;
    Opt[CURLOPT_SSL_VERIFYPEER] = 0;
    Opt[CURLOPT_SSL_VERIFYHOST] = 0;

    QStringList Headers = _requestHeaders;

    if (HasForm) {
        if (_postOrFormFields.isEmpty() == false) {
            for (auto iter = _postOrFormFields.begin(); iter != _postOrFormFields.end(); ++iter)
                CUrl.mime_addData(iter.key(), iter.value().toString());
        }
    } else {
        Headers.append("Content-Type: application/json");

        switch (_method) {
            case GET:
            case DELETE:
                break;

            case POST:
            case PUT:
            case PATCH:
                Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(_postOrFormFields).toJson(QJsonDocument::Compact);
        }
    }

    if (_encodedJWT.size())
        Headers.append("Authorization: Bearer " + _encodedJWT);

    Opt[CURLOPT_HTTPHEADER] = Headers;

    switch (_method) {
        case GET:
            Opt[CURLOPT_CUSTOMREQUEST] = "GET";
            break;
        case DELETE:
            Opt[CURLOPT_CUSTOMREQUEST] = "DELETE";
            break;
        case POST:
            Opt[CURLOPT_POST] = true;
            break;
        case PUT:
            Opt[CURLOPT_CUSTOMREQUEST] = "PUT";
            break;
        case PATCH:
            Opt[CURLOPT_CUSTOMREQUEST] = "PATCH";
            break;
    }

    QString CUrlResult = CUrl.exec(Opt);

    QVariantMap ResponseHeaders = CUrl.headerBuffer();

    if (_outResponseHeaders != nullptr)
        *_outResponseHeaders = ResponseHeaders;

#ifdef TARGOMAN_API_ENABLE_JWT_DASTRESHTE
    //replace JWT by x-auth-new-token
    if (ResponseHeaders.contains("x-auth-new-token")) {
        _encodedJWT = ResponseHeaders.value("x-auth-new-token").toString();

        qDebug() << "**********************************" << "JWT replaced by" << endl
                 << _encodedJWT;

        ///@TODO: complete this
        //Account::instance()->addResponseHeader("x-auth-new-token", _encodedJWT);
    }
#endif

    if (CUrl.lastError().isOk() == false) {
        auto LastError = CUrl.lastError();
        qDebug().noquote().nospace()
                << "-- CURL ERROR: (" << LastError.code() << ") " << LastError.text() << endl
                << "  RESPONSE HEADERS: " << (_outResponseHeaders != nullptr ? *_outResponseHeaders : CUrl.headerBuffer()) << endl
                << "  BUFFER: " << CUrl.buffer() << endl
                << "  RESULT: " << CUrlResult << endl
                << "  ERROR_BUFFER: " << CUrl.errorBuffer() << endl
            ;
        return QVariant();
    }

    QJsonParseError JsonError;
    QJsonDocument Doc = QJsonDocument::fromJson(CUrlResult.toUtf8(), &JsonError);

    if (JsonError.error != QJsonParseError::NoError)
        TargomanDebug(5, "Unable to parse JSON: " + JsonError.errorString() + " \"" + CUrlResult + '"');

    QVariantMap RetAsMap = Doc.toVariant().toMap();
    if (RetAsMap.contains("error")) {
        QString ErrorMessage = RetAsMap.value("error").toMap().value("message").toString();
        int ErrorCode = RetAsMap.value("error").toMap().value("code").toInt();
        throw exTargomanBase(ErrorMessage, ErrorCode);
    }

    QVariant Result = RetAsMap.value("result");
//    qDebug() << "Result:" << Result;

    return Result;
}

} //namespace Targoman::API::Helpers
