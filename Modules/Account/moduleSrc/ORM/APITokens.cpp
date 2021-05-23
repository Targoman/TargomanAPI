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

#include "APITokens.h"
#include "User.h"
#include "Service.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAPITokensStatus);

namespace Targoman {
namespace API {
namespace AAA {
using namespace ORM;

QVariant APITokens::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _filters);
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokens::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    TAPI::ORMFields_t ExtraFilters;
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, ExtraFilters);

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, ExtraFilters);
}

bool APITokens::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName()));
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint32 APITokens::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toUInt();
}

APITokens::APITokens() :
    clsTable(AAASchema,
              "tblAPITokens",
              { ///<ColName                         Type                   Validation                      Default    UpBy   Sort  Filter Self  Virt   PK
                {tblAPITokens::aptID,               S(quint64),            QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {tblAPITokens::aptToken,            S(QString),            QFV.asciiAlNum().maxLenght(50), QRequired, UPNone, true, false},
                {tblAPITokens::apt_usrID,           S(quint32),            QFV.integer().minValue(1),      QRequired, UPNone},
                {tblAPITokens::apt_svcID,           S(quint32),            QFV.integer().minValue(1),      QRequired, UPAdmin},
                {tblAPITokens::aptLang,             S(TAPI::ISO639_2_t),   QFV,                            "en",      UPAdmin},
                {tblAPITokens::aptValidateIP,       S(bool),               QFV,                            false,     UPAdmin},
                {tblAPITokens::aptExtraPriviledges, S(TAPI::PrivObject_t), QFV,                            QNull,     UPAdmin,false, false},
                {tblAPITokens::aptExpiryDate,       S(TAPI::DateTime_t),   QFV,                            QNull,     UPAdmin},
                {tblAPITokens::aptLastActivity,     S(TAPI::DateTime_t),   QFV,                            QInvalid,  UPNone},
                {tblAPITokens::aptAccessCount,      S(quint32),            QFV.integer().minValue(1),      QInvalid,  UPNone},
                {tblAPITokens::aptCreatedBy_usrID,  ORM_CREATED_BY},
                {tblAPITokens::aptCreationDateTime, ORM_CREATED_ON},
                {tblAPITokens::aptUpdatedBy_usrID,  ORM_UPDATED_BY},
                {tblAPITokens::aptStatus,           S(TAPI::enuAPITokensStatus::Type),QFV,                 TAPI::enuAPITokensStatus::Active, UPStatus},
              },
              { ///< Col                           Reference Table                 ForeignCol             Rename      LeftJoin
                {tblAPITokens::apt_svcID,          R(AAASchema,tblService::Name),  tblService::svcID,    {},         true},
                {tblAPITokens::apt_usrID,          R(AAASchema,tblUser::Name),     tblUser::usrID,        "Owner_",   true},
                {tblAPITokens::aptCreatedBy_usrID, ORM_JOIN_CREATOR},
                {tblAPITokens::aptUpdatedBy_usrID, ORM_JOIN_UPDATER},
              })
{
}

}
}
}
