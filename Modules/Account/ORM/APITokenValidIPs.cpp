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

namespace Targoman {
namespace API {
namespace AAA {

using namespace ORM;

QVariant APITokenValidIPs::apiGET(GET_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_GET,this->moduleBaseName())) == false)
        this->setSelfFilters({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS, _filters);

    return this->selectFromTable(AAADAC, {}, {}, GET_METHOD_CALL_ARGS);
}

bool APITokenValidIPs::apiDELETE(DELETE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_DELETE,this->moduleBaseName())) == false)
        this->setSelfFilters({{"apt_usrID", clsJWT(_JWT).usrID()}}, _EXTRAPATH, _ORMFILTERS);

    return this->deleteByPKs(AAADAC, DELETE_METHOD_CALL_ARGS, true);
}

bool APITokenValidIPs::apiUPDATE(UPDATE_METHOD_ARGS_IMPL)
{
    if(Authorization::hasPriv(_JWT, this->privOn(EHTTP_PATCH,this->moduleBaseName())) == false)
        this->setSelfFilters({{"apt_usrID", clsJWT(_JWT).usrID()}}, {}, _ORMFILTERS);
    return this->update(AAADAC, UPDATE_METHOD_CALL_ARGS);
}

quint64 APITokenValidIPs::apiCREATE(CREATE_METHOD_ARGS_IMPL)
{
    Authorization::checkPriv(_JWT, this->privOn(EHTTP_PUT,this->moduleBaseName()));
    return this->create(AAADAC, CREATE_METHOD_CALL_ARGS).toULongLong();
}

APITokenValidIPs::APITokenValidIPs() :
    clsTable("AAA",
              "tblAPITokenValidIPs",
              { ///<ColName             Type                 Validation                      Default    RO   Sort  Filter Self  Virt   PK
                {"tviID",               S(quint64),          QFV.integer().minValue(1),      ORM_PRIMARY_KEY},
                {"tvi_aptID",           S(quint64),          QFV.integer().minValue(1),      QInvalid,  true},
                {"tviIP",               S(quint64),          QFV.integer().minValue(1),      QInvalid},
                {"tviIPReadable",       S(QString),          QFV.allwaysInvalid(),           QInvalid,  true,false, false},
                {"tviCreatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),      QInvalid,  true},
                {"tviCreationDateTime", S(TAPI::DateTime_t), QFV,                            QNull,     true},
                {"tviUpdatedBy_usrID",  S(quint32),          QFV.integer().minValue(1),      QNull,     true},
                {"tviStatus",           S(TAPI::enuGenericStatus::Type), QFV,                TAPI::enuGenericStatus::Active},
              },
              { ///< Col               Reference Table     ForeignCol   Rename     LeftJoin
                {"tvi_aptID",          "AAA.tblAPITokens",  "aptID"},
                {"tviCreatedBy_usrID", "AAA.tblUser",      "usrID",     "Creator_", true},
                {"tviUpdatedBy_usrID", "AAA.tblUser",      "usrID",     "Updater_", true}
              })
{
}

}
}
}
