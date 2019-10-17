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

#include "ActionLog.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void ActionLog::init()
{;}

QVariant ActionLog::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:ActionLog:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

ActionLog::ActionLog() :
    intfTable("AAA",
              "tblActionLog",
              "atl",
              { ///<ColName             Validation                      Sort   Filter AS  RO   PK
                {"atlID",               QFV.integer().minValue(1),      true,  true, "", true, true},
                {"atlBy_usrID",         QFV.integer().minValue(1),      true,  true, "", true, true},
                {"atlInsertionDateTime",QFV.dateTime(),                 true,  true, "", true},
                {"atlType",             QFV.asciiAlNum().maxLenght(50), true,  true, "", true},
                {"atlType",             QFV.allwaysInvalid(),           false,false, "", true},
              },
              { ///< Col                Reference Table    ForeignCol   Rename     LeftJoin
                {"atlBy_usrID",        "AAA.tblUser",      "usrID",     "By"},
              })
{
    this->registerMyRESTAPIs();
}
