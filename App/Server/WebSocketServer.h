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

#ifndef TARGOMAN_API_SERVER_WEBSOCKETSERVER_HPP
#define TARGOMAN_API_SERVER_WEBSOCKETSERVER_HPP

#ifdef TARGOMAN_API_ENABLE_WEBSOCKET

#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

#include "libQFieldValidator/QFieldValidator.h"
#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Logger.h"

#include "RESTAPIRegistry.h"

namespace Targoman {
namespace API {
namespace Server {

class WebSocketServer : public QObject{
    Q_OBJECT
    static inline QString makeConfig(const QString& _name) {return "/WebSocketServer/" + _name;}
    static Targoman::Common::Configuration::tmplConfigurable<QString>      Name;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint16>Port;
    static Targoman::Common::Configuration::tmplConfigurable<bool>         JustLocal;
    static Targoman::Common::Configuration::tmplConfigurable<bool>         Secure;

public:
    void start();
    void stopListening();
    inline bool isActive() {return Name.value().size() && Port.value() > 0;}
signals:
    void sigNewConnection(QWebSocket* _connection);
    void closed();

private slots:
    void onNewConnection();
    void processTextMessage(QString _message);
    void processBinaryMessage(QByteArray _message);
    void socketDisconnected();

private:
    QScopedPointer<QWebSocketServer> WS;
    QList<QWebSocket *> Clients;
};

}
}
}
#endif // TARGOMAN_API_SERVER_ENABLE_WEBSOCKET

#endif // TARGOMAN_API_SERVER_CLSWEBSOCKETSERVER_HPP
