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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include <QCoreApplication>
#include <QThread>
#include <QTcpSocket>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>

#include "QHttp/qhttpfwd.hpp"

#include "RESTServer.h"
#include "ServerConfigs.h"
#include "clsRequestHandler.h"
#include "clsRedisConnector.h"
#include "WebSocketServer.h"
#include "RESTAPIRegistry.h"
#include "Interfaces/Server/QJWT.h"
#include "APICache.hpp"

namespace Targoman::API {
namespace Server {

using namespace Targoman::Common;
using namespace qhttp::server;

static qhttp::server::QHttpServer gHTTPServer;
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
static WebSocketServer gWSServer;
#endif
static clsUpdateAndPruneThread *gStatUpdateThread;

RESTServer::RESTServer() :
    IsStarted(false) { ; }

void RESTServer::start(fnIsInBlackList_t _fnIPInBlackList) {
    if (this->IsStarted)
        throw exTargomanInitialization("RESTServer can be run single instance only");

    this->fnIPInBlackList = _fnIPInBlackList;

    QString BasePath = ServerCommonConfigs::BasePath.value();

    if (BasePath.endsWith('/') == false)
        BasePath += '/';

    if (BasePath.startsWith('/') == false)
        BasePath = '/' + BasePath;

    if (ServerCommonConfigs::CacheConnector.value().size() && QUrl::fromUserInput(ServerCommonConfigs::CacheConnector.value()).isValid() == false)
        throw exRESTRegistry("Invalid connector url specified for central cache");

#ifdef TARGOMAN_API_ENABLE_REDIS_PROTOCOL
    if (ServerCommonConfigs::CacheConnector.value().startsWith(TARGOMAN_M2STR(QHTTP_REDIS_PROTOCOL)))
        CentralCache::setup(new clsRedisConnector(ServerCommonConfigs::CacheConnector.value()));
#endif

    if (ServerCommonConfigs::CacheConnector.value().size() && CentralCache::isValid() == false)
        throw exRESTRegistry("Unsupported cache connector protocol.");

    ServerCommonConfigs::BasePathWithVersion = BasePath + ServerCommonConfigs::Version.value();
    if (ServerCommonConfigs::BasePathWithVersion.endsWith('/') == false)
        ServerCommonConfigs::BasePathWithVersion += '/';

    this->IsStarted = true;

    if (ServerCommonConfigs::StatisticsInterval.value()) {
        gStatUpdateThread = new clsUpdateAndPruneThread();
        connect(gStatUpdateThread, &clsUpdateAndPruneThread::finished, gStatUpdateThread, &QObject::deleteLater);
        gStatUpdateThread->start();
    }

    QObject::connect(&gHTTPServer, &QHttpServer::newConnection, [this](QHttpConnection* _con) {

        TargomanLogInfo(7, QString(100, '*'));

        if (!this->validateConnection(_con->tcpSocket()->peerAddress(), _con->tcpSocket()->peerPort()))
            _con->killConnection();
    });

    QHostAddress ListenAddress = ServerCommonConfigs::JustLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any;

    gHTTPServer.listen(ListenAddress, ServerCommonConfigs::ListenPort.value(), [&, BasePath](QHttpRequest* _req, QHttpResponse* _res) {

        clsRequestHandler* RequestHandler = new clsRequestHandler(_req, _res);

        try {
            QString Path = _req->url().adjusted(QUrl::NormalizePathSegments | QUrl::RemoveAuthority).path(QUrl::PrettyDecoded);

            if (Path != _req->url().path())
                return RequestHandler->redirect(Path, false);

            if (ServerCommonConfigs::PublicPath.value().size() && QFile::exists(ServerCommonConfigs::PublicPath.value() + Path))
                return RequestHandler->sendFile(ServerCommonConfigs::PublicPath.value(), Path);

            if (Path.startsWith(BasePath) == false)
                return RequestHandler->sendError(qhttp::ESTATUS_NOT_FOUND, "Path not found: '" + Path + "'", {}, {}, true);

            if (Path.startsWith(ServerCommonConfigs::BasePathWithVersion) == false)
                return RequestHandler->sendError(qhttp::ESTATUS_NOT_ACCEPTABLE, "Invalid Version or version not specified", {}, {}, true);

            if (Path == ServerCommonConfigs::BasePathWithVersion )
                return RequestHandler->sendError(qhttp::ESTATUS_NOT_ACCEPTABLE, "No API call provided", {}, {}, true);

            TargomanLogInfo(7, QString(100, '*'));
            TargomanLogInfo(7, "(" << RequestHandler->Index() << ") "
                            << "New API Call ["
                            << _req->connection()->tcpSocket()->peerAddress().toString()
                            << ":"
                            << _req->connection()->tcpSocket()->peerPort()
                            << "]: "
                            << _req->methodString ()
                            << " "
                            << Path
                            << "?"
                            << _req->url().query());

            //----------------------------------
            try {
                auto headers = _req->headers();
                QStringList headersLog;
                for (auto Iter = headers.begin(); Iter != headers.end(); ++Iter)
                    headersLog.append(QString("    %1: %2").arg(Iter.key().toStdString().c_str()).arg(Iter.value().toStdString().c_str()));
                TargomanLogInfo(7, "headers: " << "\n" << headersLog.join("\n"));
            } catch (...) { ; }

            //----------------------------------
            RequestHandler->process(Path.mid(ServerCommonConfigs::BasePathWithVersion.size() - 1));
        } catch (exHTTPError& ex) {
            RequestHandler->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), ex.additionalInfo(), {}, ex.httpCode() >= 500);
        } catch (exTargomanBase& ex) {
            RequestHandler->sendError(static_cast<qhttp::TStatusCode>(ex.httpCode()), ex.what(), {}, {}, ex.httpCode() >= 500);
        }
    });

