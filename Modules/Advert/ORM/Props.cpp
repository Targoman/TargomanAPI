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

#include "Props.h"
#include "Helpers/AAA/AAA.hpp"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace QHttp;

void Props::init()
{;}

QVariant Props::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:Props:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Props::Props() :
    clsTable("Advert",
              "tblProps",
              { ///<ColName             Validation                        Sort   Filter RO   PK
                {"prp_binID",           QFV.integer().minValue(1),        true,  true, true, true},
                {"prp_locID",           QFV.integer().minValue(1)},
                {"prpOrder",            QFV_Enum(enuAdvertOrder)},
                {"prpKeyword",          QFV.unicodeAlNum().maxLenght(50)},
                {"prpStartDate",        QFV.dateTime()},
                {"prpEndDate",          QFV.dateTime()},
                {"prpCreatedBy_usrID",  QFV.integer().minValue(1),        true,  true,  true},
                {"prpCreationDateTime", QFV.dateTime(),                   true,  true,  true},
                {"prpUpdatedBy_usrID",  QFV.integer().minValue(1)},
              },
              { ///< Col                Reference Table        ForeignCol   Rename     LeftJoin
                {"prp_binID",           "Advert.tblBin",       "binID"},
                {"prp_locID",           "Advert.tblLocations", "locID"},
                {"prpCreatedBy_usrID",  "AAA.tblUser",         "usrID",     "Creator_", true},
                {"prpUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
              })
{
    this->registerMyRESTAPIs();
}

}
}
}
