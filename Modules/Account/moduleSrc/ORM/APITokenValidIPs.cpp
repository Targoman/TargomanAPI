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

#include "APITokenValidIPs.h"
#include "User.h"

namespace Targoman::API::AccountModule::ORM {

APITokenValidIPs::APITokenValidIPs() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokenValidIPs::Name,
        tblAPITokenValidIPs::Private::ORMFields,
        tblAPITokenValidIPs::Private::Relations,
        tblAPITokenValidIPs::Private::Indexes
    ) { ; }

QVariant IMPL_ORMGET(APITokenValidIPs) {
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, _APICALLBOOM.getUserID()}}, _filters);

    auto fnTouchQuery = [](SelectQuery &_query) {
        _query.innerJoin(tblAPITokens::Name);
    };

    return this->Select(GET_METHOD_ARGS_CALL_INTERNAL_BOOM, {}, 0, fnTouchQuery);
}

quint64 IMPL_ORMCREATE(APITokenValidIPs) {
    Authorization::checkPriv(_APICALLBOOM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_INTERNAL_BOOM);
}

bool IMPL_ORMUPDATE(APITokenValidIPs) {
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, _APICALLBOOM.getUserID());

//    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, _APICALLBOOM.getUserID()}}, ExtraFilters);

    return this->Update(UPDATE_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters);
}

bool IMPL_ORMDELETE(APITokenValidIPs) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(_APICALLBOOM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, _APICALLBOOM.getUserID());
//    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, _APICALLBOOM.getUserID()}}, ExtraFilters);

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_INTERNAL_BOOM, ExtraFilters, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

} //namespace Targoman::API::AccountModule::ORM
