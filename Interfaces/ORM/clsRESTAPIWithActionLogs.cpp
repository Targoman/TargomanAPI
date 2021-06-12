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

#include "clsRESTAPIWithActionLogs.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Interfaces/Common/QtTypes.hpp"
#include "libTargomanDBM/clsDAC.h"

#include "Interfaces/ORM/QueryBuilders.h"
using namespace Targoman::API::ORM;

namespace Targoman {
namespace API {
namespace ORM {

QVariant clsRESTAPIWithActionLogs::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{this->Module + ":ActionLogs:CRUD~0100"});
//    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);

    ApiSelectQuery query = ApiSelectQuery(*this, GET_METHOD_CALL_ARGS);

    return query.one();
}

}
}
}
