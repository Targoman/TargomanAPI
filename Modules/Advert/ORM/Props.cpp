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
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Props::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:Props:CRUD~0100"});

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

Props::Props() :
    clsTable(AdvertSchema,
              "tblProps",
              { ///<ColName             Type                    Validation                      RO   Sort  Filter Self  Virt   PK
                {"prp_binID",           S(quint32),             QFV.integer().minValue(1),        ORM_PRIMARY_KEY},
                {"prp_locID",           S(quint32),             QFV.integer().minValue(1)},
                {"prpOrder",            S(Targoman::API::enuAdvertOrder::Type)},
                {"prpKeyword",          S(QString),             QFV.unicodeAlNum().maxLenght(50)},
                {"prpStartDate",        S(QHttp::DateTime_t),   QFV},
                {"prpEndDate",          S(QHttp::DateTime_t),   QFV},
                {"prpCreatedBy_usrID",  S(quint32),             QFV.integer().minValue(1),        true},
                {"prpCreationDateTime", S(QHttp::DateTime_t),   QFV,                              true},
                {"prpUpdatedBy_usrID",  S(quint32),             QFV.integer().minValue(1)},
              },
              { ///< Col                Reference Table        ForeignCol   Rename     LeftJoin
                {"prp_binID",           "Advert.tblBin",       "binID"},
                //{"prp_locID",           "Advert.tblLocations", "locID"},
                {"prpCreatedBy_usrID",  "AAA.tblUser",         "usrID",     "Creator_", true},
                {"prpUpdatedBy_usrID",  "AAA.tblUser",         "usrID",     "Updater_", true},
              })
{
}

}
}
}
