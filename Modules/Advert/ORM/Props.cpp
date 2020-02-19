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

#include "Props.h"
#include "Defs.hpp"
#include "Locations.h"
#include "Bin.h"

namespace Targoman {
namespace API {
namespace Advertisement {
using namespace ORM;

QVariant Props::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool Props::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
}

bool Props::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()))){
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    }
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint64 Props::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);

    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

Props::Props() :
    clsTable(AdvertSchema,
              tblProps::Name,
              { ///<ColName                     Type                    Validation                        UpBy   Sort  Filter Self  Virt   PK
                {tblProps::prp_binID,           S(quint32),             QFV.integer().minValue(1),        ORM_PRIMARY_KEY},
                {tblProps::prp_locID,           S(quint32),             QFV.integer().minValue(1),        ORM_PRIMARY_KEY},
                {tblProps::prpOrder,            S(Targoman::API::enuAdvertOrder::Type), QFV,              QInvalid,  UPAll},
                {tblProps::prpKeyword,          S(QString),             QFV.unicodeAlNum().maxLenght(50), QInvalid,  UPAll},
                {tblProps::prpStartDate,        S(QHttp::DateTime_t),   QFV,                              QInvalid,  UPAll},
                {tblProps::prpEndDate,          S(QHttp::DateTime_t),   QFV,                              QInvalid,  UPAll},
                {tblProps::prpCreatedBy_usrID,  S(quint32),             QFV.integer().minValue(1),        QInvalid,  UPNone},
                {tblProps::prpCreationDateTime, S(QHttp::DateTime_t),   QFV,                              QNull,     UPNone},
                {tblProps::prpUpdatedBy_usrID,  S(quint32),             QFV.integer().minValue(1),        QNull,     UPNone},
              },
              { ///< Col                        Reference Table                     ForeignCol   Rename     LeftJoin
                {tblProps::prp_binID,           R(AdvertSchema,tblBin::Name),       tblBin::binID },
                {tblProps::prp_locID,           R(AdvertSchema,tblLocations::Name), tblLocations::locID },
                {tblProps::prpCreatedBy_usrID,  R(AAASchema,tblUser::Name),         tblUser::usrID,   "Creator_", true},
                {tblProps::prpUpdatedBy_usrID,  R(AAASchema,tblUser::Name),         tblUser::usrID,   "Updater_", true}
              })
{
}

}
}
}