    if (gHTTPServer.isListening()) {
        TargomanLogInfo(1, "REST Server is listening on "<<ListenAddress.toString()<<":"<<ServerCommonConfigs::ListenPort.value()<<ServerCommonConfigs::BasePathWithVersion);
    } else {
        TargomanLogError("Unable to start server to listen on "<<ListenAddress.toString()<<":"<<ServerCommonConfigs::ListenPort.value());
        QCoreApplication::exit(-1);
    }

#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
    if (gWSServer.isActive()) {
        QObject::connect(&gWSServer, &WebSocketServer::sigNewConnection, [this](QWebSocket* _con) {
            if (!validateConnection (_con->peerAddress(), _con->peerPort()))
                _con->close(QWebSocketProtocol::CloseCodePolicyViolated,"IP banned");
        });

        gWSServer.start();
    }
#endif
}

void RESTServer::stop() {
    gHTTPServer.stopListening();
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
    gWSServer.stopListening();
#endif

    this->IsStarted = false;
    if (gStatUpdateThread)
        gStatUpdateThread->quit();
}

TAPI::stuStatistics RESTServer::stats() {
    return gServerStats;
}

QStringList RESTServer::registeredAPIs(bool _showParams, bool _showTypes, bool _prettifyTypes) {
    return RESTAPIRegistry::registeredAPIs("", _showParams, _showTypes, _prettifyTypes);
}

/*
void RESTServer::registerUserDefinedType(const char* _typeName, intfAPIArgManipulator* _argManipulator) {
//    TargomanInfo(9, "registering User Defined Type: (" << _typeName << ")");

    Q_ASSERT_X(QMetaType::type(_typeName), QString("registerUserDefinedType typeName(%1)").arg(_typeName).toStdString().c_str(), "Seems that registering syntax is erroneous");

    gUserDefinedTypesInfo.insert(QMetaType::type(_typeName) - TAPI_BASE_USER_DEFINED_TYPEID, _argManipulator);
}
*/

bool RESTServer::validateConnection(const QHostAddress& _peerAddress, quint16 _peerPort) {
    enuIPBlackListStatus::Type IPBlackListStatus = enuIPBlackListStatus::Unknown;

    if (this->fnIPInBlackList
        && (IPBlackListStatus = this->fnIPInBlackList(_peerAddress)) != enuIPBlackListStatus::Ok) {

        TargomanLogWarn(1,"Connection from " + _peerAddress.toString() + " was closed by security provider due to: "+enuIPBlackListStatus::toStr(IPBlackListStatus));

        gServerStats.Blocked.inc();

        return false;
    }

    gServerStats.Connections.inc();

    TargomanLogInfo(7, "New connection accepted from:" << _peerAddress.toString() << ":" << _peerPort);
    return true;
}

} //namespace Server

/***********************************************************************************************/
//void registerUserDefinedType(const char* _typeName, intfAPIArgManipulator* _argManipulator) {
//    Server::RESTServer::instance().registerUserDefinedType(_typeName, _argManipulator);
//}

} //namespace Targoman::API
