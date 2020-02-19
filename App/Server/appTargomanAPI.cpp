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

#include <QCoreApplication>
#include <QPluginLoader>

#include "libTargomanDBM/clsDAC.h"

#include "Interfaces/Common/intfAPIModule.h"
#include "Interfaces/NLP/FormalityChecker.h"
#include "Interfaces/NLP/TextProcessor.hpp"

#include "appTargomanAPI.h"
#include "RESTServer.h"
#include "RESTAPIRegistry.h"
#include "ServerConfigs.h"
#include "OpenAPIGenerator.h"

namespace Targoman {
namespace API {
namespace Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleLoader, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleUnable2LoadFile, exModuleLoader);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAPIModule, exModuleLoader);
#pragma clang diagnostic pop


appTargomanAPI::appTargomanAPI(QObject *parent) : QObject(parent)
{;}

void appTargomanAPI::slotExecute()
{
    try{
        // Load modules
        QMap<QString, intfAPIModule::stuDBInfo> RequiredDBs;
        QDir PluginsDir = QDir(ServerConfigs::ModulesPath.value());
        foreach (QString FileName, PluginsDir.entryList(QDir::Files))
        {
            TargomanDebug(5,"Loading Module: "<<PluginsDir.absoluteFilePath(FileName));
            QPluginLoader* Loader = new QPluginLoader(PluginsDir.absoluteFilePath(FileName));
            if (!Loader)
                throw exModuleUnable2LoadFile(QString("Unable to load %1 ").arg(FileName));
            QObject* ModuleObject = Loader->instance();
            if (ModuleObject) {
                intfAPIModule* Module = qobject_cast<intfAPIModule*>(ModuleObject);
                if(!Module)
                    throw exInvalidAPIModule(QString("Seems that this an incorrect module: %1").arg(FileName));

                foreach(auto ModuleMethod, Module->listOfMethods())
                    RESTAPIRegistry::registerRESTAPI(ModuleMethod.Module, ModuleMethod.Method);

                if(Module->requiredDB().Schema.size())
                    RequiredDBs.insert(Module->moduleBaseName(), Module->requiredDB());

                if(Module->requiresTextProcessor())
                    NLP::TextProcessor::instance().init(
                                Targoman::Common::Configuration::ConfigManager::instance().configSettings());

                if(Module->requiresFormalityChecker())
                    NLP::FormalityChecker::instance();
            }else{
                TargomanWarn(5,"Unable to load Module :"<<Loader->errorString());
                if (Loader->isLoaded())
                    Loader->unload(); //Unload plugin if it canot be used by the application
                delete Loader;
            }
        }
        //Prepare database connections
        if(RequiredDBs.size()) {
            DBManager::clsDAC::addDBEngine(DBManager::enuDBEngines::MySQL);
            QSet<QString> ConnectionStrings;
            if(ServerConfigs::MasterDB::Host.value().size()
               && ServerConfigs::MasterDB::Schema.value().size()){
                intfAPIModule::stuDBInfo MasterDBInfo = {
                                                            ServerConfigs::MasterDB::Schema.value(),
                                                            ServerConfigs::MasterDB::Port.value(),
                                                            ServerConfigs::MasterDB::Host.value(),
                                                            ServerConfigs::MasterDB::User.value(),
                                                            ServerConfigs::MasterDB::Pass.value()
                                                        };
                ConnectionStrings.insert(MasterDBInfo.toConnStr(true));
                DBManager::clsDAC::setConnectionString(MasterDBInfo.toConnStr());
            }


            for(auto DBInfoIter = RequiredDBs.begin(); DBInfoIter != RequiredDBs.end(); ++DBInfoIter) {
                if(DBInfoIter->Host.size()
                   && ConnectionStrings.contains(DBInfoIter->toConnStr(true)) == false) {
                    ConnectionStrings.insert(DBInfoIter->toConnStr(true));
                    if(ConnectionStrings.isEmpty())
                        DBManager::clsDAC::setConnectionString(DBInfoIter->toConnStr());
                    else
                        DBManager::clsDAC::setConnectionString(DBInfoIter->toConnStr(), DBInfoIter.key());
                }
            }
        }

        RESTServer::instance().start();

        OpenAPIGenerator::retrieveJson();

        TargomanInfo(5, "\n" + RESTAPIRegistry::registeredAPIs("",true,true).join("\n"));

    }catch(Common::exTargomanBase& e){
        TargomanLogError(e.what());
        QCoreApplication::exit(-1);
    }
}

}
}
}
