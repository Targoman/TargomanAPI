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
 @author S. Mehran M. Ziabary <ziabary@targoman.com>
 */

#include "ActiveAds.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Bin.h"
#include "Locations.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant ActiveAds::apiGET(GET_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName()));
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool ActiveAds::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName()));
    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}


ActiveAds::ActiveAds() :
    clsTable(AdvertSchema,
              tblActiveAds::Name,
              { ///<ColName                  Type           Validation                        Default     UpBy   Sort  Filter Self  Virt   PK
                {tblActiveAds::act_binID,    S(quint32),    QFV.integer().minValue(1),        ORM_PRIMARY_KEY},
                {tblActiveAds::act_locID,    S(quint32),    QFV.integer().minValue(1),        ORM_PRIMARY_KEY},
                {tblActiveAds::actOrder,     S(Targoman::API::enuAdvertOrder::Type),QFV,      QInvalid, UPNone},
                {tblActiveAds::actOnKeyword, S(QString),    QFV.unicodeAlNum().maxLenght(50), QInvalid, UPNone},
              },
              { ///< Col                     Reference Table                 ForeignCol   Rename     LeftJoin
                {tblActiveAds::act_binID,    R(AdvertSchema,tblBin::Name),   tblBin::binID},
                {tblActiveAds::act_locID,    R(AdvertSchema,tblLocations::Name), tblLocations::locID },
              })
{
}

}
}
}
