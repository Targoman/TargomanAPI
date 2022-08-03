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

namespace Targoman::API::AdvertModule::ORM {

Props::Props() :
    intfSQLBasedModule(
        AdvertSchema,
        tblProps::Name,
        tblProps::Private::ORMFields,
        tblProps::Private::Relations,
        tblProps::Private::Indexes
) { ; }

QVariant IMPL_ORMGET(Props) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    auto fnTouchQuery = [](ORMSelectQuery &_query) {
        _query.innerJoin(tblBin::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL, {}, 0, fnTouchQuery);
}

quint64 IMPL_ORMCREATE(Props) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        _createInfo.insert(tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID());
//    this->setSelfFilters({{tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID()}}, _createInfo);

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL);
}

bool IMPL_ORMUPDATE(Props) {
    QVariantMap ExtraFilters;

    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH,this->moduleBaseName())))
        ExtraFilters.insert(tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID());
//    this->setSelfFilters({{tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID()}}, ExtraFilters);

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL, ExtraFilters);
}

bool IMPL_ORMDELETE(Props) {
    QVariantMap ExtraFilters;
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID());
//    this->setSelfFilters({{tblBin::Fields::binID, APICALLBOOM_PARAM.getUserID()}}, ExtraFilters);

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL, ExtraFilters);
}

} //namespace Targoman::API::AdvertModule::ORM
