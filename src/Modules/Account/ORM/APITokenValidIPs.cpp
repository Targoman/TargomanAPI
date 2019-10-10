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

#include "APITokenValidIPs.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void APITokenValidIPs::init()
{

}

QVariant APITokenValidIPs::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::hasPriv(_JWT,{"Account:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

APITokenValidIPs::APITokenValidIPs() :
    intfTable("AAA",
              "tblAPITokenValidIPs",
              "tvi",
              { ///<ColName            Validation                           Sort   Filter AS  RO   PK
                {"tviID",               QFV.integer().minValue(1),          true,  true, "", true,  true},
                {"tvi_aptID",           QFV.integer().minValue(1),          true,  true, "", false, true},
                {"tviIP",               QFV.integer().minValue(1),          true,  true, "", false, true},
                {"tviIPReadable",       QFV.allwaysInvalid(),               false,false, "", true},
                {"tviCreatedBy_usrID",  QFV.integer().minValue(1),          true,  true, "", true},
                {"tviCreationDateTime", QFV.dateTime(),                     true,  true, "", true},
                {"tviUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"tviStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuGenericStatus::options().join("|"))))},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"tvi_aptID",          "AAA.tblAPIToken",  "aptID"},
                {"tviCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"tviUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
    this->registerMyRESTAPIs();
}

