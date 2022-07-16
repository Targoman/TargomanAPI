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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "ServerCommon.h"
#include "libTargomanCommon/Configuration/Validators.hpp"

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman::API::Server {

QString ServerCommonConfigs::BasePathWithVersion;

tmplConfigurable<QString> ServerCommonConfigs::BasePath(
    ServerCommonConfigs::makeConfig("BasePath"),
    "Base path to access the REST API",
    "rest",
    ReturnTrueCrossValidator(),
    "",
    "PATH",
    "base-path",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerCommonConfigs::Version(
    ServerCommonConfigs::makeConfig("Version"),
    "REST API version to be appended to base path",
    "v1",
    ReturnTrueCrossValidator(),
    "",
    "VERSION",
    "version",
    enuConfigSource::Arg | enuConfigSource::File);
tmplConfigurable<QString> ServerCommonConfigs::ContactEmail(
    ServerCommonConfigs::makeConfig("ContactEmail"),
    "Contact Email",
    "info@sample.dom",
    ReturnTrueCrossValidator(),
    "",
    "EMAIL",
    "contact-email",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> ServerCommonConfigs::JustLocal(
    ServerCommonConfigs::makeConfig("JustLocal"),
    "If set to true it will just listen to local connections",
    false,
    ReturnTrueCrossValidator(),
    "",
    "",
    "just-local",
    enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint8> ServerCommonConfigs::StatisticsInterval(
    ServerCommonConfigs::makeConfig("StatisticsInterval"),
    "Listen port for main REST server",
    1,60,
    10,
    ReturnTrueCrossValidator(),
    "",
    "INTERVAL",
    "stats-interval",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> ServerCommonConfigs::IndentedJson(
    ServerCommonConfigs::makeConfig("IndentedJson"),
    "If set to true Json outputs will be indented",
    false,
    ReturnTrueCrossValidator(),
    "",
    "",
    "indented-json",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<bool> ServerCommonConfigs::MultiThreaded(
    ServerCommonConfigs::makeConfig("MultiThreaded"),
    "If set to false then application will run in single-threaded mode and will be blocked on time-consuming API calls",
    true,
    ReturnTrueCrossValidator(),
    "",
    "",
    "",
    enuConfigSource::File);

tmplRangedConfigurable<qint32> ServerCommonConfigs::APICallTimeout(
    ServerCommonConfigs::makeConfig("APICallTimeout"),
    "default timeout for API Calls. It can be reconfigured on each API Call. set to -1 for infinite",
    -1, 60 * 1000,
    -1,
    ReturnTrueCrossValidator(),
    "",
    "MILISECONDS",
    "default-api-timeout",
    enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> ServerCommonConfigs::MaxUploadSize(
    ServerCommonConfigs::makeConfig("MaxUploadSize"),
    "Max Upload size for any type of request",
    10*1024, 10*1024*1024,
    10*1024*1024,
    ReturnTrueCrossValidator(),
    "",
    "SIZE",
    "max-upload-size",
    enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint32> ServerCommonConfigs::FileMaxChunk(
    ServerCommonConfigs::makeConfig("FileMaxChunk"),
    "Max file size to send on each chunk.",
    10*1024, 10*1024*1024,
    10*1024,
    ReturnTrueCrossValidator(),
    "",
    "SIZE",
    "chunk-size",
    enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<qint64> ServerCommonConfigs::MaxUploadedFileSize(
    ServerCommonConfigs::makeConfig("MaxUploadedFileSize"),
    "Max Upload size for files",
    1*1024, 100*1024*1024,
    100*1024*1024,
    ReturnTrueCrossValidator(),
    "",
    "SIZE",
    "max-file-upload-size",
    enuConfigSource::Arg | enuConfigSource::File);

tmplRangedConfigurable<quint32> ServerCommonConfigs::MaxCachedItems(
    ServerCommonConfigs::makeConfig("MaxCachedItems"),
    "Maximum API calls to be cached",
    0,65000,
    5000,
    ReturnTrueCrossValidator(),
    "",
    "SIZE",
    "max-cache-size",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerCommonConfigs::CacheConnector(
    ServerCommonConfigs::makeConfig("CacheConnector"),
    "Connection string to connect to Redis cache server",
    "",
    ReturnTrueCrossValidator(),
    "",
    "CONNECTION_STRING",
    "cache-connector",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<QString> ServerCommonConfigs::AccessControl(
    ServerCommonConfigs::makeConfig("AccessControl"),
    "Default access control",
    "*",
    ReturnTrueCrossValidator(),
    "",
    "ACCESS-CONTROL-STRING",
    "",
    enuConfigSource::File);

tmplConfigurable<FilePath_t> ServerCommonConfigs::BaseOpenAPIObjectFile(
    ServerCommonConfigs::makeConfig("BaseOpenAPIObjectFile"),
    "Input file path where default OpenAPI object file resides",
    "",
    Validators::tmplPathAccessValidator<
    TARGOMAN_PATH_ACCESS(enuPathAccess::File | enuPathAccess::Readable),
    false>,
    "",
    "FILEPATH",
    "openapi-file",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t> ServerCommonConfigs::SwaggerUI(
    ServerCommonConfigs::makeConfig("SwaggerUI"),
    "Directory where swaggerUI files reside",
    "",
    Validators::tmplPathAccessValidator<
    TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
    false>,
    "",
    "DIRECTORY",
    "swaggerui-path",
    enuConfigSource::Arg | enuConfigSource::File);

tmplConfigurable<FilePath_t> ServerCommonConfigs::PublicPath(
    ServerCommonConfigs::makeConfig("PublicFolder"),
    "Directory static files are stored",
    "",
    Validators::tmplPathAccessValidator<
        TARGOMAN_PATH_ACCESS(enuPathAccess::Dir | enuPathAccess::Readable),
        false
    >,
    "",
    "DIRECTORY",
    "public-path",
    enuConfigSource::Arg | enuConfigSource::File);

/****************************************************************************/
tmplConfigurable<QString> ServerCommonConfigs::InstanceID(
    ServerCommonConfigs::makeConfig("InstanceID"),
    "Server Instance ID",
    "",
    ReturnTrueCrossValidator(),
    "",
    "UUID",
    "instance-id",
    enuConfigSource::Arg
);

tmplRangedConfigurable<quint16> ServerCommonConfigs::ListenPort(
    ServerCommonConfigs::makeConfig("ListenPort"),
    "Listen port for main REST server",
    1000,65000,
    10000,
    ReturnTrueCrossValidator(),
    "p",
    "PORT",
    "listen-port",
    enuConfigSource::Arg | enuConfigSource::File
);

tmplConfigurable<QString> ServerCommonConfigs::DBPrefix(
    ServerCommonConfigs::makeConfig("DBPrefix"),
    "Prefix to prepend to all database schemas",
    "",
    ReturnTrueCrossValidator(),
    "",
    "PREFIX",
    "dbprefix",
    enuConfigSource::Arg | enuConfigSource::File
);

const QString PrependSchema(const QString &_schema) {
    return ServerCommonConfigs::DBPrefix.value() + _schema;
}

} //namespace Targoman::API::Server
