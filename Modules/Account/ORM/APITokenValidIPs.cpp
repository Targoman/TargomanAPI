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

#include "APITokenValidIPs.h"
#include "User.h"
#include "APITokens.h"

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant APITokenValidIPs::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable({}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokenValidIPs::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(DELETE_METHOD_CALL_ARGS, true);
}

bool APITokenValidIPs::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        this->setSelfFilters({{tblAPITokens::apt_usrID, clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    return this->update(UPDATE_METHOD_CALL_ARGS);
}

quint64 APITokenValidIPs::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(CREATE_METHOD_CALL_ARGS).toULongLong();
}

APITokenValidIPs::APITokenValidIPs() :
    clsTable(AAASchema,
              tblAPITokenValidIPs::Name,
              { ///<ColName             Type                 Validation                      Default    RO   Sort  Filter Self  Virt   PK
                {tblAPITokenValidIPs::tviID,               S(quint64),          QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {tblAPITokenValidIPs::tvi_aptID,           S(quint64),          QFV.integer().minValue(1),      QInvalid,  true},
                {tblAPITokenValidIPs::tviIP,               S(quint64),          QFV.integer().minValue(1),      QInvalid},
                {tblAPITokenValidIPs::tviIPReadable,       S(QString),          QFV.allwaysInvalid(),           QInvalid,  true,false, false},
                {tblAPITokenValidIPs::tviCreatedBy_usrID,  S(quint32),          QFV.integer().minValue(1),      QInvalid,  true},
                {tblAPITokenValidIPs::tviCreationDateTime, S(TAPI::DateTime_t), QFV,                            QNull,     true},
                {tblAPITokenValidIPs::tviUpdatedBy_usrID,  S(quint32),          QFV.integer().minValue(1),      QNull,     true},
                {tblAPITokenValidIPs::tviStatus,           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active},
              },
              { ///< Col                                  Reference Table                   ForeignCol              Rename     LeftJoin
                {tblAPITokenValidIPs::tvi_aptID,          R(AAASchema,tblAPITokens::Name),  tblAPITokens::aptID},
                {tblAPITokenValidIPs::tviCreatedBy_usrID, R(AAASchema,tblUser::Name),       tblUser::usrID,         "Creator_", true},
                {tblAPITokenValidIPs::tviUpdatedBy_usrID, R(AAASchema,tblUser::Name),       tblUser::usrID,         "Updater_", true}
              })
{
}

}
}
}
