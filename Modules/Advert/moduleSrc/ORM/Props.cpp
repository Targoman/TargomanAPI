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
//#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AdvertModule::ORM {

//using namespace ORM;

QVariant Props::apiGET(APISession<true> &_SESSION, GET_METHOD_ARGS_IMPL_APICALL) {
    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::binID, _SESSION.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblBin::Name);
    };

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, 0, QueryLambda);
}

quint64 Props::apiCREATE(APISession<true> &_SESSION, CREATE_METHOD_ARGS_IMPL_APICALL) {
    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        _createInfo.insert(tblBin::binID, _SESSION.getUserID());
//    this->setSelfFilters({{tblBin::binID, _SESSION.getUserID()}}, _createInfo);

    return /*Targoman::API::Query::*/this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool Props::apiUPDATE(APISession<true> &_SESSION, UPDATE_METHOD_ARGS_IMPL_APICALL) {
    QVariantMap ExtraFilters;

    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_PATCH,this->moduleBaseName())))
        ExtraFilters.insert(tblBin::binID, _SESSION.getUserID());
//    this->setSelfFilters({{tblBin::binID, _SESSION.getUserID()}}, ExtraFilters);

    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

bool Props::apiDELETE(APISession<true> &_SESSION, DELETE_METHOD_ARGS_IMPL_APICALL) {
    QVariantMap ExtraFilters;
    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblBin::binID, _SESSION.getUserID());
//    this->setSelfFilters({{tblBin::binID, _SESSION.getUserID()}}, ExtraFilters);

    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

Props::Props() :
    intfSQLBasedModule(AdvertSchema,
              tblProps::Name,
              { ///<ColName                     Type                    Validation                        UpBy   Sort  Filter Self  Virt   PK
                {tblProps::prp_binID,           ORM_PRIMARYKEY_32},
                {tblProps::prp_locID,           ORM_PRIMARYKEY_32},
                {tblProps::prpOrder,            S(Targoman::API::AdvertModule::enuAdvertOrder::Type), QFV,                       Targoman::API::AdvertModule::enuAdvertOrder::Normal,  UPOwner},
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
              }) { ; }

} //namespace Targoman::API::AdvertModule::ORM
