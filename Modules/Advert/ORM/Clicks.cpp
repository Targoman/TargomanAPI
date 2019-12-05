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

#include "Clicks.h"
#include "Helpers/AAA/AAA.hpp"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace QHttp;

void Clicks::init()
{;}

QVariant Clicks::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT,{"Advert:Clicks:CRUD~0100"});

    return this->selectFromTable(AAADACInstance(), {}, {}, GET_METHOD_CALL_ARGS);
}

Clicks::Clicks() :
    clsTable("Advert",
              "tblClicks",
              { ///<ColName             Type                    Validation                      RO   Sort  Filter Self  Virt   PK
                {"clkID",               S(quint64),             QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"clk_binID",           S(quint32),             QFV.integer().minValue(1),      true, true, true},
                {"clk_locID",           S(quint32),             QFV.integer().minValue(1),      true, true, true},
                {"clkDateTime",         S(QHttp::DateTime_t),   QFV,                            true},
                {"clkIP",               S(quint32),             QFV.ipv4(),                     true},
                {"clkDevice",           S(quint32),             QFV.asciiAlNum().maxLenght(50), true},
                {"clkScreenSize",       S(quint32),             QFV.asciiAlNum().maxLenght(50), true},
                {"clkOS",               S(quint32),             QFV.asciiAlNum().maxLenght(50), true},
                {"clkBrowser",          S(quint32),             QFV.asciiAlNum().maxLenght(50), true},
              },
              { ///< Col             Reference Table                 ForeignCol   Rename     LeftJoin
                {"clk_binID",        "Advert.tblBin",                "binID"},
                //{"clk_locID",        "Advert.tblLocations",          "locID"},
              })
{
    this->registerMyRESTAPIs();
}

}
}
}
