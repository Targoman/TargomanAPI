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
#include "App/Server/QJWT.h"

using namespace Targoman::API::AAA;

namespace Targoman::API::Helpers {

using namespace Common;
using namespace Common::Configuration;

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
        TAPI::JWT_t _JWT,
        RESTClientHelper::enuHTTPMethod _method,
        const QString& _api,
        const QVariantMap& _urlArgs,
        const QVariantMap& _postFields,
        QString _aPIURL
    )
{
//    clsJWT JWT(_JWT);
//    QString EncodedJWT = JWT.session();

//    QString EncodedJWT = clsJWT::createSigned(_JWT);

    QString EncodedJWT = Targoman::API::Server::QJWT::createSigned(_JWT,
        {},
        300, //Targoman::API::Server::QJWT::TTL.value(),
        {}
    );

    return RESTClientHelper::callAPI(
        EncodedJWT,
        _method,
        _api,
        _urlArgs,
        _postFields,
        _aPIURL
    );
}

QVariant RESTClientHelper::callAPI(
        QString _encodedJWT,
        RESTClientHelper::enuHTTPMethod _method,
        const QString& _api,
        const QVariantMap& _urlArgs,
        const QVariantMap& _postFields,
        QString _aPIURL
    )
{
    if (_aPIURL.isEmpty())
        _aPIURL = ClientConfigs::RESTServerAddress.value();

    QtCUrl CUrl;
    CUrl.setTextCodec("UTF-8");

    auto makeURL = [_aPIURL, _api, _urlArgs]() {
        QUrlQuery URLQuery;

        for (auto ArgIter = _urlArgs.begin(); ArgIter != _urlArgs.end(); ++ArgIter)
            URLQuery.addQueryItem(ArgIter.key(), ArgIter.value().toString());

        QUrl URL(_aPIURL + "/" + _api);
        URL.setQuery(URLQuery);
        QString URLStr = URL.toString().replace(" ", "%20").replace("+","%2B");

//            qDebug() << "Request" << HTTPMethodString[_method] << URLStr;

        return URLStr;
    };

    QtCUrl::Options Opt;
    Opt[CURLOPT_URL] = makeURL();
    Opt[CURLOPT_TIMEOUT] = 10000;
    Opt[CURLOPT_FAILONERROR] = true;
    QStringList Headers = QStringList({"Content-Type: application/json"});

    if (_encodedJWT.size())
        Headers.append("Authorization: Bearer " + _encodedJWT);

    Opt[CURLOPT_HTTPHEADER] = Headers;

    switch (_method)
    {
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

    switch (_method)
    {
        case GET:
        case DELETE:
            break;
        case POST:
        case PUT:
        case PATCH:
            Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(_postFields).toJson(QJsonDocument::Compact);
    }

    QString CUrlResult = CUrl.exec(Opt);

    if (CUrl.lastError().isOk() == false)
    {
        qDebug() << CUrl.errorBuffer();
        return QVariant();
    }
    else
    {
        QJsonParseError JsonError;
        QJsonDocument Doc = QJsonDocument::fromJson(CUrlResult.toUtf8(),& JsonError);

        if (JsonError.error != QJsonParseError::NoError)
            qDebug() << "Unable to parse JSON: " + JsonError.errorString() + '"' + CUrlResult + '"';

        QVariant Result = Doc.toVariant().toMap().value("result");

//            qDebug() << "Result:" << Result;

        return Result;
    }
}

} //namespace Targoman::API::Helpers
