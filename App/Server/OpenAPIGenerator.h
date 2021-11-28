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
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#ifndef TARGOMAN_API_SERVER_OPENAPIGENERATOR_H
#define TARGOMAN_API_SERVER_OPENAPIGENERATOR_H

#include "RESTAPIRegistry.h"

namespace Targoman::API::Server {

class OpenAPIGenerator
{
public:
    static QJsonObject retrieveJson(const QString& _host = "127.0.0.1", const quint16 _port = 80);

private:
    static QJsonObject CachedJson;
};

} //namespace Targoman::API::Server

#endif // TARGOMAN_API_SERVER_OPENAPIGENERATOR_H
