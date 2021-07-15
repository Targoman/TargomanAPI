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
 * @author S. Mehran M. Ziabary <ziabary@targoman.com>
 * @author Kambiz Zandi <kambizzandi@gmail.com>
 */

#include "Clicks.h"
#include "Defs.hpp"
#include "Bin.h"
#include "Locations.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace Advertisement {

using namespace ORM;

QVariant Clicks::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

Clicks::Clicks() :
    clsTable(AdvertSchema,
              tblClicks::Name,
              { ///<ColName                 Type                    Validation                      UpBy   Sort  Filter Self  Virt   PK
                {tblClicks::clkID,          ORM_PRIMARY_KEY64},
                {tblClicks::clk_binID,      S(quint32),             QFV.integer().minValue(1),      QInvalid, UPNone},
                {tblClicks::clk_locID,      S(quint32),             QFV.integer().minValue(1),      QInvalid, UPNone},
                {tblClicks::clkDateTime,    S(TAPI::DateTime_t),    QFV,                            QInvalid, UPNone},
                {tblClicks::clkIP,          S(quint32),             QFV.ipv4(),                     QInvalid, UPNone},
                {tblClicks::clkDevice,      S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                {tblClicks::clkScreenSize,  S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                {tblClicks::clkOS,          S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
                {tblClicks::clkBrowser,     S(quint32),             QFV.asciiAlNum().maxLenght(50), QNull,    UPNone},
              },
              { ///< Col                 Reference Table                     ForeignCol   Rename     LeftJoin
                {tblClicks::clk_binID,   R(AdvertSchema,tblBin::Name),       tblBin::binID},
                {tblClicks::clk_locID,   R(AdvertSchema,tblLocations::Name), tblLocations::locID },
              })
{
}

}
}
}
