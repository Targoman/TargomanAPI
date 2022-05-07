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

using namespace Targoman::Common;
using namespace Targoman::Common::Configuration;

namespace Targoman::API::Server {

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
