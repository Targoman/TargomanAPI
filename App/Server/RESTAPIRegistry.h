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


#ifndef TARGOMAN_API_SERVER_RESTAPIREGISTRY_H
#define TARGOMAN_API_SERVER_RESTAPIREGISTRY_H

#include <QGenericArgument>
#include <QMetaMethod>
#include "Interfaces/Common/intfAPIModule.h"
#include "clsAPIObject.h"
#include "NumericTypes.hpp"

namespace Targoman {
namespace API {
namespace Server {

class clsAPIObject;

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exRESTRegistry, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop


class RESTAPIRegistry
{
public:
    static inline QString makeRESTAPIKey(const QString& _httpMethod, const QString& _path){
        return  _httpMethod.toUpper() + " " + (_path.endsWith('/') ? _path.mid(0, _path.size() - 1) : _path);
    }

    static inline clsAPIObject*
    getAPIObject(const QString _httpMethod, const QString& _path){
        return RESTAPIRegistry::Registry.value(RESTAPIRegistry::makeRESTAPIKey(_httpMethod, _path));
    }
#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
    static inline clsAPIObject* getWSAPIObject(const QString& _path){
        return RESTAPIRegistry::WSRegistry.value(RESTAPIRegistry::makeRESTAPIKey("WS", _path));
    }
#endif

    static void registerRESTAPI(intfAPIModule* _module, const QMetaMethod& _method);
    static QStringList registeredAPIs(const QString& _module, bool _showParams = false, bool _showTypes = false, bool _prettifyTypes = true);
    static QJsonObject retriveOpenAPIJson();

private:
    static inline QString isValidType(int _typeID, bool _validate4Input);
    static void validateMethodInputAndOutput(const QMetaMethod& _method);
    static void addRegistryEntry(QHash<QString, clsAPIObject*>& _registry, intfAPIModule* _module, const QMetaMethodExtended& _method, const QString& _httpMethod, const QString& _methodName);
    static int  getCacheSeconds(const QMetaMethod& _method, const char* _type);
    static QMap<QString, QString> extractMethods(QHash<QString, clsAPIObject*>& _registry, const QString& _module, bool _showTypes, bool _prettifyTypes);

private:
    static QHash<QString, clsAPIObject*>  Registry;
    static QJsonObject CachedOpenAPI;

#ifdef TARGOMAN_API_ENABLE_WEBSOCKET
    static QHash<QString, clsAPIObject*>  WSRegistry;
#endif
};

}
}
}

#endif // TARGOMAN_API_SERVER_RESTAPIREGISTRY_H
