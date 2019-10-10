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

#include "BlockingRule.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void BlockingRule::init()
{

}

QVariant BlockingRule::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

BlockingRule::BlockingRule() :
    intfTable("AAA",
              "tblBlockingRules",
              "blr",
              { ///<ColName            Validation                           Sort   Filter AS  RO   PK
                {"blrID",               QFV.integer().minValue(1),          true,  true, "", true, true},
                {"blr_ipbIP",           QFV.integer().minValue(1),          true,  true, "", false, true},
                {"blr_ipIPReadable",    QFV.allwaysInvalid(),               false,false, "", true},
                {"blrStartingTime",     QFV.dateTime(),                     true,  true},
                {"blrEndingTime",       QFV.dateTime(),                     true,  true},
                {"blrCause",            QFV.allwaysValid(),                 false,false},
                {"blrCreatedBy_usrID",  QFV.integer().minValue(1),          true,  true, "", true},
                {"blrCreationDateTime", QFV.dateTime(),                     true,  true, "", true},
                {"blrUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"blrStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuGenericStatus::options().join("|"))))},
              },
              { ///< Col       Reference Table             ForeignCol   Rename     LeftJoin
                {"blrCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"blrUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}
