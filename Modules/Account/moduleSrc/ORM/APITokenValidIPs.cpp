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
#include "APITokens.h"

//#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman::API::AccountModule::ORM {

QVariant APITokenValidIPs::apiGET(APISession<true> &_SESSION, GET_METHOD_ARGS_IMPL_APICALL) {
    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, _SESSION.getUserID()}}, _filters);

    auto QueryLambda = [](SelectQuery &_query) {
        _query.innerJoin(tblAPITokens::Name);
    };

    return /*Targoman::API::Query::*/this->Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL, {}, 0, QueryLambda);
}

quint64 APITokenValidIPs::apiCREATE(APISession<true> &_SESSION, CREATE_METHOD_ARGS_IMPL_APICALL) {
    Authorization::checkPriv(_SESSION.getJWT(), this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return /*Targoman::API::Query::*/this->Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool APITokenValidIPs::apiUPDATE(APISession<true> &_SESSION, UPDATE_METHOD_ARGS_IMPL_APICALL) {
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::apt_usrID, _SESSION.getUserID());

//    this->setSelfFilters({{tblAPITokens::apt_usrID, _SESSION.getUserID()}}, ExtraFilters);

    return /*Targoman::API::Query::*/this->Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

bool APITokenValidIPs::apiDELETE(APISession<true> &_SESSION, DELETE_METHOD_ARGS_IMPL_APICALL) {
    TAPI::ORMFields_t ExtraFilters;

    if (Authorization::hasPriv(_SESSION.getJWT(), this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        ExtraFilters.insert(tblAPITokens::apt_usrID, _SESSION.getUserID());
//    this->setSelfFilters({{tblAPITokens::apt_usrID, _SESSION.getUserID()}}, ExtraFilters);

    return /*Targoman::API::Query::*/this->DeleteByPks(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

APITokenValidIPs::APITokenValidIPs() :
    intfSQLBasedModule(
        AAASchema,
        tblAPITokenValidIPs::Name,
        {///< ColName                                   Type                 Validation                      Default     UpBy   Sort  Filter Self  Virt   PK
            { tblAPITokenValidIPs::tviID,               ORM_PRIMARYKEY_64 },
            { tblAPITokenValidIPs::tvi_aptID,           S(quint64),          QFV.integer().minValue(1),      QRequired,  UPNone },
            { tblAPITokenValidIPs::tviIP,               S(quint64),          QFV.integer().minValue(1),      QRequired,  UPOwner },
            { tblAPITokenValidIPs::tviIPReadable,       S(QString),          QFV.allwaysInvalid(),           QInvalid,   UPNone, false, false },
            { tblAPITokenValidIPs::tviStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { ORM_INVALIDATED_AT_FIELD },
            { tblAPITokenValidIPs::tviCreationDateTime, ORM_CREATED_ON },
            { tblAPITokenValidIPs::tviCreatedBy_usrID,  ORM_CREATED_BY },
            { tblAPITokenValidIPs::tviUpdatedBy_usrID,  ORM_UPDATED_BY },
        },
        { ///< Col                                     Reference Table                   ForeignCol              Rename     LeftJoin
            { tblAPITokenValidIPs::tvi_aptID,          R(AAASchema,tblAPITokens::Name),  tblAPITokens::aptID },
            ORM_RELATION_OF_CREATOR(tblAPITokenValidIPs::tviCreatedBy_usrID),
            ORM_RELATION_OF_UPDATER(tblAPITokenValidIPs::tviUpdatedBy_usrID),
        },
        {
            { {
                tblAPITokenValidIPs::tvi_aptID,
                tblAPITokenValidIPs::tviIP,
                ORM_INVALIDATED_AT_FIELD_NAME,
              }, enuDBIndex::Unique },
        }
    ) { ; }

} //namespace Targoman::API::AccountModule::ORM
