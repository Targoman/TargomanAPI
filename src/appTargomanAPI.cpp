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
 @author S. Mohammad M. Ziabary <ziabary@targoman.com>
 */
#include <QCoreApplication>

#include "appTargomanAPI.h"
#include "Configs.h"
#include "libTargomanDBM/clsDAC.h"
#include "Modules/Translation.h"

using namespace QHttp;
using namespace Targoman::DBManager;

namespace Targoman{
namespace Apps {

appTargomanAPI::appTargomanAPI(QObject *parent) : QObject(parent)
{;}

void Targoman::Apps::appTargomanAPI::slotExecute()
{
    try{
        RESTServer::stuConfig Configs;

        Configs.BasePath = gConfigs::BasePath.value();
        Configs.Version = gConfigs::Version.value();
        Configs.StatisticsInterval = gConfigs::StatisticsInterval.value();
        Configs.ListenPort = gConfigs::ListenPort.value();
        Configs.ListenAddress = gConfigs::JustLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any;
        Configs.JWTSecret = gConfigs::JWTSecret.value();
        Configs.JWTHashAlgorithm = gConfigs::JWTHashAlgorithm.value();
        Configs.SimpleCryptKey = gConfigs::SimpleCryptKey.value();
        Configs.IndentedJson = gConfigs::IndentedJson.value();
        Configs.MaxUploadSize = gConfigs::MaxUploadSize.value();
        Configs.MaxUploadedFileSize = gConfigs::MaxUploadedFileSize.value();
        Configs.MaxCachedItems = gConfigs::MaxCachedItems.value();
        Configs.CacheConnector = gConfigs::CacheConnector.value();

#ifdef QHTTP_ENABLE_WEBSOCKET
        Configs.WebSocketServerName = gConfigs::WebSocketServerName.value();
        Configs.WebSocketServerPort = gConfigs::WebSocketServerPort.value();
        Configs.WebSocketServerAdderss = gConfigs::WebSocketServerJustLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any;
        Configs.WebSocketSecure = gConfigs::WebSocketServerSecure.value();;
#endif

        if(gConfigs::BaseOpenAPIObjectFile.value().size()){
            QFile File(gConfigs::BaseOpenAPIObjectFile.value());
            File.open(QIODevice::ReadOnly);
            if (File.isReadable() == false)
                throw exTargomanAPI("Unable to open: <" + gConfigs::BaseOpenAPIObjectFile.value() + "> for reading");
            QJsonDocument JsonDoc = QJsonDocument::fromBinaryData(File.readAll());
            if(JsonDoc.isNull() || JsonDoc.isObject() == false)
                throw exTargomanAPI("Invalid json reading from: <" + gConfigs::BaseOpenAPIObjectFile.value() + ">");
            Configs.BaseOpenAPIObject = JsonDoc.object();
        }

        //Configs.fnIPInBlackList;

        clsDAC::addDBEngine(enuDBEngines::MySQL);
        clsDAC::setConnectionString(QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5").arg(
                                        gConfigs::DBHost.value()).arg(
                                        gConfigs::DBPort.value()).arg(
                                        gConfigs::DBUser.value()).arg(
                                        gConfigs::DBPass.value()).arg(
                                        gConfigs::DBSchema.value()));

        Translation::instance().init();


        RESTServer::configure (Configs);
        RESTServer::start();

        //QCoreApplication::exit(0);
    }catch(Common::exTargomanBase& e){
        TargomanLogError(e.what());
        QCoreApplication::exit(-1);
    }
}

}
}
