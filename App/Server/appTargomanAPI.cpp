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
#include "Interfaces/Helpers/SecurityHelper.h"
//#include "Interfaces/NLP/FormalityChecker.h"
//#include "Interfaces/NLP/TextProcessor.hpp"

#include "appTargomanAPI.h"
#include "RESTServer.h"
#include "RESTAPIRegistry.h"
#include "ServerConfigs.h"
#include "OpenAPIGenerator.h"
#include "StaticModule.h"

using namespace Targoman::Common;

namespace Targoman::API::Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleLoader, Targoman::Common::exTargomanBase);
TARGOMAN_ADD_EXCEPTION_HANDLER(exModuleUnable2LoadFile, exModuleLoader);
TARGOMAN_ADD_EXCEPTION_HANDLER(exInvalidAPIModule, exModuleLoader);
TARGOMAN_ADD_EXCEPTION_HANDLER(exAPIModuleInitiailization, exModuleLoader);
#pragma clang diagnostic pop

appTargomanAPI::appTargomanAPI(QObject *parent) : QObject(parent) { ; }

void appTargomanAPI::slotExecute() {
    try {
//        ServerCommonConfigs::InstanceID.setFromVariant(QString("TAPI-%1").arg(Helpers::SecurityHelper::UUIDtoMD5()));
        ServerCommonConfigs::InstanceID.setFromVariant(QString("TAPI-%1").arg(QSysInfo::machineHostName()));
        TargomanDebug(0) << "Instance-ID: " << ServerCommonConfigs::InstanceID.value();

        QMap<QString, stuModuleDBInfo> RequiredDBs;

        auto RegisterModule = [&RequiredDBs](intfPureModule *_module) {
            _module->setInstancePointer();

            TargomanDebug(0) << "Registering module <" << _module->moduleFullName() << ">";

            foreach (auto ModuleMethod, _module->listOfMethods())
                RESTAPIRegistry::registerRESTAPI(ModuleMethod.Module, ModuleMethod.Method);

            QMap<QString, stuModuleDBInfo> DBInfos = _module->requiredDBs();

            for (auto it = DBInfos.constBegin();
                 it != DBInfos.constEnd();
                 it++
            ) {
                if (it->Schema.size() && (RequiredDBs.contains(it.key()) == false))
                    RequiredDBs.insert(/*_module->moduleBaseName()*/ it.key(), it.value());
            }

            //_module->initializeModule();
        };

        //-- StaticModule --
        static StaticModule *StaticModuleInstance = new StaticModule();
        RegisterModule(StaticModuleInstance);
//            throw exAPIModuleInitiailization("Unable to init StaticModule");

        //-- Dynamic Modules --
//#ifndef QT_DEBUG
        auto LoadedModules = Configuration::ConfigManager::instance().loadedPlugins();
        if (LoadedModules.isEmpty())
            throw exTargomanAPI("No module was loaded. Maybe you forgot to specify --plugins");

        foreach (auto Plugin, LoadedModules) {
            intfPureModule* Module = qobject_cast<intfPureModule*>(Plugin.Instance);

            if (!Module)
                throw exInvalidAPIModule(QString("Seems that this an incorrect module: %1").arg(Plugin.File));

            RegisterModule(Module);
//                throw exAPIModuleInitiailization(QString("Unable to init module: %1").arg(Plugin.File));
        }
//#endif

        //-- Prepare database connections --
        TargomanDebug(0, "Registering db connections");

        QSet<QString> ConnectionStrings;

        if (ServerConfigs::MasterDB::Host.value().size()
                && ServerConfigs::MasterDB::Schema.value().size()) {
            stuModuleDBInfo MasterDBInfo = {
                ServerConfigs::MasterDB::Schema.value(),
                ServerConfigs::MasterDB::Port.value(),
                ServerConfigs::MasterDB::Host.value(),
                ServerConfigs::MasterDB::User.value(),
                ServerConfigs::MasterDB::Pass.value()
            };

            QString ConnStr = MasterDBInfo.toConnStr(ServerCommonConfigs::DBPrefix.value()/*, true*/);
            ConnectionStrings.insert(ConnStr);

            TargomanDebug(0, "Registering " << ConnStr);
            DBManager::clsDAC::addDBEngine(DBManager::enuDBEngines::MySQL);
            DBManager::clsDAC::setConnectionString(ConnStr);
        }

        if (RequiredDBs.size()) {
            for (auto DBInfoIter = RequiredDBs.begin(); DBInfoIter != RequiredDBs.end(); ++DBInfoIter) {
                QString ConnStr = DBInfoIter->toConnStr(ServerCommonConfigs::DBPrefix.value()/*true*/);

                if (DBInfoIter->Host.size()
                        && DBInfoIter->Schema.size()
                        && ConnectionStrings.contains(ConnStr) == false) {
                    ConnectionStrings.insert(ConnStr);

                    TargomanDebug(0, "Registering <" << DBInfoIter.key() << "> " << ConnStr);
                    DBManager::clsDAC::addDBEngine(DBManager::enuDBEngines::MySQL, DBInfoIter.key(), DBInfoIter->Schema);
                    DBManager::clsDAC::setConnectionString(
                                ConnStr,
                                DBInfoIter.key(),
                                DBInfoIter->Schema
                                );
                }
            }
        }

        //-- init modules
        StaticModuleInstance->initializeModule();

        foreach (auto Plugin, LoadedModules) {
            intfPureModule* Module = qobject_cast<intfPureModule*>(Plugin.Instance);
            Module->initializeModule();
        }

        RESTServer::instance().start();

        OpenAPIGenerator::retrieveJson();

        RESTAPIRegistry::dumpAPIs();

        TargomanDebug(5).nospace() .noquote() << ">>>>>" << " [" << ServerCommonConfigs::InstanceID.value() << "] Server ready." << " (version: " << TARGOMAN_M2STR(PROJ_VERSION) << ")" << " <<<<<";

    } catch (Targoman::Common::exTargomanBase& e) {
        TargomanLogError(e.what());
        QCoreApplication::exit(-1);
    }
}

} //namespace Targoman::API::Server
