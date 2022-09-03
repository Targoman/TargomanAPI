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

#ifndef TARGOMAN_API_SERVERCOMMON_H
#define TARGOMAN_API_SERVERCOMMON_H

#include "libTargomanCommon/Configuration/tmplConfigurable.h"

namespace Targoman::API::Server {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
TARGOMAN_ADD_EXCEPTION_HANDLER(exRESTRegistry, Targoman::Common::exTargomanBase);
#pragma clang diagnostic pop

struct stuModuleDBInfo {
    QString Host;
    quint16 Port;
    QString User;
    QString Pass;
    QString Schema;

    stuModuleDBInfo(QString _schema = "", quint16 _port = 0, QString _host = "", QString _user = "", QString _pass = "");
    QString toConnStr(const QString &_dbPrefix="", bool _noSchema=false);
};

struct ServerCommonConfigs
{
    static inline QString makeConfig(const QString& _name) { return "/Server/" + _name; }

    static Targoman::Common::Configuration::tmplConfigurable<QString>       BasePath;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       Version;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       ContactEmail;
    static Targoman::Common::Configuration::tmplConfigurable<bool>          JustLocal;
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

    static Targoman::Common::Configuration::tmplConfigurable<QString>       InstanceID;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint16> ListenPort;
    static Targoman::Common::Configuration::tmplConfigurable<QString>       DBPrefix;
};

const QString PrependSchema(const QString &_schema);

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVERCOMMON_H
