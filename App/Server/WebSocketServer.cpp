/******************************************************************************
#   TargomanAPI: REST API for Targoman
#
#   Copyright 2014-2019 by Targoman Intelligent Processing <http://tip.co.ir>
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
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET

#include "WebSocketServer.h"
#include "ServerConfigs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

namespace Targoman {
namespace API {
namespace Server {

using namespace Common::Configuration;

tmplConfigurable<QString> WebSocketServer::Name(
        WebSocketServer::makeConfig("Name"),
        "If set then a socket server with specified name will be started",
        "",
        ReturnTrueCrossValidator(),
        "",
        "NAME",
        "ws-name",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint16> WebSocketServer::Port(
        WebSocketServer::makeConfig("Port"),
        "Listen port for WebSocket server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "w",
        "PORT",
        "ws-listen-port",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> WebSocketServer::JustLocal(
        WebSocketServer::makeConfig("JustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "ws-just-local",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> WebSocketServer::Secure(
        WebSocketServer::makeConfig("Secure"),
        "Start web socket server in secure mode",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "ws-secure",
        enuConfigSource::Arg | enuConfigSource::File);



void WebSocketServer::start()
{
    this->WS.reset(new QWebSocketServer(this->Name.value(),
                                        this->Secure.value()?
                                            QWebSocketServer::SecureMode :
                                            QWebSocketServer::NonSecureMode));


    if (this->WS->listen(QHostAddress::Any, this->Port.value())) {
        TargomanLogInfo(1, "WebSocketServer is listening on "<<this->Port.value()<< " Name: "<<this->Name.value());
        connect(this->WS.data(), &QWebSocketServer::newConnection, this, &WebSocketServer::onNewConnection);
        connect(this->WS.data(), &QWebSocketServer::closed, this, &WebSocketServer::closed);
    }
}

void WebSocketServer::stopListening()
{
    this->WS->close ();
    qDeleteAll(this->Clients.begin(), this->Clients.end());
}

void WebSocketServer::onNewConnection()
{

    QWebSocket *pSocket = this->WS->nextPendingConnection();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &WebSocketServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived, this, &WebSocketServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &WebSocketServer::socketDisconnected);

    this->Clients << pSocket;

    emit sigNewConnection(pSocket);
}

void WebSocketServer::processTextMessage(QString _message)
{

    QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());
    TargomanDebug(5, "Text Message Received:" << _message);

    if (pSocket) {
        auto sendError = [pSocket](qhttp::TStatusCode _code, QString _message){
            QJsonObject ErrorInfo = QJsonObject({
                                                    {"code", _code},
                                                    {"message", _message}
                                                });
            pSocket->sendTextMessage(QJsonDocument(QJsonObject({{"error",
                                                                 ErrorInfo
                                                                }})).toJson(ServerConfigs::IndentedJson.value() ?
                                                                                QJsonDocument::Indented :
                                                                                QJsonDocument::Compact).data());
        };

        try{
            QJsonParseError Error;
            QJsonDocument JSON = QJsonDocument::fromJson(_message.toUtf8(), &Error);
            if(JSON.isNull())
                throw exHTTPBadRequest(QString("Invalid JSON request: %1").arg(Error.errorString()));

            QString ExtraAPIPath;
            QJsonObject JSONReqObject = JSON.object();
            QString API = JSONReqObject.begin().key();
            if(API.isEmpty())
                return sendError(qhttp::ESTATUS_BAD_REQUEST, "No API path specified");

            clsAPIObject* APIObject = RESTAPIRegistry::getWSAPIObject (API);
            if(!APIObject) {
                QString Path = API;
                if(Path.endsWith('/'))
                    Path.truncate(Path.size() - 1);
                ExtraAPIPath = Path.mid(Path.lastIndexOf('/'));
                Path = Path.mid(0, Path.lastIndexOf('/'));
                APIObject = RESTAPIRegistry::getWSAPIObject(Path);
            }

            if(!APIObject)
                return sendError(qhttp::ESTATUS_NOT_FOUND, "WS API not found ("+API+")");

            QVariantMap APIArgs = JSONReqObject.begin().value().toObject().toVariantMap();
            QStringList Queries;
            for(auto Map = APIArgs.begin(); Map != APIArgs.end(); ++Map)
                Queries.append(Map.key() + '=' + Map.value().toString());

            /*if(ExtraAPIPath)
                Queries.append(REQUES)*/


            QByteArray Data = QJsonDocument(QJsonObject({{"result",
                                                          QJsonValue::fromVariant(APIObject->invoke(Queries))
                                                         }})).toJson(ServerConfigs::IndentedJson.value() ?
                                                                         QJsonDocument::Indented :
                                                                         QJsonDocument::Compact);
            pSocket->sendTextMessage(Data.data());
        }catch(Targoman::Common::exTargomanBase& ex){
            sendError(static_cast<qhttp::TStatusCode>(ex.code()), ex.what());
        }catch(QFieldValidator::exRequiredParam &ex){
            sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what());
        }catch(QFieldValidator::exInvalidValue &ex){
            sendError(qhttp::ESTATUS_BAD_REQUEST, ex.what());
        }catch(std::exception &ex){
            sendError(qhttp::ESTATUS_INTERNAL_SERVER_ERROR, ex.what());
        }
    }
}

void WebSocketServer::processBinaryMessage(QByteArray _message)
{
    QWebSocket *pSocket = qobject_cast<QWebSocket *>(sender());
    TargomanDebug(5, "Binary Message Received:" << _message);
    if (pSocket) {
        pSocket->sendBinaryMessage(_message);
    }
}

void WebSocketServer::socketDisconnected()
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        TargomanLogDebug(5, "Client Disconnected: "<<pClient)
                this->Clients.removeAll(pClient);
        pClient->deleteLater();
    }
}

}
}
}
#endif //TARGOMAN_API_ENABLE_WEBSOCKET
