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

#include "IPBin.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void IPBin::init()
{;}

QVariant IPBin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Account:IPBin:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

IPBin::IPBin() :
    intfTable("AAA",
              "tblIPBin",
              { ///<ColName             Validation                 Sort   Filter RO    PK
                {"ipbIP",               QFV.integer().minValue(1), true,  true, true, true},
                {"ipbReadable",         QFV.unicodeAlNum().maxLenght(50)},
                {"ipbFirstAccess",      QFV.dateTime(),            true,  true, true},
                {"ipbAccessCount",      QFV.integer().minValue(0), true,  true, true},
                {"ipbLastAccess",       QFV.dateTime(),            true,  true, true},
                {"ipbBlockingTime",     QFV.allwaysInvalid(),      false, false, true},
                {"ipbBlockedBy_usrID",  QFV.integer().minValue(1)},
                {"ipbStatus", QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuIPBinStatus::options().join("|"))))},
              },
              { ///< Col               Reference Table     ForeignCol Rename      LeftJoin
                {"ipbBlockedBy_usrID", "AAA.tblUser",      "usrID",   "Blocker_", true},
              })
{
    QHTTP_REGISTER_TARGOMAN_ENUM(Targoman::API::enuIPBinStatus);
    this->registerMyRESTAPIs();
}


