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

#include "AdvBin.h"
#include "Helpers/AAA/AAA.hpp"

using namespace Targoman;
using namespace Targoman::API;
using namespace QHttp;

void AdvBin::init()
{;}

QVariant AdvBin::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advertisement:AdvBin:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

AdvBin::AdvBin() :
    intfTable("Advertisement",
              "tblAdvBin",
              { ///<ColName             Validation                      Sort   Filter RO   PK
                {"adbID",               QFV.integer().minValue(1),      true,  true,  true, true},
                {"adbCreatedBy_usrID",  QFV.integer().minValue(1),      true,  true,  true, true},
                {"advType",             QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuAdvType::options().join("|"))))},
                {"adbTitle",            QFV.unicodeAlNum(true, ":،")},
                {"adbDesc",             QFV.unicodeAlNum(true, ":،")},
                {"adbPrettyURL",        QFV.url()},
                {"adbURL",              QFV.url()},
                {"adbShown",            QFV.integer(),                  true,  true,  true},
                {"adbClicks",           QFV.integer(),                  true,  true,  true},
                {"adbCreationDateTime", QFV.dateTime(),                 true,  true,  true},
                {"adbUpdatedBy_usrID",  QFV.integer().minValue(1)},
                {"adbStatus",           QFV.matches(QRegularExpression(QString("^[%1]$").arg(enuPendableStatus::options().join("|"))))},
              },
              { ///< Col             Reference Table                 ForeignCol   Rename     LeftJoin
                {"adv_adbID",        "Advertisement.tblAdvBin",      "adbID"},
                {"adv_ordID",        "Advertisement.tblAdvOrders",   "ordID"},
              })
{
    this->registerMyRESTAPIs();
}
