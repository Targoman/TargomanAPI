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
#include <QPluginLoader>

#include "libTargomanDBM/clsDAC.h"
#include "libTargomanCommon/Configuration/ConfigManager.h"

#include "Interfaces/API/intfPureModule.h"
//#include "Interfaces/NLP/FormalityChecker.h"
//#include "Interfaces/NLP/TextProcessor.hpp"

#include "appTargomanAPI.h"
#include "RESTServer.h"
#include "RESTAPIRegistry.h"
#include "ServerConfigs.h"
#include "OpenAPIGenerator.h"

using namespace Targoman::Common;

namespace Targoman::API::Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleLoader, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleUnable2LoadFile, exModuleLoader);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAPIModule, exModuleLoader);
TARGOMAN_ADD_EXCEPTION_HANDLER(exAPIModuleInitiailization, exModuleLoader);
#pragma clang diagnostic pop

appTargomanAPI::appTargomanAPI(QObject *parent) : QObject(parent)
{}

void appTargomanAPI::slotExecute()
{
    try {
        // Load modules
        QMap<QString, intfPureModule::stuDBInfo> RequiredDBs;

        auto LoadedModules = Configuration::ConfigManager::instance().loadedPlugins();
        if (LoadedModules.isEmpty())
            throw exTargomanAPI("No module was loaded. Maybe you forgot to specify --plugins");

        foreach (auto Plugin, LoadedModules) {
            intfPureModule* Module = qobject_cast<intfPureModule*>(Plugin.Instance);
            if (!Module)
                throw exInvalidAPIModule(QString("Seems that this an incorrect module: %1").arg(Plugin.File));

            TargomanDebug(0, "Loading module <" << Module->moduleFullName() << ">");

            foreach(auto ModuleMethod, Module->listOfMethods())
                RESTAPIRegistry::registerRESTAPI(ModuleMethod.Module, ModuleMethod.Method);

            auto DBInfo = Module->requiredDB();
            if (DBInfo.Schema.size())
                RequiredDBs.insert(Module->moduleBaseName(), DBInfo);

            if (!Module->init())
                throw exAPIModuleInitiailization(QString("Unable to init module: %1").arg(Plugin.File));
        }

        //Prepare database connections
        if (RequiredDBs.size())
        {
            DBManager::clsDAC::addDBEngine(DBManager::enuDBEngines::MySQL);

            QSet<QString> ConnectionStrings;

            if (ServerConfigs::MasterDB::Host.value().size()
                    && ServerConfigs::MasterDB::Schema.value().size())
            {
                intfPureModule::stuDBInfo MasterDBInfo = {
                                                            ServerConfigs::MasterDB::Schema.value(),
                                                            ServerConfigs::MasterDB::Port.value(),
                                                            ServerConfigs::MasterDB::Host.value(),
                                                            ServerConfigs::MasterDB::User.value(),
                                                            ServerConfigs::MasterDB::Pass.value()
                                                        };

                ConnectionStrings.insert(MasterDBInfo.toConnStr(/*true*/));
                DBManager::clsDAC::setConnectionString(MasterDBInfo.toConnStr());
            }

            for (auto DBInfoIter = RequiredDBs.begin(); DBInfoIter != RequiredDBs.end(); ++DBInfoIter)
            {
                if (DBInfoIter->Host.size()
                        && DBInfoIter->Schema.size()
                        && ConnectionStrings.contains(DBInfoIter->toConnStr(/*true*/)) == false)
                {
                    DBManager::clsDAC::addDBEngine(DBManager::enuDBEngines::MySQL, DBInfoIter.key(), DBInfoIter->Schema);

                    ConnectionStrings.insert(DBInfoIter->toConnStr(/*true*/));

//                    if (ConnectionStrings.isEmpty())
//                        DBManager::clsDAC::setConnectionString(DBInfoIter->toConnStr());
//                    else
                        DBManager::clsDAC::setConnectionString(
                                    DBInfoIter->toConnStr(),
                                    DBInfoIter.key(),
                                    DBInfoIter->Schema
                                    );
                }
            }
        }

        RESTServer::instance().start();

        OpenAPIGenerator::retrieveJson();

        TargomanInfo(5, "\n" + RESTAPIRegistry::registeredAPIs("", true, true).join("\n"));
    }
    catch(Targoman::Common::exTargomanBase& e) {
        TargomanLogError(e.what());
        QCoreApplication::exit(-1);
    }
}

} //namespace Targoman::API::Server
