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
TARGOMAN_ADD_EXCEPTION_HANDLER(exTargomanAPI, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop

struct ServerConfigs
{
    static inline QString makeConfig(const QString& _name){return "/Server/" + _name;}
    static Targoman::Common::Configuration::tmplConfigurable<QString>       BasePath;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       Version;
    static Targoman::Common::Configuration::tmplConfigurable<bool>          JustLocal;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint16> ListenPort;
    static Targoman::Common::Configuration::tmplConfigurable<bool>          IndentedJson;
    static Targoman::Common::Configuration::tmplRangedConfigurable<qint64>  MaxUploadSize;
    static Targoman::Common::Configuration::tmplRangedConfigurable<qint64>  MaxUploadedFileSize;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>    BaseOpenAPIObjectFile;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint8>  StatisticsInterval;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint32> MaxCachedItems;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       CacheConnector;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       AccessControl;
    static Targoman::Common::Configuration::tmplConfigurable<bool>          MultiThreaded;
    static Targoman::Common::Configuration::tmplRangedConfigurable<qint32>  APICallTimeout;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>    PublicPath;
    static Targoman::Common::Configuration::tmplConfigurable<FilePath_t>    SwaggerUI;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint32> FileMaxChunk;

    static QString BasePathWithVersion;

    struct MasterDB{
        static inline QString makeConfig(const QString& _name){return "/MasterDB/" + _name;}
        static Targoman::Common::Configuration::tmplConfigurable<QString>      Host;
        static Targoman::Common::Configuration::tmplRangedConfigurable<quint16>Port;
        static Targoman::Common::Configuration::tmplConfigurable<QString>      User;
        static Targoman::Common::Configuration::tmplConfigurable<QString>      Pass;
        static Targoman::Common::Configuration::tmplConfigurable<QString>      Schema;
    };
};

constexpr  quint16 TAPI_BASE_USER_DEFINED_TYPEID = QMetaType::User + 3;

extern TAPI::stuStatistics gServerStats;

}
}
}

#endif // TARGOMAN_API_SERVER_CONFIGS_H
