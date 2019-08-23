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
#ifndef TARGOMAN_APPS_CONFIGS_H
#define TARGOMAN_APPS_CONFIGS_H

#include <QHostAddress>

#include "libTargomanCommon/Configuration/tmplConfigurable.h"
#include "libTargomanCommon/Macros.h"
#include "QHttp/QRESTServer.h"

ENUM_CONFIGURABLE(QHttp::enuJWTHashAlgs)

namespace Targoman{
namespace Apps {

TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanAPI, Common::exTargomanBase);

class gConfigs
{
public:
    static inline QString appConfig(const QString& _name){
        return "/App/" + _name;
    }
    static Common::Configuration::tmplConfigurable<QString>      BasePath;
    static Common::Configuration::tmplConfigurable<QString>      Version;
    static Common::Configuration::tmplConfigurable<bool>         JustLocal;
    static Common::Configuration::tmplRangedConfigurable<quint16>ListenPort;
    static Common::Configuration::tmplRangedConfigurable<quint8> StatisticsInterval;
    static Common::Configuration::tmplConfigurable<QString>      JWTSecret;
    static Common::Configuration::tmplConfigurable<QHttp::enuJWTHashAlgs::Type>    JWTHashAlgorithm;
    static Common::Configuration::tmplConfigurable<quint64>      SimpleCryptKey;
    static Common::Configuration::tmplConfigurable<bool>         IndentedJson;
    static Common::Configuration::tmplRangedConfigurable<qint64> MaxUploadSize;
    static Common::Configuration::tmplRangedConfigurable<qint64> MaxUploadedFileSize;
    static Common::Configuration::tmplRangedConfigurable<quint32> MaxCachedItems;
    static Common::Configuration::tmplConfigurable<QString>      CacheConnector;
    static Common::Configuration::tmplConfigurable<FilePath_t>   BaseOpenAPIObjectFile;

    static Common::Configuration::tmplConfigurable<QString>      DBHost;
    static Common::Configuration::tmplRangedConfigurable<quint16>DBPort;
    static Common::Configuration::tmplConfigurable<QString>      DBUser;
    static Common::Configuration::tmplConfigurable<QString>      DBPass;
    static Common::Configuration::tmplConfigurable<QString>      DBSchema;

#ifdef QHTTP_ENABLE_WEBSOCKET
    static Common::Configuration::tmplConfigurable<QString>      WebSocketServerName;
    static Common::Configuration::tmplRangedConfigurable<quint16>WebSocketServerPort;
    static Common::Configuration::tmplConfigurable<bool>         WebSocketServerJustLocal;
    static Common::Configuration::tmplConfigurable<bool>         WebSocketServerSecure;
#endif

    ~gConfigs();
};

}
}
#endif // TARGOMAN_APPS_CONFIGS_H
