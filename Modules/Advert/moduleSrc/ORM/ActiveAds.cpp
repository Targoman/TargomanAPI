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

#include "ActiveAds.h"
#include "Interfaces/AAA/AAA.hpp"
#include "Defs.hpp"
#include "Bin.h"
#include "Locations.h"
//#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AdvertModule::ORM {

//using namespace ORM;

QVariant ActiveAds::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

bool ActiveAds::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

ActiveAds::ActiveAds() :
    intfSQLBasedModule(AdvertSchema,
              tblActiveAds::Name,
              { ///<ColName                  Type           Validation                        Default     UpBy   Sort  Filter Self  Virt   PK
                {tblActiveAds::act_binID,    ORM_PRIMARYKEY_32},
                {tblActiveAds::act_locID,    ORM_PRIMARYKEY_32},
                {tblActiveAds::actOrder,     S(Targoman::API::AdvertModule::enuAdvertOrder::Type),QFV,               Targoman::API::AdvertModule::enuAdvertOrder::Normal, UPNone},
                {tblActiveAds::actOnKeyword, S(QString),    QFV.unicodeAlNum().maxLenght(50), QInvalid, UPNone},
              },
              { ///< Col                     Reference Table                 ForeignCol   Rename     LeftJoin
                {tblActiveAds::act_binID,    R(AdvertSchema,tblBin::Name),   tblBin::binID},
                {tblActiveAds::act_locID,    R(AdvertSchema,tblLocations::Name), tblLocations::locID },
              })
{
}

} //namespace Targoman::API::AdvertModule::ORM
