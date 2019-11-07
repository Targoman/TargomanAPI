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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */
#include <QCoreApplication>

#include "appTargomanAPI.h"
#include "Configs.h"
#include "libTargomanDBM/clsDAC.h"
#include "Modules/Translation/Translation.h"
#include "Modules/Account/Account.h"
#include "Modules/Advert/Advert.h"
#include "Modules/NGT/NGTv1.h"

using namespace QHttp;
using namespace Targoman::DBManager;

namespace Targoman{
namespace Apps {

appTargomanAPI::appTargomanAPI(QObject *parent) : QObject(parent)
{;}

void appTargomanAPI::slotExecute()
{
    try{
        RESTServer::stuConfig Configs;

        Configs.BasePath = gConfigs::Rest::BasePath.value();
        Configs.Version = gConfigs::Rest::Version.value();
        Configs.StatisticsInterval = gConfigs::Rest::StatisticsInterval.value();
        Configs.ListenPort = gConfigs::Rest::ListenPort.value();
        Configs.ListenAddress = gConfigs::Rest::JustLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any;
        Configs.IndentedJson = gConfigs::Rest::IndentedJson.value();
        Configs.MaxUploadSize = gConfigs::Rest::MaxUploadSize.value();
        Configs.MaxUploadedFileSize = gConfigs::Rest::MaxUploadedFileSize.value();
        Configs.MaxCachedItems = gConfigs::Rest::MaxCachedItems.value();
        Configs.CacheConnector = gConfigs::Rest::CacheConnector.value();
        Configs.AccessControl = gConfigs::Rest::AccessControl.value();

        Configs.JWTSecret = gConfigs::JWT::Secret.value();
        Configs.JWTHashAlgorithm = gConfigs::JWT::HashAlgorithm.value();
        Configs.SimpleCryptKey = gConfigs::JWT::SimpleCryptKey.value();

#ifdef QHTTP_ENABLE_WEBSOCKET
        Configs.WebSocketServerName = gConfigs::WS::Name.value();
        Configs.WebSocketServerPort = gConfigs::WS::Port.value();
        Configs.WebSocketServerAdderss = gConfigs::WS::JustLocal.value() ? QHostAddress::LocalHost : QHostAddress::Any;
        Configs.WebSocketSecure = gConfigs::WS::Secure.value();;
#endif

        if(gConfigs::Rest::BaseOpenAPIObjectFile.value().size()){
            QFile File(gConfigs::Rest::BaseOpenAPIObjectFile.value());
            File.open(QIODevice::ReadOnly);
            if (File.isReadable() == false)
                throw exTargomanAPI("Unable to open: <" + gConfigs::Rest::BaseOpenAPIObjectFile.value() + "> for reading");
            QJsonDocument JsonDoc = QJsonDocument::fromBinaryData(File.readAll());
            if(JsonDoc.isNull() || JsonDoc.isObject() == false)
                throw exTargomanAPI("Invalid json reading from: <" + gConfigs::Rest::BaseOpenAPIObjectFile.value() + ">");
            Configs.BaseOpenAPIObject = JsonDoc.object();
        }

        //Configs.fnIPInBlackList;


        clsDAC::addDBEngine(enuDBEngines::MySQL);
        clsDAC::setConnectionString(QString("HOST=%1;PORT=%2;USER=%3;PASSWORD=%4;SCHEMA=%5").arg(
                                        gConfigs::DB::Host.value()).arg(
                                        gConfigs::DB::Port.value()).arg(
                                        gConfigs::DB::User.value()).arg(
                                        gConfigs::DB::Pass.value()).arg(
                                        gConfigs::DB::Schema.value()));

        //Initialize API modules
#ifdef ENABLE_MODULE_TRANSLATION
        Translation::instance().init();
#endif
#ifdef ENABLE_MODULE_ACCOUNT
        Account::instance().init();
#endif
#ifdef ENABLE_MODULE_ADVERT
        Advert::instance().init();
#endif
#ifdef ENABLE_MODULE_NGT
        Ngtv1::instance().init();
#endif

        // Initialize REST Server
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
