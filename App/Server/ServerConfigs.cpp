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

#include "ServerConfigs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "3rdParty/ISO639/ISO639.h"

namespace Targoman {
namespace API {
namespace Server {

using namespace Common;
using namespace Common::Configuration;

tmplConfigurable<QString> ServerConfigs::BasePath(
        ServerConfigs::makeConfig("BasePath"),
        "Base path to access the REST API",
        "rest",
        ReturnTrueCrossValidator(),
        "",
        "PATH",
        "base-path",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerConfigs::Version(
        ServerConfigs::makeConfig("Version"),
        "REST API version to be appended to base path",
        "v1",
        ReturnTrueCrossValidator(),
        "",
        "VERSION",
        "version",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> ServerConfigs::JustLocal(
        ServerConfigs::makeConfig("JustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "just-local",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint16> ServerConfigs::ListenPort(
        ServerConfigs::makeConfig("ListenPort"),
        "Listen port for main REST server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "p",
        "PORT",
        "listen-port",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint8> ServerConfigs::StatisticsInterval(
        ServerConfigs::makeConfig("StatisticsInterval"),
        "Listen port for main REST server",
        1,60,
        10,
        ReturnTrueCrossValidator(),
        "",
        "INTERVAL",
        "stats-interval",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> ServerConfigs::IndentedJson(
        ServerConfigs::makeConfig("IndentedJson"),
        "If set to true Json outputs will be indented",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "indented-json",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> ServerConfigs::MaxUploadSize(
        ServerConfigs::makeConfig("MaxUploadSize"),
        "Max Upload size for any type of request",
        10*1024, 10*1024*1024,
        10*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-upload-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> ServerConfigs::MaxUploadedFileSize(
        ServerConfigs::makeConfig("MaxUploadedFileSize"),
        "Max Upload size for files",
        1*1024, 100*1024*1024,
        100*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-file-upload-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint32> ServerConfigs::MaxCachedItems(
        ServerConfigs::makeConfig("MaxCachedItems"),
        "Maximum API calls to be cached",
        0,65000,
        5000,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-cache-size",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerConfigs::CacheConnector(
        ServerConfigs::makeConfig("CacheConnector"),
        "Connection string to connect to Redis cache server",
        "",
        ReturnTrueCrossValidator(),
        "",
        "CONNECTION_STRING",
        "cache-connector",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerConfigs::AccessControl(
        ServerConfigs::makeConfig("AccessControl"),
        "Default access control",
        "*",
        ReturnTrueCrossValidator(),
        "",
        "ACCESS-CONTROL-STRING",
        "",
        enuConfigSource::File);

tmplConfigurable<FilePath_t>     ServerConfigs::BaseOpenAPIObjectFile(
        ServerConfigs::makeConfig("BaseOpenAPIObjectFile"),
        "Input file path where default OpenAPI object file resides",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Readable),
        false>,
        "",
        "FILEPATH",
        "openapi-file",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t>     ServerConfigs::ModulesPath(
        ServerConfigs::makeConfig("ModulesPath"),
        "Directory where API modules are stored",
        "",
        Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
        false>,
        "m",
        "DIRECTORY",
        "modules-path",
        enuConfigSource::Arg | enuConfigSource::File);

QString ServerConfigs::BasePathWithVersion;
QJsonObject ServerConfigs::BaseOpenAPIObject;
QList<intfAPIArgManipulator*> gOrderedMetaTypeInfo;
QList<intfAPIArgManipulator*> gUserDefinedTypesInfo;
TAPI::stuStatistics gServerStats;

/****************************************************************************/
/*tmplConfigurable<QString> gConfigs::DB::Host(
        gConfigs::DB::makeConfig("Host"),
        "Database Host address",
        "127.0.0.1",
        ReturnTrueCrossValidator(),
        "",
        "HOST",
        "db-host",
        enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint16> gConfigs::DB::Port(
        gConfigs::DB::makeConfig("Port"),
        "Database port",
        10,65000,
        3306,
        ReturnTrueCrossValidator(),
        "",
        "PORT",
        "db-port",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> gConfigs::DB::User(
        gConfigs::DB::makeConfig("User"),
        "Database username for connection",
        "root",
        ReturnTrueCrossValidator(),
        "",
        "USER",
        "db-user",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> gConfigs::DB::Pass(
        gConfigs::DB::makeConfig("Pass"),
        "Database password",
        "",
        ReturnTrueCrossValidator(),
        "",
        "PASS",
        "db-pass",
        enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> gConfigs::DB::Schema(
        gConfigs::DB::makeConfig("Schema"),
        "Database schema",
        "MT",
        ReturnTrueCrossValidator(),
        "",
        "SCHEMA",
        "db-schema",
        enuConfigSource::Arg | enuConfigSource::File);

/****************************************************************************/
/*static auto langValidator = [](const intfConfigurable& _item, QString& _errorMessage) -> bool{
    if(ISO639isValid(_item.toVariant().toString().toLatin1().constData()))
        return true;
    _errorMessage = QString("Invalid language code <%1> on <%2>").arg(
                _item.toVariant().toString()).arg(
                _item.configPath()
                );
    return false;
};

gConfigs::Server::Server(const QString& _basePath) :
    URL(_basePath + "URL",
         "URL to connect to server",
         "http://127.0.0.1:5000",
         ReturnTrueCrossValidator(),
         "","","",
         enuConfigSource::File
         ),
    Class(_basePath + "Class",
         "Target class that this server will translate (formal|informal)",
         "",
         ReturnTrueCrossValidator(),
         "","","",
         enuConfigSource::File
         ),
    SourceLang(_basePath + "SourceLang",
         "Source language to translate",
         "",
         langValidator,
         "","","",
         enuConfigSource::File
         ),
    DestLang(_basePath + "DestLang",
         "Target language to translate",
         "",
         langValidator,
         "","","",
         enuConfigSource::File
         ),
    SupportsIXML(_basePath + "SupportsIXML",
         "Wheter this engine supports IXML or not",
         true,
         ReturnTrueCrossValidator(),
         "","","",
         enuConfigSource::File
         ),
    Active(_basePath + "Active",
         "Host or IP Address of Server",
         true,
         ReturnTrueCrossValidator(),
         "","","",
         enuConfigSource::File
         ),
    Statistics(gConfigs::Server::stuStatistics(_basePath))
{}

gConfigs::Server::stuStatistics::stuStatistics(const QString& _basePath) :
    OkResponses(_basePath + "Stats/OkResponses",
                    "Count of OK Responses",
                    0,
                    ReturnTrueCrossValidator(),
                    "","","",
                    enuConfigSource::ReadOnly
                    ),
    FailedResponses(_basePath + "Stats/Load15Min",
                     "Count of Failed Responses",
                     0,
                     ReturnTrueCrossValidator(),
                     "","","",
                     enuConfigSource::ReadOnly
                     )
  {}

tmplConfigurableMultiMap<gConfigs::Server> gConfigs::TranslationServers(
        clsConfigPath("TranslationServers"),
        "List of valid translation servers to connect to them separated by their translation engine");
*/

}
}
}

