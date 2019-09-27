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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "Configs.h"
#include "libTargomanCommon/Configuration/Validators.hpp"
#include "ISO639.h"

namespace Targoman{
namespace Apps {

using namespace Common;
using namespace Common::Configuration;


//Just to avoid warning on vtable
gConfigs::~gConfigs()
{;}

tmplConfigurable<QString> gConfigs::Rest::BasePath(
        gConfigs::Rest::makeConfig("BasePath"),
        "Base path to access the REST API",
        "rest",
        ReturnTrueCrossValidator(),
        "",
        "PATH",
        "base-path",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::Rest::Version(
        gConfigs::Rest::makeConfig("Version"),
        "REST API version to be appended to base path",
        "v10",
        ReturnTrueCrossValidator(),
        "",
        "VERSION",
        "version",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::Rest::JustLocal(
        gConfigs::Rest::makeConfig("JustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "just-local",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::Rest::ListenPort(
        gConfigs::Rest::makeConfig("ListenPort"),
        "Listen port for main REST server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "p",
        "PORT",
        "listen-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint8> gConfigs::Rest::StatisticsInterval(
        gConfigs::Rest::makeConfig("StatisticsInterval"),
        "Listen port for main REST server",
        1,60,
        10,
        ReturnTrueCrossValidator(),
        "",
        "INTERVAL",
        "stats-interval",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::Rest::IndentedJson(
        gConfigs::Rest::makeConfig("IndentedJson"),
        "If set to true Json outputs will be indented",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "indented-json",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<qint64> gConfigs::Rest::MaxUploadSize(
        gConfigs::Rest::makeConfig("MaxUploadSize"),
        "Max Upload size for any type of request",
        10*1024,10*1024*1024,
        10*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-upload-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<qint64> gConfigs::Rest::MaxUploadedFileSize(
        gConfigs::Rest::makeConfig("MaxUploadedFileSize"),
        "Max Upload size for files",
        1*1024,100*1024*1024,
        100*1024*1024,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-file-upload-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint32> gConfigs::Rest::MaxCachedItems(
        gConfigs::Rest::makeConfig("MaxCachedItems"),
        "Maximum API calls to be cached",
        0,65000,
        5000,
        ReturnTrueCrossValidator(),
        "",
        "SIZE",
        "max-cache-size",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::Rest::CacheConnector(
        gConfigs::Rest::makeConfig("CacheConnector"),
        "Connection string to connect to Redis cache server",
        "",
        ReturnTrueCrossValidator(),
        "",
        "CONNECTION_STRING",
        "cache-connector",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<FilePath_t>     gConfigs::Rest::BaseOpenAPIObjectFile(
        gConfigs::Rest::makeConfig("BaseOpenAPIObjectFile"),
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
tmplConfigurable<QString> gConfigs::JWT::Secret(
        gConfigs::JWT::makeConfig("JWTSecret"),
        "Secret to be used for hashing JWT",
        "~5KHeTc7.C^Ln%<X~YnE4<Kr",
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-secret",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QHttp::enuJWTHashAlgs::Type> gConfigs::JWT::HashAlgorithm(
        gConfigs::JWT::makeConfig("JWTHashAlgorithm"),
        "Hash algorithm toe be used for JWT",
        QHttp::enuJWTHashAlgs::HS256,
        ReturnTrueCrossValidator(),
        "",
        "ALGORITHM",
        "jwt-hash-alg",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<quint64> gConfigs::JWT::SimpleCryptKey(
        gConfigs::JWT::makeConfig("SimpleCryptKey"),
        "Secret to be used for encrypting private JWT objects",
        static_cast<quint64>(43121109170974191),
        ReturnTrueCrossValidator(),
        "",
        "SECRET",
        "jwt-private-key",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));


/****************************************************************************/
tmplConfigurable<double>     gConfigs::Classifier::Threshold(
        gConfigs::Classifier::makeConfig("Threshold"),
        "Threshold to be used by classifier",
        0.8,
        ReturnTrueCrossValidator(),
        "", "", "fasttext-threshold",
        static_cast<enuConfigSource::Type>(
            enuConfigSource::Arg  |
            enuConfigSource::File));

tmplConfigurable<FilePath_t>     gConfigs::Classifier::ModelPath(
        gConfigs::Classifier::makeConfig("ModelPath"),
        "Directory where FastText models are stored.",
        "",
        Validators::tmplPathAccessValidator<
        static_cast<enuPathAccess::Type>(enuPathAccess::Dir | enuPathAccess::Readable),
        false>,
        "", "", "fasttext-models-path",
        static_cast<enuConfigSource::Type>(
            enuConfigSource::Arg  |
            enuConfigSource::File));

tmplConfigurable<QString> gConfigs::Classifier::ModelPattern(
        gConfigs::Classifier::makeConfig("ModelPattern"),
        "File pattern for FastText models must have <%LANG%> pattern as language placeholder",
        "fasttext-%LANG%.bin",
        ReturnTrueCrossValidator(),
        "",
        "",
        "fasttext-model-pattrn",
        static_cast<enuConfigSource::Type>(
            enuConfigSource::Arg  |
            enuConfigSource::File));

tmplConfigurable<bool> gConfigs::Classifier::SupportsIXML(
        gConfigs::Classifier::makeConfig("SupportsIXML"),
        "Wheter the classifier was trained with IXML markups or not",
        true,
        ReturnTrueCrossValidator(),
        "", "", "fasttext-supports-ixml",
        static_cast<enuConfigSource::Type>(
            enuConfigSource::Arg  |
            enuConfigSource::File));

/****************************************************************************/
tmplConfigurable<FilePath_t> gConfigs::TextProcessor::AbbreviationFile(
        gConfigs::TextProcessor::makeConfig("AbbreviationFile"),
        "File path to abbreviations table file. Relative to config file path if not specified as absolute.",
        "",
        Validators::tmplPathAccessValidator<
        static_cast<enuPathAccess::Type>(enuPathAccess::File | enuPathAccess::Readable),
        true>
        );
tmplConfigurable<FilePath_t> gConfigs::TextProcessor::NormalizationFile(
        gConfigs::TextProcessor::makeConfig("NormalizationFile"),
        "File path to normalization rules file. Relative to config file path if not specified as absolute.",
        "",
        Validators::tmplPathAccessValidator<
        static_cast<enuPathAccess::Type>(enuPathAccess::File | enuPathAccess::Readable),
        true>
        );
tmplConfigurable<FilePath_t> gConfigs::TextProcessor::SpellCorrectorBaseConfigPath(
        gConfigs::TextProcessor::makeConfig("SpellCorrectorBaseConfigPath"),
        "Directory path to spell correction specific files. Relative to config file path if not specified as absolute.",
        "",
        Validators::tmplPathAccessValidator<
        static_cast<enuPathAccess::Type>(enuPathAccess::Dir | enuPathAccess::Readable),
        false>
        );
clsFileBasedConfig       gConfigs::TextProcessor::SpellCorrectorLanguageBasedConfigs(
        gConfigs::TextProcessor::makeConfig("SpellCorrectorLanguageBasedConfigs"),
        "Specific configurations for each language. See TargomanTextProcessor documents for more info."
        );

/****************************************************************************/
tmplConfigurable<QString> gConfigs::DB::Host(
        gConfigs::DB::makeConfig("Host"),
        "Database Host address",
        "127.0.0.1",
        ReturnTrueCrossValidator(),
        "",
        "HOST",
        "db-host",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::DB::Port(
        gConfigs::DB::makeConfig("Port"),
        "Database port",
        10,65000,
        3306,
        ReturnTrueCrossValidator(),
        "",
        "PORT",
        "db-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DB::User(
        gConfigs::DB::makeConfig("User"),
        "Database username for connection",
        "root",
        ReturnTrueCrossValidator(),
        "",
        "USER",
        "db-user",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DB::Pass(
        gConfigs::DB::makeConfig("Pass"),
        "Database password",
        "",
        ReturnTrueCrossValidator(),
        "",
        "PASS",
        "db-pass",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<QString> gConfigs::DB::Schema(
        gConfigs::DB::makeConfig("Schema"),
        "Database schema",
        "MT",
        ReturnTrueCrossValidator(),
        "",
        "SCHEMA",
        "db-schema",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

/****************************************************************************/
static auto langValidator = [](const intfConfigurable& _item, QString& _errorMessage) -> bool{
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

/****************************************************************************/
#ifdef QHTTP_ENABLE_WEBSOCKET
tmplConfigurable<QString> gConfigs::WS::Name(
        gConfigs::WS::makeConfig("Name"),
        "If set then a socket server with specified name will be started",
        "",
        ReturnTrueCrossValidator(),
        "",
        "NAME",
        "ws-name",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplRangedConfigurable<quint16> gConfigs::WS::Port(
        gConfigs::WS::makeConfig("Port"),
        "Listen port for WebSocket server",
        1000,65000,
        10000,
        ReturnTrueCrossValidator(),
        "w",
        "PORT",
        "ws-listen-port",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::WS::JustLocal(
        gConfigs::WS::makeConfig("JustLocal"),
        "If set to true it will just listen to local connections",
        false,
        ReturnTrueCrossValidator(),
        "",
        "",
        "ws-just-local",
        static_cast<enuConfigSource::Type>(enuConfigSource::Arg | enuConfigSource::File));

tmplConfigurable<bool> gConfigs::WS::Secure(
        gConfigs::WS::makeConfig("Secure"),
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
