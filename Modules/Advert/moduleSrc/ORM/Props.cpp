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

#include "Props.h"
#include "Defs.hpp"
#include "Locations.h"
#include "Bin.h"

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Props::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 Props::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, _createInfo);

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Props::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    QVariantMap ExtraFilters;

    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName())))
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, ExtraFilters);

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

bool Props::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    QVariantMap ExtraFilters;
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, ExtraFilters);

    return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

Props::Props() :
    clsTable(AdvertSchema,
              tblProps::Name,
              { ///<ColName                     Type                    Validation                        UpBy   Sort  Filter Self  Virt   PK
                {tblProps::prp_binID,           ORM_PRIMARY_KEY32},
                {tblProps::prp_locID,           ORM_PRIMARY_KEY32},
                {tblProps::prpOrder,            S(TAPI::enuAdvertOrder::Type), QFV,                       TAPI::enuAdvertOrder::Normal,  UPOwner},
                {tblProps::prpKeyword,          S(QString),             QFV.unicodeAlNum().maxLenght(50), QRequired,  UPOwner},
                {tblProps::prpStartDate,        S(TAPI::DateTime_t),    QFV,                              QRequired,  UPOwner},
                {tblProps::prpEndDate,          S(TAPI::DateTime_t),    QFV,                              QRequired,  UPOwner},
                {tblProps::prpCreatedBy_usrID,  ORM_CREATED_BY},
                {tblProps::prpCreationDateTime, ORM_CREATED_ON},
                {tblProps::prpUpdatedBy_usrID,  ORM_UPDATED_BY},
              },
              { ///< Col                        Reference Table                     ForeignCol   Rename     LeftJoin
                {tblProps::prp_binID,           R(AdvertSchema,tblBin::Name),       tblBin::binID },
                {tblProps::prp_locID,           R(AdvertSchema,tblLocations::Name), tblLocations::locID },
                ORM_RELATION_OF_CREATOR(tblProps::prpCreatedBy_usrID),
                ORM_RELATION_OF_UPDATER(tblProps::prpUpdatedBy_usrID),
              })
{
}

}
}
}
