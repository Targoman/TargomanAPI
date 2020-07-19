/******************************************************************************
 * TargomanAAA: Authentication, Authorization, Accounting framework           *
 *                                                                            *
 * Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>  *
 *                                                                            *
 * TargomanAAA is free software: you can redistribute it and/or modify        *
 * it under the terms of the GNU Lesser General Public License as published   *
 * by the Free Software Foundation, either version 3 of the License, or       *
 * (at your option) any later version.                                        *
 *                                                                            *
 * TargomanAAA is distributed in the hope that it will be useful,             *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 * GNU Lesser General Public License for more details.                        *
 * You should have received a copy of the GNU Lesser General Public License   *
 * along with Targoman. If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                            *
 ******************************************************************************/
/**
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */
#ifndef TESTCOMMON_HPP
#define TESTCOMMON_HPP

#include <QtTest/QtTest>
#include "3rdParty/QtCUrl/src/QtCUrl.h"
#include "libTargomanDBM/clsDAC.h"
using namespace Targoman::DBManager;

enum HTTPMethod{
    GET,
    POST,
    PUT,
    PATCH,
    DELETE,
};

static const char* HTTPMethodString[] = {
  "GET",
  "POST",
  "PUT",
  "PATCH",
  "DELETE"
};

extern QString APIURL;
extern QString gEncodedJWT;
extern QJsonObject gJWT;
extern QString gEncodedAdminJWT;
extern quint32 gUserID;
extern quint32 gAdminUserID;
extern QVariant gInvalid;

class clsBaseTest: public QObject{
protected:
    QVariant callAPI(HTTPMethod _method, const QString& _api, const QVariantMap& _urlArgs = {}, const QVariantMap& _postFields = {}){
      return callAPIImpl(gEncodedJWT, _method, _api, _urlArgs, _postFields);
    }

    QVariant callAdminAPI(HTTPMethod _method, const QString& _api, const QVariantMap& _urlArgs = {}, const QVariantMap& _postFields = {}){
      return callAPIImpl(gEncodedAdminJWT, _method, _api, _urlArgs, _postFields);
    }

private:
    QVariant callAPIImpl(QString _encodedJWT , HTTPMethod _method, const QString& _api, const QVariantMap& _urlArgs = {}, const QVariantMap& _postFields = {}){
        QtCUrl CUrl;
        CUrl.setTextCodec("UTF-8");

        auto makeURL = [_method, _api, _urlArgs](){
            QUrlQuery URLQuery;
            for(auto ArgIter = _urlArgs.begin(); ArgIter != _urlArgs.end(); ++ArgIter)
                URLQuery.addQueryItem(ArgIter.key(), ArgIter.value().toString());
            QUrl URL(APIURL + "/" + _api);
            URL.setQuery(URLQuery);
            QString URLStr = URL.toString().replace(" ", "%20").replace("+","%2B");
            qDebug()<<"Request"<<HTTPMethodString[_method]<<URLStr;
            return URLStr;
        };

        QtCUrl::Options Opt;
        Opt[CURLOPT_URL] = makeURL();
        Opt[CURLOPT_TIMEOUT] = 10;
        Opt[CURLOPT_FAILONERROR] = true;
        QStringList Headers = QStringList({"Content-Type: application/json"});
        if(_encodedJWT.size())
            Headers.append("Authorization: Bearer " + _encodedJWT);
        Opt[CURLOPT_HTTPHEADER] = Headers;

        switch(_method){
        case GET: Opt[CURLOPT_CUSTOMREQUEST] = "GET"; break;
        case DELETE: Opt[CURLOPT_CUSTOMREQUEST] = "DELETE"; break;
        case POST: Opt[CURLOPT_POST] = true; break;
        case PUT: Opt[CURLOPT_CUSTOMREQUEST] = "PUT"; break;
        case PATCH: Opt[CURLOPT_CUSTOMREQUEST] = "PATCH"; break;
        }

        switch(_method){
        case GET:
        case DELETE:
            break;
        case POST:
        case PUT:
        case PATCH:
            Opt[CURLOPT_POSTFIELDS] = QJsonDocument::fromVariant(_postFields).toJson(QJsonDocument::Compact);
        }

        QString CUrlResult = CUrl.exec(Opt);

        if(CUrl.lastError().isOk() == false){
            qDebug()<<CUrl.errorBuffer();
            return gInvalid;
        }else{
            QJsonParseError JsonError;
            QJsonDocument Doc = QJsonDocument::fromJson(CUrlResult.toUtf8(),& JsonError);
            if(JsonError.error != QJsonParseError::NoError)
                qDebug()<< "Unable to parse JSON: " + JsonError.errorString() + '"' + CUrlResult + '"';
            QVariant Result = Doc.toVariant().toMap().value("result");
            qDebug()<<"Result: "<<Result;
            return Result;
        }
    }
};

#endif // TESTCOMMON_HPP
