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

struct ServerCommonConfigs
{
    static inline QString makeConfig(const QString& _name) {return "/Server/" + _name;}

    static Targoman::Common::Configuration::tmplConfigurable<QString> InstanceID;
    static Targoman::Common::Configuration::tmplRangedConfigurable<quint16> ListenPort;
    static Targoman::Common::Configuration::tmplConfigurable<QString> DBPrefix;
};

const QString PrependSchema(const QString &_schema);

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVERCOMMON_H
