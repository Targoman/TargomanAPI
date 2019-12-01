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

#include "ActiveAds.h"
#include "Helpers/AAA/AAA.hpp"
#include "Defs.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace QHttp;

void ActiveAds::init()
{;}

QVariant ActiveAds::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:ActiveAds:CRUD~0100"});
    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

ActiveAds::ActiveAds() :
    clsTable("Advert",
              "tblActiveAds",
              { ///<ColName      Type           Validation                      RO   Sort  Filter  PK
                {"act_binID",    T(quint32),    QFV.integer().minValue(1),      true,  true, true, true},
                {"act_locID",    T(quint32),    QFV.integer().minValue(1),      true,  true, true, true},
                {"actOrder",     T(Targoman::API::enuAdvertOrder::Type)},
                {"actOnKeyword", T(QString),    QFV.unicodeAlNum().maxLenght(50)},
              },
              { ///< Col             Reference Table            ForeignCol   Rename     LeftJoin
                {"act_binID",        "Advert.tblBin",           "binID"},
                {"act_locID",        "Advert.tblLocations",     "locID"},
              })
{
    this->registerMyRESTAPIs();
}

}
}
}
