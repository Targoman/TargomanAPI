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

#ifndef TARGOMAN_API_SERVER_CONFIGS_H
#define TARGOMAN_API_SERVER_CONFIGS_H

#include <QHostAddress>

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

#include "Interfaces/Common/GenericTypes.h"

namespace Targoman{
namespace API {
namespace Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanAPI, Common::exTargomanBase);
#pragma clang diagnostic pop

struct ServerConfigs
{
    static inline QString makeConfig(const QString& _name){return "/Server/" + _name;}
    static Common::Configuration::tmplConfigurable<QString>       BasePath;
    static Common::Configuration::tmplConfigurable<QString>       Version;
    static Common::Configuration::tmplConfigurable<bool>          JustLocal;
    static Common::Configuration::tmplRangedConfigurable<quint16> ListenPort;
    static Common::Configuration::tmplConfigurable<bool>          IndentedJson;
    static Common::Configuration::tmplRangedConfigurable<qint64>  MaxUploadSize;
    static Common::Configuration::tmplRangedConfigurable<qint64>  MaxUploadedFileSize;
    static Common::Configuration::tmplConfigurable<FilePath_t>    BaseOpenAPIObjectFile;
    static Common::Configuration::tmplConfigurable<FilePath_t>    ModulesPath;
    static Common::Configuration::tmplRangedConfigurable<quint8>  StatisticsInterval;
    static Common::Configuration::tmplRangedConfigurable<quint32> MaxCachedItems;
    static Common::Configuration::tmplConfigurable<QString>       CacheConnector;
    static Common::Configuration::tmplConfigurable<QString>       AccessControl;

    static QString BasePathWithVersion;
    static QJsonObject BaseOpenAPIObject;

    struct MasterDB{
        static inline QString makeConfig(const QString& _name){return "/MasterDB/" + _name;}
        static Common::Configuration::tmplConfigurable<QString>      Host;
        static Common::Configuration::tmplRangedConfigurable<quint16>Port;
        static Common::Configuration::tmplConfigurable<QString>      User;
        static Common::Configuration::tmplConfigurable<QString>      Pass;
        static Common::Configuration::tmplConfigurable<QString>      Schema;
    };
/*
    struct Server{
        Server(const QString& _basePath);
        Common::Configuration::tmplConfigurable<QUrl> URL;
        Common::Configuration::tmplConfigurable<QString>  Class;
        Common::Configuration::tmplConfigurable<QString>  SourceLang;
        Common::Configuration::tmplConfigurable<QString>  DestLang;
        Common::Configuration::tmplConfigurable<bool>     SupportsIXML;
        Common::Configuration::tmplConfigurable<bool>     Active;
        struct stuStatistics{
            stuStatistics(const QString& _basePath);
            Common::Configuration::tmplConfigurable<quint64> OkResponses;
            Common::Configuration::tmplConfigurable<quint64> FailedResponses;
        }Statistics;
    };
    static Common::Configuration::tmplConfigurableMultiMap<Server> TranslationServers;
*/
};

constexpr  quint16 TAPI_BASE_USER_DEFINED_TYPEID = 1026;

extern QList<intfAPIArgManipulator*> gOrderedMetaTypeInfo;
extern QList<intfAPIArgManipulator*> gUserDefinedTypesInfo;
extern TAPI::stuStatistics gServerStats;

}
}
}

#endif // TARGOMAN_API_SERVER_CONFIGS_H
