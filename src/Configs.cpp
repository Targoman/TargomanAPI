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

#include "Configs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"


namespace Targoman{
namespace Apps {

using namespace Common;
using namespace Common::Configuration;


//Just to avoid warning on vtable
gConfigs::~gConfigs()
{;}

tmplConfigurable<QString> gConfigs::BasePath(
        gConfigs::appConfig("BasePath"),
        "Base path to access the REST API",
        "rest",
        ReturnTrueCrossValidator(),
        "",
        "PATH",
        "base-path",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::Version(
        gConfigs::appConfig("Version"),
        "REST API version to be appended to base path",
        "v10",
        ReturnTrueCrossValidator(),
        "",
        "VERSION",
        "version",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::JustLocal(
        gConfigs::appConfig("JustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "just-local",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::ListenPort(
        gConfigs::appConfig("ListenPort"),
        "Listen port for main REST server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "p",
        "PORT",
        "listen-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint8> gConfigs::StatisticsInterval(
        gConfigs::appConfig("StatisticsInterval"),
        "Listen port for main REST server",
        1,60,
        10,
        ReturnTrueCrossValidator(),
        "",
        "INTERVAL",
        "stats-interval",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::JWTSecret(
        gConfigs::appConfig("JWTSecret"),
        "Secret to be used for hashing JWT",
        "~5KHeTc7.C^Ln%<X~YnE4<Kr",
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-secret",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QHttp::enuJWTHashAlgs::Type> gConfigs::JWTHashAlgorithm(
        gConfigs::appConfig("JWTHashAlgorithm"),
        "Hash algorithm toe be used for JWT",
        QHttp::enuJWTHashAlgs::HS256,
        ReturnTrueCrossValidator(),
        "",
        "ALGORITHM",
        "jwt-hash-alg",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<quint64> gConfigs::SimpleCryptKey(
        gConfigs::appConfig("SimpleCryptKey"),
        "Secret to be used for encrypting private JWT objects",
        static_cast<quint64>(43121109170974191),
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-private-key",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::IndentedJson(
        gConfigs::appConfig("IndentedJson"),
        "If set to true Json outputs will be indented",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "indented-json",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<qint64> gConfigs::MaxUploadSize(
        gConfigs::appConfig("MaxUploadSize"),
        "Max Upload size for any type of request",
        10*1024,10*1024*1024,
        10*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-upload-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<qint64> gConfigs::MaxUploadedFileSize(
        gConfigs::appConfig("MaxUploadedFileSize"),
        "Max Upload size for files",
        1*1024,100*1024*1024,
        100*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-file-upload-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint32> gConfigs::MaxCachedItems(
        gConfigs::appConfig("MaxCachedItems"),
        "Maximum API calls to be cached",
        0,65000,
        5000,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-cache-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::CacheConnector(
        gConfigs::appConfig("CacheConnector"),
        "Connection string to connect to Redis cache server",
        "",
        ReturnTrueCrossValidator(),
        "",
        "CONNECTION_STRING",
        "cache-connector",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<FilePath_t>     gConfigs::BaseOpenAPIObjectFile(
        gConfigs::appConfig("BaseOpenAPIObjectFile"),
        "Input file path where default OpenAPI object file resides",
        "",
        Validators::tmplPathAccessValidator<
        static_cast<enuPathAccess::Type>(enuPathAccess::File | enuPathAccess::Readable),
        false>,
        "",
        "FILEPATH",
        "openapi-file",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

/****************************************************************************/
tmplConfigurable<QString> gConfigs::DBHost(
        gConfigs::appConfig("DBHost"),
        "Database Host address",
        "127.0.0.1",
        ReturnTrueCrossValidator(),
        "",
        "HOST",
        "db-host",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::DBPort(
        gConfigs::appConfig("DBPort"),
        "Database port",
        10,65000,
        3306,
        ReturnTrueCrossValidator(),
        "",
        "PORT",
        "db-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DBUser(
        gConfigs::appConfig("DBUser"),
        "Database username for connection",
        "root",
        ReturnTrueCrossValidator(),
        "",
        "USER",
        "db-user",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DBPass(
        gConfigs::appConfig("DBPass"),
        "Database password",
        "",
        ReturnTrueCrossValidator(),
        "",
        "PASS",
        "db-pass",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DBSchema(
        gConfigs::appConfig("DBSchema"),
        "Database schema",
        "Targoman",
        ReturnTrueCrossValidator(),
        "",
        "SCHEMA",
        "db-schema",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

/****************************************************************************/
#ifdef QHTTP_ENABLE_WEBSOCKET
tmplConfigurable<QString> gConfigs::WebSocketServerName(
        gConfigs::appConfig("WebSocketServerName"),
        "If set then a socket server with specified name will be started",
        "",
        ReturnTrueCrossValidator(),
        "",
        "NAME",
        "ws-name",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::WebSocketServerPort(
        gConfigs::appConfig("ListenPort"),
        "Listen port for WebSocket server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "w",
        "PORT",
        "ws-listen-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::WebSocketServerJustLocal(
        gConfigs::appConfig("WebSocketServerJustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "ws-just-local",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::WebSocketServerSecure(
        gConfigs::appConfig("WebSocketServerJustLocal"),
        "Start web socket server in secure mode",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "ws-secure",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));
#endif
}
}

ENUM_CONFIGURABLE_IMPL(QHttp::enuJWTHashAlgs)
