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
 * @author S.Mehran M.Ziabary <ziabary@targoman.com>
 */


#ifndef TARGOMAN_API_SERVER_CLSREQUESTHANDLER_H
#define TARGOMAN_API_SERVER_CLSREQUESTHANDLER_H

#include <QTemporaryFile>
#include "QHttp/QHttpServer"

#include "3rdParty/multipart-parser/MultipartReader.h"

#include "ServerConfigs.h"
#include "RESTAPIRegistry.h"

namespace Targoman {
namespace API {
namespace Server {

class clsRequestHandler;

class clsUpdateAndPruneThread : public QThread{
    Q_OBJECT
private:
    void run() Q_DECL_FINAL;
};

class clsMultipartFormDataRequestHandler : public MultipartReader{
public:
    clsMultipartFormDataRequestHandler(clsRequestHandler* _parent, const QByteArray& _marker) :
        MultipartReader(_marker.toStdString()),
        pRequestHandler(_parent),
        LastWrittenBytes(0){
        this->onPartBegin = clsMultipartFormDataRequestHandler::onMultiPartBegin;
        this->onPartData = clsMultipartFormDataRequestHandler::onMultiPartData;
        this->onPartEnd = clsMultipartFormDataRequestHandler::onMultiPartEnd;
        this->onEnd = clsMultipartFormDataRequestHandler::onDataEnd;
        this->userData = reinterpret_cast<void*>(this);
    }

    size_t feed(const char *_buffer, long long  _len){
        return static_cast<size_t>(MultipartReader::feed(_buffer, _len));
    }

private:
    static void onMultiPartBegin(const MultipartHeaders& _headers, void *_userData);
    static void onMultiPartData(const char *_buffer, long long _size, void *_userData);
    static void onMultiPartEnd(void *_userData);
    static void onDataEnd(void *_userData);

    void storeDataInRequest();

private:
    clsRequestHandler*              pRequestHandler;
    QScopedPointer<QTemporaryFile>  LastTempFile;
    std::string                     LastMime;
    std::string                     LastFileName;
    std::string                     ToBeStoredItemName;
    std::string                     LastItemName;
    QString                         LastValue;
    int                             LastWrittenBytes;
    QStringList                     SameNameItems;

    friend class clsRequestHandler;
};

class clsRequestHandler : public QObject
{
    Q_OBJECT
    struct stuResult {
        qhttp::TStatusCode StatusCode;
        QVariant Result;
        stuResult(const QVariant& _result = {}, qhttp::TStatusCode _code = qhttp::ESTATUS_OK) :
            StatusCode(_code),
            Result(_result)
        {}
    };

public:
    clsRequestHandler(qhttp::server::QHttpRequest* _req,
                      qhttp::server::QHttpResponse* _res,
                      QObject *_parent = nullptr);
    void process(const QString& _api);
    void findAndCallAPI(QString _api);
    void sendError(qhttp::TStatusCode _code,
                   const QString& _message,
                   bool _closeConnection = false);
    void sendFile(const QString& _basePath, const QString _path);
    void sendResponse(qhttp::TStatusCode _code, QVariant _response);
    void sendCORSOptions();
    void redirect(const QString _path, bool _appendBase = true, bool _permananet = true);

private:
    void sendResponseBase(qhttp::TStatusCode _code, QJsonObject _dataObject, bool _closeConnection = false);
    stuResult run(clsAPIObject* _apiObject, QStringList& _queries, const QString& _pksByPath);
    QString toIPv4(const QString _ip);

private slots:
    void slotSendFileData();

private:
    QByteArray                                          RemainingData;
    qhttp::server::QHttpRequest*                        Request;
    qhttp::server::QHttpResponse*                       Response;
    QScopedPointer<clsMultipartFormDataRequestHandler>  MultipartFormDataHandler;
    QFutureWatcher<stuResult>                           FutureWatcher;
    QTimer                                              FutureTimer;
    QMimeDatabase                                       MIMEDB;
    QScopedPointer<QFile>                               FileHandler;

    friend class clsMultipartFormDataRequestHandler;
};

}
}
}

#endif // TARGOMAN_API_SERVER_CLSREQUESTHANDLER_H


