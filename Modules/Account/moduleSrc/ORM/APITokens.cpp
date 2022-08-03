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

#include "APITokens.h"
#include "User.h"

TAPI_REGISTER_TARGOMAN_ENUM(Targoman::API::AccountModule, enuAPITokensStatus);

namespace Targoman::API::AccountModule::ORM {

APITokens::APITokens() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokens::Name,
        tblAPITokens::Private::ORMFields,
        tblAPITokens::Private::Relations,
        tblAPITokens::Private::Indexes
    ) { ; }

QVariant IMPL_ORMGET(APITokens) {
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getUserID()}}, _filters);

    return this->Select(GET_METHOD_ARGS_CALL_VALUES);
}

quint64 IMPL_ORMCREATE(APITokens) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return this->Create(CREATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMUPDATE(APITokens) {
    Authorization::checkPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_PATCH, this->moduleBaseName()));

    return this->Update(UPDATE_METHOD_ARGS_CALL_VALUES);
}

bool IMPL_ORMDELETE(APITokens) {
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(APICALLBOOM_PARAM, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getUserID());
//    this->setSelfFilters({{tblAPITokens::Fields::apt_usrID, APICALLBOOM_PARAM.getUserID()}}, ExtraFilters);

    return this->DeleteByPks(DELETE_METHOD_ARGS_CALL_VALUES, ExtraFilters);
}

} //namespace Targoman::API::AccountModule::ORM
