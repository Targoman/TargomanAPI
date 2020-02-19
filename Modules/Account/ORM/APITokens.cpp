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
#include "Services.h"

TAPI_REGISTER_TARGOMAN_ENUM(TAPI,enuAPITokensStatus);

namespace Targoman {
namespace API {
namespace AAA {
using namespace ORM;

QVariant APITokens::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);
    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokens::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS);
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
                {tblAPITokens::aptToken,            S(QString),            QFV.asciiAlNum().maxLenght(50), QInvalid,  UPNone, true, false},
                {tblAPITokens::apt_usrID,           S(quint32),            QFV.integer().minValue(1),      0,         UPNone},
                {tblAPITokens::apt_svcID,           S(quint32),            QFV.integer().minValue(1),      0,         UPAdmin},
                {tblAPITokens::aptLang,             S(TAPI::ISO639_2_t),   QFV,                            "en",      UPAdmin},
                {tblAPITokens::aptValidateIP,       S(bool),               QFV,                            false,     UPAdmin},
                {tblAPITokens::aptExtraPriviledges, S(TAPI::JSON_t),       QFV,                            QNull,     UPAdmin,false, false},
                {tblAPITokens::aptExpiryDate,       S(TAPI::DateTime_t),   QFV,                            QNull,     UPAdmin},
                {tblAPITokens::aptLastActivity,     S(TAPI::DateTime_t),   QFV,                            QNull ,    UPNone},
                {tblAPITokens::aptAccessCount,      S(quint32),            QFV.integer().minValue(1),      0,         UPNone},
                {tblAPITokens::aptCreatedBy_usrID,  S(quint32),            QFV.integer().minValue(1),      QInvalid,  UPNone},
                {tblAPITokens::aptCreationDateTime, S(TAPI::DateTime_t),   QFV,                            QNull,     UPNone},
                {tblAPITokens::aptUpdatedBy_usrID,  S(quint32),            QFV.integer().minValue(1),      QNull,     UPNone},
                {tblAPITokens::aptStatus,           S(TAPI::enuAPITokensStatus::Type),QFV,                 TAPI::enuAPITokensStatus::Active, UPAdmin},
              },
              { ///< Col                           Reference Table                 ForeignCol             Rename      LeftJoin
                {tblAPITokens::apt_svcID,          R(AAASchema,tblServices::Name), tblServices::svcID,    {},         true},
                {tblAPITokens::apt_usrID,          R(AAASchema,tblUser::Name),     tblUser::usrID,        "Owner_",   true},
                {tblAPITokens::aptCreatedBy_usrID, R(AAASchema,tblUser::Name),     tblUser::usrID,        "Creator_", true},
                {tblAPITokens::aptUpdatedBy_usrID, R(AAASchema,tblUser::Name),     tblUser::usrID,        "Updater_", true}
              })
{
}

}
}
}
