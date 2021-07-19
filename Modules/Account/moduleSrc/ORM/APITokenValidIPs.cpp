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

#include "Interfaces/ORM/APIQueryBuilders.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant APITokenValidIPs::apiGET(GET_METHOD_ARGS_IMPL_APICALL)
{
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _filters);

    return Targoman::API::Query::Select(*this, GET_METHOD_CALL_ARGS_INTERNAL_CALL);

//    return query.one();

    //    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS_APICALL);
}

quint64 APITokenValidIPs::apiCREATE(CREATE_METHOD_ARGS_IMPL_APICALL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT, this->moduleBaseName()));

    return Targoman::API::Query::Create(*this, CREATE_METHOD_CALL_ARGS_INTERNAL_CALL);
}

bool APITokenValidIPs::apiUPDATE(UPDATE_METHOD_ARGS_IMPL_APICALL)
{
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);

    return Targoman::API::Query::Update(*this, UPDATE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters);
}

bool APITokenValidIPs::apiDELETE(DELETE_METHOD_ARGS_IMPL_APICALL)
{
    TAPI::ORMFields_t ExtraFilters;
    if (Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE, this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);

    return Targoman::API::Query::Delete(*this, DELETE_METHOD_CALL_ARGS_INTERNAL_CALL, ExtraFilters, true);
//    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS_APICALL, ExtraFilters, true);
}

APITokenValidIPs::APITokenValidIPs() :
    clsTable(
        AAASchema,
        tblAPITokenValidIPs::Name,
        {///< ColName                                   Type                 Validation                      Default     UpBy   Sort  Filter Self  Virt   PK
            { tblAPITokenValidIPs::tviID,               ORM_PRIMARY_KEY64 },
            { tblAPITokenValidIPs::tvi_aptID,           S(quint64),          QFV.integer().minValue(1),      QRequired,  UPNone },
            { tblAPITokenValidIPs::tviIP,               S(quint64),          QFV.integer().minValue(1),      QRequired,  UPOwner },
            { tblAPITokenValidIPs::tviIPReadable,       S(QString),          QFV.allwaysInvalid(),           QInvalid,   UPNone, false, false },
            { tblAPITokenValidIPs::tviStatus,           ORM_STATUS_FIELD(TAPI::enuGenericStatus, TAPI::enuGenericStatus::Active) },
            { "_tviVersion",                            ORM_VERSION_FIELD },
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
                tblAPITokenValidIPs::tviStatus,
                "_tviVersion",
              }, enuDBIndex::Unique },
        }
    )
{}

}
}
}
